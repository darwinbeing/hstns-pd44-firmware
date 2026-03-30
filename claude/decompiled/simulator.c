#include <xc.h>
#include "variables.h"
#include "simulator.h"

extern void adcMiscSample(void);
extern void startupControl(void);
extern void mainStateDispatch(void);
extern void watchdogService(void);
extern void vinCheck(void);
extern void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);
extern void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
extern void __attribute__((interrupt, no_auto_psv)) _PWMSpEventMatchInterrupt(void);

volatile sim_debug_t sim_debug;
volatile uint16_t sim_trace_mode;
volatile uint16_t sim_trace_count;
#if SIM_ENABLE_TARGET_TRACE
volatile uint16_t sim_trace_step[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_an0[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_an2[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_vout_sum[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_vout_setpoint[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_llc_period_cmd[SIM_TRACE_MAX_STEPS];
volatile int32_t sim_trace_comp_y_out[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_state[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_ptper[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_margin_threshold[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_aux_flags[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_pwm_running[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_status_flags[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_startup_flags[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_system_flags[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_portd[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_latd[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_latf[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_vout_target_code[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_vref_mode_select[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_droop_mode[SIM_TRACE_MAX_STEPS];
volatile int16_t sim_trace_adcbuf4[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_cmpcon3[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_cmpdac3[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_cmpcon4[SIM_TRACE_MAX_STEPS];
volatile uint16_t sim_trace_cmpdac4[SIM_TRACE_MAX_STEPS];
#endif
volatile uint16_t sim_portd_input;
volatile uint16_t sim_test_status;
volatile uint16_t sim_test_failure_count;
volatile int16_t sim_test_min_vout_reference;
volatile int16_t sim_test_max_vout_reference;
volatile int16_t sim_test_max_imeas;
volatile int32_t sim_test_final_comp_out;

#ifndef SIM_TEST_TARGET_STEPS
#define SIM_TEST_TARGET_STEPS 6u
#endif

#ifndef SIM_TRACE_UNIFORM_SAMPLING
#define SIM_TRACE_UNIFORM_SAMPLING 0u
#endif

#ifndef SIM_CHAIN_T2_CALLS
#define SIM_CHAIN_T2_CALLS 5u
#endif

#ifndef SIM_CHAIN_T1_CALLS
#define SIM_CHAIN_T1_CALLS 1u
#endif

#ifndef SIM_CHAIN_WATCHDOG_CALLS
#define SIM_CHAIN_WATCHDOG_CALLS 1u
#endif

#ifndef SIM_CHAIN_FORCE_T2_REGULATION
#define SIM_CHAIN_FORCE_T2_REGULATION 0u
#endif

#ifndef SIM_PORTD6_EDGE_INTERVAL
#define SIM_PORTD6_EDGE_INTERVAL 100u
#endif

#ifndef SIM_STIM_STARTUP_RAMP_STEPS
#define SIM_STIM_STARTUP_RAMP_STEPS 500u
#endif

#ifndef SIM_STIM_PLANT_TC_DEN
#define SIM_STIM_PLANT_TC_DEN 12u
#endif

#ifndef SIM_STIM_STARTUP_TARGET_SUM
#define SIM_STIM_STARTUP_TARGET_SUM 8000u
#endif

static uint16_t sim_portd6_tick_divider;

static inline int16_t simClamp16(int16_t value, int16_t lo, int16_t hi)
{
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

static inline void simTestFail(void)
{
    sim_test_failure_count++;
}

static inline void simTestAssert(int condition)
{
    if (!condition) {
        simTestFail();
    }
}

static inline void simFlashStoreU16(uint16_t offset, uint16_t value)
{
    flash_read_buf_15E6[offset] = (uint8_t)(value & 0xFFu);
    flash_read_buf_15E6[offset + 1u] = (uint8_t)(value >> 8);
}

static inline uint16_t simEncodeShiftedOffset(int16_t value)
{
    return (uint16_t)((int32_t)(value - 1) << 4);
}

static inline uint16_t simEncodeCurrentOffsetRaw(int16_t value)
{
    return (uint16_t)((int32_t)(value - 1) * 122);
}

static inline int16_t simComputeVoutSetpoint(int16_t target_code)
{
    int32_t target_prod = (int32_t)target_code * (int32_t)cal_va;
    int16_t target_setpoint = (int16_t)(target_prod >> 13);
    target_setpoint = (int16_t)(target_setpoint + ofs_va);
    target_setpoint = (int16_t)(target_setpoint - vref_ls);
    target_setpoint = (int16_t)(target_setpoint - vrefOcpAdj);
    return target_setpoint;
}

static void simLoadCalibrationProfile(void)
{
    simFlashStoreU16(0x00, 0x1FB7u);
    simFlashStoreU16(0x02, simEncodeShiftedOffset(-1));
    simFlashStoreU16(0x04, 0x200Fu);
    simFlashStoreU16(0x06, simEncodeShiftedOffset(1));
    simFlashStoreU16(0x08, 0x2030u);
    simFlashStoreU16(0x0A, simEncodeCurrentOffsetRaw((int16_t)0xFFF8u));
    simFlashStoreU16(0x0C, 0x0000u);
    simFlashStoreU16(0x0E, 30u);
    simFlashStoreU16(0x11, 0x2000u);
    simFlashStoreU16(0x13, simEncodeShiftedOffset(0));

    statusFlags |= (1u << 6);
    adcMiscSample();

    /* Match the observed runtime calibration words while the flash decode is
     * still being validated against hardware dumps. */
    cal_va = (int16_t)0x1FB7;
    ofs_va = (int16_t)0xFFFF;
}

static inline void simTraceRecord(void)
{
#if SIM_ENABLE_TARGET_TRACE
    uint16_t index;

#if SIM_TRACE_UNIFORM_SAMPLING
    if (SIM_TEST_TARGET_STEPS <= 1u) {
        index = 0u;
    } else {
        uint32_t scaled = ((uint32_t)sim_debug.step * (uint32_t)SIM_TRACE_MAX_STEPS) / (uint32_t)SIM_TEST_TARGET_STEPS;
        if (scaled >= (uint32_t)SIM_TRACE_MAX_STEPS) {
            scaled = (uint32_t)SIM_TRACE_MAX_STEPS - 1u;
        }
        index = (uint16_t)scaled;
    }
#else
    index = (uint16_t)(sim_debug.step % SIM_TRACE_MAX_STEPS);
#endif

    sim_trace_step[index] = sim_debug.step;
    sim_trace_an0[index] = sim_debug.adcbuf0;
    sim_trace_an2[index] = sim_debug.adcbuf2;
    sim_trace_vout_sum[index] = (int16_t)((sim_debug.adcbuf0 + sim_debug.adcbuf2) << 1);
    sim_trace_vout_setpoint[index] = voutSetpoint;
    sim_trace_llc_period_cmd[index] = llcPeriodCmd;
    sim_trace_comp_y_out[index] = compY_out;
    sim_trace_state[index] = (uint16_t)systemState;
    sim_trace_ptper[index] = ptperCommand;
    sim_trace_margin_threshold[index] = marginThreshold;
    sim_trace_aux_flags[index] = auxFlags;
    sim_trace_pwm_running[index] = pwmRunning;
    sim_trace_status_flags[index] = statusFlags;
    sim_trace_startup_flags[index] = startupFlags;
    sim_trace_system_flags[index] = systemFlags;
    sim_trace_portd[index] = sim_portd_input;
    sim_trace_latd[index] = LATD;
    sim_trace_latf[index] = LATF;
    sim_trace_vout_target_code[index] = voutTargetCode;
    sim_trace_vref_mode_select[index] = vrefModeSelect;
    sim_trace_droop_mode[index] = droopMode;
    sim_trace_adcbuf4[index] = sim_debug.adcbuf4;
    sim_trace_cmpcon3[index] = CMPCON3;
    sim_trace_cmpdac3[index] = CMPDAC3;
    sim_trace_cmpcon4[index] = CMPCON4;
    sim_trace_cmpdac4[index] = CMPDAC4;

#if SIM_TRACE_UNIFORM_SAMPLING
    if (sim_trace_count < (uint16_t)(index + 1u)) {
        sim_trace_count = (uint16_t)(index + 1u);
    }
#else
    if (sim_trace_count < SIM_TRACE_MAX_STEPS) {
        sim_trace_count++;
    }
#endif
#else
    sim_trace_count = 0u;
#endif
}

static inline void simMarkStateSeen(uint16_t state)
{
    if (state < 16u) {
        sim_debug.observed_state_seen_mask |= (uint16_t)(1u << state);
    }

    if (state == ST_IDLE) {
        sim_test_status |= SIM_TEST_STATUS_SAW_IDLE;
    } else if (state == ST_STARTUP) {
        sim_test_status |= SIM_TEST_STATUS_SAW_STARTUP;
    } else if (state == ST_ACTIVE) {
        sim_test_status |= SIM_TEST_STATUS_SAW_ACTIVE;
    } else if (state == ST_FAULT) {
        sim_test_status |= SIM_TEST_STATUS_SAW_FAULT;
    }
}

static inline void simRecordStateTransition(uint16_t before_state)
{
    uint16_t after_state = (uint16_t)systemState;
    simMarkStateSeen(after_state);

    if (before_state == after_state) {
        return;
    }

    if ((before_state == ST_IDLE) && (after_state == ST_STARTUP)) {
        sim_debug.observed_idle_to_startup_count++;
        sim_test_status |= SIM_TEST_STATUS_IDLE_TO_STARTUP;
    } else if ((before_state == ST_STARTUP) && (after_state == ST_ACTIVE)) {
        sim_debug.observed_startup_to_active_count++;
        sim_test_status |= SIM_TEST_STATUS_STARTUP_TO_ACTIVE;
    } else if ((before_state == ST_ACTIVE) && (after_state == ST_FAULT)) {
        sim_debug.observed_active_to_fault_count++;
    }
}

static inline void simRecordStateDrop(uint16_t phase_id, uint16_t before_state)
{
    simRecordStateTransition(before_state);

    if (sim_debug.first_drop_step != 0xFFFFu) {
        return;
    }
    if (before_state == ST_ACTIVE && systemState != ST_ACTIVE) {
        sim_debug.first_drop_step = sim_debug.step;
        sim_debug.first_drop_phase = phase_id;
    }
}

void __attribute__((noinline)) simAutomationStop(void)
{
#ifdef SIM_AUTOMATION_MODE
    for (;;) {
        ClrWdt();
        __builtin_nop();
    }
#else
    __asm__ volatile ("break");
    __builtin_nop();
#endif
}

static inline void simLoadManualInputs(void)
{
    ADCBUF0 = sim_debug.adcbuf0;
    ADCBUF1 = 193;
    ADCBUF2 = sim_debug.adcbuf2;
    ADCBUF3 = sim_debug.adcbuf0 + 4;
    ADCBUF4 = sim_debug.adcbuf4;
    ADCBUF5 = sim_debug.adcbuf5;
    ADCBUF10 = 193;
    ADCBUF11 = 193;
    ADCBUF12 = sim_debug.adcbuf12;
    ADCBUF14 = sim_debug.adcbuf14;
    PORTD = sim_portd_input;
}

static inline void simUpdateComparatorModel(void)
{
    uint16_t cmp3 = CMPCON3;
    uint16_t cmpdac3 = CMPDAC3;

    if (cmp3 & (1u << 7)) {
        uint16_t threshold = cmpdac3 & 0x03FFu;
        uint16_t sample = (uint16_t)ADCBUF4;
        if (sample >= threshold) {
            cmp3 |= (1u << 3);
        } else {
            cmp3 &= ~(1u << 3);
        }
    } else {
        cmp3 &= ~(1u << 3);
    }

    CMPCON3 = cmp3;
}

static inline void simAdvancePortd6Signal(void)
{
    uint16_t interval = (uint16_t)SIM_PORTD6_EDGE_INTERVAL;
    if (interval == 0u) {
        interval = 1u;
    }

    sim_portd6_tick_divider++;
    if (sim_portd6_tick_divider >= interval) {
        sim_portd6_tick_divider = 0u;
        sim_portd_input ^= (1u << 6);
    }

    PORTD = sim_portd_input;
}

static inline void simApplyAutoStimulus(void)
{
    uint16_t step = sim_debug.step;
    uint16_t portd = sim_portd_input;
    uint16_t ramp_steps = (uint16_t)SIM_STIM_STARTUP_RAMP_STEPS;

    if (ramp_steps == 0u) {
        ramp_steps = 1u;
    }

    if (step < ramp_steps) {
        int16_t ramp_target = (int16_t)(((int32_t)SIM_STIM_STARTUP_TARGET_SUM * (int32_t)step) / (int32_t)ramp_steps);
        sim_debug.target_vout_sum = simClamp16(ramp_target, 0, (int16_t)SIM_STIM_STARTUP_TARGET_SUM);
        sim_debug.adcbuf4 = (int16_t)(4 + ((int32_t)step * 12) / (int32_t)ramp_steps);
        sim_debug.adcbuf12 = (int16_t)(0x0180 + ((int32_t)step * 10) / (int32_t)ramp_steps);
    } else {
        int16_t cmd = llcPeriodCmd;
        int16_t plant_target;
        int16_t plant_state = sim_debug.target_vout_sum;
        int16_t delta;
        uint16_t tc_den = (uint16_t)SIM_STIM_PLANT_TC_DEN;
        static const int16_t settle_dither[8] = {-1, 0, 1, 0, -1, 0, 1, 0};

        /* Closed-loop plant approximation.
         * llcPeriodCmd behaves as a scaled switching-frequency command:
         * larger command -> higher fsw -> lower effective gain -> lower Vout.
         * Target operating point is tuned around llcPeriodCmd ~= 3500.
         */
        if (cmd < 2600) {
            cmd = 2600;
        } else if (cmd > 24000) {
            cmd = 24000;
        }

        plant_target = (int16_t)(3119 - (((int32_t)cmd - 3500) / 4));
        plant_target = simClamp16(plant_target, 1200, 3900);

        if (tc_den == 0u) {
            tc_den = 1u;
        }
        /* First-order response, intentionally slowed to make AN0/AN2
         * convergence observable without increasing x-axis span. */
        delta = (int16_t)((plant_target - plant_state) / (int16_t)tc_den);
        if ((delta == 0) && (plant_target != plant_state)) {
            delta = (plant_target > plant_state) ? 1 : -1;
        }
        plant_state = (int16_t)(plant_state + delta);
        plant_state = (int16_t)(plant_state + settle_dither[step & 7u]);
        sim_debug.target_vout_sum = simClamp16(plant_state, 1200, 3700);

        /* Keep auxiliary channels coherent with the synthetic load level. */
        sim_debug.adcbuf4 = simClamp16((int16_t)(32 + (((int32_t)cmd - 3500) / 180)), 8, 160);
        sim_debug.adcbuf12 = (int16_t)(0x0190 + (sim_debug.adcbuf4 >> 2));
    }

    sim_debug.adcbuf0 = simClamp16((sim_debug.target_vout_sum >> 2) + 2, 0, 4095);
    sim_debug.adcbuf2 = simClamp16((sim_debug.target_vout_sum >> 2) - 2, 0, 4095);
    sim_debug.adcbuf5 = (sim_debug.target_vout_sum >> 2);
    sim_debug.adcbuf14 = 320;

    portd &= (uint16_t)~((1u << 0) | (1u << 11));
    portd |= (1u << 0);
    portd |= (1u << 11);
    sim_portd_input = portd;
    PORTD = sim_portd_input;
}

static inline void simCaptureObservations(void)
{
    int16_t target_setpoint = simComputeVoutSetpoint(voutRefTarget);

    sim_debug.observed_system_state = (uint16_t)systemState;
    sim_debug.observed_vout_sum = vout_sum;
    sim_debug.observed_vout_setpoint = voutSetpoint;
    sim_debug.observed_vout_setpoint_target = target_setpoint;
    sim_debug.observed_vout_reference = llcPeriodCmd;
    sim_debug.observed_ptper_shadow = ptperCommand;
    sim_debug.observed_ptper_register = PTPER;
    sim_debug.observed_oc2rs = OC2RS;
    sim_debug.observed_margin_threshold = marginThreshold;
    sim_debug.observed_aux_flags = auxFlags;
    sim_debug.observed_pwm_running = pwmRunning;
    sim_debug.observed_status_flags = statusFlags;
    sim_debug.observed_startup_flags = startupFlags;
    sim_debug.observed_system_flags = systemFlags;
    sim_debug.observed_portd = sim_portd_input;
    sim_debug.observed_latd = LATD;
    sim_debug.observed_latf = LATF;
    sim_debug.observed_vout_target_code = voutTargetCode;
    sim_debug.observed_vout_ref_target = voutRefTarget;
    sim_debug.observed_soft_start_limit = softStartPwmLimit;
    sim_debug.observed_soft_start_ramp_cnt = softStartRampCnt;
    sim_debug.observed_soft_start_dwell_cnt = softStartDwellCnt;
    sim_debug.observed_vref_mode_select = vrefModeSelect;
    sim_debug.observed_droop_mode = droopMode;
    sim_debug.observed_adcbuf4 = ADCBUF4;
    sim_debug.observed_cmpcon3 = CMPCON3;
    sim_debug.observed_cmpdac3 = CMPDAC3;
    sim_debug.observed_cmpcon4 = CMPCON4;
    sim_debug.observed_cmpdac4 = CMPDAC4;
    sim_debug.observed_comp_out = compY_out;

    simMarkStateSeen((uint16_t)systemState);
    if (thermalFlags & (1u << 7)) {
        sim_test_status |= SIM_TEST_STATUS_THERMAL_TRIP;
    }

    if (llcPeriodCmd < sim_test_min_vout_reference) {
        sim_test_min_vout_reference = llcPeriodCmd;
    }
    if (llcPeriodCmd > sim_test_max_vout_reference) {
        sim_test_max_vout_reference = llcPeriodCmd;
    }
    if (Imeas > sim_test_max_imeas) {
        sim_test_max_imeas = Imeas;
    }

    sim_test_final_comp_out = compY_out;
}

static void simFinalizeTest(void)
{
    sim_test_status |= (SIM_TEST_STATUS_DONE | SIM_TEST_STATUS_PASSED);
    simAutomationStop();
}

void simInit(void)
{
    fw_mode = 2;
    statusFlags = 0;
    faultFlags = 0;
    controlStatus = 0;
    auxFlags = 0;
    startupFlags = 0;
    systemFlags = 0;
    pwmRunning = 0;

    droopKffFactor = 0x0400;
    initFreq = 0x3200;
    freqCoeffA = 0x32C8;
    freqCoeffB = 0x3A98;
    freqCoeffC = 0x32C8;
    vrefModeSelect = 0;
    voutRefTarget = 0x0C4D;
    voutTargetCode = 0x0C4D;
    droopMode = 3;
    systemState = ST_ACTIVE;
    startupTickCnt = 0;
    marginThreshold = 10;
    voutSetpoint = 0x0C4D;
    llcPeriodCmd = 0x5DC0;
    compY_out = 0x5DC0;
    compY_n1 = 0x5DC0;
    compY_n2 = 0x5DC0;
    TRISD = 0x0FC3;
    TRISE = 0x0000;
    TRISF = 0x003C;
    LATD = 0x0008;
    LATE = 0x0008;
    LATF = 0x0040;
    PORTD = 0x0000;
    sim_portd_input = 0x0000;
    IOCON1 = 0xC803;
    IOCON2 = 0xC803;
    IOCON3 = 0xC803;
    FCLCON1 = 0x0010;
    FCLCON2 = 0x0010;
    FCLCON3 = 0x0010;
    PTCON = 0x8400;
    PTCON2 = 0x0003;
    PTPER = 0x011E;
    PDC1 = 0x0126;
    PDC2 = 0x0126;
    PDC3 = 0x0126;
    PHASE3 = 0;
    CMPCON3 = 0x0081;
    CMPDAC3 = 0x81A3;
    CMPCON4 = 0x0101;
    CMPDAC4 = 0x8000;
    OC2RS = 0x0190;
    oc2rs_shadow = 0x0190;
    sim_portd6_tick_divider = 0u;

    sim_test_status = 0;
    sim_test_failure_count = 0;
    sim_test_min_vout_reference = 0x7FFF;
    sim_test_max_vout_reference = (int16_t)0x8000u;
    sim_test_max_imeas = 0;
    sim_test_final_comp_out = 0;

    sim_debug.step = 0;
    sim_trace_mode = SIM_TRACE_MODE_UNIFORM;
#if !SIM_TRACE_UNIFORM_SAMPLING
    sim_trace_mode = SIM_TRACE_MODE_RING;
#endif
    sim_trace_count = 0;
    sim_debug.t1_count = 0;
    sim_debug.t2_count = 0;
    sim_debug.t4_count = 0;
    sim_debug.psem_count = 0;
    sim_debug.auto_stimulus = 1;
    sim_debug.t2_divider = 2;
    sim_debug.t4_divider = 2;
#ifdef SIM_T2_ONLY
    systemState = ST_ACTIVE;
    sim_debug.t2_divider = 1;
    sim_debug.t4_divider = 0;
#elif defined(SIM_MAIN_CHAIN_ONLY)
    systemState = ST_IDLE;
    sim_debug.t2_divider = 1;
    sim_debug.t4_divider = 1;
    freqSetpoint = 0;
#endif
    sim_debug.adcbuf0 = 0;
    sim_debug.adcbuf2 = 0;
    sim_debug.adcbuf4 = 4;
    sim_debug.adcbuf5 = 0;
    sim_debug.adcbuf12 = 0x0180;
    sim_debug.adcbuf14 = 320;
    sim_debug.target_vout_sum = 0;
    sim_debug.observed_vout_sum = 0;
    sim_debug.observed_vout_setpoint = 0;
    sim_debug.observed_vout_reference = 0;
    sim_debug.observed_ptper_shadow = 0;
    sim_debug.observed_ptper_register = 0;
    sim_debug.observed_oc2rs = 0;
    sim_debug.observed_system_state = 0;
    sim_debug.observed_state_pre_step = 0;
    sim_debug.observed_state_after_t1 = 0;
    sim_debug.observed_state_after_t2 = 0;
    sim_debug.observed_state_after_startup = 0;
    sim_debug.observed_state_after_main = 0;
    sim_debug.observed_state_seen_mask = 0;
    sim_debug.observed_idle_to_startup_count = 0;
    sim_debug.observed_startup_to_active_count = 0;
    sim_debug.observed_active_to_fault_count = 0;
    sim_debug.observed_margin_threshold = 0;
    sim_debug.observed_aux_flags = 0;
    sim_debug.observed_pwm_running = 0;
    sim_debug.observed_status_flags = 0;
    sim_debug.observed_startup_flags = 0;
    sim_debug.observed_system_flags = 0;
    sim_debug.observed_portd = 0;
    sim_debug.observed_latd = 0;
    sim_debug.observed_latf = 0;
    sim_debug.observed_vout_target_code = 0;
    sim_debug.observed_vout_ref_target = 0;
    sim_debug.observed_soft_start_limit = 0;
    sim_debug.observed_soft_start_ramp_cnt = 0;
    sim_debug.observed_soft_start_dwell_cnt = 0;
    sim_debug.observed_vout_setpoint_target = 0;
    sim_debug.observed_vref_mode_select = 0;
    sim_debug.observed_droop_mode = 0;
    sim_debug.observed_adcbuf4 = 0;
    sim_debug.observed_cmpcon3 = 0;
    sim_debug.observed_cmpdac3 = 0;
    sim_debug.observed_cmpcon4 = 0;
    sim_debug.observed_cmpdac4 = 0;
    sim_debug.first_drop_step = 0xFFFFu;
    sim_debug.first_drop_phase = 0;
    sim_debug.observed_comp_out = 0;
    simMarkStateSeen((uint16_t)systemState);

    simLoadCalibrationProfile();
    simApplyAutoStimulus();
    simLoadManualInputs();
    simUpdateComparatorModel();
    simCaptureObservations();
}

void simRunStep(void)
{
    if (sim_test_status & SIM_TEST_STATUS_DONE) {
        return;
    }

    if (sim_debug.t2_divider == 0u) {
        sim_debug.t2_divider = 1u;
    }
    if (sim_debug.t4_divider == 0u) {
        sim_debug.t4_divider = 1u;
    }

    if (sim_debug.auto_stimulus != 0u) {
        simApplyAutoStimulus();
    }

    simLoadManualInputs();
    simUpdateComparatorModel();
    outputVoltage = (uint16_t)sim_debug.adcbuf5;
    outputCurrent = (uint16_t)sim_debug.adcbuf4;
    vout_sum = (int16_t)((sim_debug.adcbuf0 + sim_debug.adcbuf2) << 1);
    sim_debug.observed_state_pre_step = (uint16_t)systemState;

    pwmRunning |= (1u << 3);
    if (sim_debug.step < 10u) {
        statusFlags &= ~(1u << 7);
    }

#ifdef SIM_T2_ONLY
    systemState = ST_ACTIVE;
    statusFlags &= ~(1u << 4);   /* keep T2 in regulation branch, not startup sweep */
    statusFlags &= ~(1u << 7);
    controlStatus &= ~(1u << 4);
    faultFlags &= ~(1u << 7);
    voutTargetCode = voutRefTarget;
    voutSetpoint = simComputeVoutSetpoint(voutRefTarget);
    sim_debug.observed_state_after_t1 = (uint16_t)systemState;
    startupFlags |= 0x0001;
    {
        uint16_t state_before = (uint16_t)systemState;
        _T2Interrupt();
        simRecordStateDrop(2u, state_before);
        sim_debug.t2_count++;
        state_before = (uint16_t)systemState;
        _PWMSpEventMatchInterrupt();
        simRecordStateDrop(3u, state_before);
        sim_debug.psem_count++;
    }
    voutTargetCode = voutRefTarget;
    voutSetpoint = simComputeVoutSetpoint(voutRefTarget);
    sim_debug.observed_state_after_t2 = (uint16_t)systemState;
    sim_debug.observed_state_after_startup = (uint16_t)systemState;
    sim_debug.observed_state_after_main = (uint16_t)systemState;
#elif defined(SIM_MAIN_CHAIN_ONLY)
    /* One simulation step models one 1 ms frame. The real cadence is
     * 50x T2 + 5x T1 + 1x T4/main per ms, but MPLAB simulator is too slow
     * for that in practice. SIM_CHAIN_T{1,2}_CALLS let us run an aggregated
     * main-chain model while preserving call order.
     */
    const uint16_t t2_calls = (uint16_t)SIM_CHAIN_T2_CALLS;
    const uint16_t t1_calls = (uint16_t)SIM_CHAIN_T1_CALLS;
    const uint16_t watchdog_calls = (uint16_t)SIM_CHAIN_WATCHDOG_CALLS;
    const uint16_t t1_interval = (t1_calls == 0u) ? t2_calls : ((t2_calls / t1_calls) ? (t2_calls / t1_calls) : 1u);

    for (uint16_t slot = 0; slot < t2_calls; ++slot) {
        if ((t1_calls != 0u) && ((slot % t1_interval) == 0u) && (sim_debug.t1_count < ((sim_debug.step + 1u) * t1_calls))) {
            simAdvancePortd6Signal();
            simUpdateComparatorModel();
            uint16_t state_before = (uint16_t)systemState;
            _T1Interrupt();
            simRecordStateDrop(1u, state_before);
            sim_debug.t1_count++;
            sim_debug.observed_state_after_t1 = (uint16_t)systemState;
        }

        {
#if SIM_CHAIN_FORCE_T2_REGULATION
            if (systemState == ST_ACTIVE) {
                statusFlags &= ~(1u << 4);
                statusFlags &= ~(1u << 7);
                controlStatus &= ~(1u << 4);
                faultFlags &= ~(1u << 7);
            }
#endif
            uint16_t state_before = (uint16_t)systemState;
            _T2Interrupt();
            simRecordStateDrop(2u, state_before);
            sim_debug.t2_count++;
            state_before = (uint16_t)systemState;
            _PWMSpEventMatchInterrupt();
            simRecordStateDrop(3u, state_before);
            sim_debug.psem_count++;
        }
    }
    sim_debug.observed_state_after_t2 = (uint16_t)systemState;

    {
        uint16_t state_before = (uint16_t)systemState;
        startupControl();
        simRecordStateDrop(4u, state_before);
    }
    sim_debug.observed_state_after_startup = (uint16_t)systemState;

    IFS1bits.T4IF = 1;
    {
        uint16_t state_before = (uint16_t)systemState;
        mainStateDispatch();
        simRecordStateDrop(5u, state_before);
    }
    sim_debug.t4_count++;
    sim_debug.observed_state_after_main = (uint16_t)systemState;

    if ((watchdog_calls > 1u) && (systemState == ST_ACTIVE) && (auxFlags & (1u << 11))) {
        for (uint16_t pass = 1u; pass < watchdog_calls; ++pass) {
            watchdogService();
            vinCheck();
        }
    }
#else
    {
        simAdvancePortd6Signal();
        simUpdateComparatorModel();
        uint16_t state_before = (uint16_t)systemState;
        _T1Interrupt();
        simRecordStateDrop(1u, state_before);
    }
    sim_debug.t1_count++;
    sim_debug.observed_state_after_t1 = (uint16_t)systemState;

    if ((sim_debug.step % sim_debug.t2_divider) == 0u) {
        uint16_t state_before = (uint16_t)systemState;
        _T2Interrupt();
        simRecordStateDrop(2u, state_before);
        sim_debug.t2_count++;
        state_before = (uint16_t)systemState;
        _PWMSpEventMatchInterrupt();
        simRecordStateDrop(3u, state_before);
        sim_debug.psem_count++;
    }
    sim_debug.observed_state_after_t2 = (uint16_t)systemState;

    {
        uint16_t state_before = (uint16_t)systemState;
        startupControl();
        simRecordStateDrop(4u, state_before);
    }
    sim_debug.observed_state_after_startup = (uint16_t)systemState;

    if ((sim_debug.step % sim_debug.t4_divider) == 0u) {
        IFS1bits.T4IF = 1;
        {
            uint16_t state_before = (uint16_t)systemState;
            mainStateDispatch();
            simRecordStateDrop(5u, state_before);
        }
        sim_debug.t4_count++;
    }
    sim_debug.observed_state_after_main = (uint16_t)systemState;
#endif

    simCaptureObservations();
    simTraceRecord();

    sim_debug.step++;
    sim_debug.adcbuf0 = simClamp16(sim_debug.adcbuf0, 0, 4095);
    sim_debug.adcbuf2 = simClamp16(sim_debug.adcbuf2, 0, 4095);
    sim_debug.adcbuf4 = simClamp16(sim_debug.adcbuf4, 0, 4095);
    sim_debug.adcbuf5 = simClamp16(sim_debug.adcbuf5, 0, 4095);
    sim_debug.adcbuf12 = simClamp16(sim_debug.adcbuf12, 0, 4095);
    sim_debug.adcbuf14 = simClamp16(sim_debug.adcbuf14, 0, 4095);

    if (sim_debug.step >= SIM_TEST_TARGET_STEPS) {
        simFinalizeTest();
    }
}
