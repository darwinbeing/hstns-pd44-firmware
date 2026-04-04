#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
RUNNER="$ROOT_DIR/scripts/run_mplab_scl.sh"
MAP_PATH="$ROOT_DIR/HSTNS-PD44.X/dist/default/debug/HSTNS-PD44.X.debug.map"

SCL_FILE="${SCL_FILE:-$ROOT_DIR/scripts/stimulus/hstns_noload_startup_llc_rd11_hold_ramp0_psemkick.scl}"
SCL_FLASH_DUMP="${SCL_FLASH_DUMP:-$ROOT_DIR/AT45DB021E.bin}"
SCL_HOST_SAMPLES="${SCL_HOST_SAMPLES:-120}"
SCL_SAMPLE_WAIT_MS="${SCL_SAMPLE_WAIT_MS:-2}"
SCL_INIT_WAIT_MS="${SCL_INIT_WAIT_MS:-2}"
SCL_POST_HALT_WAIT_MS="${SCL_POST_HALT_WAIT_MS:-1}"
SIM_OSCILLATOR_FREQUENCY="${SIM_OSCILLATOR_FREQUENCY:-50000000}"
SCL_REBUILD="${SCL_REBUILD:-0}"

OUT_ROOT="${SCL_OUTPUT_ROOT:-$ROOT_DIR/out/sim_kff_branch_check}"
mkdir -p "$OUT_ROOT"

if [[ ! -f "$RUNNER" ]]; then
    echo "runner not found: $RUNNER" >&2
    exit 2
fi
if [[ ! -f "$MAP_PATH" ]]; then
    echo "map not found: $MAP_PATH" >&2
    exit 2
fi
if [[ ! -f "$SCL_FILE" ]]; then
    echo "scl file not found: $SCL_FILE" >&2
    exit 2
fi
if [[ ! -f "$SCL_FLASH_DUMP" ]]; then
    echo "flash dump not found: $SCL_FLASH_DUMP" >&2
    exit 2
fi

eval "$(python3 - <<'PY' "$MAP_PATH"
import re,sys
mp=sys.argv[1]
want=[
  'runtimeFlags','statusFlags','voltageError','droopKffFactor',
  'droopPeriod','uvpDebounce','droopBoostFlags'
]
pat=re.compile(r"^\s*0x([0-9A-Fa-f]+)\s+_([A-Za-z0-9_]+)\s*$")
addr={}
with open(mp,encoding='utf-8',errors='ignore') as f:
    for line in f:
        m=pat.match(line)
        if m:
            addr[m.group(2)]=int(m.group(1),16)
missing=[k for k in want if k not in addr]
if missing:
    raise SystemExit('missing map symbols: '+','.join(missing))
for k in want:
    print(f"ADDR_{k.upper()}=0x{addr[k]:04X}")
PY
)"

TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/kffcheck.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT

cat > "$TMP_DIR/force_46d8.mdb" <<EOF2
write /rh $ADDR_DROOPKFFFACTOR 0x0000
write /rh $ADDR_RUNTIMEFLAGS 0x0100
write /rh $ADDR_DROOPPERIOD 0x0014
EOF2

cat > "$TMP_DIR/force_4718.mdb" <<EOF2
write /rh $ADDR_DROOPKFFFACTOR 0x0000
write /rh $ADDR_RUNTIMEFLAGS 0x0200
write /rh $ADDR_UVPDEBOUNCE 0x0014
EOF2

cat > "$TMP_DIR/force_468a.mdb" <<EOF2
write /rh $ADDR_DROOPKFFFACTOR 0x0000
write /rh $ADDR_STATUSFLAGS 0x0200
write /rh $ADDR_VOLTAGEERROR 0x001E
write /rh $ADDR_DROOPBOOSTFLAGS 0x0000
write /rh $ADDR_RUNTIMEFLAGS 0x0000
EOF2

cat > "$TMP_DIR/force_46f4.mdb" <<EOF2
write /rh $ADDR_DROOPKFFFACTOR 0x0000
write /rh $ADDR_STATUSFLAGS 0x0400
write /rh $ADDR_VOLTAGEERROR 0xFFE0
write /rh $ADDR_RUNTIMEFLAGS 0x0000
EOF2

run_case() {
    local name="$1"
    local per_sample_file="$2"
    local out_dir="$OUT_ROOT/$name"
    mkdir -p "$out_dir"

    if [[ -n "$per_sample_file" ]]; then
        SCL_PER_SAMPLE_MDB_CMDS_FILE="$per_sample_file" \
        SCL_OUTPUT_DIR="$out_dir" \
        SCL_REBUILD="$SCL_REBUILD" \
        SCL_FILE="$SCL_FILE" \
        SCL_HOST_SAMPLES="$SCL_HOST_SAMPLES" \
        SCL_SAMPLE_WAIT_MS="$SCL_SAMPLE_WAIT_MS" \
        SCL_INIT_WAIT_MS="$SCL_INIT_WAIT_MS" \
        SCL_POST_HALT_WAIT_MS="$SCL_POST_HALT_WAIT_MS" \
        SIM_OSCILLATOR_FREQUENCY="$SIM_OSCILLATOR_FREQUENCY" \
        SCL_FLASH_DUMP="$SCL_FLASH_DUMP" \
        SCL_EXTRA_SYMBOLS="droopKffFactor,voltageError,statusFlags,runtimeFlags,droopPeriod,uvpDebounce,droopBoostFlags" \
        "$RUNNER" >/dev/null
    else
        SCL_OUTPUT_DIR="$out_dir" \
        SCL_REBUILD="$SCL_REBUILD" \
        SCL_FILE="$SCL_FILE" \
        SCL_HOST_SAMPLES="$SCL_HOST_SAMPLES" \
        SCL_SAMPLE_WAIT_MS="$SCL_SAMPLE_WAIT_MS" \
        SCL_INIT_WAIT_MS="$SCL_INIT_WAIT_MS" \
        SCL_POST_HALT_WAIT_MS="$SCL_POST_HALT_WAIT_MS" \
        SIM_OSCILLATOR_FREQUENCY="$SIM_OSCILLATOR_FREQUENCY" \
        SCL_FLASH_DUMP="$SCL_FLASH_DUMP" \
        SCL_EXTRA_SYMBOLS="droopKffFactor,voltageError,statusFlags,runtimeFlags,droopPeriod,uvpDebounce,droopBoostFlags" \
        "$RUNNER" >/dev/null
    fi

    python3 - <<'PY' "$out_dir/scl_trace.csv" "$name"
import csv,sys
path,name=sys.argv[1],sys.argv[2]
rows=list(csv.DictReader(open(path,encoding='utf-8')))
if not rows:
    print(f"{name}: empty trace")
    raise SystemExit(0)

def to_i(v):
    return int(v,0)

kff=[to_i(r['droopKffFactor']) for r in rows]
st=[to_i(r['statusFlags']) for r in rows]
rt=[to_i(r['runtimeFlags']) for r in rows]
ve=[to_i(r['voltageError']) for r in rows]

cnt_400=sum(1 for v in kff if v==0x400)
cnt_5dc=sum(1 for v in kff if v==0x5DC)
print(
    f"{name}: samples={len(rows)} kff400={cnt_400} kff5dc={cnt_5dc} "
    f"kff_min={min(kff)} kff_max={max(kff)} "
    f"st9={sum(1 for x in st if x&0x200)} st10={sum(1 for x in st if x&0x400)} "
    f"rt8={sum(1 for x in rt if x&0x100)} rt9={sum(1 for x in rt if x&0x200)} "
    f"verr_min={min(ve)} verr_max={max(ve)}"
)
PY
}

echo "== KFF branch check =="
echo "out: $OUT_ROOT"

run_case baseline ""
run_case force_46d8 "$TMP_DIR/force_46d8.mdb"
run_case force_4718 "$TMP_DIR/force_4718.mdb"
run_case force_468a "$TMP_DIR/force_468a.mdb"
run_case force_46f4 "$TMP_DIR/force_46f4.mdb"

echo "done"
