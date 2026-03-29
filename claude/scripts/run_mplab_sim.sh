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
SIM_TRACE_PRINT_STEPS="${SIM_TRACE_PRINT_STEPS:-$SIM_TEST_TARGET_STEPS}"
SIM_MODE="${SIM_MODE:-full}"
SIM_CHAIN_T2_CALLS="${SIM_CHAIN_T2_CALLS:-5}"
SIM_CHAIN_T1_CALLS="${SIM_CHAIN_T1_CALLS:-1}"
SIM_STACK_BYTES="${SIM_STACK_BYTES:-512}"
SIM_PRINT_MDB_LOG="${SIM_PRINT_MDB_LOG:-0}"
SIM_EXTRA_CPPFLAGS="${SIM_EXTRA_CPPFLAGS:-}"
OUTPUT_DIR="${SIM_OUTPUT_DIR:-$ROOT_DIR/out/sim}"
LOG_FILE="$OUTPUT_DIR/hstns_pd44_mdb.log"
TRACE_CSV="$OUTPUT_DIR/sim_trace.csv"
TRACE_SVG="$OUTPUT_DIR/sim_trace.svg"

mkdir -p "$OUTPUT_DIR"

if (( SIM_TRACE_PRINT_STEPS > SIM_TRACE_MAX_STEPS )); then
    SIM_TRACE_PRINT_STEPS="$SIM_TRACE_MAX_STEPS"
fi

cleanup() {
    :
}
trap cleanup EXIT

make TYPE_IMAGE=DEBUG_RUN MP_EXTRA_CC_PRE="-DSIMULATION_MODE -DSIM_AUTOMATION_MODE -DSIM_TEST_TARGET_STEPS=${SIM_TEST_TARGET_STEPS} -DSIM_TRACE_MAX_STEPS=${SIM_TRACE_MAX_STEPS} ${SIM_EXTRA_CPPFLAGS}" MP_EXTRA_LD_POST=",--stack=${SIM_STACK_BYTES}" CONF=default -B -C "$PROJECT_DIR" >/dev/null

expect <<EOF > "$LOG_FILE"
set timeout 60
log_user 1

proc wait_prompt {} {
    expect ">"
}

spawn $MDB_SH
wait_prompt
send "Device dsPIC33FJ64GS606\r"
wait_prompt
send "Hwtool SIM\r"
wait_prompt
send "Program $ELF_PATH\r"
wait_prompt
send "Run\r"
wait_prompt
send "Wait $SIM_WAIT_SECONDS\r"
wait_prompt
send "Halt\r"
wait_prompt
send "Print sim_debug.step\r"
wait_prompt
send "Print sim_test_status\r"
wait_prompt
send "Print sim_test_failure_count\r"
wait_prompt
send "Print sim_debug.step\r"
wait_prompt
send "Print sim_debug.t1_count\r"
wait_prompt
send "Print sim_debug.t2_count\r"
wait_prompt
send "Print sim_debug.t4_count\r"
wait_prompt
send "Print sim_debug.observed_system_state\r"
wait_prompt
send "Print sim_debug.observed_state_pre_step\r"
wait_prompt
send "Print sim_debug.observed_state_after_t1\r"
wait_prompt
send "Print sim_debug.observed_state_after_t2\r"
wait_prompt
send "Print sim_debug.observed_state_after_startup\r"
wait_prompt
send "Print sim_debug.observed_state_after_main\r"
wait_prompt
send "Print sim_debug.observed_margin_threshold\r"
wait_prompt
send "Print sim_debug.observed_aux_flags\r"
wait_prompt
send "Print sim_debug.observed_pwm_running\r"
wait_prompt
send "Print sim_debug.observed_status_flags\r"
wait_prompt
send "Print sim_debug.observed_startup_flags\r"
wait_prompt
send "Print sim_debug.observed_system_flags\r"
wait_prompt
send "Print sim_debug.observed_portd\r"
wait_prompt
send "Print sim_debug.observed_latd\r"
wait_prompt
send "Print sim_debug.observed_latf\r"
wait_prompt
send "Print sim_debug.observed_vout_target_code\r"
wait_prompt
send "Print sim_debug.observed_vout_ref_target\r"
wait_prompt
send "Print sim_debug.observed_soft_start_limit\r"
wait_prompt
send "Print sim_debug.observed_soft_start_ramp_cnt\r"
wait_prompt
send "Print sim_debug.observed_soft_start_dwell_cnt\r"
wait_prompt
send "Print sim_debug.observed_vref_mode_select\r"
wait_prompt
send "Print sim_debug.observed_droop_mode\r"
wait_prompt
send "Print sim_debug.observed_adcbuf4\r"
wait_prompt
send "Print sim_debug.observed_cmpcon3\r"
wait_prompt
send "Print sim_debug.observed_cmpdac3\r"
wait_prompt
send "Print sim_debug.observed_cmpcon4\r"
wait_prompt
send "Print sim_debug.observed_cmpdac4\r"
wait_prompt
send "Print sim_debug.first_drop_step\r"
wait_prompt
send "Print sim_debug.first_drop_phase\r"
wait_prompt
send "Print sim_debug.target_vout_sum\r"
wait_prompt
send "Print sim_debug.observed_vout_sum\r"
wait_prompt
send "Print sim_debug.observed_vout_setpoint\r"
wait_prompt
send "Print sim_debug.observed_vout_setpoint_target\r"
wait_prompt
send "Print sim_debug.observed_vout_reference\r"
wait_prompt
send "Print sim_debug.observed_ptper_shadow\r"
wait_prompt
send "Print sim_debug.observed_ptper_register\r"
wait_prompt
send "Print sim_trace_mode\r"
wait_prompt
send "Print sim_trace_count\r"
wait_prompt
send "Print sim_test_min_vout_reference\r"
wait_prompt
send "Print sim_test_max_vout_reference\r"
wait_prompt
send "Print sim_test_max_imeas\r"
wait_prompt
send "Print sim_test_final_comp_out\r"
wait_prompt
for {set i 0} {\$i < $SIM_TRACE_PRINT_STEPS} {incr i} {
    send [format "Print sim_trace_step\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_an0\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_an2\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_vout_sum\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_vout_setpoint\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_llc_period_cmd\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_comp_y_out\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_state\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_ptper\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_margin_threshold\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_aux_flags\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_pwm_running\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_status_flags\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_startup_flags\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_system_flags\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_portd\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_latd\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_latf\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_vout_target_code\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_vref_mode_select\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_droop_mode\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_adcbuf4\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_cmpcon3\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_cmpdac3\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_cmpcon4\\[%d\\]\r" \$i]
    wait_prompt
    send [format "Print sim_trace_cmpdac4\\[%d\\]\r" \$i]
    wait_prompt
}
send "Quit\r"
expect eof
EOF

if [[ "$SIM_PRINT_MDB_LOG" == "1" ]]; then
    cat "$LOG_FILE"
fi

read_print_value() {
    local symbol_name="$1"
    awk -v symbol="$symbol_name" '
        $0 ~ symbol "=" {
            getline;
            gsub(/^[[:space:]]+|[[:space:]]+$/, "", $0);
            print;
            exit;
        }
    ' "$LOG_FILE"
}

STATUS_VALUE="$(read_print_value "sim_test_status")"
FAIL_COUNT="$(read_print_value "sim_test_failure_count")"
STEP_COUNT="$(read_print_value "sim_debug.step")"
T1_COUNT="$(read_print_value "sim_debug.t1_count")"
T2_COUNT="$(read_print_value "sim_debug.t2_count")"
T4_COUNT="$(read_print_value "sim_debug.t4_count")"
OBS_STATE="$(read_print_value "sim_debug.observed_system_state")"
OBS_STATE_PRE_STEP="$(read_print_value "sim_debug.observed_state_pre_step")"
OBS_STATE_AFTER_T1="$(read_print_value "sim_debug.observed_state_after_t1")"
OBS_STATE_AFTER_T2="$(read_print_value "sim_debug.observed_state_after_t2")"
OBS_STATE_AFTER_STARTUP="$(read_print_value "sim_debug.observed_state_after_startup")"
OBS_STATE_AFTER_MAIN="$(read_print_value "sim_debug.observed_state_after_main")"
OBS_MARGIN_THRESHOLD="$(read_print_value "sim_debug.observed_margin_threshold")"
OBS_AUX_FLAGS="$(read_print_value "sim_debug.observed_aux_flags")"
OBS_PWM_RUNNING="$(read_print_value "sim_debug.observed_pwm_running")"
OBS_STATUS_FLAGS="$(read_print_value "sim_debug.observed_status_flags")"
OBS_STARTUP_FLAGS="$(read_print_value "sim_debug.observed_startup_flags")"
OBS_SYSTEM_FLAGS="$(read_print_value "sim_debug.observed_system_flags")"
OBS_PORTD="$(read_print_value "sim_debug.observed_portd")"
OBS_LATD="$(read_print_value "sim_debug.observed_latd")"
OBS_LATF="$(read_print_value "sim_debug.observed_latf")"
OBS_VOUT_TARGET_CODE="$(read_print_value "sim_debug.observed_vout_target_code")"
OBS_VOUT_REF_TARGET="$(read_print_value "sim_debug.observed_vout_ref_target")"
OBS_SOFT_START_LIMIT="$(read_print_value "sim_debug.observed_soft_start_limit")"
OBS_SOFT_START_RAMP_CNT="$(read_print_value "sim_debug.observed_soft_start_ramp_cnt")"
OBS_SOFT_START_DWELL_CNT="$(read_print_value "sim_debug.observed_soft_start_dwell_cnt")"
OBS_VREF_MODE_SELECT="$(read_print_value "sim_debug.observed_vref_mode_select")"
OBS_DROOP_MODE="$(read_print_value "sim_debug.observed_droop_mode")"
OBS_ADCBUF4="$(read_print_value "sim_debug.observed_adcbuf4")"
OBS_CMPCON3="$(read_print_value "sim_debug.observed_cmpcon3")"
OBS_CMPDAC3="$(read_print_value "sim_debug.observed_cmpdac3")"
OBS_CMPCON4="$(read_print_value "sim_debug.observed_cmpcon4")"
OBS_CMPDAC4="$(read_print_value "sim_debug.observed_cmpdac4")"
FIRST_DROP_STEP="$(read_print_value "sim_debug.first_drop_step")"
FIRST_DROP_PHASE="$(read_print_value "sim_debug.first_drop_phase")"
TARGET_VOUT_SUM="$(read_print_value "sim_debug.target_vout_sum")"
OBS_VOUT_SUM="$(read_print_value "sim_debug.observed_vout_sum")"
OBS_VOUT_SETPOINT="$(read_print_value "sim_debug.observed_vout_setpoint")"
OBS_VOUT_SETPOINT_TARGET="$(read_print_value "sim_debug.observed_vout_setpoint_target")"
OBS_VOUT_REFERENCE="$(read_print_value "sim_debug.observed_vout_reference")"
OBS_PTPER_SHADOW="$(read_print_value "sim_debug.observed_ptper_shadow")"
OBS_PTPER_REGISTER="$(read_print_value "sim_debug.observed_ptper_register")"
TRACE_MODE="$(read_print_value "sim_trace_mode")"
TRACE_COUNT="$(read_print_value "sim_trace_count")"

if [[ -z "${STATUS_VALUE}" || -z "${FAIL_COUNT}" || -z "${STEP_COUNT}" ]]; then
    echo "Simulation log did not contain complete result symbols" >&2
    exit 2
fi

python3 - <<'PY' "$LOG_FILE" "$TRACE_CSV" "${TRACE_COUNT:-0}" "$PLOT_SCRIPT" "$TRACE_SVG"
import csv
import re
import subprocess
import sys

log_path, csv_path, trace_count_raw, plot_script, svg_path = sys.argv[1:]
trace_count = int(trace_count_raw or "0")

with open(log_path, encoding="utf-8") as f:
    text = f.read()

def read_symbol(symbol):
    pattern = re.compile(re.escape(symbol) + r"=\s*\n\s*([-0-9]+)")
    match = pattern.search(text)
    if not match:
        return ""
    return match.group(1)

setpoint_target = read_symbol("sim_debug.observed_vout_setpoint_target")
if not setpoint_target:
    setpoint_target = "0"

rows = []
for i in range(trace_count):
    cmpcon3 = read_symbol(f"sim_trace_cmpcon3[{i}]")
    cmpcon4 = read_symbol(f"sim_trace_cmpcon4[{i}]")
    cmp3_out = ""
    cmp4_out = ""
    cmpdac3_code = ""
    cmpdac4_code = ""
    if cmpcon3:
        cmp3_out = str((int(cmpcon3) >> 3) & 1)
    if cmpcon4:
        cmp4_out = str((int(cmpcon4) >> 3) & 1)
    cmpdac3_raw = read_symbol(f"sim_trace_cmpdac3[{i}]")
    cmpdac4_raw = read_symbol(f"sim_trace_cmpdac4[{i}]")
    if cmpdac3_raw:
        cmpdac3_code = str(int(cmpdac3_raw) & 0x3FF)
    if cmpdac4_raw:
        cmpdac4_code = str(int(cmpdac4_raw) & 0x3FF)
    row = {
        "step": read_symbol(f"sim_trace_step[{i}]"),
        "an0": read_symbol(f"sim_trace_an0[{i}]"),
        "an2": read_symbol(f"sim_trace_an2[{i}]"),
        "vout_sum": read_symbol(f"sim_trace_vout_sum[{i}]"),
        "vout_setpoint": read_symbol(f"sim_trace_vout_setpoint[{i}]"),
        "vout_setpoint_target": setpoint_target,
        "llc_period_cmd": read_symbol(f"sim_trace_llc_period_cmd[{i}]"),
        "comp_y_out": read_symbol(f"sim_trace_comp_y_out[{i}]"),
        "state": read_symbol(f"sim_trace_state[{i}]"),
        "ptper": read_symbol(f"sim_trace_ptper[{i}]"),
        "margin_threshold": read_symbol(f"sim_trace_margin_threshold[{i}]"),
        "aux_flags": read_symbol(f"sim_trace_aux_flags[{i}]"),
        "pwm_running": read_symbol(f"sim_trace_pwm_running[{i}]"),
        "status_flags": read_symbol(f"sim_trace_status_flags[{i}]"),
        "startup_flags": read_symbol(f"sim_trace_startup_flags[{i}]"),
        "system_flags": read_symbol(f"sim_trace_system_flags[{i}]"),
        "portd": read_symbol(f"sim_trace_portd[{i}]"),
        "latd": read_symbol(f"sim_trace_latd[{i}]"),
        "latf": read_symbol(f"sim_trace_latf[{i}]"),
        "vout_target_code": read_symbol(f"sim_trace_vout_target_code[{i}]"),
        "vref_mode_select": read_symbol(f"sim_trace_vref_mode_select[{i}]"),
        "droop_mode": read_symbol(f"sim_trace_droop_mode[{i}]"),
        "adcbuf4": read_symbol(f"sim_trace_adcbuf4[{i}]"),
        "cmpcon3": cmpcon3,
        "cmpdac3": cmpdac3_raw,
        "cmpcon4": cmpcon4,
        "cmpdac4": cmpdac4_raw,
        "cmp3_out": cmp3_out,
        "cmp4_out": cmp4_out,
        "cmpdac3_code": cmpdac3_code,
        "cmpdac4_code": cmpdac4_code,
    }
    if not row["step"]:
        continue
    rows.append(row)

with open(csv_path, "w", newline="", encoding="utf-8") as f:
    rows.sort(key=lambda row: int(row["step"]))
    writer = csv.DictWriter(
        f,
        fieldnames=["step", "an0", "an2", "vout_sum", "vout_setpoint", "vout_setpoint_target", "llc_period_cmd", "comp_y_out", "state", "ptper", "margin_threshold", "aux_flags", "pwm_running", "status_flags", "startup_flags", "system_flags", "portd", "latd", "latf", "vout_target_code", "vref_mode_select", "droop_mode", "adcbuf4", "cmpcon3", "cmpdac3", "cmpcon4", "cmpdac4", "cmp3_out", "cmp4_out", "cmpdac3_code", "cmpdac4_code"],
    )
    writer.writeheader()
    writer.writerows(rows)

if rows:
    subprocess.run([sys.executable, plot_script, csv_path, svg_path], check=True)
PY

diagnostics=()
warnings=()

if (( STEP_COUNT < SIM_TEST_TARGET_STEPS )); then
    diagnostics+=("step<target(${STEP_COUNT}<${SIM_TEST_TARGET_STEPS})")
fi
if [[ "$SIM_MODE" == "chain" ]] && (( T1_COUNT < STEP_COUNT * SIM_CHAIN_T1_CALLS )); then
    diagnostics+=("t1_count<step_ratio(${T1_COUNT}<$((${STEP_COUNT} * SIM_CHAIN_T1_CALLS)))")
elif [[ "$SIM_MODE" != "t2" ]] && (( T1_COUNT < STEP_COUNT )); then
    diagnostics+=("t1_count<step(${T1_COUNT}<${STEP_COUNT})")
fi
if (( T2_COUNT < 1 )); then
    diagnostics+=("t2_not_run")
fi
if [[ "$SIM_MODE" == "chain" ]] && (( T2_COUNT < STEP_COUNT * SIM_CHAIN_T2_CALLS )); then
    diagnostics+=("t2_count<step_ratio(${T2_COUNT}<$((${STEP_COUNT} * SIM_CHAIN_T2_CALLS)))")
fi
if [[ "$SIM_MODE" == "chain" ]] && (( T4_COUNT < STEP_COUNT )); then
    diagnostics+=("t4_count<step(${T4_COUNT}<${STEP_COUNT})")
elif [[ "$SIM_MODE" != "t2" ]] && (( T4_COUNT < 2 )); then
    diagnostics+=("t4_not_run_enough")
fi
if [[ "$SIM_MODE" != "t2" ]] && (( OBS_STATE != 2 )); then
    diagnostics+=("systemState!=ACTIVE(${OBS_STATE})")
fi
if (( OBS_PTPER_SHADOW < 0 )); then
    diagnostics+=("ptper_shadow_negative(${OBS_PTPER_SHADOW})")
fi
if (( OBS_PTPER_SHADOW != OBS_PTPER_REGISTER )); then
    diagnostics+=("ptper_register_mismatch(${OBS_PTPER_SHADOW}!=$OBS_PTPER_REGISTER)")
fi
if (( TRACE_COUNT < STEP_COUNT )); then
    if [[ "${TRACE_MODE:-0}" == "1" ]]; then
        warnings+=("trace_uniform_sampled(${TRACE_COUNT}_pts_of_${STEP_COUNT})")
    elif (( TRACE_COUNT == SIM_TRACE_MAX_STEPS )); then
        warnings+=("trace_window(last_${TRACE_COUNT}_of_${STEP_COUNT})")
    else
        warnings+=("trace_incomplete(${TRACE_COUNT}<${STEP_COUNT})")
    fi
fi

if (( ${#diagnostics[@]} > 0 )); then
    printf 'Simulation failed: %s\n' "${diagnostics[*]}" >&2
    echo "Observed: status=${STATUS_VALUE} failure_count=${FAIL_COUNT} step=${STEP_COUNT} t1=${T1_COUNT} t2=${T2_COUNT} t4=${T4_COUNT} state=${OBS_STATE} pre=${OBS_STATE_PRE_STEP} after_t1=${OBS_STATE_AFTER_T1} after_t2=${OBS_STATE_AFTER_T2} after_startup=${OBS_STATE_AFTER_STARTUP} after_main=${OBS_STATE_AFTER_MAIN} margin=${OBS_MARGIN_THRESHOLD} aux=0x$(printf '%X' "${OBS_AUX_FLAGS:-0}") pwm=0x$(printf '%X' "${OBS_PWM_RUNNING:-0}") stat=0x$(printf '%X' "${OBS_STATUS_FLAGS:-0}") stup=0x$(printf '%X' "${OBS_STARTUP_FLAGS:-0}") sys=0x$(printf '%X' "${OBS_SYSTEM_FLAGS:-0}") portd=0x$(printf '%X' "${OBS_PORTD:-0}") latd=0x$(printf '%X' "${OBS_LATD:-0}") latf=0x$(printf '%X' "${OBS_LATF:-0}") cmp3=0x$(printf '%X' "${OBS_CMPCON3:-0}") dac3=0x$(printf '%X' "${OBS_CMPDAC3:-0}") cmp4=0x$(printf '%X' "${OBS_CMPCON4:-0}") dac4=0x$(printf '%X' "${OBS_CMPDAC4:-0}") adc4=${OBS_ADCBUF4} droop=${OBS_DROOP_MODE} vtarget=${OBS_VOUT_TARGET_CODE} vref_target=${OBS_VOUT_REF_TARGET} ss_limit=${OBS_SOFT_START_LIMIT} ss_ramp=${OBS_SOFT_START_RAMP_CNT} ss_dwell=${OBS_SOFT_START_DWELL_CNT} vrefsel=${OBS_VREF_MODE_SELECT} drop_step=${FIRST_DROP_STEP} drop_phase=${FIRST_DROP_PHASE} vout_sum=${OBS_VOUT_SUM} vout_setpoint=${OBS_VOUT_SETPOINT} vout_setpoint_target=${OBS_VOUT_SETPOINT_TARGET} vout_ref=${OBS_VOUT_REFERENCE} ptper_shadow=${OBS_PTPER_SHADOW} ptper_reg=${OBS_PTPER_REGISTER}" >&2
    echo "Trace: csv=${TRACE_CSV} svg=${TRACE_SVG}" >&2
    exit 1
fi

echo "Simulation passed: status=${STATUS_VALUE} failure_count=${FAIL_COUNT} step=${STEP_COUNT} state=${OBS_STATE} pre=${OBS_STATE_PRE_STEP} after_t1=${OBS_STATE_AFTER_T1} after_t2=${OBS_STATE_AFTER_T2} after_startup=${OBS_STATE_AFTER_STARTUP} after_main=${OBS_STATE_AFTER_MAIN} margin=${OBS_MARGIN_THRESHOLD} aux=0x$(printf '%X' "${OBS_AUX_FLAGS:-0}") pwm=0x$(printf '%X' "${OBS_PWM_RUNNING:-0}") stat=0x$(printf '%X' "${OBS_STATUS_FLAGS:-0}") stup=0x$(printf '%X' "${OBS_STARTUP_FLAGS:-0}") sys=0x$(printf '%X' "${OBS_SYSTEM_FLAGS:-0}") portd=0x$(printf '%X' "${OBS_PORTD:-0}") latd=0x$(printf '%X' "${OBS_LATD:-0}") latf=0x$(printf '%X' "${OBS_LATF:-0}") cmp3=0x$(printf '%X' "${OBS_CMPCON3:-0}") dac3=0x$(printf '%X' "${OBS_CMPDAC3:-0}") cmp4=0x$(printf '%X' "${OBS_CMPCON4:-0}") dac4=0x$(printf '%X' "${OBS_CMPDAC4:-0}") adc4=${OBS_ADCBUF4} droop=${OBS_DROOP_MODE} vtarget=${OBS_VOUT_TARGET_CODE} vref_target=${OBS_VOUT_REF_TARGET} ss_limit=${OBS_SOFT_START_LIMIT} ss_ramp=${OBS_SOFT_START_RAMP_CNT} ss_dwell=${OBS_SOFT_START_DWELL_CNT} vrefsel=${OBS_VREF_MODE_SELECT} drop_step=${FIRST_DROP_STEP} drop_phase=${FIRST_DROP_PHASE} vout_sum=${OBS_VOUT_SUM} vout_setpoint=${OBS_VOUT_SETPOINT} vout_setpoint_target=${OBS_VOUT_SETPOINT_TARGET} vout_ref=${OBS_VOUT_REFERENCE} ptper=${OBS_PTPER_SHADOW}"
if (( ${#warnings[@]} > 0 )); then
    echo "Warnings: ${warnings[*]}"
fi
echo "Trace: csv=${TRACE_CSV} svg=${TRACE_SVG}"
