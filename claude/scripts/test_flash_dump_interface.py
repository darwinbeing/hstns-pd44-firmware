#!/usr/bin/env python3
"""
Host-side flash interface replay test using a raw AT45DB021E dump.

This script emulates the firmware-facing AT45DB access patterns used by:
  - flashReadPage6()      (0x4260)
  - flashReadPage7()      (0x427E)
  - flashProgramRead32()  (0x429C)
  - flashPageReadWrite()  (0x41BE, read path)

It validates that command-path results match direct reads from the dump file.
"""

from __future__ import annotations

import argparse
import pathlib
import random
import struct
import sys
from dataclasses import dataclass
from typing import Iterable


def _u16le(buf: bytes, off: int) -> int:
    return struct.unpack_from("<H", buf, off)[0]


def _s16(v: int) -> int:
    return v - 0x10000 if (v & 0x8000) else v


@dataclass
class FlashDump:
    data: bytes
    page_size: int

    def __post_init__(self) -> None:
        if self.page_size <= 0:
            raise ValueError("page_size must be positive")
        if len(self.data) % self.page_size != 0:
            raise ValueError(
                f"dump size {len(self.data)} is not aligned to page_size {self.page_size}"
            )
        self.num_pages = len(self.data) // self.page_size
        self._latched_page = 0

    def _slice(self, page: int, offset: int, length: int) -> bytes:
        if page < 0 or page >= self.num_pages:
            raise IndexError(f"page out of range: {page}")
        if offset < 0 or length < 0:
            raise IndexError("offset/length must be non-negative")
        if offset + length > self.page_size:
            raise IndexError(
                f"page {page} offset/length out of range: off={offset} len={length}"
            )
        base = page * self.page_size + offset
        return self.data[base : base + length]

    # 0x81 path
    def at45dbStartPageRead(self, page_num: int) -> None:
        self._latched_page = page_num

    # 0x84 path
    def at45dbReadFromBuffer(self, length: int, offset: int = 0) -> bytes:
        return self._slice(self._latched_page, offset, length)

    # 0xD2 path
    def at45dbPageProgramRead(self, length: int, page_hi: int, page_lo: int) -> bytes:
        # Firmware uses page_hi/page_lo fields; dump-side replay treats page_hi as page index
        # and page_lo as in-page byte offset.
        return self._slice(page_hi, page_lo, length)


def _check_equal(got: bytes, exp: bytes, label: str) -> None:
    if got != exp:
        raise AssertionError(f"{label}: mismatch (len={len(got)})")


def _flash_verify_checksum(buf: bytes, start: int, end: int, expected: int) -> tuple[int, int]:
    total = sum(buf[start : end + 1]) & 0xFFFF
    mismatch = 0 if ((total + expected) & 0xFF) == 0 else 1
    return total, mismatch


def _iter_cal_regions() -> Iterable[tuple[int, int, int, int]]:
    # (bit_index, start, end, expected_byte_index)
    yield (0, 0x00, 0x06, 0x07)
    yield (1, 0x08, 0x26, 0x27)
    yield (2, 0x28, 0x6E, 0x6F)
    yield (3, 0x70, 0x73, 0x74)
    yield (4, 0x75, 0x8C, 0x73)
    yield (5, 0x8D, 0x90, 0x91)
    yield (6, 0x92, 0x9E, 0x90)
    yield (7, 0x9F, 0xA2, 0xA3)
    yield (8, 0xA4, 0xB0, 0xA2)
    yield (9, 0xB1, 0xB4, 0xB5)
    yield (10, 0xB6, 0xC7, 0xB4)
    yield (11, 0xC8, 0xCB, 0xCC)
    yield (12, 0xCD, 0xFF, 0xCB)


def _run_cal_checksum_page6(flash: FlashDump) -> int:
    page6 = flash._slice(6, 0, 256)
    result_mask = 0
    print("Page6 checksum replay (flashCalibrationLoad / flashVerifyChecksum):")
    for bit, start, end, exp_idx in _iter_cal_regions():
        expected = page6[exp_idx]
        total, mismatch = _flash_verify_checksum(page6, start, end, expected)
        result_mask |= (mismatch & 1) << bit
        status = "FAIL" if mismatch else "OK"
        print(
            f"  bit{bit:02d} [{start:02X}..{end:02X}] + b[{exp_idx:02X}]=0x{expected:02X} "
            f"sum=0x{total:04X} -> {status}"
        )
    print(f"flashChecksumResult(bit12..bit0)=0x{result_mask:04X}")
    return result_mask


def _run_replay_tests(flash: FlashDump, sample_count: int) -> None:
    # flashReadPage6: page 6, 256 bytes via 0x81 + 0x84
    flash.at45dbStartPageRead(6)
    read6 = flash.at45dbReadFromBuffer(256, 0)
    exp6 = flash._slice(6, 0, 256)
    _check_equal(read6, exp6, "flashReadPage6")

    # flashReadPage7: page 7, 32 bytes via 0x81 + 0x84
    flash.at45dbStartPageRead(7)
    read7 = flash.at45dbReadFromBuffer(32, 0)
    exp7 = flash._slice(7, 0, 32)
    _check_equal(read7, exp7, "flashReadPage7")

    # flashProgramRead32: 0xD2 path; compare against direct slice and 0x81/0x84 path.
    rng = random.Random(0x48445354)  # deterministic
    for i in range(sample_count):
        page = rng.randrange(0, flash.num_pages)
        # keep room for 32-byte read
        off = rng.randrange(0, flash.page_size - 32 + 1)
        d2 = flash.at45dbPageProgramRead(32, page, off)
        exp = flash._slice(page, off, 32)
        _check_equal(d2, exp, f"flashProgramRead32 sample#{i} direct")
        flash.at45dbStartPageRead(page)
        seq = flash.at45dbReadFromBuffer(32, off)
        _check_equal(seq, exp, f"flashProgramRead32 sample#{i} seq")

    # flashPageReadWrite read side (0x41BE): page read into 256-byte RAM buffer.
    for page in (0, 1, 5, 6, 7, flash.num_pages - 1):
        flash.at45dbStartPageRead(page)
        got = flash.at45dbReadFromBuffer(256, 0)
        exp = flash._slice(page, 0, 256)
        _check_equal(got, exp, f"flashPageReadWrite page={page}")

    print(f"PASS: flash interface replay matched dump for {sample_count} sampled 32B reads")


def _print_cal_page7(flash: FlashDump) -> None:
    p7 = flash._slice(7, 0, 32)
    cal_va = _u16le(p7, 0x00)
    ofs_va_raw = _u16le(p7, 0x02)
    cal_va2 = _u16le(p7, 0x04)
    ofs_va2_raw = _u16le(p7, 0x06)
    cal_a_gain = _u16le(p7, 0x08)
    cal_a_offset_raw = _u16le(p7, 0x0A)
    cal_pdc5 = _u16le(p7, 0x0C)
    cal_var_1e42 = _u16le(p7, 0x0E)
    cal_vb = _u16le(p7, 0x11)
    ofs_vb_raw = _u16le(p7, 0x13)

    cal = {
        "cal_va(0x1D32)": cal_va,
        "ofs_va_raw(0x1D30_src)": ofs_va_raw,
        "cal_va2(0x1D2E)": cal_va2,
        "ofs_va2_raw(0x1D2C_src)": ofs_va2_raw,
        "cal_a_gain(0x1D26)": cal_a_gain,
        "cal_a_offset_raw(0x1D24_src)": cal_a_offset_raw,
        "cal_pdc5(0x1E44)": cal_pdc5,
        "cal_var_1E42(0x1E42)": cal_var_1e42,
        "cal_vb(0x1D2A)": cal_vb,
        "ofs_vb_raw(0x1D28_src)": ofs_vb_raw,
    }
    print("Page7 calibration words:")
    for k, v in cal.items():
        print(f"  {k}: 0x{v:04X} ({v})")

    # Reconstruct runtime values used in adcMiscSample()
    ofs_va = (_s16(ofs_va_raw) >> 4) + 1
    ofs_va2 = (_s16(ofs_va2_raw) >> 4) + 1
    ofs_vb = (_s16(ofs_vb_raw) >> 4) + 1
    if cal_a_gain == 0x2000 and cal_a_offset_raw == 0:
        cal_a_offset = 0
    else:
        cal_a_offset = int(_s16(cal_a_offset_raw) / 122) + 1

    print("Derived runtime coefficients (adcMiscSample mapping):")
    print(f"  ofs_va(0x1D30): 0x{(ofs_va & 0xFFFF):04X} ({ofs_va})")
    print(f"  ofs_va2(0x1D2C): 0x{(ofs_va2 & 0xFFFF):04X} ({ofs_va2})")
    print(f"  ofs_vb(0x1D28): 0x{(ofs_vb & 0xFFFF):04X} ({ofs_vb})")
    print(f"  cal_a_offset(0x1D24): 0x{(cal_a_offset & 0xFFFF):04X} ({cal_a_offset})")


def main() -> int:
    parser = argparse.ArgumentParser(description="Replay AT45DB flash interface on dumped BIN")
    parser.add_argument(
        "--dump",
        default="AT45DB021E.bin",
        help="path to AT45DB021E dump file (default: AT45DB021E.bin)",
    )
    parser.add_argument(
        "--page-size",
        type=int,
        default=256,
        help="page size in bytes used by replay model (default: 256)",
    )
    parser.add_argument(
        "--samples",
        type=int,
        default=64,
        help="number of random 32-byte sample checks (default: 64)",
    )
    parser.add_argument(
        "--show-cal",
        action="store_true",
        help="print calibration words decoded from page 7",
    )
    parser.add_argument(
        "--check-cal-checksum",
        action="store_true",
        help="replay flashCalibrationLoad page6 checksum verification and print bitmask",
    )
    args = parser.parse_args()

    dump_path = pathlib.Path(args.dump)
    if not dump_path.exists():
        print(f"ERROR: dump file not found: {dump_path}", file=sys.stderr)
        return 2

    data = dump_path.read_bytes()
    flash = FlashDump(data=data, page_size=args.page_size)
    print(
        f"Loaded dump: {dump_path} bytes={len(data)} page_size={flash.page_size} pages={flash.num_pages}"
    )

    _run_replay_tests(flash, sample_count=args.samples)
    if args.show_cal:
        _print_cal_page7(flash)
    if args.check_cal_checksum:
        _run_cal_checksum_page6(flash)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
