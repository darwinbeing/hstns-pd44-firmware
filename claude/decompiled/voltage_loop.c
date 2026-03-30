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
#ifdef SIMULATION_MODE
#include "simulator.h"
#endif

/* Forward declaration */
void pwmOverrideEnable(void);

/* ============================================================================
 * Local helpers for 0x433C path
 * 0x4326: threshold compare helper used by 0x433C
 * 0x430C: state/counter latch helper used by 0x433C
 * ============================================================================ */
static uint16_t thresholdCompare433C(uint16_t value, uint16_t thresh1,
                                     uint16_t thresh2, uint16_t prev_state)
{
    uint16_t result = prev_state;

    /* 432A..4338
     * if (value >= thresh1) result = 1;
     * else if (value <= thresh2) result = 0;
     * else result keeps prev_state.
     */
    if (value >= thresh1) {
        result = 1u;
    } else if (value <= thresh2) {
        result = 0u;
    }

    return (uint8_t)result; /* ZE W0,W0 */
}

static uint16_t latchCounter430C(uint16_t new_level, uint16_t limit,
                                 volatile int16_t *counter, uint16_t prev_level)
{
    uint16_t result = prev_level;

    if (new_level != prev_level) {
        uint16_t cnt = (uint16_t)(*counter + 1);
        *counter = (int16_t)cnt;

        /* 4318 BRA NC,0x4322 with SUB W0,W1,[W15] (W0-W1):
         * cnt < limit -> keep prev and keep counter.
         * cnt >= limit -> clear counter and accept new level.
         */
        if (cnt >= limit) {
            *counter = 0;
            result = new_level;
        }
    } else {
        *counter = 0;
    }

    return (uint8_t)result; /* ZE W3,W0 */
}

/* ============================================================================
 * adcBuf12OvercurrentLatch() — 0x433C-0x43D2
 *
 * Reads ADCBUF12 (overcurrent sense), applies threshold detection with
 * debounce via helper functions, then updates pwmRunning.bit3.
 *
 * In ACTIVE state (systemState=2), oc_flag==0 starts a ramp-down sequence
 * on voutSetpoint and can force shutdown/override.
 *
 * In non-ACTIVE states, it clears the ramp latch and may force IDLE
 * (systemState=0) with marginThreshold=0x09C4.
 * ============================================================================ */
void adcBuf12OvercurrentLatch(void)
{
    uint16_t adc12 =
#ifdef SIMULATION_MODE
        (uint16_t)sim_debug.adcbuf12;
#else
        (uint16_t)ADCBUF12;
#endif

    /* 433E..4362: comparator + latch update for pwmRunning.bit3 */
    {
        uint16_t prev_bit3 = (pwmRunning >> 3) & 1u;
        uint16_t thermal_b1 = (thermalFlags >> 1) & 1u;
        uint16_t cmp = thresholdCompare433C(adc12, 0x026Cu, 0x0136u, thermal_b1);
        uint16_t new_level = (cmp == 0u) ? 1u : 0u; /* CP0.B / MOV #1,W9 */
        uint16_t latched = latchCounter430C(new_level, 3u, &debounceAdc12, prev_bit3);
        uint16_t w = pwmRunning;
        w = (uint16_t)((w & (uint16_t)~(1u << 3)) | ((latched & 1u) << 3));
        pwmRunning = w;
    }

    {
        uint16_t oc_flag = pwmRunning & (1u << 3); /* W8 */
        uint16_t state = (uint16_t)systemState;    /* W0 at 4370 */

        if (state == 2u) {
            /* 4378..43B4 */
            if (oc_flag == 0u) {
                uint16_t dm_rel = (uint16_t)droopMode - 3u; /* 437E..4380 */
                if (dm_rel > 1u) {
                    /* 43C2 path from ACTIVE + droopMode not in {3,4} */
                    pwmRunning &= (uint16_t)~(1u << 0);
                    pmbusAlertFlags |= (1u << 0);
                    pwmOverrideEnable();
                    systemState = (int16_t)oc_flag; /* 0 */
                    marginThreshold = (int16_t)0x09C4;
                    return;
                }

                statusFlags |= (1u << 13);       /* BSET 0x125B,#5 */
                {
                    uint16_t cnt = (uint16_t)ocRampCounter + 1u;
                    ocRampCounter = (int16_t)cnt;
                    /* 4390 SUB 0x1D4C with W0=2*cnt: memory update on voutSetpoint */
                    voutSetpoint = (int16_t)(voutSetpoint - (int16_t)(cnt << 1));
                    if (cnt <= 0x004Bu) {
                        return;                   /* 4396 BRA LEU 43D0 */
                    }
                }

                /* 4398..43AA */
                ocRampCounter = (int16_t)oc_flag; /* 0 */
                pwmRunning &= (uint16_t)~(1u << 0);
                pmbusAlertFlags |= (1u << 0);
                pwmOverrideEnable();
                systemState = (int16_t)oc_flag;   /* 0 */
                marginThreshold = (int16_t)0x09C4;
                statusFlags &= (uint16_t)~(1u << 13);
                return;
            }

            /* 43AC..43B4 */
            if (statusFlags & (1u << 13)) {
                statusFlags &= (uint16_t)~(1u << 13);
                ocRampCounter = 0;
            }
            return;
        }

        /* 43B6..43D0 : non-ACTIVE path */
        statusFlags &= (uint16_t)~(1u << 13);
        ocRampCounter = 0;
        if (oc_flag != 0u) {
            return;
        }
        if (state != 0u) {
            pwmRunning &= (uint16_t)~(1u << 0);
            pmbusAlertFlags |= (1u << 0);
        }
        pwmOverrideEnable();
        systemState = (int16_t)oc_flag;           /* 0 */
        marginThreshold = (int16_t)0x09C4;
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
    int16_t adc4 =
#ifdef SIMULATION_MODE
        sim_debug.adcbuf4;
#else
        ADCBUF4;
#endif
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
 *   4) If voutCalibrated > threshold AND llcPeriodCmd > 0x1F3F AND bit4 set:
 *      immediate OVP: set all PTPER/PDC to 0, arm PWM, set systemState=3
 * ============================================================================ */
void voutCalibrationAndOvpDetect(void)
{
    int16_t vavg_div4 = vout_avg_sum >> 2;           /* W8 */

    /* Calibrate ADCBUF5: (ADCBUF5 * cal_va2) >> 13 + ofs_va2 */
    int16_t adc5 =
#ifdef SIMULATION_MODE
        sim_debug.adcbuf5;
#else
        ADCBUF5;
#endif
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
        } else {
            ovpCounter = 0;
        }
    } else if (vavg_div4 > ovp_thresh) {
        /* 0x44B6..0x44D4:
         *   trigger when vout is above threshold and
         *   (llcPeriodCmd >= 0x1F3F OR statusFlags bit4 is set).
         */
        if (llcPeriodCmd >= 0x1F3F || (statusFlags & (1u << 4))) {
            pdc1 = 0;
            pdc2 = 0;
            pdc3 = 0;
            controlStatus |= (1u << 4);
            ovpCounter = 0;
            faultResetTimer = 0xC8;
        } else {
            ovpCounter = 0;
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
    Nop(); Nop(); Nop();
    IOCON1bits.PENL = 0;   /* 0x423 bit0 */
    Nop(); Nop(); Nop();
    IOCON2bits.PENH = 0;   /* 0x443 bit1 */
    Nop(); Nop(); Nop();
    IOCON2bits.PENL = 0;   /* 0x443 bit0 */
    Nop(); Nop(); Nop();
    IOCON3bits.PENH = 0;   /* 0x463 bit1 */
    Nop(); Nop(); Nop();
    IOCON3bits.PENL = 0;   /* 0x463 bit0 */
}

/* ============================================================================
 * frequencyLimitControl() — 0x4508-0x456C
 *
 * Frequency/period limiting logic:
 *   1) If faultFlags bits 4+5 both set (0x30):
 *      - Set 0x1DCE=1 (limit active flag)
 *      - If llcPeriodCmd > 0x206C: clamp all y[n] states to 0x206C, enable PWM
 *      - If llcPeriodCmd <= 0x1FA3: call pwmDisableAll
 *   2) If 0x1DCE was set: clear faultFlags bit5, clear 0x1DCE, disable PWM
 *   3) faultFlags bit10: secondary PWM3 PENH enable/disable toggle
 * ============================================================================ */
void frequencyLimitControl(void)
{
    if ((faultFlags & 0x30) == 0x30) {
        /* Both bits 4+5 set: frequency limit active */
        freqLimitActive = 1;

        if (llcPeriodCmd > 0x206C) {                     /* > 8300 */
            /* Clamp all 2P2Z state to 0x206C */
            compY_out = 0x206C;
            compY_n1 = 0x206C;
            compY_n2 = 0x206C;
            pwmOverrideEnable();                   /* 0x4B50 */
        } else if (llcPeriodCmd <= 0x1FA3) {             /* <= 8099 */
            pwmDisableAll();                       /* 0x44DC */
        }
        /* else: llcPeriodCmd in (0x1FA3, 0x206C] — do nothing */
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
        Nop(); Nop(); Nop();
    } else {
        if (pwm3PenhFlag != 0) {
            pwm3PenhFlag = 0;
            IOCON3bits.PENH = 0;
            Nop(); Nop(); Nop();
        }
    }
}

/* ============================================================================
 * pwmOverrideEnable() — 0x4B50-0x4B6C
 *
 * Enables PWM override on all three channels by setting OVRENH/OVRENL bits
 * on IOCON1/2/3 with 3-NOP synchronization delays.
 * This forces PWM outputs to their override values (typically low),
 * effectively disabling normal PWM switching.
 * ============================================================================ */
void pwmOverrideEnable(void)
{
    IOCON1bits.OVRENH = 1;   /* 0x423 bit1 */
    Nop(); Nop(); Nop();
    IOCON1bits.OVRENL = 1;   /* 0x423 bit0 */
    Nop(); Nop(); Nop();
    IOCON2bits.OVRENH = 1;   /* 0x443 bit1 */
    Nop(); Nop(); Nop();
    IOCON2bits.OVRENL = 1;   /* 0x443 bit0 */
    Nop(); Nop(); Nop();
    IOCON3bits.OVRENH = 1;   /* 0x463 bit1 */
    Nop(); Nop(); Nop();
    IOCON3bits.OVRENL = 1;   /* 0x463 bit0 */
}
