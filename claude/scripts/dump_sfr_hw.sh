#!/bin/zsh
# ============================================================================
# dump_sfr_hw.sh — Read SFR/RAM from real hardware via PICkit 5
#
# Modes:
#   ./scripts/dump_sfr_hw.sh               — Program hex + read config/program memory
#   ./scripts/dump_sfr_hw.sh --no-program   — Attach only, read config/program memory
#   ./scripts/dump_sfr_hw.sh --debug        — Debug mode: build decompiled C project,
#                                             run with debug executive, halt, read SFRs
# ============================================================================
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
HEX_FILE="$ROOT_DIR/DSPIC33FJ64GS606.hex"
MDB_SH="/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh"
OUTPUT_DIR="$ROOT_DIR/out/hw_dump"
LOG_FILE="$OUTPUT_DIR/sfr_dump.txt"
PARSED_CSV="$OUTPUT_DIR/sfr_parsed.csv"

MODE="program"   # program | no-program | debug
if [[ "${1:-}" == "--no-program" ]]; then
    MODE="no-program"
elif [[ "${1:-}" == "--debug" ]]; then
    MODE="debug"
fi

mkdir -p "$OUTPUT_DIR"

MDB_CMD_FILE="$(mktemp "${TMPDIR:-/tmp}/hw_dump_mdb.XXXXXX")"
trap 'rm -f "$MDB_CMD_FILE"' EXIT

# ---- Build MDB commands ----
{
    echo "Device dsPIC33FJ64GS606"
    echo "Set programoptions.pgmspeed.pk5 Slow"
    echo "Set poweroptions.powerenable true"
    echo "Set voltagevalue 3.25"
    echo "Hwtool PICkit5 -p"

    if [[ "$MODE" == "debug" ]]; then
        # Debug mode: build the decompiled C project with debug executive,
        # program it, run, halt, read SFRs
        PROJECT_DIR="$ROOT_DIR/HSTNS-PD44.X"
        ELF_PATH="$PROJECT_DIR/dist/default/debug/HSTNS-PD44.X.debug.elf"

        # Build the project first (outside MDB)
        echo "# Debug mode: using ELF with debug executive"
        echo "Program \"$ELF_PATH\""
        echo "Run"
        echo "Sleep 3000"
        echo "Halt"
        echo "Sleep 500"

        # ---- SFR reads (only work in debug mode) ----
        # I2C2 (critical: SCLREL verification)
        echo "x /r5xh 0x0216"
        # UART1
        echo "x /r5xh 0x0220"
        # SPI2
        echo "x /r4xh 0x0260"
        # GPIO F, G
        echo "x /r4xh 0x02C0"
        echo "x /r4xh 0x02F0"
        # GPIO D
        echo "x /r4xh 0x02A8"
        # PWM
        echo "x /r3xh 0x0400"
        echo "x /r1xh 0x0418"
        echo "x /r1xh 0x041A"
        echo "x /r1xh 0x0420"
        echo "x /r1xh 0x0422"
        echo "x /r1xh 0x0424"
        echo "x /r1xh 0x0426"
        echo "x /r1xh 0x0438"
        echo "x /r1xh 0x043A"
        echo "x /r1xh 0x0440"
        echo "x /r1xh 0x0442"
        echo "x /r1xh 0x0458"
        echo "x /r1xh 0x045A"
        echo "x /r1xh 0x0460"
        echo "x /r1xh 0x0462"
        echo "x /r1xh 0x0464"
        echo "x /r1xh 0x0466"
        # Comparators
        echo "x /r3xh 0x0A00"
        echo "x /r3xh 0x0A10"
        echo "x /r3xh 0x0A20"
        echo "x /r3xh 0x0A30"
        # ADC
        echo "x /r1xh 0x0300"
        echo "x /r16xh 0x0380"
        # Timers
        echo "x /r8xh 0x0100"
        # OC2RS
        echo "x /r1xh 0x018C"
        # Interrupts
        echo "x /r8xh 0x0080"

        # ---- RAM variable reads ----
        echo "x /r6xh 0x1E18"
        echo "x /r1xh 0x125A"
        echo "x /r1xh 0x1264"
        echo "x /r1xh 0x126A"
        echo "x /r1xh 0x1266"
        echo "x /r1xh 0x1268"
        echo "x /r9xh 0x191E"
        echo "x /r1xh 0x1606"
        echo "x /r1xh 0x1388"
        echo "x /r1xh 0x138C"
        echo "x /r1xh 0x1390"
        echo "x /r1xh 0x1292"
        echo "x /r1xh 0x1DA4"
        echo "x /r1xh 0x1D4C"
        echo "x /r1xh 0x1D4A"
        echo "x /r1xh 0x1DA0"
        echo "x /r1xh 0x1D74"
        echo "x /r4xh 0x1D6A"
        echo "x /r8xh 0x1D82"
        echo "x /r1xh 0x27FE"
        echo "x /r1xh 0x1E10"
        echo "x /r1xh 0x1E12"
        echo "x /r1xh 0x1E16"
        echo "x /r1xh 0x1D3E"
        echo "x /r1xh 0x1E38"
    else
        # Non-debug mode: program hex + read config/program memory
        if [[ "$MODE" == "program" ]]; then
            echo "Program \"$HEX_FILE\""
        fi

        # Read configuration words (readable even without debug)
        echo "x /c8xh 0xF80000"

        # Read program memory at key init function addresses
        # initOscillator: 0x5500
        echo "x /p16xw 0x5500"
        # initUart: 0x556A
        echo "x /p16xw 0x556A"
        # initI2C2: 0x5116
        echo "x /p8xw 0x5116"
        # initSPI2: 0x514E
        echo "x /p8xw 0x514E"
        # flashPageProgramRead: 0x41A2
        echo "x /p16xw 0x41A2"
        # flashPageReadWrite: 0x41BE
        echo "x /p16xw 0x41BE"
        # BSET 0x217, #4 instruction at various addresses
        echo "x /p1xw 0x41B8"
        echo "x /p1xw 0x41DA"
        echo "x /p1xw 0x4278"
        echo "x /p1xw 0x4296"
        echo "x /p1xw 0x42AA"
    fi

    echo "Quit"
} > "$MDB_CMD_FILE"

if [[ "$MODE" == "debug" ]]; then
    # Build the decompiled C project in debug mode first
    PROJECT_DIR="$ROOT_DIR/HSTNS-PD44.X"
    ELF_PATH="$PROJECT_DIR/dist/default/debug/HSTNS-PD44.X.debug.elf"
    echo "Building project in debug mode..."
    make TYPE_IMAGE=DEBUG_RUN CONF=default -B -C "$PROJECT_DIR" 2>&1 | tail -5
    if [[ ! -f "$ELF_PATH" ]]; then
        echo "Error: ELF not found at $ELF_PATH" >&2
        exit 1
    fi
    echo "ELF built: $ELF_PATH"
fi

echo "Running MDB with PICkit5 (mode=$MODE)..."
"$MDB_SH" "$MDB_CMD_FILE" > "$LOG_FILE" 2>&1 || true
echo "Raw MDB log: $LOG_FILE"

# ---- Parse results ----
python3 - <<'PYEOF' "$LOG_FILE" "$PARSED_CSV" "$MODE"
import re
import sys
import csv

log_path, csv_path, mode = sys.argv[1], sys.argv[2], sys.argv[3]

with open(log_path, encoding="utf-8", errors="ignore") as f:
    text = f.read()

text_clean = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)

# ---- Parse x command output ----
# Format: "x /r5xh 0x0216\n<values>"  or  "x /p8xw 0x5116\n<values>"
# Values: "XXXX   XXXX   XXXX   ..."  (hex, no 0x prefix in output)
results = []  # list of (mem_type, addr, [values])

lines = text_clean.splitlines()
i = 0
while i < len(lines):
    line = lines[i].strip()
    m = re.match(r'^x\s+/([rpce])(\d+)x([hbw])\s+0x([0-9A-Fa-f]+)', line)
    if m:
        mem_type = m.group(1)
        count = int(m.group(2))
        unit = m.group(3)  # h=halfword(2B), w=word(4B), b=byte
        base_addr = int(m.group(4), 16)
        unit_size = {"b": 1, "h": 2, "w": 4}[unit]

        # Next non-empty, non-error line should have the values
        j = i + 1
        vals = []
        while j < len(lines):
            vline = lines[j].strip()
            if vline.startswith("The target device") or vline == "":
                j += 1
                continue
            # Parse hex values (no 0x prefix)
            vals = re.findall(r'([0-9A-Fa-f]+)', vline)
            break
        if vals:
            results.append((mem_type, base_addr, unit_size, [int(v, 16) for v in vals]))
    i += 1

# ---- Print report ----
print(f"\n{'='*72}")
print(f"  Hardware Dump — mode={mode}")
print(f"{'='*72}")

# Known names
sfr_names = {
    0x0080: "IFS0", 0x0082: "IFS1", 0x0084: "IFS2", 0x0086: "IFS3",
    0x0088: "IEC0", 0x008A: "IEC1", 0x008C: "IEC2", 0x008E: "IEC3",
    0x0100: "T1CON", 0x0102: "TMR1", 0x0104: "PR1",
    0x0106: "T2CON", 0x0108: "TMR2", 0x010A: "PR2",
    0x018C: "OC2RS",
    0x0206: "I2C1CON", 0x0208: "I2C1STAT", 0x020A: "I2C1ADD",
    0x020C: "I2C1MSK", 0x020E: "I2C1BRG",
    0x0216: "I2C2CON", 0x0218: "I2C2STAT", 0x021A: "I2C2ADD",
    0x021C: "I2C2MSK", 0x021E: "I2C2BRG",
    0x0220: "U1MODE", 0x0222: "U1STA", 0x0224: "U1TXREG",
    0x0226: "U1RXREG", 0x0228: "U1BRG",
    0x0260: "SPI2STAT", 0x0262: "SPI2CON1", 0x0264: "SPI2CON2", 0x0266: "SPI2BUF",
    0x0300: "ADCON",
    0x02A8: "TRISD", 0x02AA: "PORTD", 0x02AC: "LATD", 0x02AE: "ODCD",
    0x02C0: "TRISF", 0x02C2: "PORTF", 0x02C4: "LATF", 0x02C6: "ODCF",
    0x02F0: "TRISG", 0x02F2: "PORTG", 0x02F4: "LATG", 0x02F6: "ODCG",
    0x0400: "PTCON", 0x0402: "PTCON2", 0x0404: "PTPER",
    0x0418: "PWMCON1", 0x041A: "IOCON1", 0x0420: "PDC1", 0x0422: "PHASE1",
    0x0424: "DTR1", 0x0426: "ALTDTR1",
    0x0438: "PWMCON2", 0x043A: "IOCON2", 0x0440: "PDC2", 0x0442: "PHASE2",
    0x0458: "PWMCON3", 0x045A: "IOCON3", 0x0460: "PDC3", 0x0462: "PHASE3",
    0x0464: "DTR3", 0x0466: "ALTDTR3",
    0x0A00: "CMPCON1", 0x0A02: "CMPDAC1", 0x0A04: "CMPFILT1",
    0x0A10: "CMPCON2", 0x0A12: "CMPDAC2", 0x0A14: "CMPFILT2",
    0x0A20: "CMPCON3", 0x0A22: "CMPDAC3", 0x0A24: "CMPFILT3",
    0x0A30: "CMPCON4", 0x0A32: "CMPDAC4", 0x0A34: "CMPFILT4",
    # RAM
    0x1E18: "currentLimitFlags", 0x1E1A: "systemFlags", 0x1E1C: "auxFlags",
    0x1E1E: "RAM_1E1E", 0x1E20: "controlStatus", 0x1E22: "systemState",
    0x125A: "statusFlags", 0x1264: "protectionStatus", 0x126A: "runtimeFlags",
    0x1266: "thermalFlags", 0x1268: "droopMode",
    0x1928: "flashCmdFlags", 0x191E: "flash_write_offset",
    0x1606: "flash_page_addr", 0x1388: "flash_crc_lo", 0x138C: "flash_crc_hi",
    0x1390: "flash_page_count", 0x1292: "at45db_status",
    0x1DA4: "llcPeriodCmd", 0x1D4C: "voutSetpoint", 0x1D4A: "voutRefTarget",
    0x1DA0: "vout_sum", 0x1D74: "ptperCommand",
    0x1D6A: "dtr3_shadow", 0x1D6C: "pdc3_shadow", 0x1D6E: "pdc2_shadow", 0x1D70: "pdc1_shadow",
    0x27FE: "fw_mode",
    0x1E10: "marginThreshold", 0x1E12: "ioutLimitLo", 0x1E16: "ioutLimitHi",
    0x1D3E: "freqSetpoint", 0x1E38: "oc2rs_shadow",
}

# Build flat addr->value map for SFR/RAM reads
flat = {}
csv_rows = []

for mem_type, base_addr, unit_size, vals in results:
    for idx, v in enumerate(vals):
        addr = base_addr + idx * unit_size
        flat[addr] = v
        name = sfr_names.get(addr, f"{'PGM' if mem_type == 'p' else 'CFG' if mem_type == 'c' else 'REG'}_0x{addr:04X}")
        if mem_type in ('r',):
            fmt = f"0x{v:04X}"
            bfmt = f"{v:016b}"
        else:
            fmt = f"0x{v:06X}" if unit_size == 4 else f"0x{v:04X}"
            bfmt = f"{v:024b}" if unit_size == 4 else f"{v:016b}"
        csv_rows.append((f"0x{addr:04X}" if addr < 0x10000 else f"0x{addr:06X}",
                         mem_type, name, fmt, v, bfmt))

with open(csv_path, "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f)
    w.writerow(["address", "mem_type", "name", "hex_value", "dec_value", "binary"])
    for row in csv_rows:
        w.writerow(row)

if mode == "debug":
    # Print SFR results
    for mem_type, base_addr, unit_size, vals in results:
        if mem_type != 'r':
            continue
        for idx, v in enumerate(vals):
            addr = base_addr + idx * unit_size
            name = sfr_names.get(addr, f"0x{addr:04X}")
            print(f"  0x{addr:04X}  {name:<20}  0x{v:04X}  ({v:5d})  {v:016b}")
            # Special bit decode for key registers
            if addr == 0x0216:  # I2C2CON
                print(f"           I2CEN={v>>15 & 1}  SCLREL={v>>12 & 1}  STREN={v>>6 & 1}")
            elif addr == 0x0222:  # U1STA
                print(f"           UTXEN={v>>10 & 1}  TRMT={v>>8 & 1}  URXDA={v>>1 & 1}")
            elif addr == 0x0220:  # U1MODE
                print(f"           UARTEN={v>>15 & 1}  BRGH={v>>3 & 1}")
            elif addr == 0x02C4:  # LATF
                print(f"           bit1(PSU_interlock)={v>>1 & 1}")
            elif addr == 0x02F4:  # LATG
                print(f"           bit9(AT45DB_CS#)={v>>9 & 1}")
            elif addr == 0x0260:  # SPI2STAT
                print(f"           SPIEN={v>>15 & 1}")
            elif addr == 0x0400:  # PTCON
                print(f"           PTEN={v>>15 & 1}")
            elif addr == 0x1E22:  # systemState
                states = {0:"IDLE",1:"STARTUP",2:"ACTIVE",3:"FAULT",5:"HOLDOFF",6:"RELAY"}
                print(f"           → {states.get(v, 'UNKNOWN')}")

    # Verification checks
    print(f"\n  --- Init Verification ---")
    checks = []
    def check(name, addr, expected, mask=0xFFFF):
        if addr not in flat:
            checks.append(("SKIP", name, f"not read"))
            return
        actual = flat[addr] & mask
        exp = expected & mask
        if actual == exp:
            checks.append(("PASS", name, f"0x{actual:04X}"))
        else:
            checks.append(("FAIL", name, f"got 0x{actual:04X} expected 0x{exp:04X}"))

    check("U1MODE", 0x0220, 0x8006)
    check("U1BRG", 0x0228, 0x028A)
    check("SPI2STAT.SPIEN", 0x0260, 0x8000, 0x8000)
    check("LATG.9 CS#", 0x02F4, 0x0200, 0x0200)
    check("marginThreshold=10", 0x1E10, 10)
    check("ioutLimitLo=25", 0x1E12, 25)
    check("ioutLimitHi=25", 0x1E16, 25)
    check("freqSetpoint=350", 0x1D3E, 350)
    check("oc2rs_shadow=400", 0x1E38, 400)

    for s, n, d in checks:
        mark = {"PASS":"  OK  ","FAIL":"*FAIL*","SKIP":" SKIP "}[s]
        print(f"  [{mark}] {n}: {d}")

else:
    # Program memory verification — check key instructions match disassembly
    print(f"\n  --- Program Memory Reads ---")
    for mem_type, base_addr, unit_size, vals in results:
        if mem_type == 'p':
            addr_str = f"0x{base_addr:06X}"
            vals_str = " ".join(f"{v:06X}" for v in vals)
            print(f"  {addr_str}: {vals_str}")
        elif mem_type == 'c':
            print(f"\n  --- Configuration Memory ---")
            for idx, v in enumerate(vals):
                addr = base_addr + idx * 2
                print(f"  0x{addr:06X}: 0x{v:04X}")

    # Verify BSET 0x217, #4 instruction (opcode A88217) at flash function addresses
    print(f"\n  --- BSET instruction verification ---")
    bset_addrs = [0x41B8, 0x41DA, 0x4278, 0x4296, 0x42AA]
    for mem_type, base_addr, unit_size, vals in results:
        if mem_type == 'p' and base_addr in bset_addrs and vals:
            instr = vals[0]
            # Expected: A88217 = BSET 0x217, #4 (I2C2CON.SCLREL)
            expected = 0xA88217
            status = "MATCH" if instr == expected else "MISMATCH"
            print(f"  0x{base_addr:06X}: 0x{instr:06X}  (expect 0x{expected:06X})  [{status}]")

print(f"\nParsed CSV: {csv_path}")
print(f"Raw log:    {log_path}")
PYEOF
