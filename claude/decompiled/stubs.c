/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — stubs.c
 * Stub / partial implementations for sub-functions not yet fully decompiled.
 *
 * These functions are called from system.c (watchdogService) and pmbus.c
 * (t1IsrI2cBody).  Original Ghidra addresses are noted for each.
 *
 * As decompilation progresses, move completed functions to their proper
 * module files and remove the stub here.
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

extern void pwmDisable(void); /* 0x4B7C */

/* ============================================================================
 * softStartTargetRamp  (0x34EE – 0x3556)
 *
 * Slews softStartPwmLimit toward the requested voltage target and returns the
 * current slewed limit.  This helper is shared by the normal-mode watchdog.
 * ============================================================================ */
static int16_t softStartTargetRamp(int16_t target)
{
    if (softStartPwmLimit == (uint16_t)0xFFB0) {
        softStartRampCnt = 0;
        softStartDwellCnt = 0;
    }

    if (softStartRampCnt <= 0x012C) {
        uint16_t next_ramp = (uint16_t)(softStartRampCnt + 1u);
        uint16_t next_limit = (uint16_t)(next_ramp * 28u + 0x50u);

        softStartRampCnt = next_ramp;
        softStartPwmLimit = next_limit;

        if ((int16_t)target <= (int16_t)next_limit) {
            softStartPwmLimit = (uint16_t)target;
            statusFlags |= (1u << 5);

            if (target == 0x06B3) {
                LATD |= (1u << 4);
                softStartDwellCnt++;
                if (softStartDwellCnt >= 0x004A) {
                    softStartRampCnt = 0;
                    statusFlags2 &= ~(1u << 5);
                    softStartDwellCnt = 0;
                    droopMode = 3;
                    droopIntegrator = 0x01668000L;
                    statusFlags2 |= 1u;
                }
            } else if (target == 0x0C4D) {
                softStartRampCnt = 0;
                statusFlags2 &= ~(1u << 5);
                droopMode = 3;
                LATD &= ~(1u << 4);
                droopIntegrator = 0;
            }
        }
    }

    return (int16_t)softStartPwmLimit;
}

/* ============================================================================
 * voutUpdateReadPath  (0x2452 – 0x2470)
 *
 * Controls LATD bit2 (GPIO C2 output) based on LATD bit3 state and
 * auxFlags bit1.  Called from t1IsrI2cBody in pmbus.c.
 * ============================================================================ */
void voutUpdateReadPath(void)
{
    uint8_t latd_lo = (uint8_t)LATD;

    if (((latd_lo & 0x08) == 0) && (auxFlags & 0x02)) {
        /* LATD.3 low and auxFlags bit1 set → assert LATD.2 */
        LATD |= 0x0004;
    } else {
        /* Otherwise → de-assert LATD.2 */
        LATD &= ~0x0004;
    }
}

/* ============================================================================
 * monitorStartupResetLatch  (0x2746 – 0x27A0)
 *
 * Detects rising edges on bits 4-8 of startupResetLatch vs. the previous
 * snapshot (startupResetShadow).  Increments per-bit event counters on each
 * edge, saturating at 0xFF.
 * ============================================================================ */
void monitorStartupResetLatch(void)
{
    if (startupResetShadow == startupResetLatch)
        return;

    /* Bit 4 rising edge */
    if (((startupResetShadow & 0x10) != (startupResetLatch & 0x10)) &&
        (startupResetLatch & 0x10)) {
        if (eventCntBit4 != 0xFF)
            eventCntBit4++;
    }
    /* Bit 5 rising edge */
    if (((startupResetShadow & 0x20) != (startupResetLatch & 0x20)) &&
        (startupResetLatch & 0x20)) {
        if (eventCntBit5 != 0xFF)
            eventCntBit5++;
    }
    /* Bit 6 rising edge */
    if (((startupResetShadow & 0x40) != (startupResetLatch & 0x40)) &&
        (startupResetLatch & 0x40)) {
        if (eventCntBit6 != 0xFF)
            eventCntBit6++;
    }
    /* Bit 7 rising edge */
    if (((startupResetShadow & 0x80) != (startupResetLatch & 0x80)) &&
        (startupResetLatch & 0x80)) {
        if (eventCntBit7 != 0xFF)
            eventCntBit7++;
    }
    /* Bit 8 rising edge */
    if (((startupResetShadow & 0x100) != (startupResetLatch & 0x100)) &&
        (startupResetLatch & 0x100)) {
        if (eventCntBit8 != 0xFF)
            eventCntBit8++;
    }

    startupResetShadow = startupResetLatch;
}

/* ============================================================================
 * tempFanHandler  (0x2398 – 0x2450)
 *
 * Temperature / fan band handler.  Selects a thermal band (1-3) based on
 * adcLiveA (0x1BE8) thresholds with hysteresis, then updates the high byte
 * of pwmRunning (0x1BF2) with the band number.
 *
 * Band thresholds depend on pwmRunning bits 1,10:
 *   mode (bits & 0x402) == 0x002: bands at 0xB40..0xD81 / 0xD81..0x1081 / 0x1680..0x2621
 *   mode (bits & 0x402) == 0x402: bands at 0xF00..0x26C1 / 0x26C1..0x3481
 * ============================================================================ */
void tempFanHandler(void)
{
    uint16_t mode = pwmRunning & 0x0402;
    uint8_t band;

    if (mode == 0x0002) {
        /* Normal-power thermal bands */
        uint16_t val = adcLiveA;

        /* Hysteresis: stay in current band unless threshold exceeded */
        if (fanDroopStepCnt == 1) {
            if ((val - 0x0AA0) > 800u) fanDroopStepCnt = 0;
        } else if (fanDroopStepCnt == 2) {
            if ((val - 0x0D20) > 0x0400u) fanDroopStepCnt = 0;
        } else if (fanDroopStepCnt == 3) {
            if ((val + 0xEA20u) > 0x10E0u) fanDroopStepCnt = 0;
        } else {
            fanDroopStepCnt = 0;
        }

        /* Assign new band if idle */
        if ((fanDroopStepCnt == 0) && (val > 0x0B3F)) {
            if (val < 0x0D81)
                fanDroopStepCnt = 1;
            else if (val < 0x1081)
                fanDroopStepCnt = 2;
            else if ((val > 0x167F) && (val < 0x2621))
                fanDroopStepCnt = 3;
        }
        band = (uint8_t)fanDroopStepCnt;
    }
    else if (mode == 0x0402) {
        /* High-power thermal bands */
        uint16_t val = adcLiveA;

        if (fanDroopStepCnt == 2) {
            if ((val - 0x0E60) > 0x19A0u) fanDroopStepCnt = 0;
        } else if (fanDroopStepCnt == 3) {
            if ((val + 0xDA80u) > 0x0FA0u) fanDroopStepCnt = 0;
        }

        if ((fanDroopStepCnt == 0) && (val > 0x0EFF)) {
            if (val < 0x26C1)
                fanDroopStepCnt = 2;
            else if (val < 0x3481)
                fanDroopStepCnt = 3;
        }
        band = (uint8_t)fanDroopStepCnt | 0x04;
    }
    else {
        fanDroopStepCnt = 0;
        band = 0;
    }

    /* Update high byte of pwmRunning (0x1BF3) with band value */
    pwmRunning = (pwmRunning & 0x00FF) | ((uint16_t)band << 8);
}

/* ============================================================================
 * uartCmdSubroutine2  (0x2042 – 0x2158)
 *
 * Power / current calculation for PMBus telemetry path.
 * Computes voutScaledQ2 (0x1BE0), adcLive1 (0x1BE2), pdcShadowA (0x1BDE),
 * pmbusLiveBC0 (0x1BC0), pmbusLiveBC0 min-clamp=6, pmbusLive197A.
 * Performs 32-bit division via FUN_rom_001100 (hardware __divsd).
 * Also checks OV/UV thresholds and sets startupResetLatch bits 2-6.
 *
 * TODO: full decompilation requires __divsd helper.  Stub preserves safe
 *       defaults until implemented.
 * ============================================================================ */
void uartCmdSubroutine2(void)
{
    if (systemState != 2) {
        /* Not in ACTIVE state — clear telemetry accumulators */
        adcLive1     = 0;
        pdcShadowA   = 0;
        pmbusLiveBC0 = 0;
        voutScaledQ2 = 0;
        pmbusLiveBC0 = 0;
    }
    /* TODO: implement full telemetry calculation from Ghidra 0x2042 */
}

/* ============================================================================
 * uartCmdSubroutine3  (0x2224 – 0x2308)
 *
 * Efficiency / ratio calculation for telemetry.  Computes adcLiveA1 (0x1BE6),
 * adcLiveA (0x1BE8), adcLiveA2 (0x1BE4) using division and scaling.
 * Also checks OT/UT thresholds and sets startupResetLatch bits 0-1.
 *
 * TODO: full decompilation requires __divsd helper.
 * ============================================================================ */
void uartCmdSubroutine3(void)
{
    uint16_t mode = pwmRunning & 0x0002;

    if (mode == 0) {
        /* PWM not running — clear telemetry */
        adcLiveA1 = 0;
        adcLiveA  = 0;
        adcLiveA2 = 0;
    } else {
        uint16_t denom = uartCmdParamExt;
        uint16_t clamped_denom = (denom < 0x0C81u) ? 0x0C80u : denom;
        uint16_t prot = protTempAdc;
        uint16_t scaled;
        uint16_t ratio_lo;
        uint16_t ratio_hi;

        adcLiveA = denom;

        if (prot < 0x008Cu) {
            scaled = (uint16_t)(prot + 0x000Fu);
        } else {
            uint32_t prod = (uint32_t)prot * 0x0472u;
            scaled = (uint16_t)(prod >> 10);
        }

        ratio_lo = (uint16_t)(((uint32_t)prot << 11) / clamped_denom);
        ratio_hi = (uint16_t)(((uint32_t)scaled << 11) / clamped_denom);

        if ((prot < 0x008Cu) && ((pwmRunning & 0x0400u) == 0)) {
            ratio_hi = (uint16_t)(ratio_hi + 7u);
        }

        if (uartCmdParam1 < 0xF001u) {
            if (uartCmdParam1 < prot) {
                adcLiveA2 = (uartCmdParam1 < 0x000Fu) ? uartCmdParam1 : prot;
            } else {
                adcLiveA2 = (uartCmdParam1 <= scaled) ? uartCmdParam1 : scaled;
            }
        }

        if (adcLiveA1 <= uartCmdParam0 && uartCmdParam0 <= ratio_hi) {
            adcLiveA1 = uartCmdParam0;
        } else if (adcLiveA1 > uartCmdParam0) {
            adcLiveA1 = ratio_hi;
        } else {
            adcLiveA1 = ratio_lo;
        }

        if (adcLiveA1 < 4u) {
            adcLiveA1 = 4;
        }
    }

    if (((pwmRunning & 0x0002u) != 0) && (vinCooldownTimer == 0)) {
        uint16_t high_clear = 0xFFFFu;
        uint16_t low_clear = 0;

        if (pmbusReadPtr1 < 0xFEDFu) {
            high_clear = (uint16_t)(pmbusReadPtr1 + 0x0120u);
        }

        if (adcLiveA < pmbusReadPtr1) {
            startupResetLatch |= 0x0002u;
        } else if (adcLiveA > high_clear) {
            startupResetLatch &= (uint16_t)~0x0002u;
        }

        if (pmbusReadPtr0 > 0x00A0u) {
            low_clear = (uint16_t)(pmbusReadPtr0 - 0x00A0u);
        }

        if (adcLiveA > pmbusReadPtr0) {
            startupResetLatch |= 0x0001u;
        } else if (adcLiveA < low_clear) {
            startupResetLatch &= (uint16_t)~0x0001u;
        }
    }
}

/* ============================================================================
 * uartCmdSubroutine6  (0x2464 – 0x2484)
 *
 * Peak tracker for adcLiveC (0x1BC8) and adcLiveA2 (0x1BE4).
 * Tracks the maximum value seen for each.
 * ============================================================================ */
void uartCmdSubroutine6(void)
{
    if ((pwmRunning & 0x0002) == 0) {
        /* PWM not running — clear peaks */
        adcLiveB  = 0;
        adcLiveC  = 0;
    } else {
        /* Track peak: keep the larger of current peak and new sample */
        if (adcLiveC <= adcLiveA2)
            adcLiveC = adcLiveA2;
        if (adcLiveB <= adcLiveA1)
            adcLiveB = adcLiveA1;
    }
}

/* ============================================================================
 * uartCmdSubroutine7  (0x2486 – 0x249A)
 *
 * Peak tracker for pdcShadowB (0x1BD0) from pdcShadowA (0x1BDE).
 * ============================================================================ */
void uartCmdSubroutine7(void)
{
    if ((pwmRunning & 0x0001) == 0) {
        /* Not running — clear */
        pdcShadowB = 0;
    } else {
        if (pdcShadowB <= pdcShadowA)
            pdcShadowB = pdcShadowA;
    }
}

/* ============================================================================
 * unknownSubroutine2594  (0x2594 – 0x2632)
 *
 * Power-enable control.  Manages LATE bit7 (GPIO output enable) based on
 * droopEnableFlags (0x1BEA), pwmRunning (0x1BF2), and several condition
 * flags.  Also calls sub_257E for parameter-based configuration.
 *
 * Simplified implementation: controls LATE.7 based on droopEnableFlags bit2.
 * ============================================================================ */
void unknownSubroutine2594(void)
{
    if (((pwmRunning & 0x0002) == 0) || (vinCooldownTimer != 0))
        return;

    /* Simplified: if droopEnableFlags bit2 set → assert LATE.7, else deassert */
    if (droopEnableFlags & 0x0004) {
        LATE |= 0x0080;
    } else {
        LATE &= ~0x0080;
    }
}

/* ============================================================================
 * droopDecayFreq  (sub_3558, 0x3558 – 0x35D2)
 *
 * Droop integrator decay + frequency target update.  Called from
 * droopMode3Watchdog to bring voutTargetCode back toward voutRefTarget.
 *
 *   1) Select decay delta:
 *      - voutTargetCode > 0xB11: fast decay (0x8659), reset ramp counter.
 *      - currentLimitFlags.0x0800 or restartFlags.0x06: slow decay (0x04FA),
 *        with ramp counter gating (hold at zero until counter > 0x226).
 *      - otherwise: fast decay (0x8659).
 *   2) Subtract delta from 32-bit integrator.
 *   3) Compute voutTargetCode = voutRefTarget − (integrator >> 14).
 *   4) If reached target (voutTargetCode >= voutRefTarget): reset integrator,
 *      clamp to target, clear flags and LATD.4.
 * ============================================================================ */
void droopDecayFreq(void)
{
    /* Step 1: select decay delta */
    if (voutTargetCode > 0x0B11) {
        /* Far from target: fast decay, reset ramp counter */
        droopDelta = 0x8659L;
        droopRampCounter = 0;
    } else if ((currentLimitFlags & 0x0800) || (restartFlags & 0x06)) {
        /* Current-limit or restart active: slow decay */
        droopDelta = 0x04FAL;

        if (runtimeFlags & 0x0008) {
            /* Ramp counter gating: hold delta at zero until count > 0x226 */
            droopRampCounter++;
            if (droopRampCounter > 0x0226) {
                droopRampCounter = 0x0226;  /* clamp counter */
            } else {
                droopDelta = 0;             /* hold: no decay yet */
            }
        }
    } else {
        /* Default: fast decay */
        droopDelta = 0x8659L;
    }

    /* Step 2: 32-bit integrator decay (subtract) */
    droopIntegrator -= droopDelta;

    /* Step 3: compute voutTargetCode from integrator */
    voutTargetCode = voutRefTarget - (int16_t)(droopIntegrator >> 14);

    /* Step 4: check if reached target */
    if (voutTargetCode >= voutRefTarget) {
        /* Reached/overshot: reset integrator, clamp to target, clean up */
        droopIntegrator = 0;
        voutTargetCode = voutRefTarget;
        faultResetTimer = 5;
        runtimeFlags &= ~0x0008;
        restartFlags &= ~0x06;
        LATD &= ~0x0010;
        protectionStatus &= ~0x0200;
    } else {
        /* Still approaching target */
        protectionStatus |= 0x0200;
    }
}

/* ============================================================================
 * droopIntegrateFreq  (sub_35D4, 0x35D4 – 0x3664)
 *
 * Droop integrator + frequency target computation.  Called from
 * droopMode3Watchdog when current-droop re-computation is needed.
 *
 *   1) Select droop delta: 0x4C75 if voutTargetCode > 0xB99, else 0x9547.
 *   2) Accumulate into 32-bit integrator (droopWorkB:droopWorkA),
 *      clamp at 0x01668000.
 *   3) If saturated AND Imeas <= 0xC0: compute initFreq with
 *      [0x125C, 0x13EC] clamp, set flags, droopMode = 4.
 *   4) Update voutTargetCode = voutRefTarget − (integrator >> 14).
 * ============================================================================ */
void droopIntegrateFreq(void)
{
    /* Step 1: select delta based on voutTargetCode vs 0xB99 threshold */
    droopDelta2 = (voutTargetCode > 0x0B99) ? 0x4C75L : 0x9547L;

    /* Step 2: 32-bit integrator accumulate */
    droopIntegrator += droopDelta2;

    if (droopIntegrator > 0x01667FFFL) {
        /* Saturated → clamp to 0x01668000 */
        droopIntegrator = 0x01668000L;

        /* Step 3: if Imeas low, compute initFreq */
        int16_t imeas = Imeas;
        if (imeas <= 0x00C0) {
            int32_t prod = (int32_t)imeas * (int16_t)0xFD5A;  /* × (−0x02A6) */
            int16_t scaled = (int16_t)(prod >> 8);
            int16_t target = scaled + 0x1418;

            if (target > 0x13EC)
                initFreq = 0x13EC;          /* upper clamp */
            else if (target > 0x125B)
                initFreq = target;
            else
                initFreq = 0x125C;          /* lower clamp */

            faultResetTimer = 5;
            runtimeFlags |= 0x0002;
            statusFlags = (statusFlags & ~0x0002) | 0x0001;
            protectionStatus |= 0x0800;
        }

        countdown_1D20 = 0x32;
        droopMode = 4;
        protectionStatus &= ~0x0400;
    } else {
        protectionStatus |= 0x0400;
    }

    /* Step 4: voutTargetCode = voutRefTarget − (integrator >> 14) */
    voutTargetCode = voutRefTarget - (int16_t)(droopIntegrator >> 14);
}

/* ============================================================================
 * droopMode0Watchdog  (0x3666 – 0x3718)
 *
 * Droop compensation mode-0 watchdog.  Manages voltage target (initFreq,
 * 0x1D34) based on current measurement (Imeas, 0x1D44) with fixed-point
 * scaling.  Updates droopMode (0x1268) and statusFlags (0x125A).
 *
 * Two operating branches:
 *   A) If conditions met (no current-limit, no fault, countdown expired):
 *      compute initFreq from Imeas, clamp to [0x2454, 0x34BC], set droopMode=3.
 *   B) Else if low-current & not latched:
 *      compute initFreq from Imeas (different scale), clamp to [0x125C, 0x13EC],
 *      set faultResetTimer=5, toggle statusFlags bits.
 *
 * Also clears statusFlags2 bit5 and sets voutRefTarget = 0x6B3 (Vref).
 * ============================================================================ */
void droopMode0Watchdog(void)
{
    if (controlStatus & 0x0001) {
        /* controlStatus bit0 set → reset droop state */
        statusFlags = 0;
        runtimeFlags  = 0;
    }
    else {
        int16_t imeas = Imeas;

        if ((((currentLimitFlags & 0x0800) == 0) &&
             ((systemFlags & 0x00A0) == 0)) ||
            (ocpThresholdHw <= (uint16_t)delayTimer)) {
            if (countdown_1D20 == 0) {
                if (protectionStatus & 0x0800) {
                    /* Branch A: compute scaled frequency target */
                    int32_t prod = (int32_t)imeas * (-0x0D3D);
                    int16_t scaled = (int16_t)(prod >> 8);
                    int16_t target = scaled + 0x33A6;

                    initFreq = 0x34BC;  /* upper clamp */
                    if (target < 0x34BC)
                        initFreq = target;
                    if (target < 0x2454)
                        initFreq = 0x2454;  /* lower clamp */

                    runtimeFlags |= 0x0002;
                    statusFlags = (statusFlags & ~0x0001) | 0x0002;
                    protectionStatus &= ~0x0800;
                }
                droopMode = 3;
            }
        }
        else if ((Imeas_cal_a < 0x00C1) &&
                 (imeas <= 0x00C0) &&
                 ((protectionStatus & 0x0800) == 0)) {
            /* Branch B: low-current droop */
            int32_t prod = (int32_t)imeas * (-0x02A6);
            int16_t scaled = (int16_t)(prod >> 8);
            int16_t target = scaled + 0x1418;

            initFreq = 0x13EC;  /* upper clamp */
            if (target < 0x13EC)
                initFreq = target;
            if (target < 0x125C)
                initFreq = 0x125C;  /* lower clamp */

            faultResetTimer = 5;
            runtimeFlags |= 0x0002;
            statusFlags = (statusFlags & ~0x0002) | 0x0001;
            protectionStatus |= 0x0800;
        }
    }

    statusFlags2 &= ~0x0020;   /* clear bit5 */
    voutRefTarget = 0x06B3;         /* set Vref setpoint */
}

/* ============================================================================
 * droopMode3Watchdog  (0x371A – 0x3788)
 *
 * Droop compensation mode-3 watchdog.  More complex than mode-0; manages
 * GPIO LATD bit4, calls sub_3558 / droopIntegrateFreq for frequency
 * computation, and monitors current threshold crossings.
 *
 * Sets voutRefTarget = 0xC4D (~12.25V scaled Vref).
 *
 * Calls droopDecayFreq (sub_3558) and droopIntegrateFreq (sub_35D4).
 * ============================================================================ */
void droopMode3Watchdog(void)
{
    voutRefTarget = 0x0C4D;  /* ~12.25V scaled Vref */

    uint16_t clFlags = currentLimitFlags & 0x0800;

    if (clFlags == 0) {
        statusFlags2 = (statusFlags2 & ~0x0020) | 0x0100;
    }

    if ((statusFlags2 & 0x0100) && (clFlags != 0)) {
        /* Current-limit active with mode-3 flag: check threshold */
        if (Imeas <= 0x00C0) {
            statusFlags2 |= 0x0020;
        }
        statusFlags2 &= ~0x0100;
    }

    if (controlStatus & 0x0001) {
        /* Reset state */
        statusFlags = 0;
        runtimeFlags  = 0;
        return;
    }

    /* Simplified: check if LATD bit4 is set and manage droop */
    if ((statusFlags & 0x0010) == 0) {
        if (((clFlags == 0) || (runtimeFlags & 0x0008)) &&
            (voutTargetCode != 0x0C4D) &&
            ((systemFlags & 0x00A0) == 0)) {
            statusFlags2 &= ~0x0020;
            droopDecayFreq();
            protectionStatus &= ~0x0400;
        }
        else if ((statusFlags2 & 0x0020) || (systemFlags & 0x00A0)) {
            droopIntegrateFreq();
            LATD |= 0x0010;
        }
        else {
            uint8_t ld = (uint8_t)LATD;
            if ((ld & 0x10) == 0) {
                voutRefTarget = 0x0C4D;
                return;
            }
            if (Imeas > 0x00D2) {
                voutRefTarget = 0x0C4D;
                return;
            }
            droopIntegrateFreq();
        }
    }
}

/* ============================================================================
 * runNormalMode  (0x378A – 0x37A8)
 *
 * Normal LLC watchdog path.  Detects 12V-present, advances the PWM enable
 * sequence, then slews voutTargetCode toward voutRefTarget using the shared
 * soft-start helper above.
 * ============================================================================ */
void runNormalMode(void)
{
    if (!(statusFlags2 & (1u << 5))) {
        if ((vout_sum > 0x02FF) && !(statusFlags & (1u << 7))) {
            statusFlags |= (1u << 7);
        } else {
            pwmDisable();
        }
    }

    voutTargetCode = softStartTargetRamp(voutRefTarget);
}

/* --------------------------------------------------------------------------
 * initI2cPointerTables (0x13C2 – 0x150A)
 *
 * Initialises I2C2 PMBus pointer/dispatch tables (ptrTable19B0, ptrTable19BA,
 * ptrTable19D0, ptrTable19F0, ptrTable1A10, ptrTable1A70, ptrTable1B90)
 * with RAM addresses of PMBus data/config registers.
 *
 * 165 instructions, all MOV #imm, W0 / MOV W0, addr pairs.
 * TODO: decompile full table initialization from Ghidra.
 * -------------------------------------------------------------------------- */
void initI2cPointerTables(void)
{
    /* stub — pointer tables left uninitialized */
}
