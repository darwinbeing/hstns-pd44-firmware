#include <xc.h>
#include <libpic30.h>
#include <stdint.h>

#include "p33Fxxxx.h"
#include "define.h"



// =================================================================
// HSTNS-PD44 LLC Fast Current Average (AN4, 8-point)
// FUN_rom_0043d4, called from TIMER2 ISR (_T2Interrupt:004b2a)
//
// 8-point sliding window of AN4 current sense.
// Result DAT_ram_1d46 used for 2P2Z coefficient and dead-time adapt.
// =================================================================

int16_t iout_adc;          // DAT_ram_0348, AN4 raw
int16_t iout_buf[8];       // DAT_ram_1db4..DAT_ram_1dc2
int16_t iout_buf_idx;      // DAT_ram_1dc4, ring index 0-7
int16_t iout_sum;          // DAT_ram_1d68, running sum
int16_t iout_avg;          // DAT_ram_1d46, = sum >> 3
int16_t iout_raw;          // DAT_ram_1d94, latest snapshot

void llc_current_fast_avg(void)
{
    int16_t sample = ADCBUF4;                  // DAT_ram_0348
    iout_raw = sample;                          // DAT_ram_1d94

    iout_buf[iout_buf_idx] = sample;            // store to ring
    iout_sum += sample;                         // running sum += new
    iout_avg = iout_sum >> 3;                   // DAT_ram_1d46 = /8

    iout_buf_idx++;                             // advance
    if (iout_buf_idx > 7) iout_buf_idx = 0;    // wrap at 8

    iout_sum -= iout_buf[iout_buf_idx];         // running sum -= oldest
}

// =================================================================
// HSTNS-PD44 LLC ADC Current Sampling
// FUN_rom_002b70, called from TIMER1 ISR (FUN_rom_0037f6:00381c)
//
// Multi-stage current measurement pipeline:
//   1) Store AN4 raw sample into 64-entry ring buffer
//   2) Sum all 64 entries ? 64-point accumulator
//   3) Dual calibration: same raw sum, two different cal paths
//      - cal_a ? DAT_ram_1e4e (used in long-term average)
//      - cal_b ? DAT_ram_1d44 (Imeas, used in OCP foldback)
//   4) Imeas scaled ? DAT_ram_1d42
//   5) 1024-point long-term average of cal_a ? DAT_ram_1d1e
//
// This runs at TIMER1 rate (5kHz), much slower than the 8-point
// fast filter in TIMER2 (llc_current_fast_avg at 50kHz).
//
// Key variables:
//   DAT_ram_0348 = AN4 raw ADC (current sense)
//   DAT_ram_1252 = ring buffer write index (0-63)
//   DAT_ram_110a..1189 = 64-entry ring buffer (32-bit entries)
//   DAT_ram_1258:1256 = 64-point sum (32-bit accumulator)
//   DAT_ram_1d26 = cal_a_gain (calibration coefficient A)
//   DAT_ram_1d24 = cal_a_offset (calibration offset A)
//   DAT_ram_1e4e = Imeas_cal_a (calibrated current A)
//   DAT_ram_1d46 = iout_avg (AN4 8-point average from TIMER2)
//   DAT_ram_1106 = Imeas_cal_b_diag (diagnostic copy)
//   DAT_ram_1d44 = Imeas (calibrated current B, used by OCP)
//   DAT_ram_1d42 = Imeas_scaled (Imeas × 0x1E80 >> 10)
//   DAT_ram_1100 = long-term ring index (0-1023)
//   DAT_ram_0900..0CFF = 1024-entry ring buffer for long-term avg
//   DAT_ram_1104:1102 = 1024-point running sum (32-bit)
//   DAT_ram_1d1e = long-term average (sum >> 10)
// =================================================================
 
 
// --- ADC ---
int16_t  adc_an4;               // DAT_ram_0348, AN4 raw
extern int16_t  iout_avg;              // DAT_ram_1d46, 8-point avg from TIMER2
 
// --- 64-point ring buffer ---
int16_t  ring64_idx;            // DAT_ram_1252, write index 0-63
int32_t  ring64_buf[64];        // DAT_ram_110a..1189
int32_t  ring64_sum;            // DAT_ram_1258:1256, running sum
 
// --- Calibration A ---
int16_t  cal_a_gain = 0x2030;            // DAT_ram_1d26
int16_t  cal_a_offset = 0xFFF8;          // DAT_ram_1d24
int16_t  Imeas_cal_a;           // DAT_ram_1e4e, output of cal path A
int16_t  Imeas_cal_a_diag;      // DAT_ram_1108, diagnostic
 
// --- Calibration B (? OCP) ---
int16_t  Imeas_cal_b_diag;      // DAT_ram_1106, diagnostic
int16_t  Imeas;                 // DAT_ram_1d44, main current measurement
int16_t  Imeas_scaled;          // DAT_ram_1d42, Imeas × 7.625
 
// --- 1024-point long-term average ---
int16_t  ring1k_idx;            // DAT_ram_1100, write index 0-1023
int16_t  ring1k_buf[1024];      // DAT_ram_0900..0CFF
int32_t  ring1k_sum;            // DAT_ram_1104:1102, running sum
int16_t  Imeas_longavg;         // DAT_ram_1d1e, long-term average
 
// --- Library ---
extern int32_t __mulsi3(int32_t a, int32_t b);
 
void llc_adc_current_sample(void)
{
    // ==========================================================
    // Step 1: Store AN4 into 64-entry ring buffer
    // ==========================================================
    // Ring buffer stores 32-bit entries (AN4 zero-extended to 32-bit)
    //
    // Register trace:
    //   W0 = DAT_ram_0348 (AN4)
    //   W2 = DAT_ram_1252 (index), W2 <<= 2 (×4 for 32-bit stride)
    //   W3 = 0x110a (buffer base)
    //   [W2+W3] = W1:W0 (store 32-bit, W1=0)
 
    ring64_buf[ring64_idx] = (int32_t)ADCBUF4;
 
    // ==========================================================
    // Step 2: Sum all 64 entries
    // ==========================================================
    // Full re-sum (not sliding window subtract):
    //   clear accum, loop through all 64 entries
    //
    // Register trace:
    //   W5:W4 = 0                               (mul.uu W4,#0,W4)
    //   W0 = 0x110a (start), W1 = 0x120a (end)
    //   loop: W5:W4 += [W0++]                   (add/addc with post-increment)
    //   until W0 == W1
    //   DAT_ram_1258:1256 = W3:W2 = W5:W4
 
    int32_t sum64 = 0;
    for (int i = 0; i < 64; i++)
        sum64 += ring64_buf[i];
    ring64_sum = sum64;
 
    // ==========================================================
    // Step 3: Calibration path A
    // ==========================================================
    // raw = sum64 >> 6 (divide by 64 = average)
    // cal_a = raw × cal_a_gain >> 13 + cal_a_offset
    //
    // Register trace:
    //   W0 = sum64 >> 6                         (sl W5,#10 | lsr W4,#6)
    //   W1 = 0 (zero extend for unsigned input)
    //   W9:W8 = sign_extend(DAT_ram_1d26)       (mul.su W0,#1)
    //   W10 = DAT_ram_1d24                       (offset)
    //   W1:W0 = __mulsi3(raw, cal_a_gain)
    //   W0 = (W1:W0) >> 13
    //   W0 = W10 + W0                            (add offset)
    //   DAT_ram_1108 = W0                         (diagnostic)
 
    int16_t avg64 = (int16_t)(sum64 >> 6);
    int16_t cal_a_result = (int16_t)(__mulsi3(
        (int32_t)avg64,
        (int32_t)cal_a_gain
    ) >> 13) + cal_a_offset;
 
    Imeas_cal_a_diag = cal_a_result;                // DAT_ram_1108
 
    // Clamp: if negative ? 0
    //   bra nn ? skip clear (nn = not negative)
    if (cal_a_result < 0)
        Imeas_cal_a = 0;
    else
        Imeas_cal_a = cal_a_result;                 // DAT_ram_1e4e
 
    // ==========================================================
    // Step 4: Calibration path B (? Imeas for OCP)
    // ==========================================================
    // Same cal_a_gain/offset, but applied to iout_avg (8-point
    // from TIMER2) instead of 64-point avg. Faster response for OCP.
    //
    // Register trace:
    //   W0 = DAT_ram_1d46 (iout_avg)
    //   W1 = sign_extend(W0)                    (asr W0,#15,W1)
    //   W3:W2 = W9:W8 (cal_a_gain)
    //   W1:W0 = __mulsi3(iout_avg, cal_a_gain)
    //   W0 = (W1:W0) >> 13 + W10 (offset)
    //   DAT_ram_1106 = W0 (diagnostic)
 
    int16_t cal_b_result = (int16_t)(__mulsi3(
        (int32_t)iout_avg,
        (int32_t)cal_a_gain
    ) >> 13) + cal_a_offset;
 
    Imeas_cal_b_diag = cal_b_result;                // DAT_ram_1106
 
    // Clamp: if negative ? 0
    if (cal_b_result < 0)
        Imeas = 0;
    else
        Imeas = cal_b_result;                       // DAT_ram_1d44
 
    // ==========================================================
    // Step 5: Scale Imeas for external reporting
    // ==========================================================
    // Imeas × 0x1E80 >> 10 = Imeas × 7.625
    // Probably converts ADC counts to physical unit (mA or 0.1A)
    //
    // Register trace:
    //   W2 = 0x1E80 (7808)
    //   W1:W0 = DAT_ram_1d44 × W2              (mul.ss)
    //   W0 = (W1:W0) >> 10                      (sl/lsr/ior pattern)
 
    Imeas_scaled = (int16_t)(((int32_t)Imeas * 0x1E80) >> 10);  // DAT_ram_1d42
 
    // ==========================================================
    // Step 6: 1024-point long-term average of cal_a
    // ==========================================================
    // Sliding window: store cal_a into 1024-entry ring buffer,
    // maintain running sum, subtract oldest when wrapping.
    //
    // Register trace:
    //   W2 = DAT_ram_1100 (ring index)
    //   W6 = 0x0900 (buffer base)
    //   [W2*2 + W6] = W3 (cal_a_result)         (store to ring)
    //
    //   running sum += cal_a
    //   W5:W4 = DAT_ram_1104:1102 + W3
    //
    //   long-term avg = running sum >> 10
    //   DAT_ram_1d1e = W0
 
    ring1k_buf[ring1k_idx] = Imeas_cal_a;
 
    ring1k_sum += (int32_t)Imeas_cal_a;
 
    Imeas_longavg = (int16_t)(ring1k_sum >> 10);    // DAT_ram_1d1e
 
    // Advance index, wrap at 1024
    ring1k_idx++;
    if (ring1k_idx > 0x3FF)
        ring1k_idx = 0;
 
    // Subtract oldest entry (sliding window)
    ring1k_sum -= (int32_t)ring1k_buf[ring1k_idx];
}
 
// =================================================================
// Signal Flow Summary:
//
//   AN4 (raw ADC, 50kHz sampling in TIMER2)
//     ?
//     ???? iout_buf[8] ? iout_avg (DAT_ram_1d46)     [TIMER2, 8-point]
//     ?      ???? cal_b ? Imeas (DAT_ram_1d44)        [for OCP, fast]
//     ?                     ???? Imeas_scaled (DAT_ram_1d42) [× 7.625]
//     ?
//     ???? ring64_buf[64] ? sum64 ? avg64             [TIMER1, 64-point]
//            ???? cal_a ? Imeas_cal_a (DAT_ram_1e4e)  [for long-term avg]
//                           ???? ring1k_buf[1024]
//                                  ???? Imeas_longavg (DAT_ram_1d1e)
//
// Three time scales:
//   8-point   (TIMER2, 50kHz) ? OCP response: ~160us window
//   64-point  (TIMER1, 5kHz)  ? mid-term filtering: ~12.8ms window
//   1024-point (TIMER1, 5kHz) ? long-term average: ~205ms window
//
// Dual calibration paths:
//   Path A: 64-pt avg × cal_gain >> 13 + offset ? DAT_ram_1e4e
//   Path B: 8-pt avg  × cal_gain >> 13 + offset ? DAT_ram_1d44 (Imeas)
//   Same gain/offset, different input source (speed vs accuracy)
//
// Ring buffer memory:
//   64-entry:   DAT_ram_110a..1189 (256 bytes, 32-bit entries)
//   1024-entry: DAT_ram_0900..0CFF (2048 bytes, 16-bit entries)
//   Total: ~2.3KB for current measurement alone
// =================================================================
// =================================================================
// HSTNS-PD44 800W LLC OCP Foldback Controller (PI)
// FUN_rom_002cd6, TIMER1 ISR, called from FUN_rom_0037f6:003830
//
// PI math:
//   e[n] = Imeas - Iref
//   u[n] = clamp( (I[n-1] + Kp × e[n]) >> 13,  0, 4000 )
//   I[n] = clamp( I[n-1] + Ki × e[n] + aw[n],  0, 0x01F40000 )
//   Kp = (gain_scale × 12000) >> 10
//   Ki = (gain_scale × 2800) >> 10
//   aw[n] = (u_clamped - u_raw) × 30
//
// e[n] < 0: normal load, u[n] = 0 (inactive)
// e[n] > 0: overcurrent, u[n] rises (0?4000 in ~10 counts)
//
// Downstream:
//   vref_2p2z = (0xC2F - trim_offset) - u[n] ? TIMER2 2P2Z ? LLC freq
// =================================================================

int32_t integrator = 0;
int16_t Iref = 0xA6;
// int16_t Iref = 0x30F;

int16_t vref_ocp_adj = 0;

// int16_t Imeas = 0;
extern int16_t vfb_sum2ch;

void llc_ocp_foldback(void)
{
    // Step1: Vout ADC ? gain scale factor (not voltage regulation)
    // Higher Vout ? gain_scale clamped to min 0x100
    // Lower Vout (droop) ? gain_scale increases ? more aggressive foldback
    int16_t vout_scaled = ((int32_t)(vfb_sum2ch >> 2) * 0x0C03 - 393600L) >> 10;
    int16_t gain_scale = 0x400 - vout_scaled;
    if (gain_scale > 0x400) gain_scale = 0x400;
    if (gain_scale < 0x100) gain_scale = 0x100;

    // Step2: Adaptive PI gains (voltage-modulated)
    // At 12.3V: gain_scale=256, Kp=3000, Ki=700
    int32_t Kp = __mulsi3((int32_t)gain_scale, 0x2EE0) >> 10;  // proportional
    int16_t Ki = __mulsi3((int32_t)gain_scale, 0x0AF0) >> 10;  // integral

    // Step3: Current error e[n] = Imeas - Iref
    // dsPIC33 "sub.w f, WREG" = WREG = f - WREG
    int16_t i_error = Imeas - Iref;

    // Step4: PI controller
    // proportional: direct path, instant response
    int16_t u_raw = (integrator + __mulsi3((int32_t)i_error, Kp)) >> 13;

    // clamp output [0, 4000]
    int16_t u_n = u_raw;
    if (u_raw < 0)    u_n = 0;
    if (u_raw > 4000) u_n = 4000;

    // anti-windup: feed clipping residual back into integrator
    int32_t aw = (int32_t)(u_n - u_raw) * 30;

    // integral: accumulation path, persistent state
    int32_t integ_new = integrator + __mulsi3((int32_t)i_error, (int32_t)Ki) + aw;

    // clamp integrator [0, 0x01F40000]
    if (integ_new > 0x01F40000L) integ_new = 0x01F40000L;
    if (integ_new < 0)           integ_new = 0;
    integrator = integ_new;

    // Step5: Output ? offsets 2P2Z reference in TIMER2
    vref_ocp_adj = u_n;  // DAT_ram_1d38
}

// --- Output ---
int16_t  vref_ls = 0;        // DAT_ram_1d3a ? vref offset load sharing, current sharing
 
// --- Library ---
extern int32_t __mulsi3(int32_t a, int32_t b);

void llc_droop_trim_calc(void)
{
    int16_t trim = (int16_t)(__mulsi3((int32_t)Imeas, 0x0F0A) >> 15);

    if (trim > 77)  trim = 77;
    if (trim < 0)   trim = 0;

    vref_ls = trim;
}