#!/usr/bin/env python3
"""
Multi-threaded Serial Frame Parser (macOS) - multi-byte header + sliding-window re-sync
Frame format: HEADER... D1 D2 ... Dn CS
  - HEADER : 1 or more bytes defined in HEADER_LENGTH_MAP
  - D1..Dn : variable-length payload (n defined per header)
  - CS     : checksum = sum(D1..Dn) & 0xFF

Re-sync strategy
----------------
When a candidate frame fails CS validation the parser drops only the first
byte and retries from byte 2, guaranteeing re-lock even when payload data
contains header sequences.

Thread layout
-------------
  SerialReaderThread  --raw bytes-->   RxQueue
  FrameParserThread   <--raw bytes--   RxQueue
                      --ParsedFrame--> PrintQueue
  PrinterThread       <--ParsedFrame-- PrintQueue
"""

import serial
import serial.tools.list_ports
import threading
import queue
import sys
import time
import logging
from collections import deque
from datetime import datetime
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple


# ------------------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------------------

# Map each header (as a bytes sequence) -> expected payload length n.
# Header can be 1 byte or multiple bytes.
HEADER_LENGTH_MAP: Dict[bytes, int] = {
    bytes([0x58])       : 7,   # 'X'         -> 7 data bytes
    bytes([0x49, 0x34]) : 4,   # 0x49 0x34   -> 4 data bytes  (edit n as needed)
    bytes([0x56, 0x34]) : 12,   # 0x56 0x34   -> 12 data bytes  (edit n as needed)
}

DEFAULT_PORT  = "/dev/tty.usbserial-0001"
BAUDRATE      = 4800
TIMEOUT       = 1.0
LOG_FILE      = "serial_log.txt"
QUEUE_MAXSIZE = 512

# Pre-compute max header length for buffer sizing
MAX_HDR_LEN = max(len(h) for h in HEADER_LENGTH_MAP)


# ------------------------------------------------------------------------------
# Data structure
# ------------------------------------------------------------------------------

@dataclass
class ParsedFrame:
    timestamp : str    # "2026-03-08 12:34:56.789"
    header    : bytes  # header bytes
    payload   : bytes  # D1..Dn
    checksum  : int    # CS byte as received
    cs_ok     : bool   # True = checksum matched
    raw       : bytes  # complete raw frame bytes
    resyncs   : int    # bytes discarded before this frame was found


# ------------------------------------------------------------------------------
# Logging
# ------------------------------------------------------------------------------

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(threadName)-14s] %(levelname)s: %(message)s",
    handlers=[
        logging.StreamHandler(sys.stdout),
        logging.FileHandler(LOG_FILE, encoding="utf-8"),
    ],
)
log = logging.getLogger(__name__)


# ------------------------------------------------------------------------------
# Thread 1 -- Serial Reader
# ------------------------------------------------------------------------------

class SerialReaderThread(threading.Thread):
    """Reads raw bytes from the serial port and pushes each into rx_queue."""

    def __init__(self, ser: serial.Serial, rx_queue: queue.Queue, stop_event: threading.Event):
        super().__init__(name="SerialReader", daemon=True)
        self._ser        = ser
        self._rx_queue   = rx_queue
        self._stop_event = stop_event

    def run(self):
        log.info("Reader started: %s @ %d baud", self._ser.port, self._ser.baudrate)
        while not self._stop_event.is_set():
            try:
                byte = self._ser.read(1)
                if byte:
                    self._rx_queue.put(byte[0], timeout=1)
            except serial.SerialException as exc:
                log.error("Serial read error: %s", exc)
                self._stop_event.set()
                break
            except queue.Full:
                log.warning("rx_queue full -- byte dropped")
        log.info("Reader stopped.")


# ------------------------------------------------------------------------------
# Thread 2 -- Frame Parser  (sliding-window re-sync, multi-byte header)
# ------------------------------------------------------------------------------

class FrameParserThread(threading.Thread):
    """
    Sliding-window frame parser supporting variable-length headers.

    Algorithm
    ---------
    1. Fill look-ahead buffer to at least MAX_HDR_LEN bytes.
    2. Try to match the longest header first (greedy), then shorter ones.
       - No match -> drop buf[0], increment resync counter, goto 1.
    3. Pull remaining bytes for this header: payload (n bytes) + CS (1 byte).
    4. Validate CS = sum(payload) & 0xFF.
       - Match    -> emit ParsedFrame, consume all frame bytes, goto 1.
       - Mismatch -> drop buf[0] only (rewind by 1), goto 1.
    """

    def __init__(
        self,
        rx_queue    : queue.Queue,
        print_queue : queue.Queue,
        header_map  : Dict[bytes, int],
        stop_event  : threading.Event,
    ):
        super().__init__(name="FrameParser", daemon=True)
        self._rx_queue    = rx_queue
        self._print_queue = print_queue
        self._stop_event  = stop_event

        # Sort headers longest-first so greedy matching works correctly
        self._headers: List[Tuple[bytes, int]] = sorted(
            header_map.items(), key=lambda x: len(x[0]), reverse=True
        )
        self._max_hdr = max(len(h) for h, _ in self._headers)

    def _fill(self, buf: deque, need: int):
        """Block-pull bytes from rx_queue until buf has at least `need` items."""
        while len(buf) < need and not self._stop_event.is_set():
            try:
                b = self._rx_queue.get(timeout=0.1)
                buf.append(b)
            except queue.Empty:
                pass

    def _match_header(self, buf: deque) -> Optional[Tuple[bytes, int]]:
        """
        Try to match a known header starting at buf[0].
        Returns (header_bytes, payload_len) or None.
        Headers are tried longest-first.
        """
        buf_bytes = bytes(list(buf)[:self._max_hdr])
        for hdr, n in self._headers:
            if buf_bytes[:len(hdr)] == hdr:
                return hdr, n
        return None

    def run(self):
        log.info("Parser started. Headers: %s",
                 {hdr.hex(): n for hdr, n in self._headers})

        buf     = deque()
        resyncs = 0

        while not self._stop_event.is_set():

            # Need enough bytes to attempt a header match
            self._fill(buf, self._max_hdr)
            if len(buf) < 1:
                continue

            match = self._match_header(buf)

            if match is None:
                # No header at current position -- discard buf[0] and slide
                buf.popleft()
                resyncs += 1
                continue

            hdr_bytes, n = match
            hdr_len      = len(hdr_bytes)
            frame_size   = hdr_len + n + 1   # header + payload + CS

            # Pull the full candidate frame into the buffer
            self._fill(buf, frame_size)
            if len(buf) < frame_size:
                continue   # stop_event fired mid-stream

            frame_bytes = [buf[i] for i in range(frame_size)]
            payload     = bytes(frame_bytes[hdr_len : hdr_len + n])
            received_cs = frame_bytes[-1]
            computed_cs = sum(payload) & 0xFF

            if received_cs == computed_cs:
                # Good frame -- consume all frame bytes
                ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                for _ in range(frame_size):
                    buf.popleft()

                if resyncs:
                    log.warning("Re-synced after %d byte(s) discarded", resyncs)

                frame = ParsedFrame(
                    timestamp = ts,
                    header    = hdr_bytes,
                    payload   = payload,
                    checksum  = received_cs,
                    cs_ok     = True,
                    raw       = bytes(frame_bytes),
                    resyncs   = resyncs,
                )
                resyncs = 0

                try:
                    self._print_queue.put(frame, timeout=1)
                except queue.Full:
                    log.warning("print_queue full -- frame dropped")

            else:
                # CS mismatch -- rewind: drop only buf[0], retry from byte 1
                log.debug(
                    "CS mismatch HDR=%s got=0x%02X want=0x%02X -- rewind 1 byte",
                    hdr_bytes.hex(), received_cs, computed_cs
                )
                buf.popleft()
                resyncs += 1

        log.info("Parser stopped.")


# ------------------------------------------------------------------------------
# Thread 3 -- Printer
# ------------------------------------------------------------------------------

class PrinterThread(threading.Thread):
    """Formats and prints each ParsedFrame as a timestamped hex dump."""

    def __init__(self, print_queue: queue.Queue, stop_event: threading.Event):
        super().__init__(name="Printer", daemon=True)
        self._print_queue = print_queue
        self._stop_event  = stop_event

    @staticmethod
    def hex_str(data: bytes) -> str:
        return " ".join(f"{b:02X}" for b in data)

    @staticmethod
    def hdr_label(hdr: bytes) -> str:
        """Human-readable header label, e.g. 0x48('H') or 0x59 0x34."""
        if len(hdr) == 1:
            c = chr(hdr[0])
            label = f"({c})" if 32 <= hdr[0] < 127 else ""
            return f"0x{hdr[0]:02X}{label}"
        return " ".join(f"0x{b:02X}" for b in hdr)

    def run(self):
        log.info("Printer started.")
        while not self._stop_event.is_set():
            try:
                frame: ParsedFrame = self._print_queue.get(timeout=0.1)
            except queue.Empty:
                continue

            cs_label    = "OK " if frame.cs_ok else f"BAD(want {sum(frame.payload) & 0xFF:02X})"
            resync_note = f"  [resynced +{frame.resyncs}B]" if frame.resyncs else ""

            print(
                f"[{frame.timestamp}]"
                f"  HDR={self.hdr_label(frame.header)}"
                f"  N={len(frame.payload)}"
                f"  DATA={self.hex_str(frame.payload)}"
                f"  CS=0x{frame.checksum:02X}[{cs_label}]"
                f"  RAW={self.hex_str(frame.raw)}"
                f"{resync_note}"
            )

        log.info("Printer stopped.")


# ------------------------------------------------------------------------------
# Port helpers
# ------------------------------------------------------------------------------

def list_ports():
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("No serial ports found. Is the USB-serial adapter connected?")
        return []
    print("Available serial ports (macOS: /dev/tty.usbserial-* or /dev/tty.usbmodem-*):")
    for i, p in enumerate(ports):
        print(f"  [{i}] {p.device}  --  {p.description}")
    return ports


def select_port(requested: Optional[str]) -> str:
    if requested:
        return requested
    ports = list_ports()
    if not ports:
        sys.exit(1)
    try:
        idx = int(input("Enter port index: "))
        return ports[idx].device
    except (ValueError, IndexError):
        print("Invalid selection.")
        sys.exit(1)


# ------------------------------------------------------------------------------
# Main
# ------------------------------------------------------------------------------

def main():
    import argparse

    parser = argparse.ArgumentParser(
        description="Multi-threaded serial frame parser with sliding-window re-sync (macOS)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 serial_parser.py --list
  python3 serial_parser.py -p /dev/tty.usbserial-0001 -b 115200
        """
    )
    parser.add_argument("-p", "--port",     default=None,
                        help="Serial port, e.g. /dev/tty.usbserial-0001")
    parser.add_argument("-b", "--baudrate", type=int, default=BAUDRATE,
                        help=f"Baud rate (default: {BAUDRATE})")
    parser.add_argument("-t", "--timeout",  type=float, default=TIMEOUT,
                        help=f"Read timeout in seconds (default: {TIMEOUT})")
    parser.add_argument("--list",           action="store_true",
                        help="List available serial ports and exit")
    args = parser.parse_args()

    if args.list:
        list_ports()
        return

    port = select_port(args.port)

    try:
        ser = serial.Serial(
            port     = port,
            baudrate = args.baudrate,
            bytesize = serial.EIGHTBITS,
            parity   = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            timeout  = args.timeout,
        )
    except serial.SerialException as e:
        log.error("Cannot open %s: %s", port, e)
        sys.exit(1)

    rx_queue    = queue.Queue(maxsize=QUEUE_MAXSIZE)
    print_queue = queue.Queue(maxsize=QUEUE_MAXSIZE)
    stop_event  = threading.Event()

    reader  = SerialReaderThread(ser, rx_queue, stop_event)
    fparser = FrameParserThread(rx_queue, print_queue, HEADER_LENGTH_MAP, stop_event)
    printer = PrinterThread(print_queue, stop_event)

    reader.start()
    fparser.start()
    printer.start()

    print(f"\nRunning on {port} @ {args.baudrate} baud")
    print(f"Known headers:")
    for hdr, n in sorted(HEADER_LENGTH_MAP.items()):
        print(f"  {hdr.hex().upper()}  ->  {n} payload bytes")
    print("Re-sync: enabled (sliding-window CS validation)")
    print("Press Ctrl+C to stop.\n")

    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nShutting down...")
    finally:
        stop_event.set()
        reader.join(timeout=2)
        fparser.join(timeout=2)
        printer.join(timeout=2)
        if ser.is_open:
            ser.close()
        print("Done.")


if __name__ == "__main__":
    main()
