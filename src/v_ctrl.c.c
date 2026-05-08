#include <xc.h>
#include <libpic30.h>
#include "p33Fxxxx.h"
#include "define.h"

// =================================================================
// HSTNS-PD44 800W LLC ADC Voltage Sampling & Calibration
// FUN_rom_002afc, TIMER1 ISR, called from FUN_rom_0037f6:00381a
//
// Dual-channel voltage ADC processing with 64-point sliding window:
//
//   Channel A (DAT_ram_034a, likely AN5):
//     ? [64-entry ring @ 0x880] ? sum >> 6 = avg
//     ? × DAT_ram_1d2e >> 13 + DAT_ram_1d2c ? DAT_ram_1260 (Va)
//
//   Channel B (DAT_ram_0346, likely AN3):
//     ? [64-entry ring @ 0x800] ? sum >> 6 = avg
//     ? × DAT_ram_1d2a >> 13 + DAT_ram_1d28 ? DAT_ram_125e (Vb)
//
//   DAT_ram_126c = Va - Vb (differential)
//
// Ring buffer index DAT_ram_1252 is shared with current ADC function.
// FUN_rom_001100 = __mulsi3
// =================================================================

int16_t vbuf_a[64];         // DAT_ram_0880..DAT_ram_0900, channel A ring
int16_t vbuf_b[64];         // DAT_ram_0800..DAT_ram_0880, channel B ring
int16_t buf_idx;            // DAT_ram_1252, shared ring index
int16_t adc_ch_a;           // DAT_ram_034a, raw ADC channel A
int16_t adc_ch_b;           // DAT_ram_0346, raw ADC channel B
int16_t cal_va;             // DAT_ram_1d2e, calibration coeff A
int16_t ofs_va;             // DAT_ram_1d2c, offset A
int16_t cal_vb;             // DAT_ram_1d2a, calibration coeff B
int16_t ofs_vb;             // DAT_ram_1d28, offset B
int16_t volt_a;             // DAT_ram_1260, calibrated voltage A
int16_t volt_b;             // DAT_ram_125e, calibrated voltage B
int16_t volt_diff;          // DAT_ram_126c, Va - Vb
int16_t vbuf_b_sum;         // DAT_ram_1d3c, raw sum of channel B

void llc_adc_voltage_sample(void)
{
    int16_t idx = buf_idx;                                  // DAT_ram_1252

    // =========================================================
    // Channel A: DAT_ram_034a ? 64-point avg ? calibrate ? Va
    // =========================================================
    vbuf_a[idx] = adc_ch_a;                                 // store to ring

    int16_t sum_a = 0;                                      // DAT_ram_1d3c cleared
    for (int i = 0; i < 64; i++)
        sum_a += vbuf_a[i];

    int16_t avg_a = sum_a >> 6;                             // /64
    // × cal_va >> 13 + offset
    int16_t Va = (int16_t)(__mulsi3((int32_t)avg_a, (int32_t)cal_va) >> 13) + ofs_va;
    volt_a = Va;                                            // DAT_ram_1260

    // =========================================================
    // Channel B: DAT_ram_0346 ? 64-point avg ? calibrate ? Vb
    // =========================================================
    vbuf_b[idx] = adc_ch_b;                                 // store to ring

    int16_t sum_b = 0;                                      // DAT_ram_1d3c cleared again
    for (int i = 0; i < 64; i++)
        sum_b += vbuf_b[i];
    vbuf_b_sum = sum_b;                                     // DAT_ram_1d3c

    int16_t avg_b = sum_b >> 6;                             // /64
    // × cal_vb >> 13 + offset
    int16_t Vb = (int16_t)(__mulsi3((int32_t)avg_b, (int32_t)cal_vb) >> 13) + ofs_vb;
    volt_b = Vb;                                            // DAT_ram_125e

    // =========================================================
    // Differential: Va - Vb
    // =========================================================
    volt_diff = Va - Vb;                                    // DAT_ram_126c
}



// =================================================================
// HSTNS-PD44 LLC Voltage Calibration & OVP Shutdown
// FUN_rom_00444c, called from TIMER2 ISR (_T2Interrupt:004b2c)
//
// Three functions in one:
//   1) Calibrate AN5 voltage sample ? vout_cal (DAT_ram_1d66)
//   2) OVP detection: voltage exceeds threshold ? activate protection
//   3) OVP shutdown: count down then kill PWM and transition to fault mode
//
// Called every TIMER2 cycle (50kHz), provides fast OVP response.
//
// Key variables:
//   DAT_ram_1d9e = vout_2cycle_sum (AN0+prev_AN0+AN2+prev_AN2)
//   DAT_ram_034a = AN5 raw ADC (Vout sense)
//   DAT_ram_1d2e = cal_coeff (0x200F)
//   DAT_ram_1d2c = cal_offset (0x0001)
//   DAT_ram_1d66 = vout_cal (calibrated voltage)
//   DAT_ram_1268 = droop_mode
//   DAT_ram_1e20 = ovp_flags (bit 4 = OVP active)
//   DAT_ram_1dca = ovp_counter
//   DAT_ram_1e22 = op_mode (2 = normal run)
//   DAT_ram_1da4 = freq_ctrl
//   DAT_ram_125a = state_flags (bit 4 = startup enabled)
//   DAT_ram_1d70 = PDC1 target
//   DAT_ram_1d6e = PDC2 target
//   DAT_ram_1d6c = PDC3 target
//   DAT_ram_1e0e = protection timeout
//   DAT_ram_1bf0 = shutdown request flags
//   DAT_ram_1bf2 = run flags
//   DAT_ram_192a = fault latch
// =================================================================

#define VOUT_CAL_COEFF  0x200F  // 8207, gain ? 1.0018
#define VOUT_CAL_OFFSET 1

// --- ADC ---
int16_t adc_4pt_sum;       // DAT_ram_1d9e, AN0+prev_AN0+AN2+prev_AN2

// --- Calibration ---
static const int16_t cal_coeff = VOUT_CAL_COEFF;          // DAT_ram_1d2e, = 0x200F (8207)
static const int16_t cal_offset = VOUT_CAL_OFFSET;         // DAT_ram_1d2c, = 0x0001
int16_t vout_cal;           // DAT_ram_1d66, calibrated voltage output
uint16_t ovp_counter;        // DAT_ram_1dca, OVP shutdown confirmation counter

extern volatile uint16_t systemState;
extern volatile uint16_t statusFlags;
extern volatile uint16_t controlStatus;
extern volatile uint16_t droopMode;
extern volatile uint16_t faultResetTimer;
extern s16 u_exec;
extern s16 pdc1;
extern s16 pdc2;
extern s16 pdc3;
extern void llcFaultShutdown(uint16_t control_flags);
 

void llc_voltage_cal_ovp(void)
{
    // ==========================================================
    // Step 1: Calibrate AN5 voltage, compare with averaged Vout
    // ==========================================================
    // v_avg = 4-sample sum >> 2 (average of AN0+AN2 over 2 cycles)
    int16_t v_avg = adc_4pt_sum >> 2;           // W8

    // AN5 × cal_coeff >> 13 + cal_offset
    // cal_coeff = 0x200F (8207), cal_offset = 1
    // effectively vout_cal ? AN5 (gain ? 1.0018)
    int16_t adc_an5 = ADCBUF5;
    int16_t v_cal = (int16_t)(__mulsi3(
        (int32_t)adc_an5,                           // DAT_ram_034a, sign-extended W1:W0
        (int32_t)cal_coeff                          // DAT_ram_1d2e, sign-extended W3:W2
    ) >> 13) + cal_offset;                          // DAT_ram_1d2c

    vout_cal = v_cal;                               // DAT_ram_1d66

    // peak detect: use whichever is higher
    // if v_avg < v_cal ? use v_cal (instantaneous is higher)
    if (v_avg < v_cal)
        v_avg = v_cal;                              // W8 = max(v_avg, v_cal)

    // ==========================================================
    // Step 2: OVP threshold selection (mode-dependent)
    // ==========================================================
    // mode 4: lower threshold (tighter protection)
    //   0x1DF = 479 ? ~7.6V (different output config?)
    // other modes: normal threshold
    //   0x36F = 879 ? ~13.9V (~13% above 12.3V nominal)
    int16_t ovp_thresh = 0x36F;                     // default: 879
    if (droopMode == 4)
        ovp_thresh = OVP_THRESHOLD_MODE4;
    else
        ovp_thresh = OVP_THRESHOLD_NORMAL;

    // ==========================================================
    // Step 3: OVP activation and shutdown sequencing
    // ==========================================================
    if (controlStatus & OVP_ARM_FLAG) {
        uint16_t cnt = ovp_counter + 1;
        ovp_counter = cnt;

        if (systemState == 2) {           // DAT_ram_1e22 = normal run
            pdc1 = 0;
            pdc2 = 0;
            pdc3 = 0;

            cnt += 2;                                  // original 0x444C accelerated count
            ovp_counter = cnt;
            if (cnt > 1) {
                ovp_counter = 1;
                llcFaultShutdown(0);
            }
        } else {
            ovp_counter = 0;
        }
        return;
    }

    if (v_avg > ovp_thresh) {
        if ((u_exec >= OVP_FREQ_CTRL_MIN) || (statusFlags & STATUS_FLAG_STARTUP)) {
            pdc1 = 0;
            pdc2 = 0;
            pdc3 = 0;
            controlStatus |= OVP_ARM_FLAG;
            ovp_counter = 0;
            faultResetTimer = 0x00C8;
        } else {
            ovp_counter = 0;
        }
        return;
    }

    ovp_counter = 0;
 
}

// =================================================================
// OVP Activation Conditions:
//
//   v_avg = max(vout_2cycle_avg, vout_cal_an5)
//
//   if (v_avg >= ovp_thresh):
//     if (freq_ctrl <= 0x1F3F):
//       ? ACTIVATE (normal operating freq, real fault)
//     elif (startup_enabled):
//       ? ACTIVATE (startup + high voltage = fault)
//     else:
//       ? IGNORE (system not running)
//
// OVP Shutdown Timeline (at 50kHz TIMER2):
//   Cycle 0: OVP detected
//            ? PDC1/2/3 = 0, ovp_flags bit4 set, counter = 0
//   Cycle 1: counter = 0+1+2 = 3, PDC1/2/3 = 0 again
//            ? counter(3) > 1 ? shutdown confirmed
//            ? llc_pwm_force_off(), op_mode = 3 (fault)
//
// Total OVP response: ~1 cycle × 20us = 20us from detection to full shutdown
//
// OVP Thresholds:
//   Normal:  879 counts ? 879/1024 × 3.3V / 0.2073 = 13.9V (+13% overvoltage)
//   Mode 4:  479 counts ? 479/1024 × 3.3V / 0.2073 = 7.6V
//
// Calibration (DAT_ram_1d2e=0x200F, DAT_ram_1d2c=1):
//   vout_cal = AN5 × 8207 >> 13 + 1 ? AN5 × 1.0018 + 1
//   Nearly 1:1 mapping, minimal correction
// =================================================================