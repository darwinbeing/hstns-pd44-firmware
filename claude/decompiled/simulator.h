#ifndef HSTNS_PD44_SIMULATOR_H
#define HSTNS_PD44_SIMULATOR_H

#include <stdint.h>

#ifndef SIM_TRACE_MAX_STEPS
#define SIM_TRACE_MAX_STEPS 64u
#endif

typedef struct {
    volatile uint16_t step;
    volatile uint16_t t1_count;
    volatile uint16_t t2_count;
    volatile uint16_t t4_count;
    volatile uint16_t psem_count;
    volatile uint16_t auto_stimulus;
    volatile uint16_t t2_divider;
    volatile uint16_t t4_divider;
    volatile int16_t adcbuf0;
    volatile int16_t adcbuf2;
    volatile int16_t adcbuf4;
    volatile int16_t adcbuf5;
    volatile int16_t adcbuf12;
    volatile int16_t adcbuf14;
    volatile int16_t target_vout_sum;
    volatile int16_t observed_vout_sum;
    volatile int16_t observed_vout_setpoint;
    volatile int16_t observed_vout_setpoint_target;
    volatile int16_t observed_vout_reference;
    volatile int16_t observed_ptper_shadow;
    volatile int16_t observed_ptper_register;
    volatile int16_t observed_oc2rs;
    volatile uint16_t observed_system_state;
    volatile uint16_t observed_state_pre_step;
    volatile uint16_t observed_state_after_t1;
    volatile uint16_t observed_state_after_t2;
    volatile uint16_t observed_state_after_startup;
    volatile uint16_t observed_state_after_main;
    volatile int16_t observed_margin_threshold;
    volatile uint16_t observed_aux_flags;
    volatile uint16_t observed_pwm_running;
    volatile uint16_t observed_status_flags;
    volatile uint16_t observed_startup_flags;
    volatile uint16_t observed_system_flags;
    volatile uint16_t observed_portd;
    volatile uint16_t observed_latd;
    volatile uint16_t observed_latf;
    volatile int16_t observed_vout_target_code;
    volatile int16_t observed_vout_ref_target;
    volatile uint16_t observed_soft_start_limit;
    volatile uint16_t observed_soft_start_ramp_cnt;
    volatile uint16_t observed_soft_start_dwell_cnt;
    volatile int16_t observed_vref_mode_select;
    volatile uint16_t observed_droop_mode;
    volatile int16_t observed_adcbuf4;
    volatile uint16_t observed_cmpcon3;
    volatile uint16_t observed_cmpdac3;
    volatile uint16_t observed_cmpcon4;
    volatile uint16_t observed_cmpdac4;
    volatile uint16_t first_drop_step;
    volatile uint16_t first_drop_phase;
    volatile int32_t observed_comp_out;
} sim_debug_t;

extern volatile sim_debug_t sim_debug;
extern volatile uint16_t sim_trace_mode;
extern volatile uint16_t sim_trace_count;
extern volatile uint16_t sim_trace_step[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_an0[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_an2[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_vout_sum[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_vout_setpoint[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_llc_period_cmd[SIM_TRACE_MAX_STEPS];
extern volatile int32_t sim_trace_comp_y_out[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_state[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_ptper[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_margin_threshold[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_aux_flags[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_pwm_running[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_status_flags[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_startup_flags[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_system_flags[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_portd[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_latd[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_latf[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_vout_target_code[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_vref_mode_select[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_droop_mode[SIM_TRACE_MAX_STEPS];
extern volatile int16_t sim_trace_adcbuf4[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_cmpcon3[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_cmpdac3[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_cmpcon4[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_trace_cmpdac4[SIM_TRACE_MAX_STEPS];
extern volatile uint16_t sim_portd_input;
extern volatile uint16_t sim_test_status;
extern volatile uint16_t sim_test_failure_count;
extern volatile int16_t sim_test_min_vout_reference;
extern volatile int16_t sim_test_max_vout_reference;
extern volatile int16_t sim_test_max_imeas;
extern volatile int32_t sim_test_final_comp_out;

void simInit(void);
void simRunStep(void);
void simAutomationStop(void);

#define SIM_TEST_STATUS_DONE          0x0001u
#define SIM_TEST_STATUS_PASSED        0x0002u
#define SIM_TEST_STATUS_THERMAL_TRIP  0x0004u
#define SIM_TEST_STATUS_SAW_ACTIVE    0x0008u
#define SIM_TEST_STATUS_SAW_FAULT     0x0010u

#define SIM_TRACE_MODE_RING           0u
#define SIM_TRACE_MODE_UNIFORM        1u

#endif
