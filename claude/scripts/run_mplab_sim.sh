#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PROJECT_DIR="$ROOT_DIR/HSTNS-PD44.X"
ELF_PATH="$PROJECT_DIR/dist/default/debug/HSTNS-PD44.X.debug.elf"
MDB_SH="/Applications/microchip/mplabx/v6.20/mplab_platform/bin/mdb.sh"
PLOT_SCRIPT="$ROOT_DIR/scripts/plot_sim_trace.py"

SIM_TEST_TARGET_STEPS="${SIM_TEST_TARGET_STEPS:-60}"
SIM_WAIT_SECONDS="${SIM_WAIT_SECONDS:-60}"
SIM_TRACE_MAX_STEPS="${SIM_TRACE_MAX_STEPS:-64}"
SIM_MODE="${SIM_MODE:-full}"
SIM_CHAIN_T2_CALLS="${SIM_CHAIN_T2_CALLS:-5}"
SIM_CHAIN_T1_CALLS="${SIM_CHAIN_T1_CALLS:-1}"
SIM_CHAIN_WATCHDOG_CALLS="${SIM_CHAIN_WATCHDOG_CALLS:-1}"
SIM_STACK_BYTES="${SIM_STACK_BYTES:-512}"
SIM_PRINT_MDB_LOG="${SIM_PRINT_MDB_LOG:-0}"
SIM_EXTRA_CPPFLAGS="${SIM_EXTRA_CPPFLAGS:-}"
SIM_HOST_SAMPLES="${SIM_HOST_SAMPLES:-200}"
SIM_SAMPLE_WAIT_MS="${SIM_SAMPLE_WAIT_MS:-5}"
SIM_POST_HALT_WAIT_MS="${SIM_POST_HALT_WAIT_MS:-1}"
SIM_MDB_RETRIES="${SIM_MDB_RETRIES:-6}"
SIM_INCLUDE_INITIAL_SAMPLE="${SIM_INCLUDE_INITIAL_SAMPLE:-1}"
SIM_INIT_WAIT_MS="${SIM_INIT_WAIT_MS:-0}"

OUTPUT_DIR="${SIM_OUTPUT_DIR:-$ROOT_DIR/out/sim}"
LOG_FILE="$OUTPUT_DIR/hstns_pd44_mdb.log"
TRACE_CSV="$OUTPUT_DIR/sim_trace.csv"
TRACE_SVG="$OUTPUT_DIR/sim_trace.svg"

mkdir -p "$OUTPUT_DIR"

make TYPE_IMAGE=DEBUG_RUN MP_EXTRA_CC_PRE="-DSIMULATION_MODE -DSIM_AUTOMATION_MODE -DSIM_TEST_TARGET_STEPS=${SIM_TEST_TARGET_STEPS} -DSIM_TRACE_MAX_STEPS=${SIM_TRACE_MAX_STEPS} ${SIM_EXTRA_CPPFLAGS}" MP_EXTRA_LD_POST=",--stack=${SIM_STACK_BYTES}" CONF=default -B -C "$PROJECT_DIR" >/dev/null

MDB_CMD_FILE="$(mktemp "${TMPDIR:-/tmp}/hstns_pd44_mdb_cmds.XXXXXX")"
trap 'rm -f "$MDB_CMD_FILE"' EXIT

sample_symbols=(
    "sim_test_status"
    "sim_debug.step"
    "sim_debug.adcbuf0"
    "sim_debug.adcbuf2"
    "sim_debug.observed_vout_sum"
    "sim_debug.observed_vout_setpoint"
    "sim_debug.observed_vout_setpoint_target"
    "sim_debug.observed_vout_reference"
    "sim_debug.observed_comp_out"
    "sim_debug.observed_system_state"
    "sim_debug.observed_ptper_shadow"
    "sim_debug.observed_ptper_register"
    "sim_debug.observed_margin_threshold"
    "sim_debug.observed_aux_flags"
    "sim_debug.observed_pwm_running"
    "sim_debug.observed_status_flags"
    "sim_debug.observed_startup_flags"
    "sim_debug.observed_system_flags"
    "sim_debug.observed_portd"
    "sim_debug.observed_latd"
    "sim_debug.observed_latf"
    "sim_debug.observed_vout_target_code"
    "sim_debug.observed_vout_ref_target"
    "sim_debug.observed_vref_mode_select"
    "sim_debug.observed_droop_mode"
    "sim_debug.observed_adcbuf4"
    "sim_debug.observed_cmpcon3"
    "sim_debug.observed_cmpdac3"
    "sim_debug.observed_cmpcon4"
    "sim_debug.observed_cmpdac4"
)

summary_symbols=(
    "sim_test_status"
    "sim_test_failure_count"
    "sim_debug.step"
    "sim_debug.t1_count"
    "sim_debug.t2_count"
    "sim_debug.t4_count"
    "sim_debug.observed_system_state"
    "sim_debug.observed_state_pre_step"
    "sim_debug.observed_state_after_t1"
    "sim_debug.observed_state_after_t2"
    "sim_debug.observed_state_after_startup"
    "sim_debug.observed_state_after_main"
    "sim_debug.observed_state_seen_mask"
    "sim_debug.observed_idle_to_startup_count"
    "sim_debug.observed_startup_to_active_count"
    "sim_debug.observed_active_to_fault_count"
    "sim_debug.observed_margin_threshold"
    "sim_debug.observed_aux_flags"
    "sim_debug.observed_pwm_running"
    "sim_debug.observed_status_flags"
    "sim_debug.observed_startup_flags"
    "sim_debug.observed_system_flags"
    "sim_debug.observed_portd"
    "sim_debug.observed_latd"
    "sim_debug.observed_latf"
    "sim_debug.observed_vout_target_code"
    "sim_debug.observed_vout_ref_target"
    "sim_debug.observed_soft_start_limit"
    "sim_debug.observed_soft_start_ramp_cnt"
    "sim_debug.observed_soft_start_dwell_cnt"
    "sim_debug.observed_vref_mode_select"
    "sim_debug.observed_droop_mode"
    "sim_debug.observed_adcbuf4"
    "sim_debug.observed_cmpcon3"
    "sim_debug.observed_cmpdac3"
    "sim_debug.observed_cmpcon4"
    "sim_debug.observed_cmpdac4"
    "sim_debug.first_drop_step"
    "sim_debug.first_drop_phase"
    "sim_debug.target_vout_sum"
    "sim_debug.observed_vout_sum"
    "sim_debug.observed_vout_setpoint"
    "sim_debug.observed_vout_setpoint_target"
    "sim_debug.observed_vout_reference"
    "sim_debug.observed_ptper_shadow"
    "sim_debug.observed_ptper_register"
    "sim_test_min_vout_reference"
    "sim_test_max_vout_reference"
    "sim_test_max_imeas"
    "sim_test_final_comp_out"
)

{
    echo "Device dsPIC33FJ64GS606"
    echo "Hwtool SIM"
    echo "Program $ELF_PATH"
    if (( SIM_INCLUDE_INITIAL_SAMPLE != 0 )); then
        echo "Run"
        echo "Wait $SIM_INIT_WAIT_MS"
        echo "Halt"
        echo "Wait $SIM_POST_HALT_WAIT_MS"
        for symbol in "${sample_symbols[@]}"; do
            echo "Print $symbol"
        done
    fi
    for ((i = 0; i < SIM_HOST_SAMPLES; i++)); do
        echo "Run"
        echo "Wait $SIM_SAMPLE_WAIT_MS"
        echo "Halt"
        echo "Wait $SIM_POST_HALT_WAIT_MS"
        for symbol in "${sample_symbols[@]}"; do
            echo "Print $symbol"
        done
    done
    for symbol in "${summary_symbols[@]}"; do
        echo "Print $symbol"
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
required = ("sim_test_status", "sim_test_failure_count", "sim_debug.step")
num_pat = r"[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)"

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
    if (( attempt >= SIM_MDB_RETRIES )); then
        break
    fi
    ((attempt++))
done

if [[ "$SIM_PRINT_MDB_LOG" == "1" ]]; then
    cat "$LOG_FILE"
fi

read_last_value() {
    local symbol_name="$1"
    python3 - <<'PY' "$LOG_FILE" "$symbol_name"
import re
import sys

log_path = sys.argv[1]
symbol = sys.argv[2]

with open(log_path, encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)
pattern = re.compile(
    re.escape(symbol) + r"\s*=\s*(?:\r?\n)?\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))"
)
values = pattern.findall(text)
if not values:
    print("")
else:
    print(int(values[-1], 0))
PY
}

STATUS_VALUE="$(read_last_value "sim_test_status")"
FAIL_COUNT="$(read_last_value "sim_test_failure_count")"
STEP_COUNT="$(read_last_value "sim_debug.step")"
T1_COUNT="$(read_last_value "sim_debug.t1_count")"
T2_COUNT="$(read_last_value "sim_debug.t2_count")"
T4_COUNT="$(read_last_value "sim_debug.t4_count")"
OBS_STATE="$(read_last_value "sim_debug.observed_system_state")"
OBS_STATE_PRE_STEP="$(read_last_value "sim_debug.observed_state_pre_step")"
OBS_STATE_AFTER_T1="$(read_last_value "sim_debug.observed_state_after_t1")"
OBS_STATE_AFTER_T2="$(read_last_value "sim_debug.observed_state_after_t2")"
OBS_STATE_AFTER_STARTUP="$(read_last_value "sim_debug.observed_state_after_startup")"
OBS_STATE_AFTER_MAIN="$(read_last_value "sim_debug.observed_state_after_main")"
OBS_STATE_SEEN_MASK="$(read_last_value "sim_debug.observed_state_seen_mask")"
OBS_IDLE_TO_STARTUP_COUNT="$(read_last_value "sim_debug.observed_idle_to_startup_count")"
OBS_STARTUP_TO_ACTIVE_COUNT="$(read_last_value "sim_debug.observed_startup_to_active_count")"
OBS_ACTIVE_TO_FAULT_COUNT="$(read_last_value "sim_debug.observed_active_to_fault_count")"
OBS_MARGIN_THRESHOLD="$(read_last_value "sim_debug.observed_margin_threshold")"
OBS_AUX_FLAGS="$(read_last_value "sim_debug.observed_aux_flags")"
OBS_PWM_RUNNING="$(read_last_value "sim_debug.observed_pwm_running")"
OBS_STATUS_FLAGS="$(read_last_value "sim_debug.observed_status_flags")"
OBS_STARTUP_FLAGS="$(read_last_value "sim_debug.observed_startup_flags")"
OBS_SYSTEM_FLAGS="$(read_last_value "sim_debug.observed_system_flags")"
OBS_PORTD="$(read_last_value "sim_debug.observed_portd")"
OBS_LATD="$(read_last_value "sim_debug.observed_latd")"
OBS_LATF="$(read_last_value "sim_debug.observed_latf")"
OBS_VOUT_TARGET_CODE="$(read_last_value "sim_debug.observed_vout_target_code")"
OBS_VOUT_REF_TARGET="$(read_last_value "sim_debug.observed_vout_ref_target")"
OBS_SOFT_START_LIMIT="$(read_last_value "sim_debug.observed_soft_start_limit")"
OBS_SOFT_START_RAMP_CNT="$(read_last_value "sim_debug.observed_soft_start_ramp_cnt")"
OBS_SOFT_START_DWELL_CNT="$(read_last_value "sim_debug.observed_soft_start_dwell_cnt")"
OBS_VREF_MODE_SELECT="$(read_last_value "sim_debug.observed_vref_mode_select")"
OBS_DROOP_MODE="$(read_last_value "sim_debug.observed_droop_mode")"
OBS_ADCBUF4="$(read_last_value "sim_debug.observed_adcbuf4")"
OBS_CMPCON3="$(read_last_value "sim_debug.observed_cmpcon3")"
OBS_CMPDAC3="$(read_last_value "sim_debug.observed_cmpdac3")"
OBS_CMPCON4="$(read_last_value "sim_debug.observed_cmpcon4")"
OBS_CMPDAC4="$(read_last_value "sim_debug.observed_cmpdac4")"
FIRST_DROP_STEP="$(read_last_value "sim_debug.first_drop_step")"
FIRST_DROP_PHASE="$(read_last_value "sim_debug.first_drop_phase")"
OBS_VOUT_SUM="$(read_last_value "sim_debug.observed_vout_sum")"
OBS_VOUT_SETPOINT="$(read_last_value "sim_debug.observed_vout_setpoint")"
OBS_VOUT_SETPOINT_TARGET="$(read_last_value "sim_debug.observed_vout_setpoint_target")"
OBS_VOUT_REFERENCE="$(read_last_value "sim_debug.observed_vout_reference")"
OBS_PTPER_SHADOW="$(read_last_value "sim_debug.observed_ptper_shadow")"
OBS_PTPER_REGISTER="$(read_last_value "sim_debug.observed_ptper_register")"

if [[ -z "${STATUS_VALUE}" || -z "${FAIL_COUNT}" || -z "${STEP_COUNT}" ]]; then
    echo "Simulation log did not contain complete result symbols (mdb_rc=${MDB_RC})" >&2
    echo "Log: ${LOG_FILE}" >&2
    exit 2
fi

OBS_STATE_SEEN_MASK="${OBS_STATE_SEEN_MASK:-0}"
OBS_IDLE_TO_STARTUP_COUNT="${OBS_IDLE_TO_STARTUP_COUNT:-0}"
OBS_STARTUP_TO_ACTIVE_COUNT="${OBS_STARTUP_TO_ACTIVE_COUNT:-0}"
OBS_ACTIVE_TO_FAULT_COUNT="${OBS_ACTIVE_TO_FAULT_COUNT:-0}"

python3 - <<'PY' "$LOG_FILE" "$TRACE_CSV" "$PLOT_SCRIPT" "$TRACE_SVG" "$SIM_HOST_SAMPLES" "$SIM_INCLUDE_INITIAL_SAMPLE"
import csv
import re
import subprocess
import sys

log_path, csv_path, plot_script, svg_path, host_samples_raw, include_initial_raw = sys.argv[1:]
host_samples = int(host_samples_raw)
include_initial = 1 if int(include_initial_raw) != 0 else 0

with open(log_path, encoding="utf-8") as f:
    text = f.read()

text = re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", text)

def read_symbol_all(symbol):
    pattern = re.compile(re.escape(symbol) + r"\s*=\s*(?:\r?\n)?\s*(-?\d+)")
    return pattern.findall(text)

series = {
    "step": read_symbol_all("sim_debug.step"),
    "an0": read_symbol_all("sim_debug.adcbuf0"),
    "an2": read_symbol_all("sim_debug.adcbuf2"),
    "vout_sum": read_symbol_all("sim_debug.observed_vout_sum"),
    "vout_setpoint": read_symbol_all("sim_debug.observed_vout_setpoint"),
    "vout_setpoint_target": read_symbol_all("sim_debug.observed_vout_setpoint_target"),
    "llc_period_cmd": read_symbol_all("sim_debug.observed_vout_reference"),
    "comp_y_out": read_symbol_all("sim_debug.observed_comp_out"),
    "state": read_symbol_all("sim_debug.observed_system_state"),
    "ptper": read_symbol_all("sim_debug.observed_ptper_shadow"),
    "margin_threshold": read_symbol_all("sim_debug.observed_margin_threshold"),
    "aux_flags": read_symbol_all("sim_debug.observed_aux_flags"),
    "pwm_running": read_symbol_all("sim_debug.observed_pwm_running"),
    "status_flags": read_symbol_all("sim_debug.observed_status_flags"),
    "startup_flags": read_symbol_all("sim_debug.observed_startup_flags"),
    "system_flags": read_symbol_all("sim_debug.observed_system_flags"),
    "portd": read_symbol_all("sim_debug.observed_portd"),
    "latd": read_symbol_all("sim_debug.observed_latd"),
    "latf": read_symbol_all("sim_debug.observed_latf"),
    "vout_target_code": read_symbol_all("sim_debug.observed_vout_target_code"),
    "vref_mode_select": read_symbol_all("sim_debug.observed_vref_mode_select"),
    "droop_mode": read_symbol_all("sim_debug.observed_droop_mode"),
    "adcbuf4": read_symbol_all("sim_debug.observed_adcbuf4"),
    "cmpcon3": read_symbol_all("sim_debug.observed_cmpcon3"),
    "cmpdac3": read_symbol_all("sim_debug.observed_cmpdac3"),
    "cmpcon4": read_symbol_all("sim_debug.observed_cmpcon4"),
    "cmpdac4": read_symbol_all("sim_debug.observed_cmpdac4"),
}

count = min(len(series["step"]), host_samples + include_initial)
rows = []
for i in range(count):
    row = {}
    for key, values in series.items():
        row[key] = values[i] if i < len(values) else ""
    if not row["step"]:
        continue
    if row["cmpcon3"]:
        row["cmp3_out"] = str((int(row["cmpcon3"]) >> 3) & 1)
    else:
        row["cmp3_out"] = ""
    if row["cmpcon4"]:
        row["cmp4_out"] = str((int(row["cmpcon4"]) >> 3) & 1)
    else:
        row["cmp4_out"] = ""
    if row["cmpdac3"]:
        row["cmpdac3_code"] = str(int(row["cmpdac3"]) & 0x3FF)
    else:
        row["cmpdac3_code"] = ""
    if row["cmpdac4"]:
        row["cmpdac4_code"] = str(int(row["cmpdac4"]) & 0x3FF)
    else:
        row["cmpdac4_code"] = ""
    rows.append(row)

with open(csv_path, "w", newline="", encoding="utf-8") as f:
    if rows:
        base_step = int(rows[0]["step"])
        for row in rows:
            row["step"] = str(int(row["step"]) - base_step)
    writer = csv.DictWriter(
        f,
        fieldnames=[
            "step", "an0", "an2", "vout_sum", "vout_setpoint", "vout_setpoint_target",
            "llc_period_cmd", "comp_y_out", "state", "ptper", "margin_threshold",
            "aux_flags", "pwm_running", "status_flags", "startup_flags", "system_flags",
            "portd", "latd", "latf", "vout_target_code", "vref_mode_select", "droop_mode",
            "adcbuf4", "cmpcon3", "cmpdac3", "cmpcon4", "cmpdac4",
            "cmp3_out", "cmp4_out", "cmpdac3_code", "cmpdac4_code"
        ],
    )
    writer.writeheader()
    writer.writerows(rows)

if rows:
    subprocess.run([sys.executable, plot_script, csv_path, svg_path], check=True)
PY

diagnostics=()
warnings=()

if (( FAIL_COUNT > 0 )); then
    diagnostics+=("failure_count>0(${FAIL_COUNT})")
fi
if (( STEP_COUNT < SIM_TEST_TARGET_STEPS )); then
    diagnostics+=("step<target(${STEP_COUNT}<${SIM_TEST_TARGET_STEPS})")
fi
if (( T2_COUNT < 1 )); then
    diagnostics+=("t2_not_run")
fi
if [[ "$SIM_MODE" != "t2" ]] && (( T1_COUNT < 1 )); then
    diagnostics+=("t1_not_run")
fi
if [[ "$SIM_MODE" == "chain" ]] && (( T4_COUNT < 1 )); then
    diagnostics+=("t4_not_run")
fi
if (( OBS_PTPER_SHADOW != OBS_PTPER_REGISTER )); then
    diagnostics+=("ptper_register_mismatch(${OBS_PTPER_SHADOW}!=$OBS_PTPER_REGISTER)")
fi
if [[ "$SIM_MODE" == "chain" ]]; then
    if (( (OBS_STATE_SEEN_MASK & (1 << 0)) == 0 )); then
        diagnostics+=("state_idle_not_seen")
    fi
    if (( (OBS_STATE_SEEN_MASK & (1 << 1)) == 0 )); then
        diagnostics+=("state_startup_not_seen")
    fi
    if (( (OBS_STATE_SEEN_MASK & (1 << 2)) == 0 )); then
        diagnostics+=("state_active_not_seen")
    fi
    if (( OBS_IDLE_TO_STARTUP_COUNT < 1 )); then
        diagnostics+=("missing_idle_to_startup_transition")
    fi
    if (( OBS_STARTUP_TO_ACTIVE_COUNT < 1 )); then
        diagnostics+=("missing_startup_to_active_transition")
    fi
fi

if (( SIM_HOST_SAMPLES < 20 )); then
    warnings+=("host_samples_low(${SIM_HOST_SAMPLES})")
fi

if (( ${#diagnostics[@]} > 0 )); then
    printf 'Simulation failed: %s\n' "${diagnostics[*]}" >&2
    echo "Observed: status=${STATUS_VALUE} failure_count=${FAIL_COUNT} step=${STEP_COUNT} t1=${T1_COUNT} t2=${T2_COUNT} t4=${T4_COUNT} state=${OBS_STATE} pre=${OBS_STATE_PRE_STEP} after_t1=${OBS_STATE_AFTER_T1} after_t2=${OBS_STATE_AFTER_T2} after_startup=${OBS_STATE_AFTER_STARTUP} after_main=${OBS_STATE_AFTER_MAIN} state_mask=0x$(printf '%X' "${OBS_STATE_SEEN_MASK:-0}") idle_to_startup=${OBS_IDLE_TO_STARTUP_COUNT} startup_to_active=${OBS_STARTUP_TO_ACTIVE_COUNT} active_to_fault=${OBS_ACTIVE_TO_FAULT_COUNT} margin=${OBS_MARGIN_THRESHOLD} aux=0x$(printf '%X' "${OBS_AUX_FLAGS:-0}") pwm=0x$(printf '%X' "${OBS_PWM_RUNNING:-0}") stat=0x$(printf '%X' "${OBS_STATUS_FLAGS:-0}") stup=0x$(printf '%X' "${OBS_STARTUP_FLAGS:-0}") sys=0x$(printf '%X' "${OBS_SYSTEM_FLAGS:-0}") portd=0x$(printf '%X' "${OBS_PORTD:-0}") latd=0x$(printf '%X' "${OBS_LATD:-0}") latf=0x$(printf '%X' "${OBS_LATF:-0}") cmp3=0x$(printf '%X' "${OBS_CMPCON3:-0}") dac3=0x$(printf '%X' "${OBS_CMPDAC3:-0}") cmp4=0x$(printf '%X' "${OBS_CMPCON4:-0}") dac4=0x$(printf '%X' "${OBS_CMPDAC4:-0}") adc4=${OBS_ADCBUF4} droop=${OBS_DROOP_MODE} vtarget=${OBS_VOUT_TARGET_CODE} vref_target=${OBS_VOUT_REF_TARGET} ss_limit=${OBS_SOFT_START_LIMIT} ss_ramp=${OBS_SOFT_START_RAMP_CNT} ss_dwell=${OBS_SOFT_START_DWELL_CNT} vrefsel=${OBS_VREF_MODE_SELECT} drop_step=${FIRST_DROP_STEP} drop_phase=${FIRST_DROP_PHASE} vout_sum=${OBS_VOUT_SUM} vout_setpoint=${OBS_VOUT_SETPOINT} vout_setpoint_target=${OBS_VOUT_SETPOINT_TARGET} vout_ref=${OBS_VOUT_REFERENCE} ptper_shadow=${OBS_PTPER_SHADOW} ptper_reg=${OBS_PTPER_REGISTER}" >&2
    echo "Trace: csv=${TRACE_CSV} svg=${TRACE_SVG}" >&2
    exit 1
fi

echo "Simulation passed: status=${STATUS_VALUE} failure_count=${FAIL_COUNT} step=${STEP_COUNT} state=${OBS_STATE} pre=${OBS_STATE_PRE_STEP} after_t1=${OBS_STATE_AFTER_T1} after_t2=${OBS_STATE_AFTER_T2} after_startup=${OBS_STATE_AFTER_STARTUP} after_main=${OBS_STATE_AFTER_MAIN} state_mask=0x$(printf '%X' "${OBS_STATE_SEEN_MASK:-0}") idle_to_startup=${OBS_IDLE_TO_STARTUP_COUNT} startup_to_active=${OBS_STARTUP_TO_ACTIVE_COUNT} active_to_fault=${OBS_ACTIVE_TO_FAULT_COUNT} margin=${OBS_MARGIN_THRESHOLD} aux=0x$(printf '%X' "${OBS_AUX_FLAGS:-0}") pwm=0x$(printf '%X' "${OBS_PWM_RUNNING:-0}") stat=0x$(printf '%X' "${OBS_STATUS_FLAGS:-0}") stup=0x$(printf '%X' "${OBS_STARTUP_FLAGS:-0}") sys=0x$(printf '%X' "${OBS_SYSTEM_FLAGS:-0}") portd=0x$(printf '%X' "${OBS_PORTD:-0}") latd=0x$(printf '%X' "${OBS_LATD:-0}") latf=0x$(printf '%X' "${OBS_LATF:-0}") cmp3=0x$(printf '%X' "${OBS_CMPCON3:-0}") dac3=0x$(printf '%X' "${OBS_CMPDAC3:-0}") cmp4=0x$(printf '%X' "${OBS_CMPCON4:-0}") dac4=0x$(printf '%X' "${OBS_CMPDAC4:-0}") adc4=${OBS_ADCBUF4} droop=${OBS_DROOP_MODE} vtarget=${OBS_VOUT_TARGET_CODE} vref_target=${OBS_VOUT_REF_TARGET} ss_limit=${OBS_SOFT_START_LIMIT} ss_ramp=${OBS_SOFT_START_RAMP_CNT} ss_dwell=${OBS_SOFT_START_DWELL_CNT} vrefsel=${OBS_VREF_MODE_SELECT} drop_step=${FIRST_DROP_STEP} drop_phase=${FIRST_DROP_PHASE} vout_sum=${OBS_VOUT_SUM} vout_setpoint=${OBS_VOUT_SETPOINT} vout_setpoint_target=${OBS_VOUT_SETPOINT_TARGET} vout_ref=${OBS_VOUT_REFERENCE} ptper=${OBS_PTPER_SHADOW}"
if (( ${#warnings[@]} > 0 )); then
    echo "Warnings: ${warnings[*]}"
fi
echo "Trace: csv=${TRACE_CSV} svg=${TRACE_SVG}"
