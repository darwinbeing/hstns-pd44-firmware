/* ============================================================================
 * HSTNS-PD44  dsPIC33FJ64GS606  Firmware Decompilation
 * File   : t1_sub_prot.c
 * Subject: T1 ISR subroutines — protection, monitoring, and control helpers
 *
 * Functions decompiled (address order):
 *   0x2FA6  flagProcess       — control flag processing / droop-mode logic
 *   0x3052  standbyCheck      — standby entry with frequency ramp-down
 *   0x30AA  diagCheck         — diagnostic / gate-drive enable check
 *   0x30E2  thresholdCompare      — 3-zone threshold comparison helper
 *   0x30F8  protectionCheck   — ADCBUF13 protection with debounce
 *   0x316E  tempSample        — ADCBUF15 temperature threshold detection
 *   0x31B2  debounceCounter       — generic debounce counter helper
 *   0x31CC  currentLimit      — current limit with PWM gate / ADCBUF5
 *   0x3242  clampToward           — single-step clamp-toward-target helper
 *   0x3254  vinCheck          — input voltage check / droop-mode VIN path
 *
 * Decompiled 2026-03-25.  All RAM addresses cross-referenced with ram_map.h.
 * ============================================================================ */

#include <stdint.h>
#include <xc.h>        /* dsPIC SFR definitions (ADCBUF, LATD, IOCON, PORTD) */
#include "variables.h"

/* All RAM variables are now declared in ram_map.h and defined in ram_vars.c */

/* External function called only in currentLimit */
extern void shutdownPwm(void);   /* 0x4B50 — pwmOverrideEnable alias */

#ifdef UNIT_TEST_MINIMAL
#define PROT_LOCAL
#else
#define PROT_LOCAL static
#endif

/* ============================================================================
 * thresholdCompare  (0x30E2 – 0x30F6)
 *
 * 3-zone threshold comparison with hysteresis.
 * Returns 0 or 1 based on which zone the value falls into,
 * but preserves W3 (the "previous state" argument) as the
 * tie-break when the value is between the two thresholds.
 *
 * Arguments (dsPIC calling convention — registers at call site):
 *   W0  = value to test
 *   W1  = low  threshold
 *   W2  = high threshold
 *   W3  = previous comparison result (0 or 1, used for hysteresis)
 *
 * Returns:
 *   W0  = 1 if value <  low_threshold  (below low)
 *          0 if low_threshold <= value <= high_threshold  (middle band)
 *          W3 if value > high_threshold  (hysteresis hold)
 *
 * Then zero-extended (ZE) so upper byte is cleared before return.
 * ============================================================================ */
PROT_LOCAL uint16_t thresholdCompare(uint16_t value, uint16_t low_thresh,
                                     uint16_t high_thresh, uint16_t prev_state)
{
    /* 0x30E2  MOV W0, W4   — save value into W4 */
    /* 0x30E4  MOV W3, W0   — W0 = prev_state (default return value) */
    uint16_t result = prev_state;

    /* 0x30E6  SUB W4, W1   — value - low_thresh, set C */
    /* 0x30E8  BRA NC, ...  — if value >= low_thresh (no borrow) skip */
    if ((uint16_t)value < (uint16_t)low_thresh) {
        /* 0x30EA  MOV #0x1, W0 — below low threshold → return 1 */
        result = 1;
    } else if ((uint16_t)value <= (uint16_t)high_thresh) {
        /* 0x30EE..0x30F2:
         * not GTU (value <= high) -> CLR W0
         */
        result = 0;
    }
    /* value > high: keep prev_state */

    /* 0x30F4  ZE W0, W0   — zero-extend low byte (clears upper byte) */
    result = (uint8_t)result;
    return result;  /* 0x30F6  RETURN */
}

/* ============================================================================
 * clampToward  (0x3242 – 0x3252)
 *
 * Move W0 one step toward W1 (clamp W0 to W1, signed).
 * If W0 > W1 : return W0-1
 * If W0 < W1 : return W0+1
 * If W0 == W1: return W1  (already at target)
 *
 * Arguments:
 *   W0  = current value
 *   W1  = target value
 * Returns:
 *   W0  = new value (one step closer to W1, or W1 itself)
 * ============================================================================ */
PROT_LOCAL int16_t clampToward(int16_t current, int16_t target)
{
    /* 0x3244  SUB W0, W1  — signed compare */
    /* 0x3246  BRA LE, ... — if current <= target go low branch */
    if (current > target) {
        /* 0x3246  DEC W0, W1  — W1 = W0 - 1 */
        /* 0x3250  MOV W1, W0 */
        return current - 1;
    }

    /* 0x324A  SUB W0, W1  — signed compare again */
    /* 0x324C  BRA GE, ... — if current >= target → already at or passed target */
    if (current < target) {
        /* 0x324E  INC W0, W1  — W1 = W0 + 1 */
        /* 0x3250  MOV W1, W0 */
        return current + 1;
    }

    /* current == target */
    return current;  /* 0x3252  RETURN */
}

/* ============================================================================
 * debounceCounter  (0x31B2 – 0x31CA)
 *
 * Generic rising-edge debounce / pulse counter helper.
 *
 * Arguments (registers at call site):
 *   W0  = new signal level (0 = inactive, non-zero = active)
 *   W1  = debounce count threshold
 *   W2  = pointer to uint16_t counter variable
 *   W3  = previous signal level (for edge detection)
 *
 * Behaviour:
 *   - If new level != previous level (W0 != W3):
 *       Increment counter at *W2.
 *       If counter >= W1: clear counter, copy W0 → W3 (latch new state).
 *       If counter <  W1: clear counter, restore W3 (no latch yet).
 *   - If new level == previous level (no edge):
 *       Clear counter at *W2.
 *
 * Returns:
 *   W0  = zero-extended W3  (latched / confirmed signal level)
 * ============================================================================ */
PROT_LOCAL uint16_t debounceCounter(uint16_t new_level, uint16_t thresh,
                                    volatile uint16_t *counter, uint16_t prev_level)
{
    /* 0x31B2  MOV W0, W4   — save new_level */
    uint16_t saved_new = new_level;

    /* 0x31B4  SUB W3, W0   — prev - new, sets Z if equal */
    /* 0x31B6  BRA Z, clear — same level: clear counter */
    if (prev_level != new_level) {
        /* Different level detected: increment counter */
        /* 0x31B8  INC [W2], W0  — W0 = *counter + 1 */
        /* 0x31BA  MOV W0, [W2]  — *counter = W0 */
        uint16_t cnt = *counter + 1;
        *counter = cnt;

        /* 0x31BC  SUB W0, W1   — cnt - thresh */
        /* 0x31BE  BRA NC, done — if cnt >= thresh → latch */
        if (cnt < thresh) {
            /* Count not yet reached: reset counter, keep old state */
            /* 0x31C0  CLR [W2]    — *counter = 0 */
            /* 0x31C2  MOV W4, W3  — restore W3 = saved_new  (keep prev unchanged) */
            *counter = 0;
            prev_level = saved_new;  /* W3 = W4 */
        }
        /* else cnt >= thresh: counter already written; W3 keeps new state
         * (W3 was NOT updated here — the latch happens because W3 already
         *  held prev_level and the ZE at the end returns W3) */
        /* Fall through to ZE */
    } else {
        /* Same level: clear counter */
        /* 0x31C6  CLR [W2]  */
        *counter = 0;
    }

    /* 0x31C8  ZE W3, W0   — return zero-extended prev_level (latched state) */
    return (uint8_t)prev_level;  /* 0x31CA  RETURN */
}

/* ============================================================================
 * flagProcess  (0x2FA6 – 0x3050)
 *
 * Main control flag processing called from T1 ISR.
 * Manages droop-mode debounce counters (0x1232, 0x1230) and flag bits
 * in runtimeFlags (bit4 = "fan load" flag, bit5 = "temp limit" flag) and
 * flags_126B (bit2 = "ready" flag) based on:
 *   - statusFlags bit5 (regulation active)
 *   - thermalFlags bit7 (over-temperature)
 *   - droopMode value (0x1268)
 *   - var_1BBE (0x1BBE) — frequency/period measurement
 *   - var_1BF2 (0x1BF2) bits [10,1]
 *   - var_1BE4 (0x1BE4) — some counter
 *   - Imeas (0x1D44) — calibrated output current
 * ============================================================================ */
#ifndef UNIT_TEST_MINIMAL

void flagProcess(void)  /* 0x2FA6 */
{
    /* 0x2FA6  MOV 0x125A, W0 */
    /* 0x2FA8  AND #0x20, W0  — test bit5 (regulation active) */
    /* 0x2FAA  BRA Z, 0x3048  — if not active, clear everything and return */
    if (!(statusFlags & 0x0020)) {
        /* Regulation not active: clear both debounce counters and flags */
        /* 0x3048  BCLR 0x126A, #5 */
        /* 0x304A  MOV W0, 0x1232   — W0 is still 0 from AND */
        /* 0x304C  BCLR 0x126B, #2  */
        /* 0x304E  MOV W0, 0x1230   */
        /* 0x3050  RETURN            */
        runtimeFlags &= ~(1u << 5);
        protCounter1232 = 0;
        runtimeFlags &= ~(1u << 10);
        protCounter1230 = 0;
        return;
    }

    /* Check for "deep droop" entry condition (droopMode == 4) */
    /* 0x2FAC  BTST 0x1BEB, #1  — test secondary enable flag */
    /* 0x2FAE  BRA Z, 0x2FDC    */
    /* 0x2FB0  BTST 0x1266, #7  — test OT flag */
    /* 0x2FB2  BRA Z, 0x2FDC    */
    if ((droopEnableFlags & (1u << 9)) && (thermalFlags & (1u << 7))) {

        /* 0x2FB4  MOV 0x1268, W2 */
        uint16_t dm = droopMode;

        /* 0x2FB6  SUB W2, #4      — droopMode == 4? */
        /* 0x2FB8  BRA NZ, 0x2FD0  */
        if (dm == 4) {
            /* droopMode == 4 path */
            /* 0x2FBA  MOV 0x1BBE, W1 */
            uint16_t bbe = protTempAdc;

            /* 0x2FBC  MOV #0x3F, W0 */
            /* 0x2FBE  SUB W1, W0    — bbe > 0x3F? */
            /* 0x2FC0  BRA GTU, 0x2FD0 */
            if (bbe <= 0x3F) {
                /* 0x2FC2  MOV 0x1BF2, W0 */
                uint16_t bf2 = pwmRunning;

                /* 0x2FC4  MOV #0x402, W1 */
                /* 0x2FC6  AND W0, W1, W0  — keep bits [10] and [1] */
                /* 0x2FC8  SUB W0, #0x2    — == 0x002? */
                /* 0x2FCA  BRA NZ, 0x2FDE  */
                if ((bf2 & 0x0402) == 0x0002) {
                    /* 0x2FCC  BSET 0x126A, #4 — set "fan load" flag */
                    runtimeFlags |= (1u << 4);
                    goto after_droop4_check;  /* BRA 0x2FDE */
                }
                goto after_droop4_check;  /* BRA NZ also lands at 0x2FDE */
            }
            /* bbe > 0x3F — fall through into droopMode != 4 path at 0x2FD0 */
        }

        /* droopMode != 4  OR  bbe > 0x3F */
        /* 0x2FD0  MOV 0x1BBE, W1 */
        {
            uint16_t bbe = protTempAdc;

            /* 0x2FD2  MOV #0x45, W0 */
            /* 0x2FD4  SUB W1, W0    — bbe > 0x45? */
            /* 0x2FD6  BRA GTU, 0x2FDC */
            if (bbe <= 0x45) {
                /* 0x2FD8  SUB W2, #3    — droopMode == 3? */
                /* 0x2FDA  BRA NZ, 0x2FDE */
                if (droopMode == 3) {
                    goto clear_fan_flag;  /* 0x2FDC */
                }
                /* else droopMode != 3 → fall through to after_droop4_check */
                goto after_droop4_check;
            }
        }

clear_fan_flag:
        /* 0x2FDC  BCLR 0x126A, #4 */
        runtimeFlags &= ~(1u << 4);
    } else {
        /* One of the enable conditions is false */
        runtimeFlags &= ~(1u << 4);
    }

after_droop4_check:
    /* ---- Manage "temp limit" flag (runtimeFlags bit5) and counter 0x1232 ---- */

    /* 0x2FDE  MOV 0x126A, W0 */
    /* 0x2FE0  AND W0, #0x10, W0  — test bit4 (fan-load flag just set/clear above) */
    /* 0x2FE2  BRA Z, 0x300A      — if bit4 clear → clear temp-limit and counter */
    if (runtimeFlags & (1u << 4)) {
        /* bit4 is set */
        /* 0x2FE4  MOV 0x1BE4, W1 */
        uint16_t be4 = adcLiveA2;

        /* 0x2FE6  SUB W1, #0xA    — be4 > 0xA? */
        /* 0x2FE8  BRA GTU, 0x2FF0 */
        if (be4 > 0xA) {
            goto clear_temp_flag;  /* 0x2FF0 */
        }

        /* 0x2FEA  MOV 0x1D44, W0   — Imeas */
        int16_t imeas = Imeas;

        /* 0x2FEC  SUB W0, #0x19    — Imeas <= 0x19? */
        /* 0x2FEE  BRA LE, 0x2FF6   */
        if (imeas <= 0x19) {
            /* 0x2FF6  SUB W1, #0x7  — be4 > 7? */
            /* 0x2FF8  BRA GTU, 0x300E */
            if (be4 > 7) {
                goto after_temp_counter;  /* 0x300E */
            }

            /* be4 <= 7 and Imeas <= 0x19: increment counter */
            /* 0x2FFA  INC 0x1232, WREG */
            /* 0x2FFC  MOV W0, 0x1232   */
            uint16_t cnt = protCounter1232 + 1;
            protCounter1232 = cnt;

            /* 0x2FFE  MOV #0x3E8, W1  — 1000 */
            /* 0x3000  SUB W0, W1       — cnt > 1000? */
            /* 0x3002  BRA LE, 0x300E   */
            if (cnt > 1000) {
                /* 0x3004  MOV W1, 0x1232  — clamp at 1000 */
                protCounter1232 = 1000;

                /* 0x3006  BSET 0x126A, #5  — set temp-limit flag */
                runtimeFlags |= (1u << 5);
                goto after_temp_counter;  /* BRA 0x300E */
            }
            goto after_temp_counter;  /* BRA LE */
        }
        /* Imeas > 0x19  OR  be4 > 0xA */

clear_temp_flag:
        /* 0x2FF0  BCLR 0x126A, #5 — clear temp-limit flag */
        runtimeFlags &= ~(1u << 5);
        /* 0x2FF2  CLR 0x1232      — clear counter */
        protCounter1232 = 0;
        /* 0x2FF4  BRA 0x300E      */
        goto after_temp_counter;
    } else {
        /* bit4 clear → clear temp-limit flag and zero counter */
        /* 0x300A  BCLR 0x126A, #5 */
        runtimeFlags &= ~(1u << 5);
        /* 0x300C  MOV W0, 0x1232   — W0 is 0 (AND result was 0) */
        protCounter1232 = 0;
    }

after_temp_counter:
    /* ---- Manage "ready" flag (flags_126B bit2) and counter 0x1230 ---- */

    /* 0x300E  MOV 0x1268, W0 */
    /* 0x3010  SUB W0, #3       — droopMode == 3? */
    /* 0x3012  BRA NZ, 0x3042   */
    if (droopMode != 3) {
        goto clear_ready_flag;  /* 0x3042 */
    }

    /* droopMode == 3 */
    /* 0x3014  MOV 0x1264, W1 */
    /* 0x3016  MOV #0x600, W0  */
    /* 0x3018  AND W1, W0, W1  — test bits [10:9] of protectionStatus */
    /* 0x301A  BRA NZ, 0x3042  */
    if (protectionStatus & 0x0600) {
        goto clear_ready_flag;
    }

    /* 0x301C  MOV 0x1BE4, W2 */
    uint16_t be4_2 = adcLiveA2;

    /* 0x301E  SUB W2, #0xC    — be4 > 0xC? */
    /* 0x3020  BRA GTU, 0x3028 */
    if (be4_2 > 0xC) {
        goto no_ready_increment;  /* 0x3028 */
    }

    /* 0x3022  MOV 0x1D44, W0  — Imeas */
    /* 0x3024  SUB W0, #0x19   — Imeas <= 0x19? */
    /* 0x3026  BRA LE, 0x302E  */
    if (Imeas <= 0x19) {
        /* 0x302E  SUB W2, #9   — be4 > 9? */
        /* 0x3030  BRA GTU, 0x3050 */
        if (be4_2 > 9) {
            return;  /* 0x3050 RETURN */
        }

        /* be4 <= 9 and Imeas <= 0x19: increment ready counter */
        /* 0x3032  INC 0x1230, WREG */
        /* 0x3034  MOV W0, 0x1230   */
        uint16_t cnt2 = protCounter1230 + 1;
        protCounter1230 = cnt2;

        /* 0x3036  MOV #0x3E8, W1  — 1000 */
        /* 0x3038  SUB W0, W1       */
        /* 0x303A  BRA LE, 0x3050   */
        if (cnt2 > 1000) {
            /* 0x303C  MOV W1, 0x1230  — clamp at 1000 */
            protCounter1230 = 1000;

            /* 0x303E  BSET 0x126B, #2 — set "ready" flag */
            runtimeFlags |= (1u << 10);
            /* 0x3040  RETURN */
            return;
        }
        return;  /* 0x3050 RETURN */
    }

no_ready_increment:
    /* Imeas > 0x19 OR be4 > 0xC */
    /* 0x3028  BCLR 0x126B, #2 */
    runtimeFlags &= ~(1u << 10);
    /* 0x302A  MOV W1, 0x1230   — W1 = protectionStatus & 0x600 result (0 here) */
    protCounter1230 = 0;
    /* 0x302C  RETURN */
    return;

clear_ready_flag:
    /* droopMode != 3 OR protectionStatus bits[10:9] set */
    /* 0x3042  BCLR 0x126B, #2 */
    runtimeFlags &= ~(1u << 10);
    /* 0x3044  CLR 0x1230      */
    protCounter1230 = 0;
    /* 0x3046  RETURN */
}

/* ============================================================================
 * standbyCheck  (0x3052 – 0x30A8)
 *
 * Checks conditions for standby entry and manages a frequency ramp-down
 * counter/register (0x1DA2 / voutSetpoint area) with hysteresis.
 *
 * Standby entry conditions (all must be true):
 *   - statusFlags bits [8] and [5] are both CLEAR  (AND #0x120 == 0)
 *   - droopMode == 0
 *   - systemState == 2  (ACTIVE state)
 *
 * When conditions hold: increment counter 0x1234, clamp to 15.
 *   If counter > 14: select ramp target (0x2710 or 0xD48) based on
 *   vout_sum vs 0x17E, write to 0x1DA2, and set flags_125B bit0.
 *   Otherwise:       write 0x2710 to 0x1DA2 unconditionally.
 *
 * Frequency ramp-down section (always runs after the above):
 *   If 0x1236 > 0xD48 AND flags_125B bit0 set:
 *     Decrement 0x1236 by 0x32 per call, clamp to 0xD48.
 *     Write 0x1236 → 0x1DA2.
 * ============================================================================ */
void standbyCheck(void)  /* 0x3052 */
{
    /* 0x3052  MOV 0x125A, W0 */
    /* 0x3054  AND #0x120, W0   — test bits 8 and 5 */
    /* 0x3056  BRA NZ, 0x308A   — if either is set, clear counter and skip */
    if (statusFlags & 0x0120) {
        goto standby_skip;  /* 0x308A */
    }

    /* 0x3058  CP0 0x1268       — droopMode == 0? */
    /* 0x305A  BRA NZ, 0x308A   */
    if (droopMode != 0) {
        goto standby_skip;
    }

    /* 0x305C  MOV 0x1E22, W0   — systemState */
    /* 0x305E  SUB W0, #2       — == 2? */
    /* 0x3060  BRA NZ, 0x308A   */
    if (systemState != 2) {
        goto standby_skip;
    }

    /* All conditions met: increment standby counter */
    /* 0x3062  INC 0x1234, WREG */
    /* 0x3064  MOV W0, 0x1234   */
    uint16_t stby_cnt = protCounter1234 + 1;
    protCounter1234 = stby_cnt;

    /* 0x3066  SUB W0, #0xE     — cnt > 14? */
    /* 0x3068  BRA LE, 0x3084   */
    if (stby_cnt > 14) {
        /* Clamp counter at 15 */
        /* 0x306A  MOV #0xF, W0 */
        /* 0x306C  MOV W0, 0x1234 */
        protCounter1234 = 15;

        /* Select ramp target based on vout_sum vs 0x17E (382) */
        /* 0x306E  MOV 0x1DA0, W1  — vout_sum */
        /* 0x3070  MOV #0x17E, W0  */
        /* 0x3072  SUB W1, W0      — vout_sum > 0x17E? */
        /* 0x3074  BRA GT, 0x307A  */
        if (vout_sum > 0x017E) {
            /* 0x307A  MOV #0xD48, W0  — lower frequency target (3400) */
            /* 0x307C  MOV W0, 0x1DA2  */
            voutRefInitial = 0x0D48;
            /* 0x307E  MOV W0, 0x1236  */
            protSetpoint1236 = 0x0D48;
        } else {
            /* 0x3076  MOV #0x2710, W0  — higher frequency target (10000) */
            /* (fall through to) */
            /* 0x307E  MOV W0, 0x1236  */
            /* 0x3080  MOV W0, 0x1DA2  via separate path below */
            protSetpoint1236 = 0x2710;
        }

        /* 0x3080  BSET 0x125B, #0  — set ramp-active flag (statusFlags bit8) */
        statusFlags |= (1u << 8);
        goto ramp_section;  /* BRA 0x308C */
    }

    /* Counter <= 14: write 0x2710 unconditionally */
    /* 0x3084  MOV #0x2710, W0  */
    /* 0x3086  MOV W0, 0x1DA2   */
    voutRefInitial = 0x2710;
    /* 0x3088  BRA 0x308C        */
    goto ramp_section;

standby_skip:
    /* Clear standby counter when conditions not met */
    /* 0x308A  CLR 0x1234 */
    protCounter1234 = 0;

ramp_section:
    /* ---- Frequency ramp-down section ---- */
    /* 0x308C  MOV 0x1236, W1 */
    /* 0x308E  MOV #0xD48, W2   — minimum target = 0xD48 (3400) */
    /* 0x3090  SUB W1, W2       — 0x1236 > 0xD48? */
    /* 0x3092  BRA LE, 0x30A8   */
    ; uint16_t ramp_val = protSetpoint1236;
    if (ramp_val > 0x0D48) {
        /* 0x3094  BTST 0x125B, #0 — ramp-active flag set? (statusFlags bit8) */
        /* 0x3096  BRA Z, 0x30A8   */
        if ((statusFlags & (1u << 8))) {
            /* Ramp down: decrement by 0x32 (50) per ISR tick */
            /* 0x3098  MOV #0xFFCE, W0  — 0xFFCE = -50 in two's complement */
            /* 0x309A  ADD W1, W0, W0   — W0 = ramp_val - 50 */
            /* 0x309C  MOV W0, 0x1236   */
            uint16_t new_val = ramp_val + (uint16_t)0xFFCE; /* = ramp_val - 50 */
            protSetpoint1236 = new_val;

            /* 0x309E  SUB W0, W2       — still > 0xD48? */
            /* 0x30A0  BRA GT, 0x30A4   — if still above, skip clamp */
            if ((int16_t)new_val <= (int16_t)0x0D48) {
                /* Clamp to minimum */
                /* 0x30A2  MOV W2, 0x1236 */
                protSetpoint1236 = 0x0D48;
            }

            /* 0x30A4  MOV 0x1236, W0   */
            /* 0x30A6  MOV W0, 0x1DA2   — write final ramp value to DAC/freq reg */
            voutRefInitial = protSetpoint1236;
        }
    }

    /* 0x30A8  RETURN */
}

/* ============================================================================
 * diagCheck  (0x30AA – 0x30E0)
 *
 * Diagnostic check that controls LATD bit7 (port D bit 7 = diagnostic output)
 * via 0x1BEC bit7, based on:
 *   - systemState (must not be 1 = STARTUP for the main path)
 *   - currentLimitFlags bit11 (current-limit active)
 *   - voutTargetCode value vs 0xC4D
 *   - LATD bit4 (hardware gate status)
 *   - droopMode value
 * ============================================================================ */
void diagCheck(void)  /* 0x30AA */
{
    /* 0x30AA  MOV 0x1E22, W0  */
    /* 0x30AC  SUB W0, #1       — systemState == ST_STARTUP? */
    /* 0x30AE  BRA NZ, 0x30B4   */
    if (systemState == ST_STARTUP) {
        /* In STARTUP state: force diagnostic output low */
        /* 0x30B0  BCLR 0x1BEC, #7 */
        startupResetLatch &= ~(1u << 7);
        /* 0x30B2  RETURN */
        return;
    }

    /* 0x30B4  MOV 0x1E18, W1  */
    uint16_t e18 = currentLimitFlags;

    /* 0x30B6  MOV #0x800, W0   — bit11 mask */
    /* 0x30B8  AND W1, W0, W2   — W2 = e18 & 0x800 (bit11) */
    /* 0x30BA  BRA Z, 0x30D4    */
    uint16_t bit11 = e18 & 0x0800;
    if (bit11) {
        /* currentLimitFlags bit11 is set */

        /* 0x30BC  MOV 0x1D4E, W1 */
        /* 0x30BE  MOV #0xC4D, W0 */
        /* 0x30C0  SUB W1, W0     — voutTargetCode == 0xC4D? */
        /* 0x30C2  BRA NZ, 0x30D4 */
        if (voutTargetCode == 0x0C4D) {
            /* 0x30C4  MOV.B LATD, WREG   — read LATD */
            /* 0x30C6  BTST.Z W0, #4      — test LATD bit4 */
            /* 0x30C8  BRA NZ, 0x30D4     — if LATD bit4 clear → continue */
            if (!LATDbits.LATD4) {
                /* LATD bit4 is clear */
                /* 0x30CA  MOV 0x1268, W0  — droopMode */
                /* 0x30CC  SUB W0, #3      — == 3? */
                /* 0x30CE  BRA NZ, 0x30D4  */
                if (droopMode == 3) {
                    /* All conditions met: set diagnostic output */
                    /* 0x30D0  BSET 0x1BEC, #7 */
                    startupResetLatch |= (1u << 7);
                    /* 0x30D2  RETURN */
                    return;
                }
            }
        }
    }

    /* 0x30D4  MOV 0x1268, W0  — droopMode */
    uint16_t dm = droopMode;

    /* 0x30D6  SUB W0, #4      — droopMode == 4? */
    /* 0x30D8  BRA Z, 0x30DE   */
    if (dm == 4) {
        /* droopMode == 4: clear diagnostic */
        /* 0x30DE  BCLR 0x1BEC, #7 */
        startupResetLatch &= ~(1u << 7);
        /* 0x30E0  RETURN */
        return;
    }

    /* 0x30DA  CP0 W2           — test W2 = bit11 value */
    /* 0x30DC  BRA NZ, 0x30E0   — if bit11 was set (W2 != 0) → clear */
    if (bit11 != 0) {
        /* 0x30DE  BCLR 0x1BEC, #7 */
        startupResetLatch &= ~(1u << 7);
    }
    /* else bit11 == 0 and droopMode != 4: no change */

    /* 0x30E0  RETURN */
}

/* ============================================================================
 * protectionCheck  (0x30F8 – 0x316C)
 *
 * Reads ADCBUF13 and uses thresholdCompare() with hysteresis to determine
 * if the protection threshold is crossed. Implements a debounce counter
 * (0x1238) before latching the result into currentLimitFlags bit11.
 * Also manages flags_1262 bits [6] and [3] based on flags_1BF2 bit4
 * and the current latch state, and controls flags_1BF2 bit4 based on
 * flags_1E19 bit3.
 *
 * Thresholds: low = 0x136 (310), high = 0x26C (620)
 * Debounce:   54 (0x36) ISR ticks before bit change is accepted,
 *             then up to 30 more ticks to also trigger secondary action
 * ============================================================================ */
void protectionCheck(void)  /* 0x30F8 */
{
    /* Push W8, W9, W10 onto software stack */
    /* 0x30F8  MOV.D W8, [W15++]  */
    /* 0x30FA  MOV W10, [W15++]   */

    /* 0x30FC  MOV 0x1266, W0 */
    /* 0x30FE  AND W0, #0x1, W9   — W9 = thermalFlags bit0 (previous prot state) */
    uint16_t prev_prot = thermalFlags & 0x0001;  /* W9 */

    /* 0x3100  MOV ADCBUF13, W0  — read protection ADC channel */
    uint16_t adc13 = ADCBUF13; /* ADCBUF13 = 0x01AC */

    /* 0x3102  MOV 0x1E18, W10  */
    uint16_t e18 = currentLimitFlags;  /* W10 */

    /* 0x3104  LSR W10, #11, W8  — extract bit11 into W8 */
    /* 0x3106  AND W8, #0x1, W8  — W8 = (currentLimitFlags >> 11) & 1 */
    uint16_t prot_latch = (e18 >> 11) & 0x0001;  /* W8 = current latched state */

    /* 0x3108  MOV W9, W3        — W3 = previous prot state (for hysteresis) */
    /* 0x310A  MOV #0x136, W2    — high threshold = 0x136 (low in comparison) */
    /* 0x310C  MOV #0x26C, W1    — low  threshold (wait: function signature uses
     *                              W1=low, W2=high; re-check call) */
    /* RCALL 0x30E2: thresholdCompare(adc13, W1=0x26C, W2=0x136, W3=prev_prot) */
    /* NOTE: at 0x310E the call passes W0=adc13, W1=0x26C, W2=0x136, W3=prev_prot
     *       Inside thresholdCompare: "below W1" returns 1, "above W2" returns 0.
     *       So: adc13 < 0x26C → 1 (above threshold = problem)
     *           adc13 > 0x136 → 0 (below threshold = safe)
     *           in-band       → prev_prot (hysteresis)
     * The thresholds are swapped relative to their names because the ADC
     * signal is inverted (higher raw = lower physical quantity). */
    uint16_t new_prot = thresholdCompare(adc13, 0x026C, 0x0136, prev_prot);
    /* 0x3110  ZE W0, W0  — already byte-wide */

    /* 0x3112  SUB W8, W0  — prot_latch == new_prot? */
    /* 0x3114  BRA Z, 0x3152 — same state: skip debounce, clear counter */
    if (prot_latch != new_prot) {
        /* State changed: debounce */

        /* 0x3116  MOV 0x1238, W1  */
        /* 0x3118  INC W1, W1      */
        /* 0x311A  MOV W1, 0x1238  */
        uint16_t dbnc = protVar1238 + 1;
        protVar1238 = dbnc;

        /* 0x311C  MOV #0x36, W0   — debounce threshold = 54 */
        /* 0x311E  SUB W1, W0       */
        /* 0x3120  BRA LEU, 0x3154  — if cnt <= 54 → not yet, keep old state */
        if (dbnc > 0x36) {
            /* Debounce expired: re-read ADC and re-compare */
            /* 0x3122  MOV ADCBUF13, W0  */
            adc13 = ADCBUF13;

            /* 0x3124  MOV W9, W3 */
            /* 0x3126  MOV #0x136, W2 */
            /* 0x3128  MOV #0x26C, W1 */
            /* 0x312A  RCALL 0x30E2    */
            new_prot = thresholdCompare(adc13, 0x026C, 0x0136, prev_prot);

            /* 0x312C  AND W0, #0x1, W0  — keep bit0 */
            new_prot &= 0x0001;

            /* 0x312E  SL W0, #11, W0    — shift into bit11 position */
            uint16_t bit11_new = (uint16_t)(new_prot << 11);

            /* 0x3130  MOV W10, W1       — W1 = original currentLimitFlags */
            /* 0x3132  BCLR W1, #11      — clear bit11 */
            /* 0x3134  IOR W1, W0, W1    — insert new bit11 */
            /* 0x3136  MOV W1, 0x1E18    — write back */
            uint16_t e18_new = (e18 & ~(1u << 11)) | bit11_new;
            currentLimitFlags = e18_new;

            /* 0x3138  CLR 0x1238        — clear debounce counter */
            protVar1238 = 0;

            /* 0x313A  BTST 0x1BF2, #4   — check secondary flag */
            /* 0x313C  BRA NZ, 0x3148    */
            if (!(pwmRunning & (1u << 4))) {
                /* flags_1BF2 bit4 clear */
                /* 0x313E  BTST.Z W1, #11   — test new bit11 */
                /* 0x3140  BRA NZ, 0x3154   — if bit11 set → skip */
                if (!(e18_new & (1u << 11))) {
                    /* Bit11 cleared (protection removed) */
                    /* 0x3142  BSET 0x1262, #6 */
                    statusFlags2 |= (1u << 6);
                    /* 0x3144  BCLR 0x1262, #3 */
                    statusFlags2 &= ~(1u << 3);
                }
                /* 0x3146  BRA 0x3154 */
            } else {
                /* flags_1BF2 bit4 set */
                /* 0x3148  BTST.Z W1, #11   — test new bit11 */
                /* 0x314A  BRA Z, 0x3154    — if bit11 clear → skip */
                if (e18_new & (1u << 11)) {
                    /* Bit11 set (protection active) */
                    /* 0x314C  BCLR 0x1262, #6 */
                    statusFlags2 &= ~(1u << 6);
                    /* 0x314E  BSET 0x1262, #3 */
                    statusFlags2 |= (1u << 3);
                }
                /* 0x3150  BRA 0x3154 */
            }
        }
        /* else dbnc <= 54: fall through to clear at 0x3154 (but counter is kept) */
        goto after_debounce;
    }

    /* State unchanged (prot_latch == new_prot): clear debounce counter */
    /* 0x3152  CLR 0x1238 */
    protVar1238 = 0;

after_debounce:
    /* 0x3154  MOV 0x1E22, W0  — systemState */
    /* 0x3156  SUB W0, #3       — == 3 (FAULT)? */
    /* 0x3158  BRA Z, 0x315E    */
    if (systemState != 3) {
        /* Not in FAULT state: clear flags_1262 bits [6] and [3] */
        /* 0x315A  BCLR 0x1262, #6 */
        statusFlags2 &= ~(1u << 6);
        /* 0x315C  BCLR 0x1262, #3 */
        statusFlags2 &= ~(1u << 3);
    }

    /* 0x315E  BTST 0x1E19, #3  — test flags_1E19 bit3 */
    /* 0x3160  BRA Z, 0x3166    */
    if (currentLimitFlags & (1u << 11)) {
        /* bit3 set: clear flags_1BF2 bit4 */
        /* 0x3162  BCLR 0x1BF2, #4 */
        pwmRunning &= ~(1u << 4);
    } else {
        /* bit3 clear: set flags_1BF2 bit4 */
        /* 0x3166  BSET 0x1BF2, #4 */
        pwmRunning |= (1u << 4);
    }

    /* Restore W8, W9, W10 */
    /* 0x3168  MOV [--W15], W10 */
    /* 0x316A  MOV.D [--W15], W8 */
    /* 0x316C  RETURN */
}

/* ============================================================================
 * tempSample  (0x316E – 0x31B0)
 *
 * Reads ADCBUF15 (temperature ADC channel) and uses thresholdCompare() to
 * determine if over-temperature condition exists. Manages debounce counter
 * 0x123A and sets flag flags_1E19 bit0 (OT latch) when threshold crossed
 * for more than 0x1F3 (499) consecutive ISR ticks.
 *
 * Thresholds (passed to thresholdCompare):
 *   low  = 0x2C9  (713 — lower hysteresis bound, ADC counts)
 *   high = 0x26C  (620 — upper hysteresis bound)
 *   (inverted ADC: higher count = lower temperature)
 *
 * When new temp state is 0 (cold/safe):
 *   - LATD bit3 must be clear (hardware enable)
 *   - flags_1E19 bit0 must be clear (not already latched)
 *   - systemState must be 5
 *   → then: counter increments
 *
 * Counter overflow at 0x2ED (749): clamp to next value (0x2EE)
 * Counter overflow at 0x1F3 (499): copy 0x1BCA → 0x1BCC, set flags_1E19 bit0
 * ============================================================================ */
void tempSample(void)  /* 0x316E */
{
    /* 0x316E  MOV ADCBUF15, W0  — read temperature ADC */
    uint16_t adc15 = ADCBUF15;

    /* 0x3170  MOV 0x1266, W3   */
    /* 0x3172  LSR W3, #10, W3  — extract bit10 */
    /* 0x3174  AND W3, #0x1, W3 — W3 = (thermalFlags >> 10) & 1 (previous OT state) */
    uint16_t prev_ot = (thermalFlags >> 10) & 0x0001;

    /* 0x3176  MOV #0x26C, W2   — high threshold = 620 */
    /* 0x3178  MOV #0x2C9, W1   — low  threshold = 713 */
    /* 0x317A  RCALL 0x30E2     — thresholdCompare(adc15, 0x2C9, 0x26C, prev_ot) */
    uint16_t new_ot = thresholdCompare(adc15, 0x02C9, 0x026C, prev_ot);

    /* 0x317C  CP0.B W0         — is new_ot == 0? */
    /* 0x317E  BRA NZ, 0x3190   — if new_ot != 0 (OT active) → increment counter */
    if (new_ot == 0) {
        /* Temperature in safe range: check gate and state */

        /* 0x3180  MOV.B LATD, WREG   — read LATD */
        /* 0x3182  BTST.Z W0, #3      — test LATD bit3 */
        /* 0x3184  BRA NZ, 0x3190     — if LATD bit3 set → increment counter */
        if (LATDbits.LATD3) {
            goto temp_increment;
        }

        /* 0x3186  BTST 0x1E19, #0    — flags_1E19 bit0 (OT latch) set? */
        /* 0x3188  BRA NZ, 0x3190     */
        if (currentLimitFlags & (1u << 8)) {
            goto temp_increment;
        }

        /* 0x318A  MOV 0x1E22, W0     — systemState */
        /* 0x318C  SUB W0, #5          — == 5? */
        /* 0x318E  BRA NZ, 0x3194     — not state 5 → do NOT increment */
        if (systemState != 5) {
            /* 0x3194  INC 0x123A       — increment counter (state != 5 path) */
            goto temp_increment;
        }

        /* All safe conditions true AND systemState == ST_HOLDOFF → clear counter */
        /* 0x3190  CLR 0x123A */
        protVar123A = 0;
        goto temp_threshold;  /* BRA 0x3196 */
    }

temp_increment:
    /* new_ot != 0 (OT detected) OR gate/latch/state conditions not met */
    /* 0x3194  INC 0x123A */
    (protVar123A)++;

temp_threshold:
    /* 0x3196  MOV 0x123A, W1    — counter value */
    ; uint16_t cnt = protVar123A;

    /* 0x3198  MOV #0x2ED, W0    — 749 */
    /* 0x319A  SUB W1, W0         */
    /* 0x319C  BRA LEU, 0x31A4    — cnt <= 749: check lower threshold */
    if (cnt > 0x02ED) {
        /* Clamp: set counter to 0x2EE (750) */
        /* 0x319E  INC W0, W0     — W0 = 0x2EE */
        /* 0x31A0  MOV W0, 0x123A */
        protVar123A = 0x02EE;
        /* 0x31A2  RETURN */
        return;
    }

    /* 0x31A4  MOV #0x1F3, W0    — 499 */
    /* 0x31A6  SUB W1, W0         */
    /* 0x31A8  BRA LEU, 0x31B0    — cnt <= 499: nothing more */
    if (cnt > 0x01F3) {
        /* Counter exceeded 499: latch OT condition */
        /* 0x31AA  MOV 0x1BCA, W0  — copy 0x1BCA to 0x1BCC */
        /* 0x31AC  MOV W0, 0x1BCC  */
        *(volatile uint16_t *)&hwConfigByte = ioutCalFactor;

        /* 0x31AE  BSET 0x1E19, #0  — set OT latch flag */
        currentLimitFlags |= (1u << 8);
    }

    /* 0x31B0  RETURN */
}

/* ============================================================================
 * currentLimit  (0x31CC – 0x3240)
 *
 * Current limit function called from T1 ISR.
 * Only active when systemState == 2, var_1E0E == 0, controlStatus bit4 clear,
 * statusFlags bit2 clear, and auxFlags bit11 (0x1E1D bit3) set.
 *
 * Reads ADCBUF5 and uses thresholdCompare() with droop-dependent thresholds:
 *   droopMode in [3,4]: low = 0x15F (351), high = 0x17F (383)
 *   otherwise           : (same thresholds, same call, W3 selects path)
 * Previous state = auxFlags bit2 (current-limit active latch).
 *
 * Uses debounceCounter() with debounce threshold = 5 and counter at 0x123C.
 *
 * Also checks PWM gate outputs (PWM2CON1L / PWM3CON1L bits [1,0]) to decide
 * whether to skip current-limit (gates fully on means no limiting needed).
 *
 * Result is written back to auxFlags bit2.
 * When no conditions are met: 0x123C is cleared.
 * ============================================================================ */
void currentLimit(void)  /* 0x31CC */
{
    /* Push W8, W9 */
    /* 0x31CC  MOV.D W8, [W15++] */

    /* 0x31CE  MOV 0x1E22, W0   — systemState */
    /* 0x31D0  SUB W0, #2        — == 2? */
    /* 0x31D2  BRA NZ, 0x323C   */
    if (systemState != 2) {
        goto current_limit_exit;
    }

    /* 0x31D4  CP0 0x1E0E        — var_1E0E == 0? */
    /* 0x31D6  BRA NZ, 0x323C   */
    if (faultResetTimer != 0) {
        goto current_limit_exit;
    }

    /* 0x31D8  BTST 0x1E20, #4   — controlStatus bit4 */
    /* 0x31DA  BRA NZ, 0x323C   */
    if (controlStatus & (1u << 4)) {
        goto current_limit_exit;
    }

    /* 0x31DC  MOV 0x125A, W0   */
    /* 0x31DE  AND W0, #0x4, W1  — test bit2 */
    /* 0x31E0  BRA NZ, 0x323C   */
    if (statusFlags & (1u << 2)) {
        goto current_limit_exit;
    }

    /* 0x31E2  BTST 0x1E1D, #3   — byte alias of auxFlags bit11 */
    /* 0x31E4  BRA Z, 0x323C    */
    if (!(auxFlags & (1u << 11))) {
        goto current_limit_exit;
    }

    /* ---- Check PWM gate status (both PWM channels) ---- */
    /* 0x31E6  MOV.B 0x423, WREG  — PWM2CON1L */
    /* 0x31E8  BTST.Z W0, #1      — test bit1 (gate high side) */
    /* 0x31EA  BRA Z, 0x31FE      — if bit1 clear → skip gate check */
    /* 0x31EC  MOV.B 0x423, WREG  */
    /* 0x31EE  BTST.Z W0, #0      — test bit0 (gate low side) */
    /* 0x31F0  BRA Z, 0x31FE      — if bit0 clear → skip gate check */
    /* 0x31F2  MOV.B 0x443, WREG  — PWM3CON1L */
    /* 0x31F4  BTST.Z W0, #1      */
    /* 0x31F6  BRA Z, 0x31FE      */
    /* 0x31F8  MOV.B 0x443, WREG  */
    /* 0x31FA  BTST.Z W0, #0      */
    /* 0x31FC  BRA NZ, 0x3238     — if all 4 gate bits set → force counter value */
    if (IOCON1bits.OVRENL && IOCON1bits.OVRENH &&
        IOCON2bits.OVRENL && IOCON2bits.OVRENH) {
        /* All gate outputs enabled: force counter register from W1 */
        /* (W1 = statusFlags & 0x4 = 0 at this point) */
        /* 0x3238  MOV W1, 0x123C  — W1 = 0 (from AND above) */
        protVar123C = 0;
        goto current_limit_exit;
    }

    /* ---- Compute current-limit threshold based on droopMode ---- */
    /* 0x31FE  MOV 0x1268, W0   */
    /* 0x3200  SUB W0, #3, W0   — W0 = droopMode - 3 */
    /* 0x3202  SUB W0, #1       — (droopMode - 3) <= 1 → droopMode in {3,4}? */
    /* 0x3204  BRA GTU, 0x323C  — if droopMode-3 > 1 (unsigned) → exit */
    uint16_t dm_adj = (uint16_t)((int16_t)droopMode - 3);
    if (dm_adj > 1) {
        /* droopMode not in {3,4} */
        goto current_limit_exit;
    }

    /* 0x3206  MOV ADCBUF5, W0  — read current-sense ADC */
    uint16_t adc5 = ADCBUF5;

    /* 0x3208  MOV 0x1E1C, W1   */
    uint16_t e1c = auxFlags;

    /* 0x320A  LSR W1, #2, W1   — shift right 2 */
    /* 0x320C  AND W1, #0x1, W8  — W8 = (auxFlags >> 2) & 1 (prev current-limit latch) */
    uint16_t prev_cl = (e1c >> 2) & 0x0001;  /* W8 */

    /* 0x320E  CLR W9           — W9 = 0 (initial new current-limit state) */
    uint16_t new_cl = 0;  /* W9 */

    /* 0x3210  MOV 0x1266, W3   */
    /* 0x3212  LSR W3, #15, W3  — W3 = thermalFlags bit15 (previous CL hysteresis) */
    uint16_t prev_cl2 = (thermalFlags >> 15) & 0x0001;

    /* 0x3214  MOV #0x17F, W2   — high threshold = 0x17F (383) */
    /* 0x3216  MOV #0x15F, W1   — low  threshold = 0x15F (351) */
    /* 0x3218  RCALL 0x30E2     — thresholdCompare(adc5, 0x15F, 0x17F, prev_cl2) */
    uint16_t cmp = thresholdCompare(adc5, 0x015F, 0x017F, prev_cl2);

    /* 0x321A  CP0.B W0         — cmp == 0? */
    /* 0x321C  BRA NZ, 0x3220   */
    if (cmp == 0) {
        /* Over current threshold: signal to debounce */
        /* 0x321E  MOV #0x1, W9 */
        new_cl = 1;
    }

    /* ---- Debounce current-limit signal ---- */
    /* 0x3220  MOV W8, W3       — W3 = prev_cl (previous latch state) */
    /* 0x3222  MOV #0x123C, W2  — pointer to debounce counter */
    /* 0x3224  MOV #0x5, W1     — debounce threshold = 5 */
    /* 0x3226  MOV W9, W0       — W0 = new_cl */
    /* 0x3228  RCALL 0x31B2     — debounceCounter(new_cl, 5, &cnt_123C, prev_cl) */
    uint16_t db_result = debounceCounter(new_cl, 5, &protVar123C, prev_cl);

    /* 0x322A  AND W0, #0x1, W0  — keep bit0 */
    db_result &= 0x0001;

    /* 0x322C  SL W0, #2, W0     — shift into bit2 position */
    uint16_t bit2_new = (uint16_t)(db_result << 2);

    /* 0x322E  MOV 0x1E1C, W1   — reload auxFlags */
    /* 0x3230  BCLR W1, #2      — clear bit2 */
    /* 0x3232  IOR W1, W0, W1   — insert new bit2 */
    /* 0x3234  MOV W1, 0x1E1C   — write back */
    auxFlags = (e1c & ~(1u << 2)) | bit2_new;

    /* 0x3236  BRA 0x323E        — skip exit */
    goto current_limit_done;

current_limit_exit:
    /* Clear debounce counter when conditions not met */
    /* 0x323C  CLR 0x123C */
    protVar123C = 0;

current_limit_done:
    /* Restore W8, W9 */
    /* 0x323E  MOV.D [--W15], W8 */
    /* 0x3240  RETURN */
    ; /* empty statement after label */
}

/* ============================================================================
 * vinCheck  (0x3254 – 0x32DE)
 *
 * Input voltage check and droop-mode voltage reference computation.
 * Only runs when statusFlags bit5 (regulation active) is set.
 *
 * Computes a droop-adjusted current-proportional offset (0x1242):
 *   offset = (Imeas * 23) >> 10   (≈ Imeas * 0.0225, clipped to [0,10])
 *
 * Then selects the reference voltage source based on 0x1BEA bits:
 *   bit8 set: voutTargetCode == 0xC4D → vref = 0xC11 + offset → 0x1240
 *   bit7 set: voutTargetCode == 0xC4D → vref_ls = (0xD000 + ioutScaleConst) >> 2 + 0xBFF
 *                                           → 0x1244; clear vref_ls (0x1D3A)
 *   bit6 set: voutTargetCode == 0xC4D → voutSetpoint = 0xC69 + offset → 0x123E
 *   none set: voutSetpoint = voutTargetCode → 0x1244  (fallback)
 *
 * After selection, clampToward() is called to ramp 0x1244 toward 0x123E
 * (when bit7 path was taken, looping back to 0x329C).
 *
 * Final computation (always):
 *   0x1246 = (0x1244 * cal_va) >> 13 + ofs_va  (calibrated output)
 *   voutSetpoint (0x1D4C) = 0x1246 - vref_ls (0x1D3A) - vrefOcpAdj (0x1D38)
 * ============================================================================ */
void vinCheck(void)  /* 0x3254 */
{
    /* 0x3254  BTST 0x125A, #5   — regulation active? */
    /* 0x3256  BRA Z, 0x32BC    */
    if (!(statusFlags & (1u << 5))) {
        goto vin_fallback;
    }

    /* ---- Compute droop offset from output current ---- */
    /* 0x3258  MOV 0x1D44, W0   — Imeas */
    int16_t imeas = Imeas;

    /* 0x325A  MUL.SU W0, #23, W0  — W1:W0 = Imeas * 23 (signed × unsigned) */
    /* 0x325C  ASR W0, #10, W0     — W0 = result >> 10 (arithmetic right shift,
     *                               acts on 16-bit W0 of the 32-bit product) */
    int16_t offset = (int16_t)((int32_t)imeas * 23) >> 10;

    /* 0x325E  MOV W0, 0x1242      */
    protVar1242 = (uint16_t)offset;

    /* 0x3260  BRA NN, 0x3264      — if non-negative keep it */
    if (offset < 0) {
        /* 0x3262  CLR 0x1242       — clamp negative to 0 */
        protVar1242 = 0;
        offset = 0;
    }

    /* Clamp offset to max 10 */
    /* 0x3264  MOV 0x1242, W0  */
    offset = (int16_t)protVar1242;

    /* 0x3266  SUB W0, #0xA       — offset > 10? */
    /* 0x3268  BRA LE, 0x326E      */
    if (offset > 10) {
        /* 0x326A  MOV #0xA, W0    */
        /* 0x326C  MOV W0, 0x1242  */
        protVar1242 = 10;
        offset = 10;
    }

    /* 0x326E  MOV 0x1BEA, W2    — load mode-select flags */
    uint16_t bea = droopEnableFlags;

    /* ---- bit8 check: precise regulation mode ---- */
    /* 0x3270  BTST.Z W2, #8    — test bit8 */
    /* 0x3272  BRA Z, 0x3286    — if clear → try bit7 */
    if (bea & (1u << 8)) {
        /* 0x3274  MOV 0x1D4E, W1  — voutTargetCode */
        /* 0x3276  MOV #0xC4D, W0  */
        /* 0x3278  SUB W1, W0       — == 0xC4D? */
        /* 0x327A  BRA NZ, 0x3286   */
        if (voutTargetCode == 0x0C4D) {
            /* 0x327C  MOV #0xC11, W1  */
            /* 0x327E  MOV 0x1242, W0  — offset */
            /* 0x3280  ADD W1, W0, W1  — W1 = 0xC11 + offset */
            /* 0x3282  MOV W1, 0x1240  */
            protVar1240 = (uint16_t)(0x0C11 + offset);
            /* 0x3284  BRA 0x32B6      — jump to apply section */
            goto vin_apply;
        }
    }

    /* ---- bit7 check: soft-start / slew-rate mode ---- */
    /* 0x3286  BTST.Z W2, #7    */
    /* 0x3288  BRA Z, 0x32A2    — if clear → try bit6 */
    if (bea & (1u << 7)) {
        /* 0x328A  MOV 0x1D4E, W1 */
        /* 0x328C  MOV #0xC4D, W0 */
        /* 0x328E  SUB W1, W0      — == 0xC4D? */
        /* 0x3290  BRA NZ, 0x32A2  */
        if (voutTargetCode == 0x0C4D) {
            /* 0x3292  MOV #0xD000, W0  */
            /* 0x3294  ADD 0x1BC6, WREG  — W0 = 0xD000 + ioutScaleConst */
            /* 0x3296  LSR W0, #2, W0    — >> 2 */
            /* 0x3298  MOV #0xBFF, W1    */
            /* 0x329A  ADD W0, W1, W0    — + 0xBFF */
            /* 0x329C  MOV W0, 0x1244    */
            uint16_t raw = (uint16_t)(0xD000u + ioutScaleConst);
            protVar1244 = (raw >> 2) + 0x0BFF;

            /* 0x329E  CLR 0x1D3A        — clear vref_ls */
            vref_ls = 0;

            /* 0x32A0  BRA 0x32C0         — jump to final computation */
            goto vin_final;
        }
    }

    /* ---- bit6 check: margining mode ---- */
    /* 0x32A2  BTST.Z W2, #6   */
    /* 0x32A4  BRA Z, 0x32BC   — if clear → fallback */
    if (bea & (1u << 6)) {
        /* 0x32A6  MOV 0x1D4E, W1 */
        /* 0x32A8  MOV #0xC4D, W0 */
        /* 0x32AA  SUB W1, W0      — == 0xC4D? */
        /* 0x32AC  BRA NZ, 0x32BC  */
        if (voutTargetCode == 0x0C4D) {
            /* 0x32AE  MOV #0xC69, W1   */
            /* 0x32B0  MOV 0x1242, W0   — offset */
            /* 0x32B2  ADD W1, W0, W1   — W1 = 0xC69 + offset */
            /* 0x32B4  MOV W1, 0x123E   — target for clampToward */
            protVar123E = (uint16_t)(0x0C69 + offset);

            /* Fall through to apply section via BRA 0x32B6 */
vin_apply:
            /* 0x32B6  MOV 0x1244, W0    — current ramp value */
            /* 0x32B8  RCALL 0x3242      — clampToward(0x1244, 0x123E) */
            /* 0x32BA  BRA 0x329C        — write result back to 0x1244 and loop */
            /* NOTE: This is effectively: 0x1244 = clampToward(0x1244, 0x123E) */
            protVar1244 = (uint16_t)clampToward((int16_t)protVar1244, (int16_t)protVar123E);
            /* After clampToward, fall through to 0x329C re-store then 0x32C0 */
            /* The BRA 0x329C at 0x32BA writes W0 back to 0x1244 then hits 0x32C0 */
            /* (Already written above) */
            goto vin_final;
        }
    }

vin_fallback:
    /* No mode bits set or regulation not active: direct copy of voutTargetCode */
    /* 0x32BC  MOV 0x1D4E, W0   */
    /* 0x32BE  MOV W0, 0x1244   */
    protVar1244 = (uint16_t)voutTargetCode;

vin_final:
    /* ---- Apply calibration and compute voutSetpoint ---- */
    /* 0x32C0  MOV 0x1244, W2   — raw voltage reference */
    /* 0x32C2  MOV 0x1D32, W0   — cal_va gain */
    /* 0x32C4  MUL.SS W2, W0, W2 — W3:W2 = 0x1244 * cal_va (32-bit product) */
    ; int32_t prod = (int32_t)(int16_t)protVar1244 * (int32_t)cal_va;

    /* The 32-bit product is in W3:W2.  The firmware extracts a 16-bit result
     * using a combination of SL and LSR/ASR to pull bits [28:13]:
     * 0x32C6  SL W3, #3, W0      — W0 = W3 << 3  (bits 31..16 shifted)
     * 0x32C8  LSR W2, #13, W2    — W2 = W2 >> 13 (bits 12..0 dropped)
     * 0x32CA  IOR W0, W2, W2     — W2 = combined = prod >> 13 (16-bit)
     * 0x32CC  ASR W3, #13, W3    — W3 = sign extension (unused except for
     *                              implicit saturation, W3 discarded)
     * Result: cal_result = (int16_t)(prod >> 13) */
    int16_t cal_result = (int16_t)(prod >> 13);

    /* 0x32CE  MOV 0x1D30, W0    — ofs_va offset */
    /* 0x32D0  ADD W2, W0, W2    — cal_result += ofs_va */
    cal_result = (int16_t)(cal_result + ofs_va);

    /* 0x32D2  MOV W2, 0x1246    — store intermediate calibrated voltage */
    protVar1246 = (uint16_t)cal_result;

    /* 0x32D4  MOV 0x1D3A, W0    — vref_ls (droop compensation) */
    /* 0x32D6  SUB W2, W0, W2    — cal_result -= vref_ls */
    cal_result -= vref_ls;

    /* 0x32D8  MOV 0x1D38, W0    — vrefOcpAdj (OCP adjustment) */
    /* 0x32DA  SUB W2, W0, W2    — cal_result -= vrefOcpAdj */
    cal_result -= vrefOcpAdj;

    /* 0x32DC  MOV W2, 0x1D4C    — store final voutSetpoint */
    voutSetpoint = cal_result;

    /* 0x32DE  RETURN */
}

#endif /* UNIT_TEST_MINIMAL */

#undef PROT_LOCAL
