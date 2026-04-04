/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — adc.c
 * T1 ISR subroutines: ADC sampling, calibration, PID droop, OCP foldback
 *
 * Functions:
 *   adcMiscSample()      — 0x2A30  Calibration data load from flash
 *   adcVoltageSample()   — 0x2AFC  Dual-channel 64-point voltage avg
 *   adcCurrentSample()   — 0x2B70  Current 64-point + 1024-point avg
 *   droopTrimCalc()      — 0x2C32  Droop/trim 32-sample ADC batch avg
 *   pidControl()          — 0x2C78  PID droop compensation
 *   ocpFoldback()         — 0x2CD6  OCP foldback PI controller
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* ============================================================================
 * adcMiscSample() — 0x2A30
 *
 * Loads calibration coefficients from flash buffer (0x15E6-0x15F8).
 * Called every T1 ISR tick but only executes when statusFlags bit6 is set
 * (indicating fresh flash data is available).
 *
 * Byte pair layout in flash buffer:
 *   0x15E6:0x15E7 -> cal_va   (0x1D32) — Vout ch A gain
 *   0x15E8:0x15E9 -> ofs_va   (0x1D30) — Vout ch A offset = (hi:lo)>>4 + 1
 *   0x15EA:0x15EB -> cal_va2  (0x1D2E) — Vout ch A2 gain
 *   0x15EC:0x15ED -> ofs_va2  (0x1D2C) — Vout ch A2 offset = (hi:lo)>>4 + 1
 *   0x15EE:0x15EF -> cal_a_gain (0x1D26) — current gain
 *   0x15F0:0x15F1 -> cal_a_offset (0x1D24) — current offset = (hi:lo)/122+1
 *   0x15F2:0x15F3 -> cal_pdc5   (0x1E44) — PDC5 calibration
 *   0x15F4:0x15F5 -> cal_var_1E42 (0x1E42)
 *   0x15F7:0x15F8 -> cal_vb   (0x1D2A) — Vout ch B gain
 *   0x15F9:0x15FA -> ofs_vb   (0x1D28) — Vout ch B offset = (hi:lo)>>4 + 1
 *
 * Assembly: 0x2A30 - 0x2AFA
 * ============================================================================ */
void adcMiscSample(void)
{
    const uint8_t *fb = (const uint8_t *)(const void *)flash_read_buf_15E6;

    if (!(statusFlags & (1u << 6)))
        return;

    statusFlags &= ~(1u << 6);

    /* Vout channel B calibration (buf[0x11]:buf[0x12]) */
    cal_vb = (int16_t)((uint16_t)(((uint16_t)fb[0x12] << 8) |
                                  (uint16_t)fb[0x11]));  /* 0x1D2A */
    ofs_vb = (int16_t)((int16_t)((uint16_t)(((uint16_t)fb[0x14] << 8) |
                                            (uint16_t)fb[0x13])) >> 4) + 1;  /* 0x1D28 */

    /* Vout channel A calibration (buf[0]:buf[1]) */
    cal_va = (int16_t)((uint16_t)(((uint16_t)fb[0x01] << 8) |
                                  (uint16_t)fb[0x00]));  /* 0x1D32 */
    ofs_va = (int16_t)((int16_t)((uint16_t)(((uint16_t)fb[0x03] << 8) |
                                            (uint16_t)fb[0x02])) >> 4) + 1;  /* 0x1D30 */

    /* Vout channel A2 (AN5) calibration (buf[4]:buf[5]) */
    cal_va2 = (int16_t)((uint16_t)(((uint16_t)fb[0x05] << 8) |
                                   (uint16_t)fb[0x04])); /* 0x1D2E */
    ofs_va2 = (int16_t)((int16_t)((uint16_t)(((uint16_t)fb[0x07] << 8) |
                                             (uint16_t)fb[0x06])) >> 4) + 1; /* 0x1D2C */

    /* Current gain (buf[8]:buf[9]) */
    uint8_t lo_ee = fb[0x08];
    uint8_t hi_ef = fb[0x09];
    cal_a_gain = (int16_t)((uint16_t)(hi_ef << 8) | lo_ee);  /* 0x1D26 */

    /* Current offset: special case when gain==0x2000 and offset bytes are 0 */
    if (lo_ee == 0 && hi_ef == 0x20 &&
        fb[0x0A] == 0 && fb[0x0B] == 0) {
        cal_a_offset = 0;                                /* 0x1D24 */
    } else {
        int16_t raw = (int16_t)((uint16_t)(((uint16_t)fb[0x0B] << 8) |
                                           (uint16_t)fb[0x0A]));
        cal_a_offset = (int16_t)(raw / 122) + 1;        /* DIV.SW by 0x7A */
    }

    /* PDC5 calibration: apply only if within valid range */
    uint16_t pdc5_val = (uint16_t)(((uint16_t)fb[0x0D] << 8) |
                                   (uint16_t)fb[0x0C]);
    cal_pdc5 = (int16_t)pdc5_val;                       /* 0x1E44 */
    /* Check: (pdc5_val + 0xFCDF) unsigned <= 0x9E, i.e. pdc5_val in [0x0321..0x03BF] */
    if ((uint16_t)(pdc5_val + 0xFCDF) <= 0x9E)
        PDC5 = pdc5_val;

    /* Auxiliary calibration value (buf[0x0E]:buf[0x0F]) */
    cal_var_1E42 = (int16_t)((uint16_t)(((uint16_t)fb[0x0F] << 8) |
                                        (uint16_t)fb[0x0E])); /* 0x1E42 */
}

/* ============================================================================
 * adcVoltageSample() — 0x2AFC
 *
 * Dual-channel voltage measurement with 64-point circular buffer averaging
 * and calibration gain/offset application.
 *
 * Channel A (AN5/ADCBUF5): stored in buffer at 0x0880, calibrated by cal_va2/ofs_va2
 * Channel B (AN3/ADCBUF3): stored in buffer at 0x0800, calibrated by cal_vb/ofs_vb
 *
 * Output:
 *   vcal_a (0x1260) = (avg_a * cal_va2) >> 13 + ofs_va2
 *   vcal_b (0x125E) = (avg_b * cal_vb)  >> 13 + ofs_vb
 *   vcal_diff (0x126C) = vcal_a - vcal_b
 *
 * Assembly: 0x2AFC - 0x2B6E
 * ============================================================================ */
void adcVoltageSample(void)
{
    uint16_t idx = (uint16_t)tick_counter;  /* 0x1252:0x1254 (only low 6 bits used) */

    /* --- Channel A: ADCBUF5 --- */
    vbuf_a[idx] = ADCBUF5;

    vraw_sum_b = 0;  /* 0x1D3C cleared (reused as temp) */

    /* Sum all 64 entries */
    uint16_t sum_a = 0;
    for (int i = 0; i < 64; i++)
        sum_a += vbuf_a[i];

    uint16_t avg_a = sum_a >> 6;

    /* Apply calibration: result = (avg * cal_va2) >> 13 + ofs_va2 */
    int32_t prod_a = __mulsi3((int32_t)avg_a, (int32_t)cal_va2);
    int16_t cal_a_result = (int16_t)(prod_a >> 13) + ofs_va2;
    vcal_a = cal_a_result;                               /* 0x1260 */

    /* --- Channel B: ADCBUF3 --- */
    vbuf_b[idx] = ADCBUF3;

    vraw_sum_b = 0;  /* 0x1D3C */

    /* Sum all 64 entries */
    uint16_t sum_b = 0;
    for (int i = 0; i < 64; i++)
        sum_b += vbuf_b[i];

    vraw_sum_b = sum_b;                                  /* 0x1D3C */
    uint16_t avg_b = sum_b >> 6;

    /* Apply calibration: result = (avg * cal_vb) >> 13 + ofs_vb */
    int32_t prod_b = __mulsi3((int32_t)avg_b, (int32_t)cal_vb);
    int16_t cal_b_result = (int16_t)(prod_b >> 13) + ofs_vb;
    vcal_b = cal_b_result;                               /* 0x125E */

    /* Difference */
    vcal_diff = cal_a_result - cal_b_result;             /* 0x126C */
}

/* ============================================================================
 * adcCurrentSample() — 0x2B70
 *
 * Current measurement with dual averaging:
 *   1) 64-sample circular buffer -> calibrated 64-point average
 *   2) 1024-sample ring buffer -> long-term average
 *
 * ADCBUF4 is stored into a 4-sample x 32-bit circular buffer (base 0x110A,
 * indexed by tick_counter * 4). The 64 32-bit entries are summed, then
 * calibrated using cal_a_gain/cal_a_offset.
 *
 * A secondary value using iout_avg (0x1D46) is also computed.
 * Imeas_scaled = Imeas * 0x1E80 >> 10 (approximately Imeas * 7.625)
 *
 * 1024-sample ring at 0x0900, index at 0x1100, wrapping at 0x3FF.
 *
 * Assembly: 0x2B70 - 0x2C30
 * ============================================================================ */
void adcCurrentSample(void)
{
    int16_t adc_val = ADCBUF4;
    uint16_t idx = (uint16_t)tick_counter;

    /* Store ADCBUF4 as 32-bit into 64-sample circular buffer */
    iout_4buf[idx] = (int32_t)adc_val;  /* zero-extended to 32-bit */

    /* Clear accumulator */
    ioutSum64 = 0;

    /* Sum all 64 32-bit samples */
    int32_t sum64 = 0;
    for (int i = 0; i < 64; i++)
        sum64 += iout_4buf[i];

    ioutSum64 = sum64;

    /* Calibrate: result = (cal_a_gain * (sum >> 6)) >> 13 + cal_a_offset */
    int16_t avg64 = (int16_t)(sum64 >> 6);
    int32_t prod = __mulsi3((int32_t)cal_a_gain, (int32_t)avg64);
    int16_t cal_result = (int16_t)(prod >> 13) + cal_a_offset;

    iout_64avg = cal_result;                             /* 0x1108 */
    Imeas_cal_a = (cal_result >= 0) ? cal_result : 0;    /* 0x1E4E, clamp to 0 */

    /* Secondary calibration using iout_avg (0x1D46) */
    int32_t prod2 = __mulsi3((int32_t)cal_a_gain, (int32_t)iout_avg);
    int16_t cal2 = (int16_t)(prod2 >> 13) + cal_a_offset;

    iout_cal_raw = cal2;                                 /* 0x1106 */
    Imeas = (cal2 >= 0) ? cal2 : 0;                     /* 0x1D44, clamp to 0 */

    /* Scaled value: Imeas * 0x1E80 >> 10 */
    int32_t scaled = (int32_t)Imeas * (int32_t)0x1E80;
    Imeas_scaled = (int16_t)(scaled >> 10);              /* 0x1D42 */

    /* --- 1024-sample ring buffer for long-term average --- */
    iout_ring[iout_ring_idx] = Imeas_cal_a;

    /* Update running accumulator */
    int32_t acc = iout_accum;
    acc += (int32_t)Imeas_cal_a;
    iout_accum = acc;

    /* Long-term average: accum >> 10 */
    Imeas_longavg = (int16_t)(acc >> 10);                /* 0x1D1E */

    /* Advance ring index, wrap at 1023 */
    iout_ring_idx++;
    if (iout_ring_idx > 0x3FF)
        iout_ring_idx = 0;

    /* Subtract oldest sample from accumulator */
    int16_t oldest = iout_ring[iout_ring_idx];
    iout_accum -= (int32_t)oldest;
}

/* ============================================================================
 * droopTrimCalc() — 0x2C32
 *
 * Accumulates ADC channels ADCBUF11, ADCBUF10, ADCBUF1 over 32 samples.
 * When 32 samples are collected, computes averages (>>7) and stores results.
 *
 * Output (every 32 T1 ticks):
 *   trim_avg_adc11 (0x1E48) = accum_adc11 >> 7
 *   trim_avg_adc10 (0x1E4C) = accum_adc10 >> 7
 *   trim_avg_adc1  (0x1E4A) = accum_adc1  >> 7
 *
 * Assembly: 0x2C32 - 0x2C66
 * ============================================================================ */
void droopTrimCalc(void)
{
    /* Accumulate ADC readings */
    trim_accum_adc11 += ADCBUF11;                        /* 0x1210 */
    trim_accum_adc10 += ADCBUF10;                        /* 0x120E */
    trim_accum_adc1  += ADCBUF1;                         /* 0x120C */

    trim_count++;                                        /* 0x120A */

    if (trim_count > 0x1F) {  /* > 31 means 32 samples collected */
        trim_count = 0;

        /* Compute averages: accumulator / 128 (not /32, so includes ×4 gain) */
        trim_avg_adc11 = trim_accum_adc11 >> 7;         /* 0x1E48 */
        trim_avg_adc10 = trim_accum_adc10 >> 7;         /* 0x1E4C */
        trim_avg_adc1  = trim_accum_adc1  >> 7;         /* 0x1E4A */

        /* Reset accumulators */
        trim_accum_adc11 = 0;
        trim_accum_adc10 = 0;
        trim_accum_adc1  = 0;
    }
}

/* ============================================================================
 * pidControl() — 0x2C78
 *
 * PID droop compensation. Computes droop adjustment based on calibrated
 * current (Imeas at 0x1D44) and droop mode (0x1268).
 *
 * Two modes:
 *   mode 4: direct multiply Imeas * 0x3B05 (high gain)
 *   other:  Imeas * 0xF0A via __mulsi3, then >> 15
 *
 * Secondary path: if vrefModeSelect == 0x6B3, also computes via 0x3B05
 *
 * Output clamped to [0, 0x4D] -> vref_ls (0x1D3A)
 *
 * Assembly: 0x2C78 - 0x2CD4
 * ============================================================================ */
void pidControl(void)
{
    int16_t imeas = Imeas;                               /* 0x1D44 */
    int16_t mode = droopMode;                           /* 0x1268 */
    int16_t result;

    if (mode == 4) {
        /* High-gain mode: direct 16x16->32 signed multiply */
        int32_t p = (int32_t)imeas * (int32_t)0x3B05;
        result = (int16_t)(p >> 15);
    } else {
        droopAdj = 0;                                   /* 0x1212 */

        /* Compute via __mulsi3: imeas * 0xF0A >> 15 */
        int32_t p = __mulsi3((int32_t)imeas, 0x0F0A);
        result = (int16_t)(p >> 15);
        droopResult = result;

        /* Secondary: if vrefModeSelect == 0x6B3, recompute with 0x3B05 */
        if (vrefModeSelect == 0x6B3) {
            int32_t p2 = __mulsi3((int32_t)imeas, 0x3B05);
            result = (int16_t)(p2 >> 15);
        }
    }

    droopResult = result;                               /* 0x1214 */

    /* Clamp to [0, 0x4D] */
    if (result > 0x4D)
        droopResult = 0x4D;
    else if (result < 0)
        droopResult = 0;

    vref_ls = droopResult;                              /* 0x1D3A */
}

/* ============================================================================
 * ocpFoldback() — 0x2CD6
 *
 * Over-Current Protection foldback using a PI controller.
 *
 * 1) Compute reference: ref = 0x400 - (vout_sum/4 * 0xC03 + offset) >> 10
 *    Clamp ref to [0x100, 0x400]
 *
 * 2) PI controller:
 *    proportional = ref * 0x2EE0 >> 10
 *    integral     = ref * 0xAF0  >> 10
 *    error        = ocpCurrentRef - Imeas (deviation from setpoint)
 *    accumulator += proportional * error + integral * error_old
 *    Clamp accumulator to [0, 0x1F400000] (32-bit)
 *
 * 3) Output: vrefOcpAdj (0x1D38) = clamped PI output >> 13
 *
 * Assembly: 0x2CD6 - 0x2DC4
 * Uses 6 bytes of stack frame + saves W8-W14
 * ============================================================================ */
void ocpFoldback(void)
{
    /* Step 1: Voltage-dependent gain scaling
     * Higher Vout → gain_scale clamped to min 0x100
     * Lower Vout (droop) → gain_scale increases → more aggressive foldback */
    int16_t vout_scaled = (int16_t)(((int32_t)(vout_sum >> 2) * 0x0C03 - 393600L) >> 10);
    int16_t gain_scale = 0x400 - vout_scaled;
    ocpError = gain_scale;                              /* 0x1D50 */

    if (gain_scale > 0x400)  gain_scale = 0x400;
    if (gain_scale < 0x100)  gain_scale = 0x100;
    ocpError = gain_scale;

    /* Step 2: Adaptive PI gains (voltage-modulated)
     * At 12.3V: gain_scale≈256 → Kp≈3000, Ki≈700 */
    int32_t Kp = __mulsi3((int32_t)gain_scale, 0x2EE0) >> 10; /* 32-bit proportional */
    ocpIntegralLo = (int16_t)Kp;                        /* 0x1D56: store truncated */

    int16_t Ki = (int16_t)(__mulsi3((int32_t)gain_scale, 0x0AF0) >> 10); /* integral */
    ocpProportional = Ki;                               /* 0x1D54 */

    /* Step 3: Current error  e[n] = Imeas - setpoint */
    int16_t i_error = Imeas - ocpCurrentRef;

    /* Step 4: PI controller (position form)
     *   Proportional: direct path, instant response
     *   u_raw = (integrator + Kp * e[n]) >> 13 */
    int16_t u_raw = (int16_t)((ocpAccumulator + __mulsi3((int32_t)i_error, Kp)) >> 13);
    ocpOutputRaw = u_raw;                               /* 0x121C */

    /* Clamp output to [0, 0xFA0] (4000) */
    int16_t u_n = u_raw;
    if (u_raw < 0)     u_n = 0;
    if (u_raw > 0xFA0) u_n = 0xFA0;
    ocpOutput = u_n;                                    /* 0x121A */

    /* Anti-windup: feed clipping residual back into integrator
     * ocpKiInput (0x1D52) holds the AW gain (typically 30) */
    int32_t aw = (int32_t)(u_n - u_raw) * (int32_t)ocpKiInput;

    /* Integral: accumulation path with anti-windup correction
     * integrator += Ki * e[n] + aw */
    int32_t integ_new = ocpAccumulator
                      + __mulsi3((int32_t)i_error, (int32_t)Ki)
                      + aw;

    /* Clamp integrator to [0, 0x01F40000] */
    if (integ_new > 0x01F40000L)  integ_new = 0x01F40000L;
    if (integ_new < 0)            integ_new = 0;
    ocpAccumulator = integ_new;                         /* 0x1216:0x1218 */

    /* Output → offsets 2P2Z reference in Timer2 ISR */
    vrefOcpAdj = u_n;                                   /* 0x1D38 */
}
