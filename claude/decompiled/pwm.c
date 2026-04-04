/* ============================================================================
 * HSTNS-PD44  dsPIC33FJ64GS606  Firmware — T1 ISR Sub-Controller Functions
 *
 * Decompiled from firmware assembly dump.
 * Each function header shows the original ROM address range and a brief
 * description of purpose.
 *
 * Register / RAM cross-reference:
 *   All named variables come from ram_map.h.
 *   Anonymous RAM locations are accessed via (volatile uint16_t *)0xXXXX casts.
 *
 * dsPIC SFR byte addresses used here (word-addressed in source):
 *   PORTD  byte @ 0x02DA  (read input state of port D)
 *   LATD   byte @ 0x02DC  (read output latch of port D)
 *   LATF   byte @ 0x02EC  (read/write output latch of port F)
 *   ADCBUF5     @ 0x034A  (ADC result register for AN5 / Vout sense)
 * ============================================================================ */

#include <stdint.h>
#include <xc.h>        /* dsPIC SFR definitions (PORTD, LATD, LATF, ADCBUF5) */
#include "variables.h"
/* All RAM variables declared in ram_map.h.
 * Mapping from original addresses:
 *   0x121E → ssDebounce       0x1220 → ocpTimer        0x1222 → foldbackTimer
 *   0x1224 → oeTimer          0x1226 → edgeCount       0x1228 → portd6Cur
 *   0x122A → portd6Prev       0x122C → stableCount     0x122E → sampleCount
 *   0x1265 → protectionStatus[15:8]  0x1BB4 → ioutAdcRaw   0x1BB6 → ocpThresholdHw
 *   0x1BE8 → adcLiveA         0x1D5A → voutLoThresh
 *   0x1264 → protectionStatus 0x1E20 → controlStatus
 */

/* ============================================================================
 * 1.  softStartRamp  (0x2DC6 – 0x2DF4)
 *
 * Debounces an external digital input (PORTD byte, bit0) against the
 * current state stored in auxFlags bit1.  If the sampled bit differs from
 * the stored state for more than 2 consecutive ISR ticks the stored state is
 * updated (bit1 of auxFlags is written with the new value) and the debounce
 * counter is reset.  If they match, the debounce counter is also cleared.
 * ============================================================================ */
void softStartRamp(void)
{
    /* Read PORTD byte (SFR byte address 0x02DA) and isolate bit0. */
    uint8_t portd_b0 = PORTDbits.RD0;

    /* Extract bit1 from auxFlags, then shift it down to bit0 so both
     * values occupy the same bit position for comparison.               */
    uint16_t stored = auxFlags;
    uint8_t  stored_b0 = (uint8_t)((stored >> 1) & 0x01u);

    if (portd_b0 == stored_b0) {
        /* Input matches stored state — reset debounce counter and return. */
        ssDebounce = 0;
        return;
    }

    /* Mismatch: increment the debounce counter. */
    uint16_t cnt = ssDebounce + 1u;
    ssDebounce = cnt;

    if (cnt <= 2u) {
        /* Not yet stable — keep waiting. */
        return;
    }

    /* Debounce threshold exceeded: update bit1 of auxFlags with the new
     * sampled value and reset the counter.
     *
     * portd_b0 is 0 or 1; shift left by 1 to place it in bit1 position.
     * Clear bit1 in the stored word then OR in the new value.             */
    uint16_t new_bit = (uint16_t)portd_b0 << 1;
    stored &= ~(1u << 1);           /* clear bit1 */
    stored |= new_bit;
    auxFlags = stored;

    ssDebounce = 0;
}

/* ============================================================================
 * 2.  softStartRamp2  (0x2DF6 – 0x2E2A)
 *
 * Phase-2 soft-start ramp: controls freqSetpoint during the startup ramp.
 *
 * Compares bit3 of a PORTD-adjacent byte (0x02DB) against bit3 of
 * auxFlags high-byte startup tick bits (0x1E1D).  When they differ the function drives freqSetpoint toward
 * its target:
 *   - If freqSetpoint == 0 the function reads bit3 of byte 0x02DB, places
 *     it in bit11 of auxFlags (overwriting the previous bit11), then returns.
 *   - Otherwise freqSetpoint is decremented by 1 each ISR tick until it
 *     reaches 0.
 *
 * When bit3 of the PORTD byte matches bit3 of auxFlags high byte (0x1E1D) the function
 * ensures freqSetpoint is sensible:
 *   - If auxFlags bit11 (0x1E1D bit3) is clear  → load the nominal value 0x15E (350).
 *   - If auxFlags bit11 (0x1E1D bit3) is set    → keep freqSetpoint at 3 (minimum).
 *   In both cases the result is written to freqSetpoint.
 * ============================================================================ */
void softStartRamp2(void)
{
    /* Bit3 of the byte at 0x02DB (second byte of PORTD word / high byte). */
    uint8_t portd_b3 = PORTDbits.RD11 ? 0x08u : 0x00u;

    /* Bit3 of auxFlags high byte (0x1E1D). */
    uint8_t flag_b3  = (auxFlags & (1u << 11)) ? 0x08u : 0x00u;

    if (portd_b3 == flag_b3) {
        /* Bits match — decide the correct freqSetpoint value. */
        uint16_t val = 3u;                          /* default: already near zero */
        if (!(auxFlags & (1u << 11))) {
            val = 0x015Eu;                          /* nominal 350 */
        }
        freqSetpoint = (int16_t)val;
        return;
    }

    /* Bits differ — ramp freqSetpoint down toward 0. */
    int16_t fs = freqSetpoint;
    if (fs == 0) {
        /* Ramp complete: copy bit3 of 0x02DB into bit11 of auxFlags. */
        uint16_t bit11 = (uint16_t)PORTDbits.RD11 << 11;
        uint16_t f     = auxFlags;
        f &= ~(1u << 11);
        f |= bit11;
        auxFlags = f;
        return;
    }

    /* Decrement and save. */
    freqSetpoint = fs - 1;
}

/* ============================================================================
 * 3.  stateControlMachine  (0x2E2C – 0x2E3A)
 *
 * Minimal LLC sub-state machine tick.  Only active when systemState == ST_IDLE.
 * Decrements marginThreshold by 1 each call until it reaches 0.
 * ============================================================================ */
void stateControlMachine(void)
{
    if (systemState != 0) {
        return;
    }

    int16_t m = marginThreshold;
    if (m == 0) {
        return;
    }

    marginThreshold = m - 1;
}

#ifndef UNIT_TEST_MINIMAL

/* ============================================================================
 * 4.  stateInit  (0x2E3C – 0x2E74)
 *
 * State-init / OCP detection function, runs in the T1 ISR.
 *
 * Early-exits (and clears the OCP timer) unless:
 *   systemState == 2  AND  controlStatus bit0 == 0  (no fault, normal run)
 *
 * OCP detection logic (only when droopMode != 3):
 *   - Compares Imeas_scaled against threshold 0x1401 (5121).
 *   - If Imeas_scaled >  0x1401: increments OCP timer; if >= 24 sets
 *     protectionStatus bit7.
 *   - Checks a second threshold 0x1781 (6017):
 *       - If Imeas_scaled <= 0x1781 and LATD bit4 is clear and
 *         protectionStatus bit10 (0x1265 bit2) is clear → return without fault.
 *       - Otherwise (Imeas_scaled > 0x1781 or gate conditions fail):
 *         set protectionStatus bit7 (OCP fault latch) and return.
 *   - If Imeas_scaled <= 0x1401: reload OCP timer with controlStatus bit0
 *     value (0 or 1) and return.
 *
 * When droopMode == 3 the OCP timer accumulation above is bypassed:
 *   - Still checks Imeas_scaled vs p_ocp_thresh (0x1BB4).
 *   - If Imeas_scaled <= p_ocp_thresh and vrefOcpAdj <= 0: skip.
 *   - Otherwise: increment p_foldback_timer; if >= 0x225 (549) set
 *     protectionStatus bit8 (0x1265 bit0, foldback active flag).
 * ============================================================================ */
void stateInit(void)
{
    /* Only run in state 2 with no active fault. */
    if (systemState != 2u) {
        ocpTimer = 0;
        return;
    }

    uint16_t cf = controlStatus;
    if (cf & 0x0001u) {             /* bit0 = fault present */
        ocpTimer = 0;
        return;
    }

    int16_t  imeas = Imeas_scaled;

    if (droopMode == 3u) {
        /* Droop mode 3: current-foldback timer path */
        int16_t ocp_thr = (int16_t)ioutAdcRaw;
        if ((uint16_t)imeas <= (uint16_t)ocp_thr) {
            /* Below threshold — check vrefOcpAdj */
            if (vrefOcpAdj <= 0) {
                /* No foldback needed — reset timer and return. */
                foldbackTimer = 0;  /* write W2 (which is 0 from AND) */
                return;
            }
        }

        /* Above OCP threshold or vrefOcpAdj > 0: increment foldback timer. */
        uint16_t ft = foldbackTimer + 1u;
        foldbackTimer = ft;

        if (ft > 0x0225u) {         /* 549 ticks */
            protectionStatus |= (1u << 8);  /* set bit0: foldback active */
        }
        return;
    }

    /* Normal droop mode: simple OCP timer */
    if ((uint16_t)imeas > 0x1401u) {    /* > 5121 ADC counts */
        uint16_t ot = ocpTimer + 1u;
        ocpTimer = ot;

        if (ot > 0x0018u) {             /* > 24 ticks */
            protectionStatus |= (1u << 7);    /* set OCP fault latch bit */
        }

        /* Second threshold check: > 0x1781 (6017) → hard OCP fault */
        if ((uint16_t)imeas > 0x1781u) {
            protectionStatus |= (1u << 7);
            return;
        }

        /* Between 0x1401 and 0x1781: check LATD bit4 and inhibit flag. */
        if (!LATDbits.LATD4) {
            /* LATD bit4 clear */
            if (!(protectionStatus & (1u << 10))) {
                /* protectionStatus bit10 clear: no inhibit — return without fault */
                return;
            }
        }

        /* LATD bit4 set or inhibit active: latch OCP fault */
        protectionStatus |= (1u << 7);
        return;
    }

    /* Imeas_scaled <= threshold: reset OCP timer.
     * W1 holds the masked bit0 of controlStatus (0 here since we checked above). */
    ocpTimer = 0;
}

/* ============================================================================
 * 5.  voltageRegulation  (0x2E76 – 0x2EAA)
 *
 * Voltage-regulation OCP foldback timer, runs in T1 ISR.
 *
 * Early-exits (and clears foldback timer) unless:
 *   systemState == 2  AND  controlStatus bit0 == 0
 *
 * When droopMode == 3:
 *   Compares Imeas_scaled (0x1D42) against p_ocp_thresh (0x1BB4).
 *   If Imeas_scaled > p_ocp_thresh OR vrefOcpAdj > 0:
 *     Increment p_foldback_timer (0x1222).
 *     If >= 0x225 (549): set protectionStatus bit8 (0x1265 bit0, foldback active).
 *   Else: reset foldback timer to W2 (0 from the AND mask).
 *
 * When droopMode != 3:
 *   Resets foldback timer (using W2 = 0) and returns.
 * ============================================================================ */
void voltageRegulation(void)
{
    if (systemState != 2u) {
        foldbackTimer = 0;
        return;
    }

    uint16_t cf = controlStatus;
    uint16_t fault = cf & 0x0001u;      /* W2 = bit0 */
    if (fault) {
        foldbackTimer = 0;
        return;
    }

    if (droopMode != 3u) {
        /* Not in droop mode 3 — reset timer and return. */
        foldbackTimer = fault;      /* fault == 0 here */
        return;
    }

    /* Droop mode 3: foldback timer logic */
    int16_t imeas   = Imeas_scaled;
    int16_t ocp_thr = (int16_t)ioutAdcRaw;

    if ((uint16_t)imeas <= (uint16_t)ocp_thr) {
        /* Current is within limit — only accumulate if vrefOcpAdj > 0. */
        if (vrefOcpAdj <= 0) {
            foldbackTimer = (uint16_t)fault;   /* = 0 */
            return;
        }
    }

    /* Over threshold or non-zero adjustment: increment timer. */
    uint16_t ft = foldbackTimer + 1u;
    foldbackTimer = ft;

    if (ft > 0x0225u) {                 /* 549 ticks (~549 × T1 period) */
        protectionStatus |= (1u << 8);        /* set foldback-active flag */
    }
}

/* ============================================================================
 * 6.  outputEnable  (0x2EAC – 0x2F1E)
 *
 * Controls LATF bit0 (output enable gate signal), runs in T1 ISR.
 *
 * Structure:
 *
 * A)  systemState != 2  (not in run state):
 *       - If LATF bit0 is already clear → return immediately.
 *       - Otherwise increment output-enable timer (0x1224).
 *         If timer <= 5: return (hold off).
 *         Else: clear timer, assert LATF bit0, return.
 *
 * B)  systemState == 2, droopMode == 3:
 *       Read ADCBUF5 (Vout sense).
 *       If ADCBUF5 <= 0x2EB (747): jump to de-assert path.
 *       If llcPeriodCmd > 0x3A97 (14999): return (do nothing at this tick).
 *       Else: fall through to de-assert.
 *
 * C)  systemState == 2, droopMode == 4:
 *       Read 0x1BE8 (Vout OE ADC).
 *       Compute lower hysteresis threshold stored in p_vout_lo_thresh (0x1D5A):
 *         if 0x1BE8 > 0x1FD0 (8144): threshold = 0x251C (9500)
 *         elif 0x1BE8 > 0x1F6F (8047): threshold = 0x1F6F + 0x229 = 0x2198
 *                                         (add #0x229 to the literal 0x1F6F)
 *         else: threshold unchanged (previous value kept)
 *       Then check ADCBUF5 vs 0x16B (363): if <= 363 jump to de-assert.
 *       Check llcPeriodCmd vs p_vout_lo_thresh:
 *         if llcPeriodCmd <= threshold: jump to de-assert.
 *         Check protectionStatus bit11 (0x1265 bit3): if clear → jump to de-assert.
 *         Else: assert LATF bit0 and return.
 *
 * De-assert path (label ~0x2EFC):
 *       If statusFlags bit4 is set (running flag): return (keep current state).
 *       If llcPeriodCmd > 0x1F40 (8000): return.
 *       De-assert LATF bit0 and return.
 * ============================================================================ */
void outputEnable(void)
{
    if (systemState != 2u) {
        /* Not in run state — check if LATF bit0 is already clear. */
        if (!LATFbits.LATF0) {
            /* Already de-asserted — do nothing. */
            return;
        }

        /* LATF bit0 is set: increment the OE timer and check threshold. */
        uint16_t oe = oeTimer + 1u;
        oeTimer = oe;

        if (oe <= 5u) {
            return;
        }

        /* Timer expired: clear timer and assert output enable. */
        oeTimer = 0;
        LATFbits.LATF0 = 1;
        return;
    }

    /* systemState == 2 */
    uint16_t dm = droopMode;

    if (dm == 3u) {
        /* Droop mode 3: gate on ADCBUF5 Vout and llcPeriodCmd headroom */
        uint16_t vout5 = (uint16_t)ADCBUF5;
        if (vout5 <= 0x02EBu) {         /* 747 ADC counts */
            goto deassert;
        }
        if (llcPeriodCmd > (int16_t)0x3A97u) {  /* 14999 — already high enough */
            return;
        }
        goto deassert;

    } else if (dm == 4u) {
        /* Droop mode 4: compute dynamic hysteresis threshold */
        uint16_t va = (uint16_t)adcLiveA;  /* 0x1BE8 */

        if (va > 0x1FD0u) {                       /* > 8144 */
            voutLoThresh = 0x251Cu;          /* 9500 */
        } else if (va > 0x1F6Fu) {                /* > 8047 */
            /* threshold = 0x1F6F + 0x229 = 0x2198 (8600) */
            voutLoThresh = (uint16_t)(0x1F6Fu + 0x229u);
        }
        /* else: leave p_vout_lo_thresh unchanged */

        /* Check ADCBUF5 against low Vout threshold (363 counts). */
        uint16_t vout5b = (uint16_t)ADCBUF5;
        if (vout5b <= 0x016Bu) {                  /* 363 */
            goto deassert;
        }

        /* Check llcPeriodCmd against the stored lower threshold. */
        int16_t vlo = (int16_t)voutLoThresh;
        if (llcPeriodCmd <= vlo) {
            goto deassert;
        }

        /* Check output-ready flag (protectionStatus bit11 / 0x1265 bit3). */
        if (!(protectionStatus & (1u << 11))) {
            goto deassert;
        }

        /* All conditions met: assert output enable. */
        LATFbits.LATF0 = 1;
        return;

    } else {
        /* droopMode != 3 and != 4: fall through to de-assert. */
        goto deassert;
    }

deassert:
    /* De-assert path: only clear LATF bit0 if the "running" flag is clear
     * and llcPeriodCmd has not yet reached 0x1F40 (8000 counts).             */
    if (statusFlags & (1u << 4)) {
        return;                             /* bit4 set: leave output on */
    }
    if (llcPeriodCmd > (int16_t)0x1F40u) {     /* 8000 */
        return;
    }
    LATFbits.LATF0 = 0;
}

/* ============================================================================
 * 7.  loadDetect  (0x2F20 – 0x2F3A)
 *
 * Light-load / no-load detection based on Imeas_scaled.
 * Also contains a simple countdown timer decrement (0x2F3C–0x2F46) that is
 * appended here because it follows directly with no intervening label and is
 * called from the same dispatcher slot in some firmware builds.
 *
 * Load detection (0x2F20):
 *   Compare Imeas_scaled (0x1D42) against p_load_thresh (0x1BB6).
 *   If Imeas_scaled < p_load_thresh (unsigned borrow / NC):
 *     Check delayTimer against 0x6E (110):
 *       If delayTimer > 0x6E: return (still timing out).
 *       Check voutTargetCode against 0xC4D (3149):
 *         If voutTargetCode == 0xC4D: return (already at limit).
 *         Else: set runtimeFlags bit3 (light-load / no-load detected) and return.
 *   If Imeas_scaled >= p_load_thresh: return immediately (load present).
 *
 * Countdown timer (0x2F3C) — decrement-to-zero helper:
 *   If countdownTimer != 0: decrement it.
 * ============================================================================ */
void loadDetect(void)
{
    int16_t  imeas    = Imeas_scaled;
    uint16_t load_thr = ocpThresholdHw;

    /* Unsigned comparison: if Imeas_scaled < load threshold → possible no-load */
    if ((uint16_t)imeas < load_thr) {
        /* Check that the delay timer has not expired yet (debounce). */
        uint16_t dt = (uint16_t)delayTimer;
        if (dt > 0x006Eu) {              /* > 110 ticks */
            return;
        }

        /* Check whether voutTargetCode is already at the sentinel value 0xC4D. */
        if (voutTargetCode == (int16_t)0x0C4Du) {
            return;
        }

        /* Confirmed no-load: signal the state machine. */
        runtimeFlags |= (1u << 3);
    }
    /* If load is present (imeas >= load_thr) just return. */
}

/* Helper countdown timer that is physically adjacent to loadDetect
 * in ROM (0x2F3C – 0x2F46).  Decrements countdownTimer toward zero.    */
void countdownTick(void)
{
    int16_t c = countdownTimer;
    if (c != 0) {
        countdownTimer = c - 1;
    }
}

/* ============================================================================
 * 8.  portdSample  (0x2F48 – 0x2FA4)
 *
 * Samples PORTD bit6 every ISR tick, detects edges, and after a fixed
 * measurement window (0x1F3 = 499 ticks) computes outputVoltage from the
 * accumulated edge count:
 *
 *   outputVoltage = edge_cnt * 0x96   (150 decimal)
 *
 * Within the window the function also maintains a "stable period" counter
 * (p_stable_cnt / 0x122C) that tracks how many consecutive window-length
 * periods had 4 or fewer edges (i.e. near-DC or very low frequency signal),
 * and uses it to set/clear controlStatus bit2.
 *
 * Step-by-step:
 *   1. Read PORTD, extract bit6 → p_portd6_cur (0x1228).
 *   2. Compare with p_portd6_prev (0x122A):
 *        - If different: update prev, increment p_edge_cnt (0x1226).
 *   3. Increment p_sample_cnt (0x122E).
 *   4. If p_sample_cnt <= 0x1F3 (499): return (window not complete).
 *   5. Window complete:
 *      a.  If systemState == 2:
 *            - If LATD bit5 is clear: clear p_stable_cnt.
 *            - Check p_edge_cnt:
 *                <= 4: increment p_stable_cnt.
 *                     if p_stable_cnt >= 0x4F (79): set controlStatus bit2.
 *                 > 7: clear p_stable_cnt, clear controlStatus bit2.
 *                5–7: leave p_stable_cnt/flag unchanged.
 *          (then fall through to compute outputVoltage regardless)
 *      b.  If systemState != 2: clear p_stable_cnt.
 *      c.  Compute: outputVoltage = p_edge_cnt * 150.
 *      d.  Clear p_edge_cnt and p_sample_cnt.
 *      e.  Set auxFlags bit8 (0x1E1D bit0, measurement-complete flag).
 * ============================================================================ */
void portdSample(void)
{
    /* Step 1: sample PORTD bit6 */
    uint16_t bit6 = (uint16_t)PORTDbits.RD6;
    portd6Cur = bit6;

    /* Step 2: edge detection */
    uint16_t prev = portd6Prev;
    if (bit6 != prev) {
        portd6Prev = bit6;
        (edgeCount)++;           /* EC1226 / INC 0x1226 */
    }

    /* Step 3: increment the sample counter */
    uint16_t sc = sampleCount + 1u;
    sampleCount = sc;

    /* Step 4: wait until window is complete */
    if (sc <= 0x01F3u) {           /* 499 ticks */
        return;
    }

    /* Step 5: window complete — process results */

    if (systemState == 2u) {
        /* Check LATD bit5 (output gate / sync signal). */
        if (!LATDbits.LATD5) {
            /* Bit5 clear: reset stable count. */
            stableCount = 0;
        }

        uint16_t ec = edgeCount;

        if (ec <= 4u) {
            /* Few edges: signal appears stable / DC → increment stable counter. */
            uint16_t stb = stableCount + 1u;
            stableCount = stb;

            if (stb >= 0x004Fu) {       /* 79 windows */
                controlStatus |= (1u << 2);   /* set bit2: stable/no-AC */
            }
        } else if (ec > 7u) {
            /* Many edges: clearly switching → clear stable state. */
            stableCount = 0;
            controlStatus &= ~(1u << 2);      /* clear bit2 */
        }
        /* 5–7 edges: ambiguous — leave stable count and flag unchanged. */

    } else {
        /* Not in run state: clear stable count. */
        stableCount = 0;
    }

    /* Compute outputVoltage from edge count and reset accumulators. */
    uint16_t ec_final = edgeCount;
    outputVoltage = (int16_t)((uint16_t)(ec_final * (uint16_t)0x0096u)); /* × 150 */

    edgeCount   = 0;
    sampleCount = 0;

    /* Signal that a fresh measurement is available. */
    auxFlags |= (1u << 8);
}

#endif /* UNIT_TEST_MINIMAL */
