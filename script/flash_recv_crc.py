#!/usr/bin/env python3
"""Read AT45DB021E flash dump with sync header + CRC16 (macOS)"""
import sys
import os
import serial
import signal

# Frame format: [0xAA] [0x55] [256 data] [CRC_H] [CRC_L] = 260 bytes

SYNC1 = 0xAA
SYNC2 = 0x55
PAGE_SIZE = 256

data = bytearray()
outfile = ""

def save_and_exit(signum, frame):
    if data and outfile:
        with open(outfile, 'wb') as f:
            f.write(data)
        print(f"\nSaved {len(data)} bytes to {outfile}")
    os._exit(0)

signal.signal(signal.SIGINT, save_and_exit)

def crc16(buf):
    crc = 0
    for b in buf:
        crc ^= b << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) if (crc & 0x8000) else crc << 1
            crc &= 0xFFFF
    return crc

def read_byte(ser):
    while True:
        b = ser.read(1)
        if b:
            return b[0]

def wait_sync(ser):
    """Wait for 0xAA 0x55 sync header"""
    state = 0
    skipped = 0
    while True:
        b = read_byte(ser)
        if state == 0:
            if b == SYNC1:
                state = 1
            else:
                skipped += 1
        elif state == 1:
            if b == SYNC2:
                if skipped > 0:
                    print(f"\n  skipped {skipped} bytes before sync", end='')
                return
            elif b == SYNC1:
                state = 1
                skipped += 1
            else:
                state = 0
                skipped += 2

def main():
    global data, outfile

    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <port> <output.bin> [pages]")
        print(f"  e.g. {sys.argv[0]} /dev/cu.usbserial-xxx flash.bin 1024")
        sys.exit(1)

    port = sys.argv[1]
    outfile = sys.argv[2]
    total_pages = int(sys.argv[3]) if len(sys.argv) > 3 else 1024

    ser = serial.Serial(port, 115200, timeout=0)
    # flush all buffers
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    ser.flushInput()

    errors = 0
    page = 0

    print(f"Receiving {total_pages} pages from {port} ...")
    print(f"Waiting for sync (0xAA 0x55) ... (Ctrl+C to stop)")

    while page < total_pages:
        # find sync header
        wait_sync(ser)

        # read 256 data + 2 CRC
        frame = bytearray()
        while len(frame) < PAGE_SIZE + 2:
            b = ser.read(1)
            if b:
                frame.extend(b)

        page_data = frame[:PAGE_SIZE]
        crc_recv = (frame[PAGE_SIZE] << 8) | frame[PAGE_SIZE + 1]
        crc_calc = crc16(page_data)

        if crc_recv == crc_calc:
            data.extend(page_data)
            status = "OK"
        else:
            data.extend(page_data)
            errors += 1
            status = f"CRC ERR recv=0x{crc_recv:04X} calc=0x{crc_calc:04X}"

        page += 1
        pct = page * 100 // total_pages
        print(f"\r  page {page}/{total_pages} ({pct}%) errors={errors} [{status}]", end='', flush=True)

    ser.close()
    with open(outfile, 'wb') as f:
        f.write(data)
    print(f"\nDone: {len(data)} bytes saved to {outfile}, {errors} CRC errors")

if __name__ == "__main__":
    main()
