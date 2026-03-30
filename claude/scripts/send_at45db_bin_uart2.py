#!/usr/bin/env python3
"""
Send AT45DB binary image to MCU over UART2 with frame format:
  AA 55 + 256-byte payload + CRC16(lo,hi)

MCU responses:
  0x06: page write OK
  0x15: CRC/program fail
  0x42: busy (output active, flash write blocked)
"""

from __future__ import annotations

import argparse
import pathlib
import sys
import time


def crc16_xmodem(data: bytes) -> int:
    crc = 0
    for b in data:
        crc ^= (b << 8)
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc & 0xFFFF


def frame_iter(blob: bytes, pad_byte: int) -> list[bytes]:
    pages = []
    idx = 0
    n = len(blob)
    while idx < n:
        chunk = blob[idx : idx + 256]
        if len(chunk) < 256:
            chunk = chunk + bytes([pad_byte]) * (256 - len(chunk))
        pages.append(chunk)
        idx += 256
    if n == 0:
        pages.append(bytes([pad_byte]) * 256)
    return pages


def main() -> int:
    parser = argparse.ArgumentParser(description="Send AT45DB.bin frames via UART2")
    parser.add_argument("--port", required=True, help="serial port (e.g. /dev/tty.usbserial-xxxx)")
    parser.add_argument("--bin", required=True, help="AT45DB image binary path")
    parser.add_argument("--baud", type=int, default=115200, help="UART baud rate (default: 115200)")
    parser.add_argument("--timeout", type=float, default=1.0, help="serial timeout in seconds")
    parser.add_argument("--retry", type=int, default=20, help="max retries per page")
    parser.add_argument("--busy-wait", type=float, default=0.05, help="wait time after busy ack")
    parser.add_argument("--pad-byte", type=lambda x: int(x, 0), default=0xFF, help="tail pad byte")
    args = parser.parse_args()

    try:
        import serial  # pyserial
    except ImportError:
        print("ERROR: pyserial is required. Install with: pip3 install pyserial", file=sys.stderr)
        return 2

    bin_path = pathlib.Path(args.bin)
    if not bin_path.exists():
        print(f"ERROR: bin file not found: {bin_path}", file=sys.stderr)
        return 2

    blob = bin_path.read_bytes()
    pages = frame_iter(blob, args.pad_byte & 0xFF)
    print(f"Input bytes={len(blob)} pages={len(pages)}")

    ser = serial.Serial(args.port, args.baud, timeout=args.timeout, write_timeout=args.timeout)
    try:
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        t0 = time.time()
        for page_idx, page in enumerate(pages):
            crc = crc16_xmodem(page)
            frame = bytearray()
            frame.append(0xAA)
            frame.append(0x55)
            frame.extend(page)
            frame.append(crc & 0xFF)
            frame.append((crc >> 8) & 0xFF)

            ok = False
            for _ in range(args.retry):
                ser.write(frame)
                ser.flush()
                ack = ser.read(1)

                if ack == b"\x06":
                    ok = True
                    break
                if ack == b"\x42":
                    time.sleep(args.busy_wait)
                    continue
                if ack == b"\x15":
                    continue

            if not ok:
                print(f"ERROR: page {page_idx} failed after {args.retry} retries", file=sys.stderr)
                return 1

            if (page_idx + 1) % 32 == 0 or (page_idx + 1) == len(pages):
                dt = time.time() - t0
                print(f"sent {page_idx + 1}/{len(pages)} pages ({(page_idx + 1) * 256} bytes), {dt:.1f}s")

    finally:
        ser.close()

    print("DONE: all frames acknowledged")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
