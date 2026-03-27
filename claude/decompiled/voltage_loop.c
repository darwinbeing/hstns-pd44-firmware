/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — voltage_loop.c
 * T2 ISR subroutines called from the 2P2Z voltage loop
 *
 * Functions:
 *   adcBuf12OvercurrentLatch()       — 0x433C  ADCBUF12 overcurrent latch + startup gate
 *   adcBuf4FastAverage()             — 0x43D4  ADCBUF4 8-point fast current average
 *   portdEdgeFaultDetect()           — 0x4400  Fault latch: PORTD edge + vout_avg OVP
 *   voutCalibrationAndOvpDetect()    — 0x444C  ADCBUF5 voltage calibration + OVP detect
 *   frequencyLimitControl()          — 0x4508  Frequency limit + PWM override control
 *   pwmDisableAll()                  — 0x44DC  Disable all PWM outputs (IOCON1/2/3)
 *   pwmOverrideEnable()              — 0x4B50  Enable PWM outputs (IOCON1/2)
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* Forward declaration */
void pwmOverrideEnable(void);

/* Helpers declared in t1_sub_prot.c */
extern uint16_t thresholdCompare(int16_t adc_val, int16_t prev_state,
                                  int16_t lo_thresh, int16_t hi_thresh);
extern uint16_t debounceCounter(int16_t new_val, int16_t prev_val,
                                 volatile int16_t *cnt, int16_t limit);

/* ============================================================================
 * adcBuf12OvercurrentLatch() — 0x433C-0x43D2
 *
 * Reads ADCBUF12 (overcurrent sense), applies threshold detection with
 * debounce via helper functions. If overcurrent persists in ACTIVE state
 * with droopMode 3 or 4, triggers fault -> state=FAULT, enables PWM override.
 *
 * Also handles startup gating: if not in ACTIVE state, certain conditions
 * force PWM enable and set systemState=0 (IDLE) with marginThreshold=2500.
 * ============================================================================ */
void adcBuf12OvercurrentLatch(void)
{
    int16_t adc12 = ADCBUF12;

    /* Extract bit3 of pwmRunning as previous state for threshold compare */
    int16_t prev_bit = (pwmRunning >> 3) & 1;
    int16_t new_state = 0;

    /* Threshold compare: ADCBUF12 vs [0x136, 0x26C] with hysteresis */
    int16_t flags_w3 = (thermalFlags >> 1) & 1;
    uint16_t cmp = thresholdCompare(adc12, flags_w3, 0x136, 0x26C);
    if (cmp == 0)
        new_state = 1;

    /* Debounce with counter at debounceAdc12, limit 3 */
    uint16_t result = debounceCounter(new_state, prev_bit,
                                       &debounceAdc12, 3);

    /* Update bit3 of pwmRunning */
    int16_t f1bf2 = pwmRunning;
    f1bf2 = (f1bf2 & ~(1 << 3)) | ((result & 1) << 3);
    pwmRunning = f1bf2;

    int16_t oc_flag = f1bf2 & (1 << 3);             /* W8 = bit3 state */

    if (systemState == 2) {
        /* ACTIVE state */
        if (oc_flag == 0) {
            /* No overcurrent: check droopMode 3 or 4 for startup gate */
            int16_t dm = droopMode;
            if (dm == 3 || dm == 4) {
                /* droopMode valid: start voutSetpoint ramp */
                statusFlags |= (1u << 13);
                int16_t cnt = ocRampCounter;
                cnt++;
                ocRampCounter = cnt;
                voutSetpoint -= cnt * 2;              /* ramp down target */

                if (cnt > 0x4B) {                    /* > 75 ticks */
                    ocRampCounter = 0;
                    pwmRunning &= ~(1u << 0);
                    pmbusAlertFlags |= (1u << 0);
                    pwmOverrideEnable();           /* 0x4B50 */
                    systemState = ST_IDLE;                    /* -> IDLE */
                    marginThreshold = 0x9C4;        /* 2500 */
                    statusFlags &= ~(1u << 13);
                }
            }
        } else {
            /* Overcurrent active: if bit5 was set, clear and reset */
            if ((statusFlags & (1u << 13))) {
                statusFlags &= ~(1u << 13);
                ocRampCounter = 0;
            }
        }
    } else {
        /* Not ACTIVE state */
        statusFlags &= ~(1u << 13);
        ocRampCounter = 0;

        if (oc_flag == 0 && systemState != 0) {
            /* Force PWM enable and reset to IDLE */
            pwmRunning &= ~(1u << 0);
            pmbusAlertFlags |= (1u << 0);
        }
        pwmOverrideEnable();                       /* 0x4B50 */
        systemState = ST_IDLE;                                /* -> IDLE */
        marginThreshold = 0x9C4;                    /* 2500 */
    }
}

/* ============================================================================
 * adcBuf4FastAverage() — 0x43D4-0x43FE
 *
 * Reads ADCBUF4 (output current fast sample), maintains 8-point circular
 * buffer at 0x1DB4, computes running sum in 0x1D68, and derives iout_avg
 * (0x1D46) = sum >> 3 (8-point average).
 *
 * Ring buffer index at 0x1DC4, wraps at 7.
 * ============================================================================ */
void adcBuf4FastAverage(void)
{
    int16_t adc4 = ADCBUF4;
    adcBuf4Raw = adc4;                               /* save raw */

    int16_t idx = ioutRingIdx8pt;

    /* Store new sample in ring buffer */
    ioutRing8pt[idx] = adc4;

    /* Update running sum: add new sample */
    int16_t sum = ioutRunSum;
    sum += adc4;
    ioutRunSum = sum;

    /* Compute 8-point average */
    iout_avg = sum >> 3;                             /* 0x1D46 */

    /* Advance index, wrap at 7 */
    idx++;
    ioutRingIdx8pt = idx;
    if (idx > 7)
        ioutRingIdx8pt = 0;

    /* Subtract oldest sample from running sum */
    int16_t new_idx = ioutRingIdx8pt;
    int16_t oldest = ioutRing8pt[new_idx];
    ioutRunSum = sum - oldest;
}

/* ============================================================================
 * portdEdgeFaultDetect() — 0x4400-0x444A
 *
 * Fault detection via PORTD edge monitoring:
 *   - Reads PORTD bit1 (via byte 0x2DB), compares with stored state
 *   - If mismatch detected while vout_avg_sum > 0x500 for 19+ ticks:
 *     triggers FAULT state (systemState=3), disables LLC, enables PWM override
 *
 * Only active in ACTIVE state (systemState==2).
 * ============================================================================ */
void portdEdgeFaultDetect(void)
{
    if (systemState != 2) {
        faultEdgeCnt = 0;
        return;
    }

    /* Read PORTD bit1 -> portdBit1State, PORTD bit0 -> portdBit0State */
    uint8_t portd_byte = (uint8_t)PORTD;
    int16_t portd_bit1 = (portd_byte >> 1) & 1;
    portdBit1State = portd_bit1;

    int16_t portd_bit0 = portd_byte & 1;
    portdBit0State = portd_bit0;

    /* Check if bit1 matches bit0 (edge detect) */
    if (portd_bit1 == portd_bit0) {
        /* No edge: check vout_avg threshold */
        if (vout_avg_sum > 0x500) {
            int16_t cnt = faultEdgeCnt;
            cnt++;
            faultEdgeCnt = cnt;

            if (cnt > 19) {                          /* 0x13 */
                faultEdgeCnt = 20;                   /* cap */
                /* FAULT: OVP latch triggered */
                controlStatus |= (1u << 6);          /* 0x1E20 bit6 */
                pwmRunRequest |= (1u << 0);
                pwmRunning &= ~(1u << 0);
                pmbusAlertFlags |= (1u << 0);
                pwmOverrideEnable();               /* 0x4B50 */
                systemState = ST_FAULT;                        /* -> FAULT */
            }
        } else {
            faultEdgeCnt = 0;
        }
    } else {
        faultEdgeCnt = 0;
    }
}

/* ============================================================================
 * voutCalibrationAndOvpDetect() — 0x444C-0x44DA
 *
 * Voltage calibration and OVP detection:
 *   1) Compute voutCalibrated = (ADCBUF5 * cal_va2) >> 13 + ofs_va2
 *      Also uses vout_avg/4 (W8) as reference for comparison
 *   2) OVP threshold depends on droopMode: 0x1DF (mode 4) or 0x36F (others)
 *   3) If controlStatus bit4 set: progressive OVP counter at 0x1DCA
 *      On timeout -> fault (systemState=3) with PWM override
 *   4) If voutCalibrated > threshold AND voutReference > 0x1F3F AND bit4 set:
 *      immediate OVP: set all PTPER/PDC to 0, arm PWM, set systemState=3
 * ============================================================================ */
void voutCalibrationAndOvpDetect(void)
{
    int16_t vavg_div4 = vout_avg_sum >> 2;           /* W8 */

    /* Calibrate ADCBUF5: (ADCBUF5 * cal_va2) >> 13 + ofs_va2 */
    int16_t adc5 = ADCBUF5;
    int32_t prod = __mulsi3((int32_t)adc5, (int32_t)cal_va2);
    int16_t cal_result = (int16_t)(prod >> 13) + ofs_va2;
    voutCalibrated = cal_result;                           /* 0x1D66 */

    /* Use max of vavg_div4 and cal_result */
    if (vavg_div4 < cal_result)
        vavg_div4 = cal_result;

    /* OVP threshold based on droopMode */
    int16_t ovp_thresh;
    if (droopMode == 4)
        ovp_thresh = 0x1DF;                          /* 479 */
    else
        ovp_thresh = 0x36F;                          /* 879 */

    /* Check controlStatus bit4 (OVP armed) */
    if (controlStatus & 0x10) {
        int16_t ovp_cnt = ovpCounter;
        ovp_cnt++;
        ovpCounter = ovp_cnt;

        if (systemState == 2) {
            /* ACTIVE: clear PWM outputs and start OVP sequence */
            pdc1 = 0;                           /* 0x1D70 */
            pdc2 = 0;                             /* 0x1D6E */
            pdc3 = 0;                            /* 0x1D6C */

            ovp_cnt += 2;                            /* accelerate */
            ovpCounter = ovp_cnt;

            if (ovp_cnt > 1) {
                ovpCounter = 1;
                /* Trigger FAULT */
                pwmRunRequest |= (1u << 0);
                pwmRunning &= ~(1u << 0);
                pmbusAlertFlags |= (1u << 0);
                pwmOverrideEnable();
                systemState = ST_FAULT;                        /* -> FAULT */
            }
        }
    } else if (vavg_div4 >= ovp_thresh) {
        /* Over voltage detected: check if voutReference > 0x1F3F and running */
        if (voutReference > 0x1F3F && (statusFlags & (1u << 4))) {
            /* Immediate OVP: set all outputs to match, arm OVP */
            pdc1 = 0;
            pdc2 = 0;
            pdc3 = 0;
            controlStatus |= (1u << 4);              /* arm OVP */
            ovpCounter = 0;
            faultResetTimer = 0xC8;      /* 200 tick cooldown */
        }
    } else {
        ovpCounter = 0;
    }
}

/* ============================================================================
 * pwmDisableAll() — 0x44DC-0x4506
 *
 * Disables all PWM outputs by clearing PENH/PENL bits on IOCON1, IOCON2,
 * and IOCON3 with 3-NOP synchronization delays between each operation.
 * ============================================================================ */
void pwmDisableAll(void)
{
    IOCON1bits.PENH = 0;   /* 0x423 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON1bits.PENL = 0;   /* 0x423 bit0 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON2bits.PENH = 0;   /* 0x443 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON2bits.PENL = 0;   /* 0x443 bit0 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON3bits.PENH = 0;   /* 0x463 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON3bits.PENL = 0;   /* 0x463 bit0 */
}

/* ============================================================================
 * frequencyLimitControl() — 0x4508-0x456C
 *
 * Frequency/period limiting logic:
 *   1) If faultFlags bits 4+5 both set (0x30):
 *      - Set 0x1DCE=1 (limit active flag)
 *      - If voutReference > 0x206C: clamp all y[n] states to 0x206C, enable PWM
 *      - If voutReference <= 0x1FA3: call pwmDisableAll
 *   2) If 0x1DCE was set: clear faultFlags bit5, clear 0x1DCE, disable PWM
 *   3) faultFlags bit10: secondary PWM3 PENH enable/disable toggle
 * ============================================================================ */
void frequencyLimitControl(void)
{
    if ((faultFlags & 0x30) == 0x30) {
        /* Both bits 4+5 set: frequency limit active */
        freqLimitActive = 1;

        if (voutReference > 0x206C) {                     /* > 8300 */
            /* Clamp all 2P2Z state to 0x206C */
            compY_out = 0x206C;
            compY_n1 = 0x206C;
            compY_n2 = 0x206C;
            pwmOverrideEnable();                   /* 0x4B50 */
        } else if (voutReference <= 0x1FA3) {             /* <= 8099 */
            pwmDisableAll();                       /* 0x44DC */
        }
        /* else: voutReference in (0x1FA3, 0x206C] — do nothing */
    } else {
        if (freqLimitActive != 0) {
            faultFlags &= ~(1u << 5);
            freqLimitActive = 0;
            pwmDisableAll();                       /* 0x44DC */
        }
    }

    /* Secondary: faultFlags bit10 (0x400) — PWM3 PENH toggle */
    if (faultFlags & 0x400) {
        pwm3PenhFlag = 1;
        IOCON3bits.PENH = 1;                         /* 0x463 bit1 */
        asm("nop"); asm("nop"); asm("nop");
    } else {
        if (pwm3PenhFlag != 0) {
            pwm3PenhFlag = 0;
            IOCON3bits.PENH = 0;
            asm("nop"); asm("nop"); asm("nop");
        }
    }
}

/* ============================================================================
 * pwmOverrideEnable() — 0x4B50-0x4B6C
 *
 * Enables PWM1 and PWM2 outputs by setting PENH/PENL bits on IOCON1 and
 * IOCON2 with 3-NOP synchronization delays.
 * (PWM3 is NOT enabled here — it's controlled separately by )
 * ============================================================================ */
void pwmOverrideEnable(void)
{
    IOCON1bits.PENH = 1;   /* 0x423 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON1bits.PENL = 1;   /* 0x423 bit0 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON2bits.PENH = 1;   /* 0x443 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON2bits.PENL = 1;   /* 0x443 bit0 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON3bits.PENH = 1;   /* 0x463 bit1 */
    asm("nop"); asm("nop"); asm("nop");
    IOCON3bits.PENL = 1;   /* 0x463 bit0 */
}
