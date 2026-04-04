#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PROJECT_DIR="$ROOT_DIR/HSTNS-PD44.X"
ELF_PATH="$PROJECT_DIR/dist/default/debug/HSTNS-PD44.X.debug.elf"
MAP_PATH="${ELF_PATH%.elf}.map"
MDB_SH="/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh"

SCL_FILE="${SCL_FILE:-$ROOT_DIR/scripts/stimulus/hstns_startup_llc.scl}"
SCL_HOST_SAMPLES="${SCL_HOST_SAMPLES:-200}"
SCL_SAMPLE_WAIT_MS="${SCL_SAMPLE_WAIT_MS:-2}"
SCL_POST_HALT_WAIT_MS="${SCL_POST_HALT_WAIT_MS:-1}"
SCL_INIT_WAIT_MS="${SCL_INIT_WAIT_MS:-2}"
SCL_INCLUDE_INITIAL_SAMPLE="${SCL_INCLUDE_INITIAL_SAMPLE:-1}"
SCL_MDB_RETRIES="${SCL_MDB_RETRIES:-12}"
SCL_WARMUP_CYCLES="${SCL_WARMUP_CYCLES:-1}"
SCL_PRINT_MDB_LOG="${SCL_PRINT_MDB_LOG:-0}"
SCL_EXTRA_CPPFLAGS="${SCL_EXTRA_CPPFLAGS:-}"
SCL_EXTRA_SYMBOLS="${SCL_EXTRA_SYMBOLS:-}"
SIM_OSCILLATOR_FREQUENCY="${SIM_OSCILLATOR_FREQUENCY:-50000000}"
SCL_FLASH_DUMP="${SCL_FLASH_DUMP:-}"
SCL_REBUILD="${SCL_REBUILD:-1}"
SCL_FLASH_INJECT_PHASE="${SCL_FLASH_INJECT_PHASE:-post_warmup}"
SCL_EXTRA_MDB_CMDS_FILE="${SCL_EXTRA_MDB_CMDS_FILE:-}"
SCL_PER_SAMPLE_MDB_CMDS_FILE="${SCL_PER_SAMPLE_MDB_CMDS_FILE:-}"

OUTPUT_DIR="${SCL_OUTPUT_DIR:-$ROOT_DIR/out/sim_scl}"
LOG_FILE="$OUTPUT_DIR/hstns_pd44_scl_mdb.log"
CSV_FILE="$OUTPUT_DIR/scl_trace.csv"

mkdir -p "$OUTPUT_DIR"

if [[ ! -f "$SCL_FILE" ]]; then
    echo "SCL file not found: $SCL_FILE" >&2
    exit 2
fi

if [[ -n "$SCL_EXTRA_MDB_CMDS_FILE" && ! -f "$SCL_EXTRA_MDB_CMDS_FILE" ]]; then
    echo "SCL extra MDB command file not found: $SCL_EXTRA_MDB_CMDS_FILE" >&2
    exit 2
fi

if [[ -n "$SCL_PER_SAMPLE_MDB_CMDS_FILE" && ! -f "$SCL_PER_SAMPLE_MDB_CMDS_FILE" ]]; then
    echo "SCL per-sample MDB command file not found: $SCL_PER_SAMPLE_MDB_CMDS_FILE" >&2
    exit 2
fi

if [[ "$SCL_REBUILD" == "1" ]]; then
    make TYPE_IMAGE=DEBUG_RUN MP_EXTRA_CC_PRE="${SCL_EXTRA_CPPFLAGS}" CONF=default -B -C "$PROJECT_DIR" >/dev/null
fi

MDB_CMD_FILE="$(mktemp "${TMPDIR:-/tmp}/hstns_pd44_scl_cmds.XXXXXX")"
trap 'rm -f "$MDB_CMD_FILE"' EXIT

sample_symbols=(
    "PORTD"
    "systemState"
    "llcPeriodCmd"
    "voutSetpoint"
    "vout_sum"
    "marginThreshold"
    "auxFlags"
    "pwmRunning"
    "statusFlags"
    "runtimeFlags"
    "startupFlags"
    "startupTickCnt"
    "ADCBUF0"
    "ADCBUF2"
    "ADCBUF12"
    "PTPER"
    "OC2RS"
    "OSCCON"
    "ACLKCON"
)

if [[ -n "$SCL_EXTRA_SYMBOLS" ]]; then
    extra_symbols_raw="${SCL_EXTRA_SYMBOLS// /}"
    for symbol in ${(s:,:)extra_symbols_raw}; do
        [[ -z "$symbol" ]] && continue
        if (( ${sample_symbols[(I)$symbol]} == 0 )); then
            sample_symbols+=("$symbol")
        fi
    done
fi

SAMPLE_SYMBOLS_CSV="${(j:,:)sample_symbols}"

WARMUP_CYCLES_EFF="$SCL_WARMUP_CYCLES"

FLASH_CAL_INJECT_CMDS=""
if [[ -n "$SCL_FLASH_DUMP" ]]; then
    if [[ ! -f "$SCL_FLASH_DUMP" ]]; then
        echo "SCL flash dump not found: $SCL_FLASH_DUMP" >&2
        exit 2
    fi

    FLASH_CAL_INJECT_CMDS="$(
        python3 - "$SCL_FLASH_DUMP" "$MAP_PATH" <<'PY'
import pathlib
import re
import sys

dump_path = pathlib.Path(sys.argv[1])
map_path = pathlib.Path(sys.argv[2])
data = dump_path.read_bytes()
if len(data) % 256 != 0:
    raise SystemExit(f"invalid dump size {len(data)}: not 256-byte aligned")
if len(data) < (8 * 256):
    raise SystemExit(f"invalid dump size {len(data)}: requires at least 8 pages")
if not map_path.exists():
    raise SystemExit(f"map file not found: {map_path}")

p7 = data[7 * 256 : 8 * 256]

def u16le(off: int) -> int:
    return p7[off] | (p7[off + 1] << 8)

def s16(v: int) -> int:
    return v - 0x10000 if (v & 0x8000) else v

def c_div_trunc(a: int, b: int) -> int:
    return int(a / b)

symbol_addr = {}
pat = re.compile(r"^\s*0x([0-9A-Fa-f]+)\s+_([A-Za-z0-9_]+)\s*$")
for line in map_path.read_text(encoding="utf-8", errors="ignore").splitlines():
    m = pat.match(line)
    if not m:
        continue
    symbol_addr[m.group(2)] = int(m.group(1), 16)

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

vals = {
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

for name, value in vals.items():
    if name not in symbol_addr:
        raise SystemExit(f"symbol not found in map: _{name}")
    print(f"write /rh 0x{symbol_addr[name]:04X} 0x{value:04X}")
PY
    )"

    # Calibration words are loaded into RAM by startup code. If injection is
    # requested after warmup and no warmup cycles are configured, the firmware
    # startup/BSS init can overwrite injected values before first sampling.
    if [[ "$SCL_FLASH_INJECT_PHASE" != "pre_start" && "$WARMUP_CYCLES_EFF" -lt 1 ]]; then
        WARMUP_CYCLES_EFF=1
    fi
fi

{
    echo "Device dsPIC33FJ64GS606"
    echo "Hwtool SIM"
    echo "Set oscillator.frequency ${SIM_OSCILLATOR_FREQUENCY}"
    echo "Program $ELF_PATH"
    echo "stim $SCL_FILE"

    # Optional early injection (before firmware startup code runs).
    if [[ "$SCL_FLASH_INJECT_PHASE" == "pre_start" && -n "$FLASH_CAL_INJECT_CMDS" ]]; then
        print -r -- "$FLASH_CAL_INJECT_CMDS"
    fi

    for ((w = 0; w < WARMUP_CYCLES_EFF; w++)); do
        echo "Run"
        echo "Wait $SCL_INIT_WAIT_MS"
        echo "Halt"
        echo "Wait $SCL_POST_HALT_WAIT_MS"
    done

    # Default: inject after warmup so startup/BSS init does not wipe values.
    if [[ "$SCL_FLASH_INJECT_PHASE" != "pre_start" && -n "$FLASH_CAL_INJECT_CMDS" ]]; then
        print -r -- "$FLASH_CAL_INJECT_CMDS"
    fi

    # Optional extra one-shot MDB commands before sampling starts.
    if [[ -n "$SCL_EXTRA_MDB_CMDS_FILE" ]]; then
        cat "$SCL_EXTRA_MDB_CMDS_FILE"
    fi

    if (( SCL_INCLUDE_INITIAL_SAMPLE != 0 )); then
        if [[ -n "$SCL_PER_SAMPLE_MDB_CMDS_FILE" ]]; then
            cat "$SCL_PER_SAMPLE_MDB_CMDS_FILE"
        fi
        echo "Run"
        echo "Wait $SCL_INIT_WAIT_MS"
        echo "Halt"
        echo "Wait $SCL_POST_HALT_WAIT_MS"
        for symbol in "${sample_symbols[@]}"; do
            echo "Print $symbol"
        done
    fi

    for ((i = 0; i < SCL_HOST_SAMPLES; i++)); do
        if [[ -n "$SCL_PER_SAMPLE_MDB_CMDS_FILE" ]]; then
            cat "$SCL_PER_SAMPLE_MDB_CMDS_FILE"
        fi
        echo "Run"
        echo "Wait $SCL_SAMPLE_WAIT_MS"
        echo "Halt"
        echo "Wait $SCL_POST_HALT_WAIT_MS"
        for symbol in "${sample_symbols[@]}"; do
            echo "Print $symbol"
        done
    done

    echo "Quit"
} > "$MDB_CMD_FILE"

has_required_symbols() {
    python3 - <<'PY' "$LOG_FILE"
import re
import sys

with open(sys.argv[1], encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)
num_pat = r"[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)"
required = ("PORTD", "systemState", "llcPeriodCmd")

for name in required:
    if not re.search(re.escape(name) + r"\s*=\s*(?:\r?\n)?\s*" + num_pat, text):
        raise SystemExit(1)

raise SystemExit(0)
PY
}

MDB_RC=0
attempt=1
while true; do
    MDB_RC=0
    "$MDB_SH" "$MDB_CMD_FILE" > "$LOG_FILE" 2>&1 || MDB_RC=$?
    if has_required_symbols; then
        break
    fi
    if (( attempt >= SCL_MDB_RETRIES )); then
        break
    fi
    ((attempt++))
done

if [[ "$SCL_PRINT_MDB_LOG" == "1" ]]; then
    cat "$LOG_FILE"
fi

if ! has_required_symbols; then
    echo "SCL simulation log did not contain complete symbols after ${attempt} attempt(s) (mdb_rc=${MDB_RC})" >&2
    echo "Log: ${LOG_FILE}" >&2
    exit 2
fi

read_last_value() {
    local symbol_name="$1"
    python3 - <<'PY' "$LOG_FILE" "$symbol_name"
import re
import sys

log_path, symbol = sys.argv[1], sys.argv[2]
with open(log_path, encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)
pattern = re.compile(re.escape(symbol) + r"\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))")
values = pattern.findall(text)
print("" if not values else int(values[-1], 0))
PY
}

read_first_value() {
    local symbol_name="$1"
    python3 - <<'PY' "$LOG_FILE" "$symbol_name"
import re
import sys

log_path, symbol = sys.argv[1], sys.argv[2]
with open(log_path, encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)
pattern = re.compile(re.escape(symbol) + r"\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))")
values = pattern.findall(text)
print("" if not values else int(values[0], 0))
PY
}

STATE_FIRST="$(read_first_value "systemState")"
STATE_LAST="$(read_last_value "systemState")"
LLC_FIRST="$(read_first_value "llcPeriodCmd")"
LLC_LAST="$(read_last_value "llcPeriodCmd")"
PORTD_LAST="$(read_last_value "PORTD")"

SAMPLE_COUNT="$(python3 - <<'PY' "$LOG_FILE" "$CSV_FILE" "$SCL_HOST_SAMPLES" "$SCL_INCLUDE_INITIAL_SAMPLE" "$SAMPLE_SYMBOLS_CSV"
import csv
import re
import sys

log_path, csv_path, host_samples_raw, include_initial_raw, symbols_csv = sys.argv[1:]
host_samples = int(host_samples_raw)
include_initial = 1 if int(include_initial_raw) != 0 else 0

symbols = [s for s in symbols_csv.split(",") if s]

with open(log_path, encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)

series = {}
for sym in symbols:
    pat = re.compile(re.escape(sym) + r"\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))")
    series[sym] = [int(x, 0) for x in pat.findall(text)]

count = min(len(series["PORTD"]), len(series["systemState"]), len(series["llcPeriodCmd"]), host_samples + include_initial)
if count <= 0:
    raise SystemExit("no complete samples to write")

rows = []
for i in range(count):
    row = {"sample": i}
    for sym in symbols:
        row[sym] = series[sym][i] if i < len(series[sym]) else ""
    rows.append(row)

with open(csv_path, "w", newline="", encoding="utf-8") as f:
    writer = csv.DictWriter(f, fieldnames=["sample"] + symbols)
    writer.writeheader()
    writer.writerows(rows)
print(count)
PY
)"

STATE_MASK="$(python3 - <<'PY' "$LOG_FILE"
import re
import sys
with open(sys.argv[1], encoding='utf-8', errors='ignore') as f:
    text = f.read()
text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)
vals = [int(v, 0) for v in re.findall(r"systemState\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))", text)]
mask = 0
for v in vals:
    if 0 <= v < 16:
        mask |= (1 << v)
print(mask)
PY
)"

printf 'SCL simulation captured: samples=%s state=%s->%s state_mask=0x%X llcPeriodCmd=%s->%s portd_final=0x%X\n' \
    "${SAMPLE_COUNT:-0}" "${STATE_FIRST:-?}" "${STATE_LAST:-?}" "$STATE_MASK" "${LLC_FIRST:-?}" "${LLC_LAST:-?}" "${PORTD_LAST:-0}"

echo "Trace: csv=${CSV_FILE} log=${LOG_FILE} scl=${SCL_FILE}"
