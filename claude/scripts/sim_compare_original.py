#!/usr/bin/env python3
import argparse
import csv
import re
import subprocess
import tempfile
from dataclasses import dataclass
from pathlib import Path


DEFAULT_SFRS = [
    "I2C2RCV",
    "I2C2TRN",
    "I2C2CON",
    "I2C2STAT",
    "I2C2ADD",
    "I2C2BRG",
    "IFS3",
    "IEC3",
]

DEFAULT_SYMBOLS = [
    "rxBufIndex",
    "rxPendFlag",
    "rxEventFlags",
    "txCtrlWord",
    "txByteCount",
    "txChecksumAccum",
    "txByteCntPreset",
    "txBusStateFlags",
    "flashCmdFlags",
    "systemFlags",
    "currentLimitFlags",
    "llcStatus",
    "statusFlags",
    "flash_write_offset",
    "flashBlockPageWord",
    "flashBlockLastOffset",
    "flashBlockByteCount",
    "flashUpdateResult",
]

FLASH_CAL_SYMBOLS = [
    "cal_va",
    "ofs_va",
    "cal_va2",
    "ofs_va2",
    "cal_vb",
    "ofs_vb",
    "cal_a_gain",
    "cal_a_offset",
    "cal_pdc5",
    "cal_var_1E42",
]

PMBUS_SEED_VALUES = {
    "rxBufIndex": 0,
    "rxPendFlag": 0,
    "rxEventFlags": 0,
    "txCtrlWord": 0,
    "txByteCount": 0,
    "txChecksumAccum": 0,
    "txByteCntPreset": 0,
    "txBusStateFlags": 0,
    "flashCmdFlags": 0,
    "systemFlags": 0,
    "currentLimitFlags": 0,
    "statusFlags": 0,
    "llcStatus": 1,
    "systemState": 0,
    "eepromCrcShadow": 0,
    "flash_write_offset": 0,
    "flashBlockPageWord": 0,
    "flashBlockLastOffset": 0,
    "flashBlockByteCount": 0,
    "flashUpdateResult": 0,
}

PMBUS_SEED_BYTES = (
    *(("flash_program_scratch", off, 0) for off in range(16)),
    *(("flash_buf_171E", off, 0) for off in range(16)),
    *(("flash_buf_181E", off, 0) for off in range(16)),
    *(("flash_read_buf_15E6", off, 0) for off in range(32)),
    *(("pmbusStringBuf", off, 0) for off in range(9)),
)

PMBUS_BYTE_OBSERVABLES = (
    *(("flash_program_scratch", off) for off in range(4)),
    *(("flash_buf_171E", off) for off in range(4)),
    *(("flash_buf_181E", off) for off in range(4)),
    *(("flash_read_buf_15E6", off) for off in range(21)),
    *(("pmbusStringBuf", off) for off in range(9)),
)

I2C2_SLAVE_ADDR = 0x58
I2C2_WRITE_ADDR = I2C2_SLAVE_ADDR << 1
I2C2_READ_ADDR = I2C2_WRITE_ADDR | 1
SI2C2IF_MASK = 0x0002
I2C_PORTG_IDLE_MASK = 0x000C
I2C_STAT_RW = 0x0004
I2C_STAT_DA = 0x0020

PMBUS_BUS_HANDLER_COVERAGE = {
    "pmbusInitPointerTable": ("sample_0000",),
    "pmbusI2c2Configure": ("sample_0000",),
    "pmbusRxPacketDecode": ("wr_",),
    "pmbusChecksumVerify": ("rd_00",),
    "pmbusEncodeResponse": ("rd_00",),
    "pmbusMultiByteTxSetup": ("multi_byte_setup",),
    "pmbusCommandCodeSupported": ("block_dispatch",),
    "pmbusValidateCommand": ("block_dispatch",),
    "pmbusBlockCommandInvalid": ("block_dispatch",),
    "pmbusWriteCommand": ("block_dispatch",),
    "pmbusCommandDispatcher": ("block_dispatch",),
    "pmbusFlashReadWindow": ("flash_read_window",),
    "pmbusFlashWriteWindow": ("flash_write_window",),
    "pmbusOperationUnlock": ("operation_unlock",),
    "pmbusProgramHeader": ("program_header",),
    "pmbusProgramBlock": ("program_block",),
    "pmbusReadbackBlock": ("readback_block",),
    "pmbusStartFlashWrite": ("start_flash_write",),
    "pmbusReadDeviceInfo": ("read_device_info",),
    "pmbusProgrammingStringByte": ("read_programming_data",),
    "pmbusReadProgrammingString": ("read_programming_data",),
    "pmbusEnterProgramming": ("store_programming_string", "read_programming_string"),
    "pmbusStoreProgrammingData": ("store_programming_data", "read_programming_data"),
    "pmbusReadFlashSummaryBlock": ("block_mode_summary",),
    "pmbusReadFlashDataBlock": ("block_mode_data_2", "block_mode_data_3"),
    "pmbusReadFlashStatusBlock": ("block_mode_status",),
    "pmbusBlockWriteModeSelect": ("block_mode_",),
    "pmbusReadConfigCommand": ("read_config",),
    "pmbusReadVoutCommand": ("read_vout",),
    "pmbusReadIoutCommand": ("read_iout",),
    "pmbusSetOperationMode": ("set_programming_mode_",),
    "pmbusPacketValidate": ("rd_00",),
    "_SI2C2Interrupt": ("addr_w", "wr_", "rd_"),
}


@dataclass(frozen=True)
class PmbusTransaction:
    label: str
    payload: tuple[int, ...]
    read_count: int
    append_pec: bool = True
    pre_writes: tuple[tuple[str, int], ...] = ()
    pre_read_writes: tuple[tuple[str, int], ...] = ()
    pre_byte_writes: tuple[tuple[str, int, int], ...] = ()


def parse_num(raw: str) -> int:
    return int(raw, 0)


def c_div_trunc(a: int, b: int) -> int:
    return int(a / b)


def s16(v: int) -> int:
    return v - 0x10000 if (v & 0x8000) else v


def parse_csv_list(raw: str | None, default: list[str]) -> list[str]:
    if raw is None or raw.strip().lower() == "default":
        return list(default)
    if raw.strip().lower() in ("", "none", "-"):
        return []
    return [s.strip() for s in raw.split(",") if s.strip()]


def byte_observable_name(sym: str, offset: int) -> str:
    return f"{sym}+0x{offset:02X}"


def parse_map_symbols(map_path: Path) -> dict[str, int]:
    symbol_addr: dict[str, int] = {}
    pat = re.compile(r"^\s*0x([0-9A-Fa-f]+)\s+_([A-Za-z0-9_]+)\s*$")
    for line in map_path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = pat.match(line)
        if m:
            symbol_addr[m.group(2)] = int(m.group(1), 16)
    return symbol_addr


def parse_gld_symbols(gld_path: Path) -> dict[str, int]:
    symbol_addr: dict[str, int] = {}
    pat = re.compile(r"^\s*([A-Za-z][A-Za-z0-9_]*)\s*=\s*0x([0-9A-Fa-f]+);")
    for line in gld_path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = pat.match(line)
        if m:
            symbol_addr[m.group(1)] = int(m.group(2), 16)
    return symbol_addr


def parse_variable_metadata(header_path: Path) -> tuple[dict[str, int], dict[str, int]]:
    original_addr: dict[str, int] = {}
    sizes: dict[str, int] = {}

    decl_pat = re.compile(
        r"^\s*extern\s+(?P<type>[^;]+?)\s+"
        r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?:\[[^\]]+\])?\s*;"
        r"\s*/\*\s*0x(?P<addr>[0-9A-Fa-f]+)"
    )
    for line in header_path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = decl_pat.match(line)
        if not m:
            continue
        name = m.group("name")
        typ = m.group("type")
        original_addr[name] = int(m.group("addr"), 16)
        if re.search(r"\b(?:u?int8_t|s8|u8|char)\b", typ):
            sizes[name] = 1
        elif re.search(r"\b(?:u?int32_t|s32|u32)\b", typ):
            sizes[name] = 4
        else:
            sizes[name] = 2

    return original_addr, sizes


def flash_cal_values(dump_path: Path) -> dict[str, int]:
    data = dump_path.read_bytes()
    if len(data) < 8 * 256 or (len(data) % 256) != 0:
        raise ValueError(f"invalid AT45 dump size: {len(data)}")

    p7 = data[7 * 256 : 8 * 256]

    def u16le(off: int) -> int:
        return p7[off] | (p7[off + 1] << 8)

    cal_va = u16le(0x00)
    ofs_va = (s16(u16le(0x02)) >> 4) + 1
    cal_va2 = u16le(0x04)
    ofs_va2 = (s16(u16le(0x06)) >> 4) + 1
    cal_a_gain = u16le(0x08)
    raw_a_off = s16(u16le(0x0A))
    if cal_a_gain == 0x2000 and raw_a_off == 0:
        cal_a_offset = 0
    else:
        cal_a_offset = c_div_trunc(raw_a_off, 122) + 1
    cal_pdc5 = u16le(0x0C)
    cal_var_1e42 = u16le(0x0E)
    cal_vb = u16le(0x11)
    ofs_vb = (s16(u16le(0x13)) >> 4) + 1

    return {
        "cal_va": cal_va & 0xFFFF,
        "ofs_va": ofs_va & 0xFFFF,
        "cal_va2": cal_va2 & 0xFFFF,
        "ofs_va2": ofs_va2 & 0xFFFF,
        "cal_vb": cal_vb & 0xFFFF,
        "ofs_vb": ofs_vb & 0xFFFF,
        "cal_a_gain": cal_a_gain & 0xFFFF,
        "cal_a_offset": cal_a_offset & 0xFFFF,
        "cal_pdc5": cal_pdc5 & 0xFFFF,
        "cal_var_1E42": cal_var_1e42 & 0xFFFF,
    }


def x_format(size: int) -> str:
    if size == 1:
        return "/r1bx"
    if size == 4:
        return "/r1wx"
    return "/r1hx"


def write_format(size: int) -> str:
    if size == 1:
        return "/rb"
    if size == 4:
        return "/rw"
    return "/rh"


def pmbus_pec(payload: list[int] | tuple[int, ...]) -> int:
    return (-(I2C2_WRITE_ADDR + sum(payload))) & 0xFF


def pmbus_payload_with_pec(tx: PmbusTransaction) -> list[int]:
    payload = [b & 0xFF for b in tx.payload]
    if tx.append_pec:
        payload.append(pmbus_pec(payload))
    return payload


def local_additive_checksum(payload: list[int]) -> int:
    return (-sum(payload)) & 0xFF


def block_dispatch_payload(items: list[tuple[int, int]]) -> tuple[int, ...]:
    payload = [0x01]
    for cmd, word in items:
        payload.extend([cmd & 0xFF, word & 0xFF, (word >> 8) & 0xFF])
    return tuple(payload)


def program_block_payload(data: list[int]) -> tuple[int, ...]:
    if not 1 <= len(data) <= 0x10:
        raise ValueError("program block data length must be 1..16")
    payload = [0x3A, len(data), 0x00, 0x00, 0x00]
    payload.extend(b & 0xFF for b in data)
    payload.append(local_additive_checksum(payload[1:]))
    return tuple(payload)


def programming_data_read_seed() -> tuple[tuple[str, int, int], ...]:
    writes: list[tuple[str, int, int]] = []
    for off, value in enumerate(range(5, 21)):
        writes.append(("flash_read_buf_15E6", off, value))
    for off, value in enumerate(range(1, 5), start=0x11):
        writes.append(("flash_read_buf_15E6", off, value))
    return tuple(writes)


def pmbus_suite(name: str) -> list[PmbusTransaction]:
    if name == "none":
        return []
    if name not in ("smoke", "all"):
        raise ValueError(f"unknown PMBus suite: {name}")

    suite = [
        PmbusTransaction("bad_pec_error", (0xE0, 0xE0, 0x00), 1, append_pec=False),
        PmbusTransaction("unsupported_command", (0x99,), 1),
        PmbusTransaction("read_device_info", (0xE0, 0xE0), 11),
        PmbusTransaction("set_programming_mode_on", (0x24, ord("P"), ord("H"), 0x01), 1),
        PmbusTransaction("read_config", (0x21, 0x01), 1),
        PmbusTransaction(
            "read_vout",
            (0x11, 0xAA),
            1,
            pre_writes=(("currentLimitFlags", 0), ("systemState", 0)),
        ),
        PmbusTransaction("read_iout", (0x12, 0x01), 1),
        PmbusTransaction("flash_write_window", (0x23, 0xC8, 0x12, 0x34, 0x56), 1),
        PmbusTransaction("flash_read_window", (0x22, 0xC8, 0x03), 4),
    ]

    if name == "smoke":
        return suite

    suite.extend(
        [
            PmbusTransaction(
                "block_dispatch_clear_group",
                block_dispatch_payload(
                    [
                        (0x05, 0x0000),
                        (0x06, 0x0000),
                        (0x07, 0x0000),
                        (0x13, 0x0000),
                        (0x16, 0x0000),
                        (0x17, 0x0000),
                        (0x23, 0x0000),
                        (0x24, 0x0000),
                        (0x30, 0x0000),
                    ]
                ),
                1,
            ),
            PmbusTransaction(
                "block_dispatch_data_group",
                block_dispatch_payload(
                    [
                        (0x31, 0x0100),
                        (0x32, 0x3000),
                        (0x33, 0x0040),
                        (0x34, 0x0040),
                        (0x35, 0x2000),
                        (0x36, 0x1700),
                        (0x37, 0x0B00),
                        (0x38, 0x0C00),
                        (0x3D, 0x0001),
                    ]
                ),
                1,
            ),
            PmbusTransaction("block_mode_summary", (0x20, 0x00, 0x00), 23),
            PmbusTransaction("block_mode_status", (0x20, 0x01, 0x00), 25),
            PmbusTransaction("block_mode_data_2", (0x20, 0x02, 0x00), 29),
            PmbusTransaction("block_mode_data_3", (0x20, 0x03, 0x00), 30),
            PmbusTransaction(
                "operation_unlock",
                (0xFF, 0x05, 0xA6, 0x02, 0x01, 0x12, 0x34),
                1,
                pre_read_writes=(("eepromCrcShadow", 0x1234),),
            ),
            PmbusTransaction(
                "program_header",
                (0x3C, ord("P"), ord("H")),
                1,
                pre_read_writes=(("systemFlags", 0x0200),),
            ),
            PmbusTransaction(
                "program_block",
                program_block_payload([0xDE, 0xAD, 0xBE, 0xEF]),
                1,
                pre_writes=(("flashCmdFlags", 0), ("statusFlags", 0)),
                pre_read_writes=(("systemFlags", 0x0200),),
            ),
            PmbusTransaction("readback_block", (0x3B, 0x04, 0x00, 0x00), 5),
            PmbusTransaction(
                "readback_block_seeded",
                (0x3B, 0x04, 0x00, 0x00),
                5,
                pre_byte_writes=(
                    ("flash_buf_181E", 0, 0xDE),
                    ("flash_buf_181E", 1, 0xAD),
                    ("flash_buf_181E", 2, 0xBE),
                    ("flash_buf_181E", 3, 0xEF),
                ),
            ),
            PmbusTransaction("store_programming_string", tuple([0xE2] + list(b"PD44TEST!")), 1),
            PmbusTransaction("read_programming_string", (0xE2,), 15),
            PmbusTransaction(
                "store_programming_data",
                tuple([0xE3] + list(range(1, 21))),
                1,
                pre_writes=(("llcStatus", 0x0101), ("flashCmdFlags", 0), ("statusFlags", 0)),
            ),
            PmbusTransaction(
                "read_programming_data",
                (0xE3,),
                21,
                pre_writes=(("llcStatus", 0x0101), ("flashCmdFlags", 0), ("statusFlags", 0)),
                pre_byte_writes=programming_data_read_seed(),
            ),
            PmbusTransaction("start_flash_write", (0x3D,), 1),
            PmbusTransaction("set_programming_mode_off", (0x24, ord("P"), ord("H"), 0x00), 1),
            PmbusTransaction("multi_byte_setup", (0x00,), 4),
        ]
    )
    return suite


def build_mdb_commands(
    *,
    image_path: Path,
    scl_path: Path,
    oscillator_frequency: int,
    warmup_cycles: int,
    init_wait_ms: int,
    sample_wait_ms: int,
    post_halt_wait_ms: int,
    samples: int,
    include_initial: bool,
    labels_prefix: str,
    pmbus_transactions: list[PmbusTransaction],
    sample_symbols: list[str],
    addresses: dict[str, int],
    sizes: dict[str, int],
    injection: dict[str, int],
    byte_injection: tuple[tuple[str, int, int], ...],
) -> tuple[str, list[str]]:
    lines = [
        "Device dsPIC33FJ64GS606",
        "Hwtool SIM",
        f"Set oscillator.frequency {oscillator_frequency}",
        f"Program {image_path}",
        f"stim {scl_path}",
    ]

    for _ in range(warmup_cycles):
        lines.extend(["Run", f"Wait {init_wait_ms}", "Halt", f"Wait {post_halt_wait_ms}"])

    for sym, value in injection.items():
        lines.append(f"write {write_format(sizes.get(sym, 2))} 0x{addresses[sym]:04X} 0x{value:04X}")
    for sym, offset, value in byte_injection:
        lines.append(f"write /rb 0x{addresses[sym] + offset:04X} 0x{value & 0xFF:02X}")

    sample_labels: list[str] = []

    def append_sample(label: str) -> None:
        sample_labels.append(label)
        for sym in sample_symbols:
            addr = addresses[sym]
            lines.append(f"x {x_format(sizes.get(sym, 2))} 0x{addr:04X}")

    def append_i2c_event(label: str, stat: int, rcv: int) -> None:
        lines.append(f"write /rh 0x{addresses['PORTG']:04X} 0x{I2C_PORTG_IDLE_MASK:04X}")
        lines.append(f"write /rh 0x{addresses['I2C2STAT']:04X} 0x{stat & 0xFFFF:04X}")
        lines.append(f"write /rh 0x{addresses['I2C2RCV']:04X} 0x{rcv & 0xFFFF:04X}")
        lines.append(f"write /rh 0x{addresses['IFS3']:04X} 0x{SI2C2IF_MASK:04X}")
        lines.extend(["Run", f"Wait {sample_wait_ms}", "Halt", f"Wait {post_halt_wait_ms}"])
        append_sample(label)

    def append_symbol_writes(writes: tuple[tuple[str, int], ...]) -> None:
        for sym, value in writes:
            lines.append(f"write {write_format(sizes.get(sym, 2))} 0x{addresses[sym]:04X} 0x{value:04X}")

    def append_byte_writes(writes: tuple[tuple[str, int, int], ...]) -> None:
        for sym, offset, value in writes:
            lines.append(f"write /rb 0x{addresses[sym] + offset:04X} 0x{value & 0xFF:02X}")

    total = samples + (1 if include_initial else 0)
    for sample_idx in range(total):
        wait_ms = init_wait_ms if include_initial and sample_idx == 0 else sample_wait_ms
        lines.extend(["Run", f"Wait {wait_ms}", "Halt", f"Wait {post_halt_wait_ms}"])
        append_sample(f"{labels_prefix}:sample_{sample_idx:04d}")

    for tx in pmbus_transactions:
        payload = pmbus_payload_with_pec(tx)
        append_symbol_writes(tx.pre_writes)
        append_i2c_event(f"{labels_prefix}:{tx.label}:addr_w", 0, I2C2_WRITE_ADDR)
        for idx, byte in enumerate(payload):
            append_i2c_event(
                f"{labels_prefix}:{tx.label}:wr_{idx:02d}_0x{byte:02X}",
                I2C_STAT_DA,
                byte,
            )
        append_symbol_writes(tx.pre_read_writes)
        append_byte_writes(tx.pre_byte_writes)
        for idx in range(tx.read_count):
            stat = I2C_STAT_RW if idx == 0 else (I2C_STAT_RW | I2C_STAT_DA)
            append_i2c_event(f"{labels_prefix}:{tx.label}:rd_{idx:02d}", stat, I2C2_READ_ADDR)

    lines.append("Quit")
    return "\n".join(lines) + "\n", sample_labels


def run_mdb(mdb_path: Path, commands: str, log_path: Path) -> None:
    with tempfile.NamedTemporaryFile("w", encoding="utf-8", delete=False) as f:
        f.write(commands)
        cmd_path = Path(f.name)
    try:
        with log_path.open("w", encoding="utf-8") as log:
            subprocess.run([str(mdb_path), str(cmd_path)], stdout=log, stderr=subprocess.STDOUT, check=False)
    finally:
        cmd_path.unlink(missing_ok=True)


def parse_x_log(log_path: Path, symbols: list[str], addresses: dict[str, int], sizes: dict[str, int]) -> list[dict[str, int]]:
    lines = log_path.read_text(encoding="utf-8", errors="ignore").splitlines()
    commands: list[str] = []
    for sym in symbols:
        commands.append(f"x {x_format(sizes.get(sym, 2))} 0x{addresses[sym]:04X}")

    series = {sym: [] for sym in symbols}
    cmd_to_symbol = dict(zip(commands, symbols))
    i = 0
    hex_line_pat = re.compile(r"^\s*([0-9A-Fa-f]{2,8})(?:\s|$)")
    while i < len(lines):
        line = lines[i].strip()
        sym = cmd_to_symbol.get(line)
        if sym is None:
            i += 1
            continue

        i += 1
        while i < len(lines):
            m = hex_line_pat.match(lines[i])
            if m:
                token = m.group(1)
                series[sym].append(int(token, 16))
                break
            if lines[i].strip().startswith("x "):
                break
            i += 1
        i += 1

    count = min((len(vals) for vals in series.values()), default=0)
    if count == 0:
        raise ValueError(f"no complete memory samples found in {log_path}")

    return [{sym: series[sym][idx] for sym in symbols} for idx in range(count)]


def write_csv(path: Path, rows: list[dict[str, int]], symbols: list[str], labels: list[str]) -> None:
    with path.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=["sample", "label"] + symbols)
        writer.writeheader()
        for idx, row in enumerate(rows):
            out = {"sample": idx, "label": labels[idx] if idx < len(labels) else ""}
            out.update(row)
            writer.writerow(out)


def compare_rows(
    decompiled: list[dict[str, int]],
    original: list[dict[str, int]],
    symbols: list[str],
    labels: list[str],
    max_mismatches: int,
) -> tuple[int, list[str]]:
    count = min(len(decompiled), len(original))
    messages: list[str] = []
    mismatches = 0
    if len(decompiled) != len(original):
        messages.append(f"sample count differs: decompiled={len(decompiled)} original={len(original)}")
        mismatches += abs(len(decompiled) - len(original))

    for idx in range(count):
        label = labels[idx] if idx < len(labels) else f"sample_{idx}"
        for sym in symbols:
            if sym == "I2C2TRN" and ":rd_" not in label:
                continue
            lhs = decompiled[idx][sym]
            rhs = original[idx][sym]
            if lhs == rhs:
                continue
            mismatches += 1
            if len(messages) < max_mismatches:
                messages.append(
                    f"sample={idx} label={label} {sym}: decompiled=0x{lhs:04X} original=0x{rhs:04X}"
                )
    return mismatches, messages


def coverage_lines(labels: list[str], enabled: bool) -> tuple[list[str], list[str]]:
    if not enabled:
        return ["PMBus bus-handler coverage disabled: pmbus_suite=none"], []

    lines: list[str] = []
    missing: list[str] = []
    for handler, needles in PMBUS_BUS_HANDLER_COVERAGE.items():
        matched = [
            label
            for label in labels
            if any(needle in label for needle in needles)
        ]
        if matched:
            rd_match = next((label for label in matched if ":rd_" in label), matched[0])
            lines.append(f"{handler}: {rd_match}")
        else:
            lines.append(f"{handler}: MISSING")
            missing.append(handler)
    return lines, missing


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description="Compare decompiled firmware against original HEX in MPLAB SIM.")
    parser.add_argument("--project-dir", default=str(root / "HSTNS-PD44.X"))
    parser.add_argument("--original-hex", default=str(root / "DSPIC33FJ64GS606.hex"))
    parser.add_argument("--scl", default=str(root / "scripts/stimulus/hstns_startup_llc.scl"))
    parser.add_argument("--out-dir", default=str(root / "out/sim_compare_original"))
    parser.add_argument("--mdb", default="/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh")
    parser.add_argument("--gld", default="/Applications/microchip/xc16/v2.10/support/dsPIC33F/gld/p33FJ64GS606.gld")
    parser.add_argument("--sfrs", default="default")
    parser.add_argument("--symbols", default="none")
    parser.add_argument("--pmbus-suite", choices=("none", "smoke", "all"), default="all")
    parser.add_argument("--pmbus-byte-observables", type=int, default=1)
    parser.add_argument("--samples", type=int, default=20)
    parser.add_argument("--sample-wait-ms", type=int, default=2)
    parser.add_argument("--init-wait-ms", type=int, default=1000)
    parser.add_argument("--post-halt-wait-ms", type=int, default=1)
    parser.add_argument("--warmup-cycles", type=int, default=1)
    parser.add_argument("--include-initial", type=int, default=1)
    parser.add_argument("--oscillator-frequency", type=int, default=50000000)
    parser.add_argument("--flash-dump", default="")
    parser.add_argument("--no-rebuild", action="store_true")
    parser.add_argument("--max-mismatches", type=int, default=40)
    args = parser.parse_args()

    project_dir = Path(args.project_dir)
    original_hex = Path(args.original_hex)
    scl_path = Path(args.scl)
    out_dir = Path(args.out_dir)
    mdb_path = Path(args.mdb)
    gld_path = Path(args.gld)
    variables_h = root / "decompiled/variables.h"
    debug_elf = project_dir / "dist/default/debug/HSTNS-PD44.X.debug.elf"
    debug_map = project_dir / "dist/default/debug/HSTNS-PD44.X.debug.map"

    if not args.no_rebuild:
        subprocess.run(
            ["make", "TYPE_IMAGE=DEBUG_RUN", "CONF=default", "-C", str(project_dir)],
            check=True,
        )

    for path in (original_hex, scl_path, mdb_path, gld_path, debug_elf, debug_map, variables_h):
        if not path.exists():
            raise SystemExit(f"required file not found: {path}")

    sfrs = parse_csv_list(args.sfrs, DEFAULT_SFRS)
    symbols = parse_csv_list(args.symbols, DEFAULT_SYMBOLS)
    pmbus_transactions = pmbus_suite(args.pmbus_suite)
    pmbus_byte_observables = (
        PMBUS_BYTE_OBSERVABLES if pmbus_transactions and args.pmbus_byte_observables else ()
    )

    decompiled_map = parse_map_symbols(debug_map)
    sfr_map = parse_gld_symbols(gld_path)
    original_map, sizes = parse_variable_metadata(variables_h)

    missing_sfrs = [s for s in sfrs if s not in sfr_map]
    missing_decompiled = [s for s in symbols if s not in decompiled_map]
    missing_original = [s for s in symbols if s not in original_map]
    pmbus_required_sfrs = ["I2C2RCV", "I2C2STAT", "IFS3", "PORTG"]
    missing_pmbus_sfrs = [s for s in pmbus_required_sfrs if s not in sfr_map]
    if missing_sfrs or missing_decompiled or missing_original or missing_pmbus_sfrs:
        if missing_sfrs:
            print("missing SFR addresses: " + ", ".join(missing_sfrs))
        if missing_decompiled:
            print("missing decompiled map symbols: " + ", ".join(missing_decompiled))
        if missing_original:
            print("missing original RAM addresses: " + ", ".join(missing_original))
        if missing_pmbus_sfrs:
            print("missing PMBus control SFR addresses: " + ", ".join(missing_pmbus_sfrs))
        return 2

    injection_values: dict[str, int] = {}
    if args.flash_dump:
        injection_values = flash_cal_values(Path(args.flash_dump))
        missing_injection = [
            s for s in FLASH_CAL_SYMBOLS if s not in decompiled_map or s not in original_map
        ]
        if missing_injection:
            print("missing calibration injection symbols: " + ", ".join(missing_injection))
            return 2

    if pmbus_transactions:
        pmbus_write_symbols = set(PMBUS_SEED_VALUES)
        pmbus_write_symbols.update(sym for sym, _, _ in PMBUS_SEED_BYTES)
        pmbus_write_symbols.update(sym for sym, _ in pmbus_byte_observables)
        for tx in pmbus_transactions:
            pmbus_write_symbols.update(sym for sym, _ in tx.pre_writes)
            pmbus_write_symbols.update(sym for sym, _ in tx.pre_read_writes)
            pmbus_write_symbols.update(sym for sym, _, _ in tx.pre_byte_writes)
        for sym in PMBUS_SEED_VALUES:
            if sym not in decompiled_map or sym not in original_map:
                print(f"missing PMBus seed symbol: {sym}")
                return 2
        for sym in sorted(pmbus_write_symbols):
            if sym not in decompiled_map or sym not in original_map:
                print(f"missing PMBus write symbol: {sym}")
                return 2
    else:
        pmbus_write_symbols = set()

    out_dir.mkdir(parents=True, exist_ok=True)
    decompiled_log = out_dir / "decompiled_mdb.log"
    original_log = out_dir / "original_mdb.log"
    decompiled_csv = out_dir / "decompiled_trace.csv"
    original_csv = out_dir / "original_trace.csv"
    report_path = out_dir / "compare_report.txt"
    coverage_path = out_dir / "pmbus_coverage.txt"

    byte_observable_names = [
        byte_observable_name(sym, offset) for sym, offset in pmbus_byte_observables
    ]
    observable_names = list(dict.fromkeys(sfrs + symbols + byte_observable_names))
    if not observable_names:
        print("no observables selected")
        return 2

    observable_sizes = {s: 2 for s in sfrs}
    observable_sizes.update({s: sizes.get(s, 2) for s in symbols})
    observable_sizes.update({s: 1 for s in byte_observable_names})

    decompiled_addresses = {s: sfr_map[s] for s in sfrs}
    decompiled_addresses.update({s: decompiled_map[s] for s in symbols})
    decompiled_addresses.update(
        {
            byte_observable_name(sym, offset): decompiled_map[sym] + offset
            for sym, offset in pmbus_byte_observables
        }
    )
    original_addresses = {s: sfr_map[s] for s in sfrs}
    original_addresses.update({s: original_map[s] for s in symbols})
    original_addresses.update(
        {
            byte_observable_name(sym, offset): original_map[sym] + offset
            for sym, offset in pmbus_byte_observables
        }
    )

    control_sfr_addresses = {s: sfr_map[s] for s in pmbus_required_sfrs}
    decompiled_run_addresses = {**decompiled_addresses, **control_sfr_addresses}
    original_run_addresses = {**original_addresses, **control_sfr_addresses}

    decompiled_injection = {s: injection_values[s] for s in injection_values}
    original_injection = {s: injection_values[s] for s in injection_values}
    if pmbus_transactions:
        decompiled_injection.update(PMBUS_SEED_VALUES)
        original_injection.update(PMBUS_SEED_VALUES)
        pmbus_byte_injection = PMBUS_SEED_BYTES
    else:
        pmbus_byte_injection = ()

    decompiled_injection_addresses = {s: decompiled_map[s] for s in decompiled_injection}
    original_injection_addresses = {s: original_map[s] for s in original_injection}
    decompiled_pmbus_write_addresses = {s: decompiled_map[s] for s in pmbus_write_symbols}
    original_pmbus_write_addresses = {s: original_map[s] for s in pmbus_write_symbols}

    print(f"running decompiled ELF: {debug_elf}")
    decompiled_cmds, decompiled_labels = build_mdb_commands(
        image_path=debug_elf,
        scl_path=scl_path,
        oscillator_frequency=args.oscillator_frequency,
        warmup_cycles=args.warmup_cycles,
        init_wait_ms=args.init_wait_ms,
        sample_wait_ms=args.sample_wait_ms,
        post_halt_wait_ms=args.post_halt_wait_ms,
        samples=args.samples,
        include_initial=args.include_initial != 0,
        labels_prefix="decompiled",
        pmbus_transactions=pmbus_transactions,
        sample_symbols=observable_names,
        addresses={
            **decompiled_run_addresses,
            **decompiled_injection_addresses,
            **decompiled_pmbus_write_addresses,
        },
        sizes={**observable_sizes, **sizes},
        injection=decompiled_injection,
        byte_injection=pmbus_byte_injection,
    )
    run_mdb(
        mdb_path,
        decompiled_cmds,
        decompiled_log,
    )

    print(f"running original HEX: {original_hex}")
    original_cmds, original_labels = build_mdb_commands(
        image_path=original_hex,
        scl_path=scl_path,
        oscillator_frequency=args.oscillator_frequency,
        warmup_cycles=args.warmup_cycles,
        init_wait_ms=args.init_wait_ms,
        sample_wait_ms=args.sample_wait_ms,
        post_halt_wait_ms=args.post_halt_wait_ms,
        samples=args.samples,
        include_initial=args.include_initial != 0,
        labels_prefix="original",
        pmbus_transactions=pmbus_transactions,
        sample_symbols=observable_names,
        addresses={
            **original_run_addresses,
            **original_injection_addresses,
            **original_pmbus_write_addresses,
        },
        sizes={**observable_sizes, **sizes},
        injection=original_injection,
        byte_injection=pmbus_byte_injection,
    )
    run_mdb(
        mdb_path,
        original_cmds,
        original_log,
    )

    labels = [label.split(":", 1)[1] for label in decompiled_labels]
    if len(decompiled_labels) != len(original_labels):
        print(
            f"internal label count mismatch: decompiled={len(decompiled_labels)} "
            f"original={len(original_labels)}"
        )
        return 2

    decompiled_rows = parse_x_log(
        decompiled_log,
        observable_names,
        decompiled_addresses,
        observable_sizes,
    )
    original_rows = parse_x_log(
        original_log,
        observable_names,
        original_addresses,
        observable_sizes,
    )
    write_csv(decompiled_csv, decompiled_rows, observable_names, labels)
    write_csv(original_csv, original_rows, observable_names, labels)

    mismatches, messages = compare_rows(
        decompiled_rows,
        original_rows,
        observable_names,
        labels,
        args.max_mismatches,
    )
    coverage, missing_coverage = coverage_lines(labels, bool(pmbus_transactions))
    if args.pmbus_suite != "all":
        missing_coverage = []
    if missing_coverage:
        messages = [f"PMBus coverage missing: {', '.join(missing_coverage)}"] + messages

    with report_path.open("w", encoding="utf-8") as f:
        f.write(f"scl={scl_path}\n")
        f.write(f"pmbus_suite={args.pmbus_suite}\n")
        f.write(f"samples_decompiled={len(decompiled_rows)}\n")
        f.write(f"samples_original={len(original_rows)}\n")
        f.write(f"sfrs={','.join(sfrs)}\n")
        f.write(f"symbols={','.join(symbols)}\n")
        f.write(f"pmbus_byte_observables={','.join(byte_observable_names)}\n")
        f.write(f"mismatches={mismatches}\n")
        for msg in messages:
            f.write(msg + "\n")
    with coverage_path.open("w", encoding="utf-8") as f:
        f.write(f"pmbus_suite={args.pmbus_suite}\n")
        for line in coverage:
            f.write(line + "\n")

    print(
        f"SIM compare: decompiled_samples={len(decompiled_rows)} "
        f"original_samples={len(original_rows)} mismatches={mismatches}"
    )
    print(
        f"Trace: decompiled={decompiled_csv} original={original_csv} "
        f"report={report_path} coverage={coverage_path}"
    )
    if messages:
        for msg in messages:
            print(msg)

    return 1 if mismatches or missing_coverage else 0


if __name__ == "__main__":
    raise SystemExit(main())
