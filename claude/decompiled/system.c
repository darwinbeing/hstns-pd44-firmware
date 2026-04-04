/* ============================================================================
 * HSTNS-PD44  dsPIC33FJ64GS606  —  T1 ISR sub-system routines
 *
 * Decompiled from firmware binary.  Every function header includes the
 * original address range found in dsPIC33FJ64GS606.txt.
 *
 * Calling convention (dsPIC33):
 *   W0..W7  – scratch / argument registers
 *   W8..W15 – callee-saved (push/pop explicitly in assembly)
 *   W15     – stack pointer
 *   SUB/ADD to [W15] sets flags without writing memory (scratch compare)
 *
 * Branch mnemonics used below:
 *   BRA NZ / BRA Z    – branch if not-zero / zero   (integer compare)
 *   BRA GT / BRA LE   – signed greater / less-equal
 *   BRA GTU / BRA LEU – unsigned greater / less-equal
 *   BRA GE            – signed greater-or-equal
 *   BRA NC / BRA C    – no-carry / carry
 *
 * All RAM addresses are resolved through ram_map.h where possible.
 * Unknown addresses are accessed as  *(volatile uint16_t *)0xXXXX.
 * ============================================================================ */

#include <stdint.h>
#include <xc.h>
#include "variables.h"

/* --------------------------------------------------------------------------
 * SFR addresses referenced in this file that are not in ram_map.h
 * -------------------------------------------------------------------------- */
/* All PWM/comparator/port SFRs use <xc.h> definitions:
 *   PDC1/PDC2/PDC3, FCLCON1/2/3 + bits, CMPCON3 + bits,
 *   ADCBUF4, ADCBUF14, LATD/LATDbits, LATF/LATFbits */

/* Forward declarations of helper routines called from these functions */
extern void shutdownPwm(void);                  /* 0x4B50 – disable PWM outputs */
extern void setFaultState(void);               /* 0x1FD6 – enter fault state */
extern void droopMode0Watchdog(void);                /* 0x3666 – droop mode 0 watchdog */
extern void droopMode3Watchdog(void);                /* 0x371A – droop mode 3 watchdog */
extern void runNormalMode(void);                /* 0x378A – normal mode watchdog */
extern void flashSaveStatusPage(void);              /* 0x3EAC – apply new fan speed */
extern uint16_t thresholdCompare(int16_t, int16_t, int16_t, int16_t);   /* 0x30E2: value, low, high, prev */
extern uint16_t debounceCounter(int16_t, int16_t, volatile int16_t *, int16_t); /* 0x31B2: new, limit, counter, prev */

/* ============================================================================
 * 1.  modeCheck  (0x32E0 – 0x3320)
 *
 * Called every T1 ISR tick.  Decides whether the LLC should remain in
 * active mode (systemState == 2) or must transition to state 3 (fault/shutdown).
 *
 * Logic summary:
 *   - If systemState != 2  → skip (not in normal run)
 *   - If auxFlags bit11 set → skip (hardware inhibit active)
 *   - droopMode must be in {3,4}; if out of range → fault
 *   - If running (statusFlags bit4 set):
 *       clear run flag, call shutdownPwm(), reset marginThreshold = 0x9C4,
 *       clear systemState
 * ============================================================================ */
void modeCheck(void)   /* 0x32E0 – 0x3320 */
{
    uint16_t state;
    uint16_t flags_ic;
    uint16_t mode;

    /* --- Check that we are in normal run mode (systemState == 2) --- */
    state = systemState;                       /* MOV 0x1E22, W0 */
    if (state != 2)                         /* SUB W0,#2,[W15] / BRA NZ */
        goto check_running;

    /* --- Check hardware inhibit bit (auxFlags bit11 = 0x800) --- */
    flags_ic = auxFlags;                  /* MOV 0x1E1C, W1 */
    if (flags_ic & 0x0800)                  /* AND W1,#0x800,W1 / BRA NZ */
        goto check_running;

    /* --- Preload voutTargetCode with sentinel value 0xFFB0 --- */
    voutTargetCode = (int16_t)0xFFB0;             /* MOV #0xFFB0,W0 / MOV W0,0x1D4E */

    /* --- Validate droopMode against assembly:
     * 32F4..32F8 does unsigned (droopMode - 3) > 1 check.
     * Passing values are exactly droopMode == 3 or 4.
     */
    mode = droopMode;                      /* MOV 0x1268, W0 */
    mode -= 3;                              /* SUB W0,#3,W0 */
    if (mode > 1) {                         /* SUB W0,#1,[W15] / BRA GTU */
        /* droopMode not in {3,4} → enter fault state */
        systemState = ST_FAULT;                       /* MOV #3,W0 / MOV W0,0x1E22 */
        shutdownPwm();                     /* CALL 0x4B50 */
        goto check_running;
    }

    /* droopMode is 3 or 4 → set run flag */
    statusFlags |= (1u << 4);               /* BSET 0x125A, #4 */
    faultResetTimer = flags_ic;                    /* MOV W1, 0x1E0E – save auxFlags copy */
    goto done;

check_running:
    /* --- If run flag (bit4) is set, check for stop conditions --- */
    if (!(statusFlags & (1u << 4)))          /* BTST 0x125A,#4 / BRA Z */
        goto done;

    /* Stop condition 1: high-byte alias bit3 of 0x1E1D == auxFlags bit11 */
    if (!(auxFlags & (1u << 11))) {        /* BTST 0x1E1D,#3 / BRA NZ */
        /* Stop condition 2: systemFlags bit1 is also clear → keep running */
        if (!(systemFlags & (1u << 1)))      /* BTST 0x1E1A,#1 / BRA Z */
            goto done;
    }

    /* Conditions met: clear run flag and shut down */
    statusFlags &= ~(1u << 4);              /* BCLR 0x125A, #4 */
    shutdownPwm();                        /* CALL 0x4B50 */
    marginThreshold = (int16_t)0x09C4;   /* MOV #0x9C4,W0 / MOV W0,0x1E10 */
    systemState = ST_IDLE;                          /* CLR 0x1E22 */

done:
    return;                                /* RETURN */
}

/* ============================================================================
 * 2.  statusUpdate  (0x3322 – 0x3384)
 *
 * Called every T1 ISR tick when the LLC is actively running.
 * Reads voltage feedback (ADCBUF5), performs OVP/UVP debounce, updates the
 * current-limit active bit in currentLimitFlags, and manages PMBUS status / LEDs.
 * If OVP/UVP is confirmed while systemState == 2, transitions to fault state.
 * ============================================================================ */
void statusUpdate(void)   /* 0x3322 – 0x3384 */
{
    uint16_t adc5;
    uint16_t lim_flags;
    uint16_t ot_bit;
    uint16_t ovp_result;    /* W9: 0 = no fault, 1 = fault detected */
    uint16_t cl_bit;        /* W8: current-limit bit extracted from currentLimitFlags */
    uint16_t new_cl;
    uint16_t ic_flags;
    uint16_t tmp;

    /* Save W8/W9 across the call */

    /* --- Guard: only run if statusFlags bit4 (run flag) is set --- */
    if (!(statusFlags & (1u << 4)))          /* BTST 0x125A,#4 / BRA Z */
        goto clear_and_return;

    /* --- Guard: only run in systemState == 2 --- */
    if (systemState != 2)                      /* SUB 0x1E22,#2,[W15] / BRA NZ */
        goto clear_and_return;

    /* --- Read output voltage ADC channel 5 --- */
    adc5 = ADCBUF5;   /* MOV ADCBUF5, W0 */

    /* --- Extract current-limit active flag from currentLimitFlags bits[12] --- */
    lim_flags = currentLimitFlags;                 /* MOV 0x1E18, W1 */
    cl_bit = (lim_flags >> 12) & 0x1;      /* LSR W1,#12,W1 / AND W1,#1,W8 */

    ovp_result = 0;                         /* CLR W9 */

    /* --- Extract over-temperature bit from thermalFlags bit15 --- */
    uint16_t flags_ot = thermalFlags;
    ot_bit = flags_ot >> 15;               /* LSR W3,#15,W3 */

    /* --- Call OVP/UVP debounce check --- */
    /* Arguments: W2 = hi threshold 0x17F, W1 = lo threshold 0x15F,
                  W3 = OT bit, ADCBUF5 in W0 */
    /* RCALL 0x30E2: thresholdCompare with OVP/UVP thresholds */
    if (thresholdCompare((int16_t)adc5, 0x15F, 0x17F, (int16_t)ot_bit) == 0)
        ovp_result = 1;

    /* RCALL 0x31B2: debounce counter for OVP latch */
    new_cl = debounceCounter((int16_t)ovp_result, 5,
                              (volatile int16_t *)&ovpUvpLatchTarget, (int16_t)cl_bit) & 1;

    /* --- Store new current-limit flag back into currentLimitFlags bit12 --- */
    new_cl <<= 12;                          /* SL W0,#12,W0 */
    lim_flags = currentLimitFlags;                 /* MOV 0x1E18, W1 */
    lim_flags &= ~(1u << 12);              /* BCLR W1, #12 */
    lim_flags |= new_cl;                    /* IOR W1, W0, W1 */
    currentLimitFlags = lim_flags;                /* MOV W1, 0x1E18 */

    /* --- If current-limit flag is now set → schedule fault handling --- */
    if (lim_flags & (1u << 12)) {           /* BTST.Z W1,#12 / BRA Z → skip */
        /* Trigger I2C/PMBUS fault notification */
        pmbusAlertFlags |= (1u << 0);             /* BSET 0x192A, #0 */
        shutdownPwm();                    /* CALL 0x4B50 */

        /* Set systemState = 3 (fault) */
        systemState = ST_FAULT;                      /* MOV #3,W0 / MOV W0,0x1E22 */

        /* Check auxFlags for bits [1,11] (mask 0x802) */
        ic_flags = auxFlags;             /* MOV 0x1E1C, W0 */
        tmp = ic_flags & 0x0802;           /* AND W0, #0x802, W0 */
        if (tmp == 2) {                     /* SUB W0,#2,[W15] / BRA NZ → skip */
            /* Only latch OVP if controlStatus == 0 (no other fault) */
            if (controlStatus == 0) {       /* CP0 0x1E20 / BRA NZ */
                /* Set OVP latch bit in protectionStatus bit13 (0x1265 bit5) */
                protectionStatus |= (1u << 13);     /* BSET 0x1265, #5 */
            }
        }
        goto done;
    }

    goto done;

clear_and_return:
    /* Clear bit4 of flags_1E19 and clear ovpUvpLatchTarget */
    currentLimitFlags &= ~(1u << 12); /* BCLR 0x1E19, #4 */
    ovpUvpLatchTarget = 0;                           /* CLR 0x1248 */

done:
    return;                                 /* RETURN */
}

/* ============================================================================
 * 3.  faultHandler  (0x3386 – 0x33F2)
 *
 * Called every T1 ISR tick.  Handles latched OVP / OCP / OCV faults.
 *
 * Entry conditions checked:
 *   protectionStatus bits [8:7] – OVP / OCP latch bits
 *   auxFlags bit2       – fault latch active
 *
 * If a fault is active AND systemState == 2:
 *   1. Clear fault-latch bits in protectionStatus low/high-byte bits and auxFlags.
 *   2. Assert controlStatus bit0 (fault present).
 *   3. Drive ADCBUF4 scaled value into the LLC dead-time shadow registers
 *      (0x1BDE, 0x1BD0, 0x1926, 0x1BCE, 0x1924).
 *   4. Set relay-on bit (statusFlags2 bit12 / 0x1263 bit4), clear arm bit (pwmRunning bit0),
 *      set run bit (pwmRunRequest bit0).
 *   5. Call setFaultState() and shutdownPwm().
 *   6. Set systemState = 3.
 *
 * If fault active but systemState != 2:
 *   Clear the fault latch bits only (no shutdown).
 * ============================================================================ */
void faultHandler(void)   /* 0x3386 – 0x33F2 */
{
    uint16_t f;
    uint16_t state;
    uint16_t adc4_raw;
    uint32_t prod;
    uint16_t dt_lo;

    /* --- Check OVP/OCP latch: protectionStatus bits [8:7] must be non-zero --- */
    f = protectionStatus;       /* MOV 0x1264, W0 */
    if ((f & 0x0180) == 0) {               /* AND #0x180,W0 / BRA NZ */
        /* No OVP/OCP latch → check auxFlags bit2 (OCV latch) */
        if (!(auxFlags & (1u << 2)))       /* BTST 0x1E1C,#2 / BRA Z */
            return;                        /* no fault at all */
    }

    /* --- Guard: only act when systemState == 2 --- */
    state = systemState;                       /* MOV 0x1E22, W0 */
    if (state != 2) {                       /* SUB W0,#2,[W15] / BRA NZ */
        /* Not in run state: just clear latch bits and return */
        goto clear_latch_only;
    }

    /* --- Full fault shutdown sequence --- */

    /* Clear OVP latch (bit7 of protectionStatus), OCP latch (protectionStatus bit8 / 0x1265 bit0),
       and OCV latch (bit2 of auxFlags) */
    protectionStatus &= ~(1u << 7);  /* BCLR 0x1264, #7 */
    protectionStatus                  &= ~(1u << 8);  /* BCLR 0x1265, #0 */
    auxFlags                   &= ~(1u << 2);  /* BCLR 0x1E1C, #2 */

    /* Assert fault-present flag */
    controlStatus |= (1u << 0);            /* BSET 0x1E20, #0 */

    /* --- Determine fault type for pwmRunRequest high-byte fault bits (0x1BF1) ---
     * If protectionStatus bit10 (OCV) is set → use bit4 path
     * Else if protectionStatus bit9 (OVP) is set → use bit4 path
     * Else if droopMode == 4 → use bit4 path
     * Otherwise → use bit5 path
     */
    f = protectionStatus;       /* re-read after clear */
    if (!(f & (1u << 10))) {               /* BTST.Z W0,#10 / BRA NZ */
        if (!(f & (1u << 9))) {            /* BTST.Z W0,#9  / BRA NZ */
            if (droopMode == 4)           /* SUB 0x1268,#4,[W15] / BRA NZ */
                goto set_bit4;
            goto set_bit5;
        }
    }
set_bit4:
    pwmRunRequest |= (1u << 12);                /* BSET 0x1BF1, #4 */
    goto compute_deadtime;
set_bit5:
    pwmRunRequest |= (1u << 13);                /* BSET 0x1BF1, #5 */

compute_deadtime:
    /* --- Scale ADCBUF4 for dead-time registers ---
     *   raw = ADCBUF4
     *   product = raw * 0x1E80   (32-bit unsigned)
     *   dt_lo  = product >> 10   [Q10 scaled dead-time]
     */
    adc4_raw = ADCBUF4; /* MOV ADCBUF4, W0 */
    prod = (uint32_t)adc4_raw * 0x1E80u;    /* MUL.UU W0, #0x1E80, W0:W1 */
    dt_lo = (uint16_t)(prod >> 10);

    pdcShadowA = dt_lo;                      /* MOV W0, 0x1BDE */
    pdcShadowB = dt_lo;                      /* MOV W0, 0x1BD0 */
    oc1rsGateTiming = dt_lo;                 /* MOV W0, 0x1926 */

    /* --- Second scaling pass for pdc3Shadow ---
     *   product2 = voutScaledQ2 * dt_lo   (MUL 0x1BE0, W3:W2 = f × WREG)
     *   out = product2 >> 14               [Q14 result]
     */
    uint32_t prod2 = (uint32_t)voutScaledQ2 * (uint32_t)dt_lo; /* MUL 0x1BE0 */
    uint16_t out_lo = (uint16_t)(prod2 >> 14);
    pdc3Shadow = out_lo;                     /* MOV W2, 0x1BCE */
    oc2rsGateTiming = out_lo;                     /* MOV W2, 0x1924 */

    /* --- Set relay/output enable and arm LLC hardware --- */
    statusFlags2 |= (1u << 12);                /* BSET 0x1263, #4  – relay on */
    pwmRunRequest |= (1u << 0);               /* BSET 0x1BF0, #0  – LLC run bit */
    pwmRunning &= ~(1u << 0);              /* BCLR 0x1BF2, #0  – clear arm */

    /* Enter hardware fault state */
    setFaultState();                     /* CALL 0x1FD6 */

    /* Trigger PMBUS fault alert */
    pmbusAlertFlags |= (1u << 0);               /* BSET 0x192A, #0 */
    shutdownPwm();                       /* CALL 0x4B50 */

    /* Advance state machine to fault state */
    systemState = ST_FAULT;                         /* MOV #3,W0 / MOV W0,0x1E22 */
    return;

clear_latch_only:
    /* systemState != 2: just clear latch bits without shutdown */
    protectionStatus &= ~(1u << 7);  /* BCLR 0x1264, #7 */
    protectionStatus                  &= ~(1u << 8);  /* BCLR 0x1265, #0 */
    auxFlags                   &= ~(1u << 2);  /* BCLR 0x1E1C, #2 */
    return;                                       /* RETURN */
}

/* ============================================================================
 * 4.  pwmUpdate  (0x33F4 – 0x34EC)
 *
 * Called every T1 ISR tick when both PWM comparators are ready.
 * Manages soft-start ramp and FCL (Fault / Current Limit) sequencer.
 *
 * The function has two major paths:
 *
 * A. Normal path  (CMPCON3 bit3 == 0,  comparator not tripped):
 *    Sets statusFlags bit2 (regulating active).
 *    If systemState == 2: drives ADCBUF4-derived values into dead-time regs,
 *    sets run/relay bits, calls setFaultState(), shutdownPwm(),
 *    systemState = 3.
 *    Then advances the FCL sequencer: clears/sets LATD bit4 on CH1,
 *    ORs 0x03 into FCLCON1–3 (enables FCL hardware), advances
 *    fclSeqState state byte, returns.
 *
 * B. Soft-start ramp  (CMPCON3 bit3 == 1, or 0x421 byte >= 0):
 *    Decrements fclSeqState and drives FCLCON1, FCLCON2, or FCLCON3 through
 *    the FCL soft-start sequence, clearing the FCL trip bits (AND with 0xFC).
 * ============================================================================ */
void pwmUpdate(void)   /* 0x33F4 – 0x34EC */
{
    uint16_t adc4_raw;
    uint32_t prod;
    uint16_t dt_lo;
    uint8_t  seq;
    uint8_t  fclcon;

    /* --- Check IOCON1 high byte bit7 (PENH): if PENH == 0 → soft-start path --- */
    if (!IOCON1bits.PENH) /* CP0.B 0x421 / BRA GE — PENH is bit15 of IOCON1 */
        goto soft_start_path;

    /* --- Check CMPCON3 bit3 (comparator fault output) --- */
    fclcon = (uint8_t)CMPCON3;               /* MOV.B CMPCON3, WREG */
    if (!(fclcon & (1u << 3)))            /* BTST.Z W0,#3 / BRA NZ */
        goto soft_start_path;

    /* --- Comparator not tripped: mark regulating active --- */
    statusFlags |= (1u << 2);             /* BSET 0x125A, #2 */

    /* --- Only do the full fault drive when systemState == 2 --- */
    if (systemState != 2)                    /* SUB 0x1E22,#2,[W15] / BRA NZ */
        goto fcl_sequencer;

    /* Assert fault-present */
    controlStatus |= (1u << 0);          /* BSET 0x1E20, #0 */

    /* Choose pwmRunRequest high-byte fault bit (0x1BF1) based on LATD bit4 (relay state) */
    if (LATDbits.LATD4)          /* BTST.Z W0,#4 / BRA Z */
        pwmRunRequest |= (1u << 12);           /* BSET 0x1BF1, #4 */
    else
        pwmRunRequest |= (1u << 13);           /* BSET 0x1BF1, #5 */

    /* Scale ADCBUF4 → dead-time registers (same calculation as fault_handler) */
    adc4_raw = ADCBUF4; /* MOV ADCBUF4, W0 */
    prod   = (uint32_t)adc4_raw * 0x1E80u;
    dt_lo  = (uint16_t)(prod >> 10);

    pdcShadowA = dt_lo;
    pdcShadowB = dt_lo;
    oc1rsGateTiming = dt_lo;

    uint32_t prod2 = (uint32_t)voutScaledQ2 * (uint32_t)dt_lo;
    uint16_t out_lo = (uint16_t)(prod2 >> 14);
    pdc3Shadow = out_lo;
    oc2rsGateTiming = out_lo;

    statusFlags2 |= (1u << 12);              /* relay on */
    pwmRunRequest |= (1u << 0);             /* LLC run */
    pwmRunning &= ~(1u << 0);            /* clear arm */
    setFaultState();                  /* CALL 0x1FD6 */

    pmbusAlertFlags |= (1u << 0);             /* PMBUS alert */
    shutdownPwm();                     /* CALL 0x4B50 */
    systemState = ST_FAULT;

fcl_sequencer:
    /* -----------------------------------------------------------------------
     * FCL sequencer: pulse LATD bit4 low then high (3 NOPs between),
     * OR 0x03 into FCLCON1, repeat for FCLCON2 and FCLCON3 with similar
     * low/high pulses on 0x441 and 0x461, then set fclSeqState = 1.
     * ----------------------------------------------------------------------- */

    /* Pulse IOCON1 POLL (bit12) low then high */
    IOCON1bits.POLL = 0;   /* BCLR 0x421, #4 — bit4 of high byte = bit12 of word = POLL */
    __builtin_nop(); __builtin_nop(); __builtin_nop();
    IOCON1bits.POLL = 1;   /* BSET 0x421, #4 */
    __builtin_nop(); __builtin_nop(); __builtin_nop();

    /* Enable FCL on channel 1: OR 0x03 into FCLCON1 */
    FCLCON1bits.FLTMOD = 3;                        /* IOR.B FCLCON1 / B76424 */
    __builtin_nop();

    /* Pulse IOCON2 POLL (bit12) low then high */
    IOCON2bits.POLL = 0;   /* BCLR 0x441, #4 — bit4 of high byte = bit12 of word = POLL */
    __builtin_nop(); __builtin_nop(); __builtin_nop();
    IOCON2bits.POLL = 1;   /* BSET 0x441, #4 */
    __builtin_nop(); __builtin_nop(); __builtin_nop();

    /* Enable FCL on channel 2 */
    FCLCON2bits.FLTMOD = 3;                        /* IOR.B FCLCON2 */
    __builtin_nop();

    /* Pulse IOCON3 POLL (bit12) low then high */
    IOCON3bits.POLL = 0;   /* BCLR 0x461, #4 — bit4 of high byte = bit12 of word = POLL */
    __builtin_nop(); __builtin_nop(); __builtin_nop();
    IOCON3bits.POLL = 1;   /* BSET 0x461, #4 */
    __builtin_nop(); __builtin_nop(); __builtin_nop();

    /* Enable FCL on channel 3 */
    FCLCON3bits.FLTMOD = 3;                        /* IOR.B FCLCON3 */
    __builtin_nop();

    /* Mark FCL sequencer as armed (state 1) */
    fclSeqState = 1;                                  /* MOV.B #1, W0 / MOV.B WREG,0x124A */
    return;

soft_start_path:
    /* -----------------------------------------------------------------------
     * Soft-start / FCL disarm path.
     * fclSeqState encodes which channel was last armed:
     *   1 → disarm FCLCON1 (AND with 0xFC twice)
     *   2 → disarm FCLCON2
     *   3 → disarm FCLCON3
     *   0 or other → return
     * ----------------------------------------------------------------------- */
    seq = fclSeqState;                                /* MOV.B 0x124A, WREG */

    if (seq == 1) {
        fclSeqState = 2;                              /* advance to state 2 */
        FCLCON1bits.FLTMOD = 0;                   /* clear FCL trip bits */
        __builtin_nop(); __builtin_nop(); __builtin_nop(); __builtin_nop();
        FCLCON1bits.FLTMOD = 0;                   /* second clear (double-buffered) */
        return;
    }
    if (seq == 2) {
        fclSeqState = 3;
        FCLCON2bits.FLTMOD = 0;
        __builtin_nop(); __builtin_nop(); __builtin_nop(); __builtin_nop();
        FCLCON2bits.FLTMOD = 0;
        return;
    }
    if (seq == 3) {
        fclSeqState = 0;                              /* reset state */
        FCLCON3bits.FLTMOD = 0;
        __builtin_nop(); __builtin_nop(); __builtin_nop(); __builtin_nop();
        FCLCON3bits.FLTMOD = 0;
        return;
    }
    /* seq == 0 or unknown → no action */
    return;
}

/* ============================================================================
 * 5.  watchdogService  (0x37AA – 0x37C8)
 *
 * Called every T1 ISR tick.  Routes execution to one of three LLC watchdog
 * helpers depending on the current droopMode value:
 *
 *   systemState must equal 2  AND  auxFlags bit11 (0x1E1D bit3) must be set,
 *   otherwise the function returns immediately without action.
 *
 *   droopMode == 0  → BRA to   (0x378A) [normal-0 helper]
 *   droopMode == 3  → BRA to   (0x371A)
 *   droopMode == 4  → RCALL 0x3666           [separate helper, then return]
 *   droopMode == other (incl. 0) also handled by CP0 → BRA 0x378A
 *
 * Note: the BRA instructions at 0x37C2/0x37C4 are tail-calls (fall into
 * the target which ends with RETURN), so they are modelled as direct calls.
 * ============================================================================ */
void watchdogService(void)   /* 0x37AA – 0x37C8 */
{
    uint16_t state;
    uint16_t mode;

    /* Guard: only active in systemState == 2 */
    state = systemState;                       /* MOV 0x1E22, W0 */
    if (state != 2)                         /* SUB W0,#2,[W15] / BRA NZ */
        return;

    /* Guard: byte alias 0x1E1D bit3 corresponds to auxFlags bit11 */
    if (!(auxFlags & (1u << 11)))          /* BTST 0x1E1D,#3 / BRA Z */
        return;

    mode = droopMode;                      /* MOV 0x1268, W0 */

    if (mode == 3) {                        /* SUB W0,#3,[W15] / BRA Z */
        droopMode3Watchdog();                   /* BRA 0x371A (tail-call) */
        return;
    }
    if (mode == 4) {                        /* SUB W0,#4,[W15] / BRA Z */
        droopMode0Watchdog();                   /* RCALL 0x3666 */
        return;
    }
    if (mode == 0) {                        /* CP0 W0 / BRA NZ → skip */
        runNormalMode();                   /* BRA 0x378A (tail-call) */
        return;
    }
    /* droopMode 1, 2, or any other value: no action */
    return;
}

/* flashPeriodicSave (0x3EBC) — moved to utilities.c */

/* ============================================================================
 * 7.    (0x583E – 0x5840)
 *
 * Very short ISR entry stub for the UART1 receive / timer tick path.
 * Executed whenever the T1 ISR fires on the secondary dispatch vector.
 *
 * Actions:
 *   1. Increment the UART1 RX byte counter at 0x1CF8.
 *   2. Fall through (BRA) to the main UART1 receive processing loop at 0x5582.
 *      (The BRA is assembled as a 16-bit relative branch with no RETURN here;
 *       the callee at 0x5582 holds the RETURN for this path.)
 *
 * Note: This function is entered via CALL 0x583E from the T1 ISR dispatcher
 * (address 0x3894).  The BRA 0x5582 is a tail-jump into the UART1 RX handler.
 * ============================================================================ */
void uartRxTickService(void)   /* 0x583E – 0x5840 */
{
    /* Increment UART1 RX byte counter */
    uartRxByteCount++;                            /* INC 0x1CF8 */

    /* Tail-jump to UART1 receive processing (0x5582).
     * In C we model this as a direct call; the original assembly uses a
     * position-relative BRA which never executes a RETURN of its own. */
    extern void uart1RxHandler(void);    /* entry at 0x5582 */
    uart1RxHandler();
}
