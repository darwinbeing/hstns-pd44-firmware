/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — isr_t2.c
 * Timer2 ISR — Voltage loop / 2P2Z compensator / LLC frequency control
 *
 * Handler address: 0x456E (via ISR dispatcher 0x0D40 -> jump table 0x1014 -> 0x456E)
 * IVT entry: T2 @ 0x0022 -> stub 0x0D40 -> mode2 -> 0x1014 -> 0x456E
 * End: RETFIE @ 0x4B4E (~750 instructions)
 *
 * This is the core "slow loop" voltage control ISR:
 *   1) DAC setpoint calculation (Kff gain scheduling)
 *   2) ADC dual-channel sampling (AN0 + AN2) with 4-point moving average
 *   3) Voltage error with slew-rate limiter and amplitude clamp
 *   4) Steady-state detection (25000 count)
 *   5) OVP/OCV threshold flag management
 *   6) Droop compensation + transient gain boost
 *   7) 2P2Z compensator (8x __mulsi3):
 *        y[n] = (d0*y[n-1] + d1*y[n-2] + Kff*(n1*e[n] + n2*e[n-1] + n3*e[n-2]) >> 7) >> 13
 *        Clamped to [2600 .. 25000]
 *   8) Soft-start frequency sweep + half-bridge sequencing
 *   9) PWM frequency/period calculation (PTPER, PDC, DTR, PHASE)
 *  10) Hardware register update via subroutines
 *
 * Saves/restores: W0-W14 + RCOUNT
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* Sub-routine declarations */
extern void adcBuf12OvercurrentLatch(void);            /* 0x433C - ADC trigger+wait */
extern void adcBuf4FastAverage(void);             /* 0x43D4 - apply PWM shadow regs */
extern void voutCalibrationAndOvpDetect(void);       /* 0x444C - deadtime + OVP cal */
extern void frequencyLimitControl(void);       /* 0x4508 - freq limit check */
extern void portdEdgeFaultDetect(void);     /* 0x4400 - fault latch */
extern void periodWrite(void);         /* 0x37CA - write PTPER/PDC to shadow */
extern void pwmOverrideEnable(void);      /* 0x4B50 - enable PWM outputs */

/* 2P2Z compensator (all variables declared in ram_map.h)
 *
 * Coefficients (loaded from flash at startup):
 *   compN1 (32-bit) = Q15(-1.95197)   n1, applied to e[n]
 *   compN2 (32-bit) = Q15( 2.424896)  n2, applied to e[n-1]
 *   compN3 (32-bit) = Q15(-0.69126)   n3, applied to e[n-2]
 *   compD0 (16-bit) = Q15( 0.25742)   d2, applied to y[n-1]
 *   compD1 (16-bit) = Q15(-0.00742)   d3, applied to y[n-2]
 *
 * State: compY_out=y[n], compY_n1=y[n-1], compY_n2=y[n-2]
 * Accumulators: compDenomAccum, compNumerAccum
 * Error: voltageError=e[n], prevError=e[n-1], prevPrevError=e[n-2]
 */

/* All RAM variables are declared in ram_map.h:
 *   droopKffFactor (0x1DA6), freqTarget (0x1DAE), freqCoeffA (0x1DA8),
 *   freqCoeffB (0x1DAA), freqCoeffC (0x1DAC), softStartCounter (0x1DE2),
 *   droopPeriod (0x1DD6), uvpDebounce (0x1DD4), hbSeqState (0x1DDA),
 *   ocpDetCount (0x1DD0), latfTimer (0x1DD8), voutRefInitial (0x1DA2),
 *   pdc1 (0x1D70), pdc2 (0x1D6E), ptper (0x1D72), dtr3_shadow (0x1D6A),
 *   pdc3 (0x1D6C), ptperCommand (0x1D74)
 */

/* ============================================================================ */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    /* ==== Section 1: DAC setpoint (Kff gain scheduling) ==== */
    /* 0x4580-0x459E */
    int16_t vr = llcPeriodCmd;                          /* 0x1DA4 */
    int16_t dac_sp = (int16_t)(((int32_t)vr * 0x44D) >> 15) - 0x4A;

    if (dac_sp <= 44)       dac_sp = 44;
    else if (dac_sp > 128)  dac_sp = 128;
    compSetpoint = dac_sp;                          /* 0x1D92 */

    /* ==== Section 2: ADC dual-channel sampling ==== */
    /* 0x45A0-0x45C0 */
    adcBuf12OvercurrentLatch();                               /* 0x433C */

    int16_t va = ADCBUF0;
    int16_t vb = ADCBUF2;
    adcAn0Raw = va;               /* save AN0 raw */
    adcAn2Raw = vb;               /* save AN2 raw */

    int16_t vs = 2 * va + 2 * vb;
    vout_sum = vs;                                  /* 0x1DA0 */

    /* 4-point moving average */
    vout_avg_sum = va + prev_vout_a + vb + prev_vout_b;  /* 0x1D9E */
    prev_vout_a = va;                               /* 0x1D9A */
    prev_vout_b = vb;                               /* 0x1D96 */

    /* ==== Section 3: Voltage error with slew-rate limiter ==== */
    /* 0x45C2-0x45F4 */
    /* 45C4: SUB 0x1D4C, WREG  => WREG = [0x1D4C] - WREG */
    int16_t error = voutSetpoint - vs;
    voltageError = error;                          /* 0x1DE0 */

    int16_t pe = prevError;                        /* 0x1DDE (saved in W14) */
    int16_t delta = error - pe;
    if (delta > 100)
        error = pe + 50;                            /* slew limit: +50 */
    else if (delta < -100)
        error = pe - 50;                            /* slew limit: -50 */

    /* Amplitude clamp [-100 .. +100] */
    if (error > 100)       error = 100;
    else if (error < -100) error = -100;
    voltageError = error;                          /* 0x1DE0 final */

    /* ==== Section 4: Steady-state detection (25000 count) ==== */
    /* 0x45F4-0x463A */
    if (statusFlags & 0x20) {                        /* bit5 = regulation active */
        if ((auxFlags & 0x802) == 0x802) {
            steadyCount++;
            if (steadyCount > 25000) {
                steadyCount = 25000;
                uint16_t flag = 0;
                if (controlStatus == 0 && vs > 0x7FF && voutTargetCode == 0xC4D)
                    flag = 1;
                statusFlags = (statusFlags & ~(1u << 11)) | (flag << 11);
            }
        } else {
            steadyCount = 0;
            statusFlags &= ~(1u << 11);
        }
    } else {
        steadyCount = 0;
        statusFlags &= ~(1u << 11);                 /* clear 0x125B bit3 */
    }

    /* ==== Section 5: OVP threshold flag (bit9) ==== */
    /* 0x463A-0x4658: set statusFlags bit9 if llcPeriodCmd in [0x1D4C..9999] */
    if (statusFlags & (1u << 11)) {                  /* 0x125B bit3 */
        if (llcPeriodCmd > 0x1D4C && llcPeriodCmd <= 0x270F) {
            statusFlags |= (1u << 9);
        } else {
            statusFlags &= ~(1u << 9);
        }
    } else {
        statusFlags &= ~(1u << 9);
    }

    /* ==== Section 5b: OCV threshold flag (bit10) ==== */
    /* 0x465A-0x4678: set statusFlags bit10 if bit11 set and llcPeriodCmd in [0x1194..0x2133] */
    if (statusFlags & (1u << 11)) {
        if (llcPeriodCmd <= 0x2133) {
            uint16_t ocv_flag = (llcPeriodCmd > 0x1194) ? 1 : 0;
            statusFlags = (statusFlags & ~(1u << 10)) | (ocv_flag << 10);
        } else {
            statusFlags &= ~(1u << 10);
        }
    } else {
        statusFlags &= ~(1u << 10);
    }

    /* ==== Section 6: Droop compensation + transient gain boost ==== */
    /* 0x467A-0x46C4 */
    if (voltageError > 20 && (statusFlags & (1u << 9)) &&
        !(droopBoostFlags & (1u << 1))) {
        /* Transient detected: set droopPeriod = 1500, cap error to 50 */
        droopKffFactor = 0x5DC;                     /* 1500 */
        voltageError = 50;                         /* 0x32 */
        error = 50;

        /* Decay y[n-1] and y[n-2] coefficients by factor 0x73/128 */
        /* y[n-1] = (y[n-1] * 0x73) >> 7, sign-extended */
        int32_t yn1 = compY_n1;
        yn1 = __mulsi3(yn1, 0x73) >> 7;
        compY_n1 = (int32_t)(int16_t)yn1;           /* sign-extend 16-bit result */

        /* y[n-2] = (y[n-2] * 0x73) >> 7 */
        int32_t yn2 = compY_n2;
        yn2 = __mulsi3(yn2, 0x73) >> 7;
        compY_n2 = (int32_t)(int16_t)yn2;           /* sign-extend 16-bit result */

        droopBoostFlags |= (1u << 1);  /* set droop boost active */
    }

    /* ==== Section 6b: OVP debounce (faultFlags bit8) ==== */
    /* 0x46C6-0x46DE */
    if (faultFlags & 0x100) {                       /* bit8 = OVP pending */
        droopPeriod++;
        if (droopPeriod > 19) {
            droopPeriod = 0;
            droopKffFactor = 0x400;                 /* restore normal gain */
            droopBoostFlags &= ~(1u << 0);
        }
    }

    /* ==== Section 6c: OCV/UVP transient handling ==== */
    /* 0x46E0-0x471E */
    uint16_t flags12 = statusFlags;                  /* W12 = 0x125A */
    if (flags12 & (1u << 10)) {
        if (voltageError < -25) {                  /* error + 0x19 < 0 */
            if (!(faultFlags & 0x100)) {            /* OVP not active */
                droopKffFactor = 0x5DC;             /* transient boost */
                droopBoostFlags |= (1u << 1);
                droopBoostFlags |= (1u << 0);
                uvpDebounce = faultFlags & 0x100;  /* save OVP state */
            }
        } else if (voltageError > 25) {            /* error > 0x19 */
            droopBoostFlags |= (1u << 0);
            droopPeriod = 0;
        }
    }

    /* UVP debounce (faultFlags bit9) */
    if (faultFlags & 0x200) {
        uvpDebounce++;
        if (uvpDebounce > 19) {
            uvpDebounce = 0;
            droopKffFactor = 0x400;
            droopBoostFlags &= ~(1u << 1);
        }
    }

    /* ================================================================
     * Section 7: 2P2Z COMPENSATOR CORE
     * 0x4720-0x47A2
     *
     * Transfer function (difference equation):
     *   denom = d0 * y[n-1] + d1 * y[n-2]
     *   numer = n1 * e[n] + n2 * e[n-1] + n3 * e[n-2]
     *   y[n]  = (denom + (Kff_scaled * numer) >> 7) >> 13
     *
     * Where:
     *   Kff_scaled = (compSetpoint * droopPeriod) >> 10
     *   d0 = 0x1D90 (16-bit), d1 = 0x1D8E (16-bit)
     *   n1 = 0x1D8A:0x1D8C (32-bit), n2 = 0x1D86:0x1D88 (32-bit)
     *   n3 = 0x1D82:0x1D84 (32-bit)
     *   y[n-1] = 0x1D7A:0x1D7C, y[n-2] = 0x1D76:0x1D78
     *   e[n] = voltageError, e[n-1] = prevError, e[n-2] = prevPrevError
     *
     * Output clamped to [0xA28 .. 0x61A8] = [2600 .. 25000]
     * ================================================================ */

    /* Kff scaling: dac_sp_scaled = (compSetpoint * droopPeriod) >> 10 */
    int16_t kff_scaled;
    {
        int32_t kff_prod = (int32_t)compSetpoint * (int32_t)droopKffFactor;
        kff_scaled = (int16_t)(kff_prod >> 10);
        compSetpoint = kff_scaled;                  /* 0x1D92 updated, also W13 */
    }

    /* Denominator: denom = d2 * y[n-1] + d3 * y[n-2] */
    int32_t denom;
    {
        /* d2 * y[n-1] (16-bit coeff x 32-bit history via __mulsi3) */
        int32_t term0 = __mulsi3((int32_t)compD0, compY_n1);

        /* d3 * y[n-2] */
        int32_t term1 = __mulsi3((int32_t)compD1, compY_n2);

        denom = term0 + term1;
        compDenomAccum = denom;
    }

    /* Numerator: numer = n1*e[n] + n2*e[n-1] + n3*e[n-2] */
    int32_t numer;
    {
        int32_t t1 = __mulsi3((int32_t)voltageError, compN1);   /* n1 * e[n] */
        int32_t t2 = __mulsi3((int32_t)pe, compN2);             /* n2 * e[n-1] */
        int32_t t3 = __mulsi3((int32_t)prevPrevError, compN3);  /* n3 * e[n-2] */

        numer = t1 + t2 + t3;
        compNumerAccum = numer;
    }

    /* Combine: y[n] = (denom + (Kff_scaled * numer) >> 7) >> 13 */
    int32_t y_out;
    {
        int32_t kff_ext = (int32_t)kff_scaled;      /* sign-extend */
        int32_t kff_numer = __mulsi3(kff_ext, numer);
        int32_t shifted = kff_numer >> 7;
        int32_t combined = denom + shifted;
        y_out = combined >> 13;
    }

    /* Store and clamp y[n] to [2600 .. 25000] */
    compY_out = y_out;

    if (y_out > 25000) {                             /* 0x61A8 */
        compY_out = 25000;
    } else if (y_out <= 2600) {                      /* 0xA27+1 = 0xA28 */
        compY_out = 2600;
    }

    /* ==== Section 8: Half-bridge sequencing ==== */
    /* 0x47C2-0x48A0 */
    if (faultFlags & (1u << 1)) {
        /* Startup request: enable PWM override */
        faultFlags &= ~(1u << 1);
        faultFlags |= (1u << 2);
        pwmOverrideEnable();                       /* 0x4B50 */

        /* Check statusFlags bit0/bit1 for half-bridge ordering */
        if ((statusFlags & 0x01) || (statusFlags & 0x02)) {
            /* Load initial frequency from var_1D34 */
            int16_t init_freq = initFreq;
            compY_out = (int32_t)init_freq;          /* sign-extend 16->32 */
            goto history_shift;
        }
    } else if (faultFlags & (1u << 2)) {
        /* Shutdown request: disable PWM outputs */
        faultFlags &= ~(1u << 2);
        faultFlags |= (1u << 0);

        int16_t init_freq = initFreq;
        compY_out = (int32_t)init_freq;              /* sign-extend 16->32 */

        if (flags12 & 0x01) {
            /* Half-bridge A: disable IOCON1 PENH/PENL */
            if (hbSeqState == 0) {
                IOCON1bits.PENH = 0;                 /* 0x423 bit1 clr */
                Nop(); Nop(); Nop();
                IOCON1bits.PENL = 0;                 /* 0x423 bit0 clr */
                hbSeqState = 1;
                goto history_shift;
            } else if (hbSeqState == 1) {
                IOCON2bits.PENH = 0;                 /* 0x463 bit1 clr */
                Nop(); Nop(); Nop();
                IOCON2bits.PENL = 0;                 /* 0x463 bit0 clr */
                hbSeqState = 0;                    /* faultFlags bit2=0 group */
                faultFlags &= ~(1u << 0);
                statusFlags &= ~(1u << 0);
                goto history_shift;
            }
        } else if (flags12 & 0x02) {
            /* Half-bridge B: similar but reversed order */
            int16_t init_f = initFreq;
            compY_out = (int32_t)init_f;             /* sign-extend 16->32 */

            if (hbSeqState == 0) {
                /* Disable all PWM outputs in sequence with NOP delays */
                IOCON1bits.PENH = 0;
                Nop(); Nop(); Nop();
                IOCON1bits.PENL = 0;
                Nop(); Nop(); Nop();
                IOCON2bits.PENH = 0;
                Nop(); Nop(); Nop();
                IOCON2bits.PENL = 0;
                Nop(); Nop(); Nop();
                hbSeqState = 1;
                goto history_shift;
            } else if (hbSeqState == 1) {
                IOCON2bits.PENH = 0;
                Nop(); Nop(); Nop();
                IOCON2bits.PENL = 0;
                hbSeqState = 0;
                faultFlags &= ~(1u << 0);
                statusFlags &= ~(1u << 1);
                goto history_shift;
            }
        }
    } else if (faultFlags & (1u << 0)) {
        /* Normal shutdown: monitor LATF bit0 off-timer */
        goto skip_softstart;
    }

    /* ==== Section 9: History shift (error & coefficient pipeline) ==== */
history_shift:
    prevError = 0;                                  /* 0x48A0 */
    prevPrevError = 0;                              /* 0x48A2 */

skip_softstart:

    /* ==== Section 9b: Coefficient history shift ==== */
    /* 0x48A4-0x48BC: shift y[n] -> y[n-1] -> y[n-2], e[n] -> e[n-1] -> e[n-2] */
    {
        compY_n2 = compY_n1;                         /* y[n-2] = y[n-1] */
        compY_n1 = compY_out;                        /* y[n-1] = y[n] */

        int16_t pe_val = prevError;
        prevPrevError = pe_val;                    /* e[n-2] = e[n-1] */
        prevError = voltageError;                  /* e[n-1] = e[n] */
    }

    /* ==== Section 10: OCP detection ==== */
    /* 0x48BC-0x48FC: if state==ACTIVE && Imeas > 0x1A3 for 250 ticks -> LATD.3 */
    if (systemState == 2 && !(auxFlags & 0x02) &&
        !(LATDbits.LATD3) && Imeas > 0x1A3) {
        standbyDebounce = 0;            /* clear flags */
        ocpDetCount++;
        if (ocpDetCount > 250) {                    /* 0xFA */
            ocpDetCount = 250;
            int16_t vcal = voutCalibrated;                 /* 0x1D66 */
            /* Threshold depends on LATD bit4 */
            int16_t thresh = (LATDbits.LATD4) ? 0x153 : 0x2D9;
            if (vcal > thresh)
                LATDbits.LATD3 = 1;                  /* enable gate drive */
        }
    } else {
        ocpDetCount = 0;
    }

    /* ==== Section 11: Soft-start frequency sweep ==== */
    /* 0x48FC-0x4A68: complex multi-branch frequency calculation */
    if (statusFlags & 0x10) {                         /* bit4 = running */
        if (!(faultFlags & (1u << 7))) {             /* not in OT mode */
            int32_t y_out_32 = compY_out;

            if (y_out_32 > 0x1F40) {                /* > 8000 */
                /* High frequency: apply current derating */
                if (Imeas <= 24) {                   /* 0x18 */
                    freqTarget = (int16_t)compY_out + 0x190;    /* +400 boost */
                } else {
                    freqTarget = (int16_t)compY_out;
                }

                if (y_out_32 > 0x2AF8) {             /* > 11000 */
                    /* Very high: set all freq coeffs to 0x4268 (17000) */
                    int16_t hf = 0x4268;
                    freqTarget = hf;
                    freqCoeffA = hf;
                    freqCoeffC = hf;
                    freqCoeffB = hf;
                } else {
                    freqCoeffC = 0x3A98;           /* 15000 */
                    freqCoeffB = 0x3A98;
                    freqCoeffA = 0x32C8;           /* 13000 */
                }
            } else {
                freqTarget = 0x1F40;                /* floor at 8000 */
            }

            /* Apply current derating to llcPeriodCmd */
            if (Imeas <= 24)
                llcPeriodCmd = (int16_t)compY_out + 0x190;
            else
                llcPeriodCmd = (int16_t)compY_out;

            /* Clamp llcPeriodCmd minimum to 0xD48 (3400) */
            if (llcPeriodCmd <= 0xD47)
                llcPeriodCmd = 0xD48;

            faultFlags |= (1u << 7);                /* set soft-start active */
            softStartCounter = 0;                      /* 0x1DE2:0x1DE4 */
        }

        /* LATF bit0 off-timer: if LATF.0 not set, count up to 20000 */
        if (!(LATF & 0x01)) {
            latfTimer++;
            if (latfTimer > 20000) {                /* 0x4E20 */
                LATFbits.LATF0 = 1;
                llcPeriodCmd += 1000;                    /* 0x3E8 */
            }
        } else {
            latfTimer = 0;
        }

        LATFbits.LATF6 = 1;                         /* enable 12V rail */

        /* Frequency ramp with voutCalibrated threshold */
        int16_t vc = voutCalibrated;                       /* 0x1D66 */
        if ((uint16_t)(vc + 0xFD34) <= 25) {        /* vc in [0x2CC..0x2E5] */
            if (!(LATF & 0x01)) {
                if (Imeas <= 24) {
                    LATFbits.LATF0 = 1;
                    llcPeriodCmd = llcPeriodCmd + 1000;
                } else {
                    llcPeriodCmd = llcPeriodCmd + 1;         /* slow ramp */
                }
            }
        } else if (vc >= 0x2CB || (LATF & 0x01)) {
            /* 0x49A6-0x49C6: for vc >= 0x2CB, or for lower vc once LATF0 is
             * already asserted, ramp by +100. If LATF0 is still low, the
             * assembly overwrites that with old_cmd + 0x44C, not +0x64+0x44C.
             */
            int16_t prev_cmd = llcPeriodCmd;
            llcPeriodCmd = prev_cmd + 100;               /* 0x64 */
            if (!(LATF & 0x01)) {
                LATFbits.LATF0 = 1;
                llcPeriodCmd = prev_cmd + 0x44C;         /* +1100 total */
            }
        }

        /* Write back llcPeriodCmd and initialize 2P2Z state for startup */
        compY_out = (int32_t)llcPeriodCmd;
        compY_n1 = compY_out;
        compY_n2 = compY_out;

        /* Initialize target and control variables */
        voutSetpoint = 0;                             /* 0x1D4C */
        voutTargetCode = 0xFFB0;                          /* -80 */
        statusFlags &= ~(1u << 5);                   /* clear regulation flag */

        /* Slow ramp with 50000-count timeout */
        if (vc <= 0x27F) {
            int16_t adc_1bde = pdcShadowA;
            if (adc_1bde <= 0x7F) {
                /* Slow ramp: increment 32-bit counter */
                softStartCounter++;
                if (softStartCounter > 50000) {        /* 0xC350 */
                    softStartCounter = 50000;
                    freqCoeffC = freqCoeffA;     /* use fast coeff */
                } else {
                    /* Interpolate: avg of freqCoeffA/2 + freqTarget/2 */
                    freqCoeffC = (freqCoeffA >> 1) + (freqTarget >> 1);
                }
            } else if (adc_1bde > 0xA0) {
                /* 0x4A18-0x4A20: high ADC region uses freqCoeffB */
                freqCoeffC = freqCoeffB;        /* 0x1DAA */
            }
            /* For adc_1bde in (0x7F..0xA0], assembly keeps freqCoeffC unchanged. */
        }

        /* Select limiting frequency */
        int16_t freq_limit;
        if (vc > 0x27F)
            freq_limit = freqTarget;                /* 0x1DAE */
        else
            freq_limit = freqCoeffC;               /* 0x1DAC */

        /* Clamp llcPeriodCmd to freq_limit */
        if (llcPeriodCmd > freq_limit) {
            llcPeriodCmd = freq_limit;                   /* cap at limit */
        }
    } else {
        /* Not running: idle mode frequency calculation */
        latfTimer = 0;
        int16_t dm = droopMode;
        int16_t idle_freq;
        if (dm == 4) {
            idle_freq = 0xBB8;                       /* 3000 */
        } else {
            int16_t vc = voutCalibrated;
            idle_freq = (vc > 0x33F) ? 0xC80 : 0xD48;  /* 3200 or 3400 */
        }
        voutRefInitial = idle_freq;                        /* 0x1DA2 */

        /* Clamp: if y_out < idle_freq, use idle_freq */
        int32_t y32 = compY_out;
        int32_t idle_ext = (int32_t)(uint16_t)idle_freq;
        if (y32 < idle_ext) {
            llcPeriodCmd = idle_freq;
        } else if (y32 > 0x5DC0) {                  /* > 24000 */
            llcPeriodCmd = 0x5DC0;
        } else {
            llcPeriodCmd = (int16_t)compY_out;
        }
    }

    /* ==== Section 12: 12V rail presence check ==== */
    /* 0x4A68-0x4AB2 */
    if (statusFlags & (1u << 7)) {                    /* bit7 = 12V present */
        if (vrefModeSelect == 0xC4D) {
            /* 12V standard: set 0x32C8 (13000) as reference */
            llcPeriodCmd = 0x32C8;
            compY_out = 0x32C8;
            compY_n1 = 0x32C8;
            compY_n2 = 0x32C8;
        } else if (vrefModeSelect == 0x6B3) {
            /* 12V alternate: set 0x38A4 (14500) */
            llcPeriodCmd = 0x38A4;
            compY_out = 0x38A4;
            compY_n1 = 0x38A4;
            compY_n2 = 0x38A4;
        }
        /* 0x4AA8-0x4AB0: clear bit7 once measured output exceeds the target. */
        if (vout_sum > voutSetpoint)
            statusFlags &= ~(1u << 7);
    }

    /* ==== Section 13: PWM period / duty cycle calculation ==== */
    /* 0x4AB2-0x4B0C */
    frequencyLimitControl();                            /* 0x4508 */

    /* llcPeriodCmd is the voltage-loop output expressed in the LLC switching
     * period domain. The T2 path derives period / duty values from that command.
     */
    int32_t ptper_prod = (int32_t)llcPeriodCmd * (int32_t)0x1BB;
    int16_t ptper_raw = (int16_t)(ptper_prod >> 15) - 0x65;  /* 0xFF9B = -101 */

    if (ptper_raw > 0x8E)       ptper_raw = 0x8E;   /* 142 */
    else if (ptper_raw < 0)     ptper_raw = 0;

    /* Deadtime / phase from iout_avg */
    int16_t iavg = iout_avg;                         /* 0x1D46 */
    int32_t dt_prod = (int32_t)iavg * (int32_t)0xEA8;
    int16_t dt_raw = (int16_t)(dt_prod >> 15) + 13;  /* +0xD */

    if (dt_raw > 0x47)       dt_raw = 0x47;         /* 71 */
    else if (dt_raw <= 0x2E) dt_raw = 0x2F;         /* 47 */

    /* 0x4AF8-0x4B08:
     *   W1:W0 = 0x00B3FB00
     *   DIV.SD W0, W5
     *   ASR W0, W2
     *   ptperCommand = W2 - 8
     *   pdc1/pdc2 = W2
     *
     * The previous decompilation introduced an extra variable right shift by
     * (compY_out - 8), which does not exist in the assembly and produced
     * obviously wrong PTPER values.
     */
    int16_t ptper_shifted = (int16_t)(__builtin_divsd(0x00B3FB00L, llcPeriodCmd) >> 1);

    ptperCommand = ptper_shifted - 8;                  /* 0x1D74 */
    ptper    = ptper_raw;                         /* 0x1D72 */
    pdc1   = ptper_shifted;                     /* 0x1D70 */
    pdc2     = ptper_shifted;                     /* 0x1D6E */
    dtr3_shadow  = dt_raw;                            /* 0x1D6A */

    /* PDC3 calculation */
    if (faultFlags & (1u << 5)) {
        pdc3 = 0;                                /* 0x1D6C */
    } else {
        if (llcPeriodCmd > 0x1A8F) {
            pdc3 = ptper_shifted;                /* full duty */
        } else {
            pdc3 = 0x36B;                        /* fixed 875 */
        }
    }

    /* ==== Section 14: Final hardware update ==== */
    /* 0x4B2A-0x4B3A */
    adcBuf4FastAverage();                                  /* 0x43D4 */
    voutCalibrationAndOvpDetect();                            /* 0x444C */
    periodWrite();                              /* 0x37CA */
    portdEdgeFaultDetect();                          /* 0x4400 */

    /* ==== Section 15: Clear interrupt & trigger PWM ==== */
    /* 0x4B34-0x4B3A */
    IFS0bits.T2IF = 0;                               /* clear T2 interrupt flag */
    IEC0bits.T2IE = 0;                               /* disable T2 interrupt */
    /* Re-enabled by next PSEM cycle */
    PTCONbits.SEIEN = 1;                             /* trigger PWM special event */
    PTCONbits.PTSIDL = 1;                            /* 0x401 bit3 */
}
