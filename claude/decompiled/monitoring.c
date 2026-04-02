/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — State Machine & Monitor Functions
 * Decompiled from firmware assembly
 *
 * Assembly address ranges covered:
 *             0x4C04 – 0x4C70
 *          0x4C72 – 0x4CA4
 *            0x4CA6 – 0x4D38
 *          0x4D3A – 0x4D84
 *          0x4D86 – 0x4D92
 *   monitorVin          0x4DBC – 0x4DE8
 *   monitorVout         0x4DEA – 0x4EFA
 *   monitorIout         0x4EFC – 0x4F90
 *   monitorTemperature  0x4F92 – 0x4FBA
 *   checkFanControl    0x4FBC – 0x4FEC
 *   checkStandbyRail   0x4FEE – 0x508C
 *   uptimeCounter       0x50D4 – 0x5106
 *   checkEnablePin     0x5108 – 0x512A
 *   vinUvpCheck        0x512C – 0x5148
 *   powerGoodCheck     0x514A – 0x518A
 *           0x518C – 0x51FC
 * ============================================================================ */

#include <stdint.h>
#include <xc.h>
#include "variables.h"

/* --------------------------------------------------------------------------
 * External / forward declarations for functions outside this file
 * -------------------------------------------------------------------------- */
extern void  pwmOverrideEnable(void);       /* 0x4B50 – enable PWM override (force outputs low) */
extern uint16_t checkCurrentLimit(void);   /* 0x4BD4 – returns non-zero when current
                                                          limiting is active */
extern void  setFaultState(void);             /* 0x1FD6 - stop PWM / disable power stage */
extern void  updateStatusLeds(void);         /* 0x571E - UART status LED update */
extern void  t1IsrI2cBody(void);             /* 0x29FA - T1 ISR I2C body */
extern void  updatePmbusStatus(void);        /* 0x546C - PMBus status / droop update */
extern void  checkOtp(void);                 /* 0x4D94 - over-temperature LED control */
extern void  flashCalibrationLoad(void);     /* 0x3B5E - flash calibration loader */
extern void  voltageErrorTracking(void);     /* 0x40C2 - peak voltage/current tracking */
extern void  currentRegulation(void);        /* 0x3F0E - flash sector / telemetry */
extern void  flashReadbackHandler(void);     /* 0x42C8 - flash readback/erase handler */
extern void  i2cProcessCommand(void);        /* 0x5AE4 - I2C2 command processor */
extern void  eepromCfgUpdate(uint8_t mode, uint8_t status, uint8_t enable);
extern uint16_t eepromCfgRead(void);

/* --------------------------------------------------------------------------
 * SFR access notes:
 *   LATD, LATE, LATF      — XC16 <xc.h> provides LATDbits, LATEbits, LATFbits
 *   IOCON1/2/3             — XC16 <xc.h> provides IOCON1bits, IOCON2bits, IOCON3bits
 *   PORTC (high byte)      — XC16 <xc.h> provides PORTCbits (RC13, RC14)
 *   PORTD (high byte)      — XC16 <xc.h> provides PORTDbits (RD10)
 *   OC2RS, I2C2ADD, I2C2STAT — XC16 <xc.h> standard SFR names
 *   vinSenseAdc            — compiler-allocated RAM (was 0x1A51)
 *   tempLimitLut[]         — compiler-allocated RAM (was 0x1BF6)
 * -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
 * Inline copy of pwmOverrideEnable (0x4B50)
 *
 * Enables PWM override on all three channels (OVRENH/OVRENL), forcing
 * half-bridge outputs to their override state (low).  Three NOP delay
 * slots follow each bit-set for synchronisation with the PWM hardware.
 * -------------------------------------------------------------------------- */
static inline void pwmOverrideEnableInline(void)
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

/* ============================================================================
 *  — IDLE state handler
 * Assembly: 0x4C04 – 0x4C70
 *
 * Called every control tick while systemState == ST_IDLE.
 * Performs a full reset of the power stage, clears all fault/status flags,
 * initialises soft-start timers and voltage thresholds, then decides whether
 * to transition to STARTUP (state 1).
 *
 * Transition to state 1 occurs when ALL of the following are true:
 *   - marginThreshold (0x1E10) has counted down to zero
 *   - auxFlags bit11 is set  (supply good indicator)
 *   - flags_1BF2  bit3 is set  (Vin present)
 *   - auxFlags  bit1 is set  (enable active)
 *   - Additionally handles AC-cycle / soft-start restart latches
 * ============================================================================ */
void state0Idle(void)
{
    /* Disable PWM outputs */
    pwmOverrideEnable();   /* RCALL 0x4B50 */

    /* Clear main control and mode flags */
    statusFlags = 0;              /* 0x125A */
    runtimeFlags = 0;              /* 0x126A */
    pwmSoftStartCnt   = 0;              /* soft-start counter */

    /* Initialise soft-start delay timer (~160 ms @ 1 kHz tick) */
    delayTimer = 0xA0;             /* 160 ticks */

    /* IOCON2 PENH – gate drive enable signal (high-side output enable) */
    IOCON2bits.PENH = 1;
    Nop();
    Nop();
    Nop();
    /* IOCON2 PENL – low-side output enable */
    IOCON2bits.PENL = 1;

    /* Clear droop / regulation flags */
    ovpDebounceFlags &= ~(1u << 5);  /* BCLR 0x1263 #5 */
    droopMode = 0;              /* 0x1268 */

    /* Initialise PID coefficient */
    ocpCurrentRef = 0x028E;           /* 654 */

    /* Clear droop integrator */
    droopIntegrator = 0;

    /* Initialise voltage error integrator lower bound */
    voutTargetCode = (int16_t)0xFFB0;  /* -80 */

    /* Vout reference initial values */
    voutRefInitial = 0x2710;           /* 10000 */
    llcPeriodCmd = 0x5DC0;            /* 24000 – LLC period command */

    /* OVP / UVP thresholds */
    compY_out = 0x5DC0;           /* 24000 */
    compY_n1 = 0x5DC0;           /* 24000 */
    compY_n2 = 0x5DC0;           /* 24000 */

    /* Secondary OC setpoint */
    secondaryOcSetpoint = 0x05DC;           /* 1500 */

    /* ---------- Evaluate transition to STARTUP ---------- */

    /* Stay in IDLE until marginThreshold counts down to zero. */
    if (marginThreshold != 0)
        goto done;

    {
        uint16_t f1e1c = auxFlags;

        /* Require auxFlags bit11 set (supply-good indicator) */
        if (!(f1e1c & (1u << 11)))
            goto done;

        /* Require Vin-present flag (flags_1BF2 bit3) */
        if (!(pwmRunning & (1u << 3)))
            goto done;

        /* Require enable signal (auxFlags bit1) */
        if (!(f1e1c & (1u << 1)))
            goto done;

        /* Handle restart latches */
        if (restartFlags & (1u << 0)) {
            /* AC-cycle restart was requested – clear it and also clear
             * auxFlags bit3 (soft-start complete) */
            restartFlags &= ~(1u << 0);
            auxFlags &= ~(1u << 3);
        } else {
            /* Normal path: set auxFlags bit3 */
            auxFlags |= (1u << 3);
            /* Clear restart latch in 0x1BEA bit2 */
            droopEnableFlags &= ~(1u << 2);
        }

        /* Reset startup tick counter and advance to STARTUP */
        startupTickCnt  = 0;
        systemState  = 1;           /* → STARTUP */
    }

done:
    /* Clear active flag */
    ovpDebounceFlags &= ~(1u << 6);  /* BCLR 0x1263 #6 */
}

/* ============================================================================
 *  — STARTUP state handler
 * Assembly: 0x4C72 – 0x4CA4
 *
 * Increments a startup tick counter (startupTickCnt).  When the counter wraps
 * to zero the function takes the "timeout" path and falls through to the
 * section that arms the power stage and transitions to ACTIVE (state 2).
 *
 * On every non-timeout tick the function also refreshes the OC2RS (output
 * compare / fan PWM) register from a constant 0x190 (400) value.
 * ============================================================================ */
void state1Startup(void)
{
    uint16_t tick;

    /* Increment startup tick counter */
    tick = startupTickCnt + 1u;
    startupTickCnt = tick;

    if (tick == 0) {
        /* Counter wrapped – startup timeout: proceed to arm the power stage */
        goto arm_stage;
    }

    /* ----- Non-timeout tick: re-initialise soft timer and clear flags ----- */
    faultResetTimer = 0xA0;                          /* fault soft-reset timer = 160 */
    protStatusByte &= ~(1u << 5); /* BCLR 0x1265 #5 */
    auxFlags &= ~(1u << 2);                   /* BCLR 0x1E1C #2 */
    controlStatus = 0;                             /* clear controlStatus */
    startupResetLatch2 = 0;
    pwmRunRequest = 0;
    startupResetLatch = 0;

    /* Select watchdog target based on droop mode */
    {
        uint16_t vref;

        if (internalStatusFlags & (1u << 3)) {
            /* Droop mode 3 active: use high-droop Vref 0x6B3 = 1715 */
            vref = 0x06B3;
        } else {
            /* Default Vref 0xC4D = 3149 */
            vref = 0x0C4D;
            if (restartFlags & (1u << 1)) {
                /* Soft-start restart: use alternate Vref 0x6B3 = 1715 */
                vref = 0x06B3;
            }
        }
        voutRefTarget = (int16_t)vref;
    }

    /* Advance to ACTIVE */
    systemState = ST_ACTIVE;                            /* → ACTIVE */

    /* Set active flag */
    ovpDebounceFlags |= (1u << 6); /* BSET 0x1263 #6 */

    /* Clear OT shutdown flag */
    internalStatusFlags &= ~(1u << 7);

arm_stage:
    /* Set OC2RS to 400 and update shadow */
    OC2RS        = 0x0190;
    oc2rs_shadow     = 0x0190;
}

/* ============================================================================
 *  — FAULT state handler
 * Assembly: 0x4CA6 – 0x4D38
 *
 * Manages the PSU in the FAULT / shutdown state.  Handles two sub-paths:
 *
 *  1. Active fault run-down (bit5 of 0x1265 is set AND controlStatus != 0):
 *     Clears flags and issues a PWM shutdown.  If controlStatus has bits
 *     1 or 3 set (OVP / OCP conditions) the AC-cycle and soft-start restart
 *     latches in 0x1E1E are armed so that when the fault clears the firmware
 *     will attempt an automatic restart.
 *     Optionally updates a current-limit droop flag (bit6 of 0x1E19) based
 *     on droopMode and LATD bit4 (output diode sense).
 *
 *  2. Fault recovery evaluation:
 *     Clears all fault flags, disables PWM, re-initialises the error
 *     integrator, then checks whether the conditions for returning to IDLE
 *     (state 0) are met:
 *       - Bits (0x802) in auxFlags are both set → go IDLE if OVP latch clear
 *       - Bit6 of 0x1262 set → go IDLE only if droop active (bit6 of 0x1E19)
 *       - Bit3 of 0x1262 set → go IDLE only if droop active
 *       - flags_1E1E bit0 set → go IDLE if controlStatus & 0xA == 0
 * ============================================================================ */
void state3Fault(void)
{
    /* ---- Sub-path 1: active run-down ---- */
    if ((protStatusByte & (1u << 5)) &&
        (controlStatus != 0)) {

        /* Clear run flag */
        protStatusByte &= ~(1u << 5);

        /* Arm restart latches if OVP (bit1) or OCP (bit3) caused the fault */
        {
            uint16_t cf = controlStatus & 0x000A;  /* AND with 0xA = bits 1,3 */
            if (cf != 0) {
                restartFlags |= (1u << 0);   /* AC-cycle restart latch */
                restartFlags |= (1u << 1);   /* soft-start restart latch */
            }
        }

        /* Disable PWM outputs and clear fault register */
        statusFlags = 0;
        runtimeFlags = 0;
        pwmRunRequest   |= (1u << 0);    /* set PWM run request */
        pwmRunning   &= ~(1u << 0);   /* clear PWM running flag */
        setFaultState();                 /* stop power stage */
        Nop();
        pmbusAlertFlags |= (1u << 0);       /* PMBus alert */
    }

    /* ---- Sub-path 2: evaluate droop-mode current-limit flag ---- */
    if (currentLimitFlags >= 0) {   /* CP0 0x1E18 then BRA LT → skip if negative */
        /* Set OT-shutdown latch */
        internalStatusFlags |= (1u << 7);

        if (internalStatusFlags & (1u << 3)) {
            /* Droop mode 3: check droopMode == 3 AND LATD bit4 == 0 */
            uint16_t dm = droopMode;
            if ((dm - 3u) != 0)
                goto clear_droop_bit;
            {
                if (!LATDbits.LATD4)
                    goto clear_droop_bit;
            }
        }
        /* Set droop-active flag */
        internalStatusFlags |= (1u << 6);
        goto after_droop;
clear_droop_bit:
        internalStatusFlags &= ~(1u << 6);
    }
after_droop:

    /* ---- Common shutdown sequence ---- */
    statusFlags = 0;
    runtimeFlags = 0;
    droopMode = 0;
    ovpDebounceFlags &= ~(1u << 6);

    pwmOverrideEnable();   /* RCALL 0x4B50 */

    /* Reinitialise integrator lower-bound */
    voutTargetCode = (int16_t)0xFFB0;   /* -80 */

    /* ---- Transition back to IDLE? ---- */
    {
        uint16_t f1e1c = auxFlags;
        uint16_t masked = f1e1c & 0x0802;   /* bits 1 and 11 */

        if (masked == 0x0802) {
            /* Both bits set – check OVP latch (0x1262 bit0) */
            uint16_t f1262 = statusFlags2;
            if (f1262 & (1u << 0)) {
                /* OVP latch is set → stay in fault */
                goto set_margin_idle;
            }
            /* No OVP latch → transition to IDLE */
            goto go_idle;
        }

        /* -- fault-latch-based checks -- */
        {
            uint16_t f1262 = statusFlags2;

            /* OT-shutdown latch (bit6): need droop active to recover */
            if (f1262 & (1u << 6)) {
                if (internalStatusFlags & (1u << 6))
                    goto go_idle;
                /* Else fall through to further checks */
            }

            /* Vin-UV latch (bit3): need droop active to recover */
            if (f1262 & (1u << 3)) {
                if (internalStatusFlags & (1u << 6))
                    goto go_idle;
                goto stay_fault;
            }

            /* If none of the above – check 0x1E1E bit0 (AC-cycle restart) */
            if (restartFlags & (1u << 0)) {
                uint16_t cf = controlStatus & 0x000A;
                if (cf == 0) {
                    /* Clear restart flags and go IDLE */
                    goto go_idle_clear_margin;
                }
            }
        }
    }
    goto stay_fault;

go_idle:
    /* Clear OT-related bits in 0x1262 */
    statusFlags2 &= ~(1u << 6);  /* BCLR 0x1262 #6 */
    statusFlags2 &= ~(1u << 5);
    statusFlags2 &= ~(1u << 3);
    goto stay_fault;   /* return after clearing */

go_idle_clear_margin:
    systemState = ST_IDLE;      /* → IDLE */
    /* Fall through to set margin */

set_margin_idle:
    marginThreshold = 0x09C4;  /* marginThreshold = 2500 */
    systemState = ST_IDLE;                            /* → IDLE */
    return;

stay_fault:
    return;
}

/* ============================================================================
 *  — STATE5 handler
 * Assembly: 0x4D3A – 0x4D84
 *
 * STATE5 is an intermediate restart/hold state entered after a fault where
 * the firmware waits for a hardware-ID-dependent timeout before transitioning
 * to IDLE (state 0).
 *
 * On entry (bit0 of 0x1E19 clear) the counters are reset.  Each call
 * increments state5MsCounter (ms sub-counter); when it exceeds 999 it rolls over
 * and increments state5SecCounter (seconds counter).
 *
 * The hardware type byte at 0x1BCC is added to 6.  If the result is non-zero
 * the function checks whether state5SecCounter (seconds elapsed) exceeds the type-
 * dependent threshold.  If so it returns to IDLE (state 0) with
 * marginThreshold = 50.
 *
 * If the result is zero (hardware type = -6) the function additionally
 * requires auxFlags bit5 AND (if Vin present) auxFlags bit1 before
 * allowing the transition.
 * ============================================================================ */
void state5Handler(void)
{
    /* On first entry (init flag not yet set) reset the timers */
    if (!(internalStatusFlags & (1u << 0))) {
        internalStatusFlags &= ~(1u << 0);   /* already clear, kept for symmetry */
        state5SecCounter   = 0;
        state5MsCounter   = 0;
        voutTargetCode   = (int16_t)0xFFB0;  /* reset integrator bound */
    }
    /* Set init-done flag for subsequent calls */
    internalStatusFlags &= ~(1u << 0);   /* ensure clear (redundant but matches asm) */

    /* Increment ms sub-counter */
    {
        uint16_t ms = state5MsCounter + 1u;
        state5MsCounter = ms;

        /* Roll over at 999 → increment seconds counter */
        if (ms > 0x03E7u) {           /* 999 */
            state5MsCounter = 0;
            state5SecCounter++;
        }
    }

    /* Read hardware type byte and add 6 */
    {
        int8_t  hw_type_adj = (int8_t)(hwConfigByte) + 6;

        if (hw_type_adj != 0) {
            /* Non-zero: use state5SecCounter (seconds) as threshold index */
            uint16_t elapsed  = state5SecCounter;
            uint8_t  type_idx = (uint8_t)hw_type_adj;  /* zero-extended */

            if (elapsed < (uint16_t)type_idx) {
                /* Hold state: not yet timed out */
                return;
            }
            /* Timed out – transition to IDLE with threshold 50 */
            marginThreshold = 0x0032;   /* 50 */
            systemState = ST_IDLE;                              /* systemState = IDLE */
            return;
        } else {
            /* hw_type == -6: special path – check 1E1C flags */
            uint16_t f1e1c = auxFlags;
            uint16_t bit5  = f1e1c & (1u << 5);

            if (bit5 == 0) {
                /* Bit5 clear: check Vin present (0x1BF2 bit3) */
                if (!(pwmRunning & (1u << 3))) {
                    /* No Vin – transition to IDLE */
                    goto go_idle_50;
                }
                /* Vin present but bit1 also required */
                if (f1e1c & (1u << 1)) {
                    /* bit1 set → stay */
                    return;
                }
            }
            goto go_idle_50;
        }
    }

go_idle_50:
    marginThreshold = 0x0032;   /* marginThreshold = 50 */
    systemState = ST_IDLE;                              /* → IDLE */
}

/* ============================================================================
 *  — STATE6 handler
 * Assembly: 0x4D86 – 0x4D92
 *
 * A minimal handler for STATE6.  Checks whether LATF bit1 (output relay /
 * gate signal) is de-asserted.  If not, sets marginThreshold = 50 and
 * transitions to IDLE (state 0).
 * ============================================================================ */
void state6Handler(void)
{
    if (!LATFbits.LATF1) {
        /* Output relay is off – return to IDLE */
        marginThreshold = 0x0032;   /* 50 */
        systemState = ST_IDLE;                              /* → IDLE */
    }
    /* If LATF bit1 is set, stay in state 6 (no action) */
}

/* ============================================================================
 * monitorVin — Vin (input voltage) monitor
 * Assembly: 0x4DBC – 0x4DE8
 *
 * Called each tick.  If vinCooldownTimer (cooldown counter) is non-zero it is
 * decremented and no action is taken.  When it is zero the function uses the
 * 32-sample average of ADCBUF10 (trim_avg_adc10 = trim_avg_adc10) to look up a
 * current-limit value from a table and stores the result in ioutLimitHi
 * (ioutLimitHi).
 *
 * ADC value is clamped to [0x23, 0xE7] before the table index is computed:
 *   index = 0xE8 - clamped_value
 *   ioutLimitHi = TEMP_LUT_BASE[index]  (zero-extended to 16-bit)
 * ============================================================================ */
void monitorVin(void)
{
    uint16_t cnt = vinCooldownTimer;

    if (cnt != 0) {
        /* Still in cooldown – decrement and return */
        vinCooldownTimer = cnt - 1u;
        return;
    }

    /* Cooldown expired: compute current limit from ADCBUF10 average */
    {
        uint16_t adc = (uint16_t)trim_avg_adc10;
        uint16_t clamped;

        if (adc <= 0x0023u) {
            clamped = 0x0023u;
        } else if (adc > 0x00E7u) {
            clamped = 0x00E8u;
        } else {
            clamped = adc;
        }

        {
            uint16_t idx   = 0x00E8u - clamped;
            uint16_t limit = (uint16_t)tempLimitLut[idx];
            ioutLimitHi = (int16_t)limit;       /* ioutLimitHi */
        }
    }
}

/* ============================================================================
 * monitorVout — Vout (output voltage) monitor / OVP
 * Assembly: 0x4DEA – 0x4EFA
 *
 * Called each tick.  If vinCooldownTimer is non-zero the function is a no-op.
 *
 * Otherwise it calculates a running OVP/fan setpoint (voutFanSetpoint) based on the
 * 32-sample average of ADCBUF1 (trim_avg_adc1 = trim_avg_adc1) and the Vout ADC
 * reading (outputVoltage):
 *
 *   1. Look up a base limit from the temperature table using the ADCBUF1
 *      average (same clamp logic as monitorVin) → stored in vinOcpLimit.
 *
 *   2. Compute a Vout-adaptive OVP limit (voutWorkValue) using piece-wise linear
 *      segments over the Vout ADC range.
 *
 *   3. If vcal_diff (0x126C) > 19 AND LATF bit0 clear AND droopMode in
 *      [0,3]: run a debounce timer (ovpDebounceCnt) and optionally scale a product
 *      (ovpMultResultLo:ovpMultResultHi) against Imeas_cal_a.  If the product exceeds 0x2183
 *      increment ovpTripCounter; if ovpTripCounter > 0x4B0 set OCP flag (0x1E21 bit2).
 *
 *   4. Compare voutOvpThreshold against W9 (ADCBUF1 clamped) and vinCooldownTimer against
 *      the base limit (W8); if both conditions are met set 0x1E21 bit2.
 * ============================================================================ */
void monitorVout(void)
{
    uint16_t adc1_avg;   /* W8 – ADCBUF1 average */
    uint16_t lut_val;    /* W9 – table lookup result */

    /* Save W8, W9 (represented as local variables) */

    if (vinCooldownTimer != 0) {
        /* Cooldown active – do nothing */
        return;
    }

    /* --- Step 1: look up limit from ADCBUF1 average --- */
    adc1_avg = (uint16_t)trim_avg_adc1;
    {
        uint16_t clamped;
        if (adc1_avg <= 0x0023u) {
            clamped = 0x0023u;
        } else if (adc1_avg > 0x00E7u) {
            clamped = 0x00E8u;
        } else {
            clamped = adc1_avg;
        }
        {
            uint16_t idx = 0x00E8u - clamped;
            lut_val = (uint16_t)tempLimitLut[idx];
            vinOcpLimit = lut_val;   /* store Vin OCP limit */
        }
    }

    /* --- Step 2: piece-wise linear OVP setpoint from Vout ADC --- */
    {
        uint16_t vout = (uint16_t)outputVoltage;
        int16_t  limit;
        uint16_t base_hi = (uint16_t)ioutLimitHi;   /* ioutLimitHi */

        if (vout <= 0x0EA5u) {
            /* Vout <= 3749: use base limit unchanged */
            limit = (int16_t)base_hi;
        } else if (vout <= 0x1FDFu) {
            /* 3750 – 8127: interpolate: limit = (vout - 0xF15A) >> 10 + base_hi */
            limit = (int16_t)(((int16_t)(vout + (uint16_t)0xF15Au) >> 10) +
                              (int16_t)base_hi);
        } else if (vout <= 0x2615u) {
            /* 8128 – 9749: limit = base_hi - 8 */
            limit = (int16_t)base_hi - 8;
        } else if (vout <= 0x2E49u) {
            /* 9750 – 11849: limit = base_hi - 6 */
            limit = (int16_t)base_hi - 6;
        } else {
            /* > 11849: limit = base_hi - 5 */
            limit = (int16_t)base_hi - 5;
        }

        /* Subtract 15 from limit to get OVP working value */
        limit -= 15;
        voutWorkValue = (uint16_t)limit;
    }

    /* --- Step 3: load voutWorkValue and clamp fan setpoint --- */
    {
        int16_t w1 = (int16_t)voutWorkValue;

        /* Clamp checks */
        if (w1 <= 0x0009) {   /* <= 9 → clamp to 0x42 = 66 */
            voutFanSetpoint = 0x0042u;
        } else if (w1 <= 0x0019) {   /* <= 25 → clamp to 0x44 = 68 */
            voutFanSetpoint = 0x0044u;
        } else if (w1 <= 0x0029u) {  /* <= 41 */
            uint16_t adj = (uint16_t)(w1 + 0x0026u);   /* + 38 */
            voutFanSetpoint = adj;
            int16_t hi   = w1 + 0x001A;               /* + 26 */
            if ((int16_t)adj > hi) {
                /* cap reached */
                goto cap_55;
            }
            /* inc by 1 then cap */
            voutFanSetpoint = (uint16_t)(hi + 1);
            if ((int16_t)voutFanSetpoint > 0x0055) {
cap_55:
                voutFanSetpoint = 0x0055u;
            }
        } else if (w1 <= 0x0031u) {  /* 42 – 49 */
            /* interpolate: out = (w1 - 0x2A) * 0x514 >> 10 + 0x50 */
            int16_t delta = w1 - 0x002A;
            int32_t scaled = ((int32_t)delta * 0x0514) >> 10;
            int16_t out16  = (int16_t)(scaled + 0x50);
            voutFanSetpoint = (uint16_t)out16;
            if (out16 > 0x55) {
                voutFanSetpoint = 0x0055u;
            }
        } else {
            /* > 49 → clamp to 0x55 = 85 */
            voutFanSetpoint = 0x0055u;
        }
    }

    /* --- Step 4: vcal_diff / OVP debounce --- */
    {
        int16_t diff = vcal_diff;   /* vcal_diff */

        if (diff <= 0x0013) {
            /* vcal_diff <= 19: clear debounce counters, set default threshold */
            ovpDebounceCnt = 0;
            ovpTripCounter = 0;
            voutOvpThreshold = 0x0069u;   /* 105 */
            goto check_threshold;
        }

        /* vcal_diff > 19 */

        /* Require LATF bit0 clear (relay off) */
        if (LATFbits.LATF0) {
            goto clear_debounce;
        }

        /* Require droopMode <= 3 */
        {
            uint16_t dm = droopMode;
            dm -= 3u;
            if (dm > 3u) {   /* BRA GTU: unsigned > 3 */
                goto clear_debounce;
            }
        }

        /* Increment ms debounce counter, cap at 1000 */
        {
            uint16_t dfa = ovpDebounceCnt + 1u;
            ovpDebounceCnt = dfa;
            if (dfa > 0x03E8u) {
                ovpDebounceCnt = 0x03E8u;
                /* Save current fan setpoint */
                voutOvpThreshold = voutFanSetpoint;
            }
        }

        /* Multiply ovpDebounceCnt (signed) by Imeas_cal_a (Imeas_cal_a) */
        {
            int32_t product = (int32_t)(int16_t)ovpDebounceCnt *
                              (int32_t)(int16_t)Imeas_cal_a;
            /* Store 32-bit result */
            ovpMultResultLo = (uint16_t)(product & 0xFFFF);
            ovpMultResultHi = (uint16_t)((uint32_t)product >> 16);

            /* Compare product against 0x2183 */
            if (product <= (int32_t)0x00002183) {
                /* Product within range – clear ovpTripCounter */
                ovpTripCounter = 0;
                goto check_threshold;
            }

            /* Product exceeded – increment ovpTripCounter and check cap */
            {
                uint16_t dfc = ovpTripCounter + 1u;
                ovpTripCounter = dfc;
                if (dfc > 0x04B0u) {
                    /* Trip OCP */
                    ovpTripCounter = 0x04B0u;
                    ocpTripFlags |= (1u << 2);   /* BSET 0x1E21 #2 */
                    goto check_threshold;
                }
            }
            goto check_threshold;
        }

clear_debounce:
        ovpDebounceCnt = 0;
        ovpTripCounter = 0;
        voutOvpThreshold = 0x0069u;   /* 105 */
    }

check_threshold:
    /* Final comparison: if ADCBUF1 lut_val > voutOvpThreshold → OCP trip */
    {
        uint16_t thresh = voutOvpThreshold;
        if (lut_val > thresh) {
            /* lut_val carries W9 */
            goto done_vout;
        }
        /* lut_val <= thresh: check adc1_avg (W8) vs 0x28 = 40 */
        if (adc1_avg > 0x0028u) {
            goto done_vout;
        }
        /* Both conditions met – set OCP flag */
        ocpTripFlags |= (1u << 2);
    }

done_vout:
    return;
}

/* ============================================================================
 * monitorIout — Iout (output current) monitor / OCP
 * Assembly: 0x4EFC – 0x4F90
 *
 * Called each tick.  If vinCooldownTimer is non-zero the function is a no-op.
 *
 * Otherwise it computes ioutLimitLo (ioutLimitLo) from trim_avg_adc11
 * (trim_avg_adc11) using the same temperature-lookup logic as monitorVin, then
 * implements a multi-tier OCP check:
 *
 * If systemFlags (signed) < 0:
 *   If iout limit > 0x55 → return (no fault)
 *   If adc11_avg > 0xF5 → return (no fault)
 *   Else clear flag bits → allowed to recover
 *
 * If systemFlags >= 0:
 *   If systemFlags bit7 is set (OT/OVP condition):
 *     Set 0x1E1E bit2, then OCP thresholds depend on droopMode:
 *       droopMode == 3: limit > 0x6A && adc11_avg <= 0xF9 → BSET 0x1E1B #7
 *       droopMode == 4: limit > 0x6A && adc11_avg <= 0xF9 → BSET 0x1E1B #7
 *     then check droopMode == 3 and adc11_avg > 0xF9 → BSET 0x192A #0 + BSET 0x1E1A #7
 *   If systemFlags bit7 is clear:
 *     droopMode == 3: limit > 0x68 && adc11_avg > 0xF9 → BSET 0x192A #0 + BSET 0x1E1A #7
 *     droopMode == 4: same thresholds as above
 * ============================================================================ */
void monitorIout(void)
{
    uint16_t adc11_avg;
    uint16_t lut_val2;    /* ioutLimitLo value from LUT */

    if (vinCooldownTimer != 0)
        return;

    /* Compute ioutLimitLo from ADCBUF11 average */
    adc11_avg = (uint16_t)trim_avg_adc11;
    {
        uint16_t clamped;
        if (adc11_avg <= 0x0023u) {
            clamped = 0x0023u;
        } else if (adc11_avg > 0x00E7u) {
            clamped = 0x00E8u;
        } else {
            clamped = adc11_avg;
        }
        {
            uint16_t idx  = 0x00E8u - clamped;
            lut_val2 = (uint16_t)tempLimitLut[idx];
            ioutLimitLo = (int16_t)lut_val2;   /* ioutLimitLo */
        }
    }

    /* --- OCP evaluation --- */
    {
        int16_t f1e1a = (int16_t)systemFlags;

        if (f1e1a < 0) {
            /* systemFlags is negative (overload state) */
            if (lut_val2 > 0x0055u)
                return;   /* limit above threshold – no fault */
            if (adc11_avg > 0x00F5u)
                return;   /* adc value too high – no fault */
            /* Recovery: clear OCP flags */
            fwUpdateFlags &= ~(1u << 7);
            systemFlags &= ~(1u << 7);
            return;
        }

        /* systemFlags >= 0 */
        if (f1e1a & (int16_t)(1u << 7)) {
            /* OT/OVP condition active (bit7 set) */
            restartFlags |= (1u << 2);   /* set OCP restart latch */

            if (lut_val2 > 0x0055u) {
                if (adc11_avg > 0x00F5u) {
                    fwUpdateFlags &= ~(1u << 7);
                    systemFlags &= ~(1u << 7);
                    return;
                }
            }

            /* Check droopMode == 4 */
            {
                uint16_t dm = droopMode;
                if ((dm - 4u) == 0) {
                    /* droopMode 4 */
                    if (lut_val2 > 0x006Au) {
                        if (adc11_avg <= 0x00F9u) {
                            fwUpdateFlags |= (1u << 7);
                            return;
                        }
                    }
                    goto check_dm3_path;
                }

                /* Check droopMode == 3 */
check_dm3_path:
                if ((dm - 3u) == 0) {
                    if (lut_val2 > 0x006Au) {
                        if (adc11_avg <= 0x00F9u) {
                            fwUpdateFlags |= (1u << 7);
                            return;
                        }
                    }
                }
            }
            return;
        }

        /* systemFlags bit7 clear (no active OT/OVP) */
        {
            uint16_t dm = droopMode;

            /* droopMode == 3 */
            if ((dm - 3u) == 0) {
                if (lut_val2 > 0x0068u) {
                    if (adc11_avg > 0x00F9u) {
                        pmbusAlertFlags |= (1u << 0);
                        systemFlags |= (1u << 7);
                        return;
                    }
                }
                return;
            }

            /* droopMode == 4 */
            if ((dm - 4u) == 0) {
                if (lut_val2 > 0x006Au) {
                    if (adc11_avg > 0x00F9u) {
                        fwUpdateFlags |= (1u << 7);
                        return;
                    }
                }
            }
        }
    }
}

/* ============================================================================
 * monitorTemperature — temperature / OTP monitor
 * Assembly: 0x4F92 – 0x4FBA
 *
 * Called each tick.  Reads tempAdcValue (temperature ADC value).
 * If the over-temperature flag (systemFlags bit5) is already set AND
 * bit6 of protStatusByte is set (active run flag), set restart latch 0x1E1E bit2.
 *
 * Regardless, if tempAdcValue > 85 (0x55) → no action (temperature OK).
 *
 * Otherwise if protStatusByte bit6 is clear (not running):
 *   If droopMode == 3 AND tempAdcValue > 102 (0x66): set PMBus alert and
 *   systemFlags bit5 (OT warning).
 *
 * If protStatusByte bit6 is set (running):
 *   The check passes without further action.
 * ============================================================================ */
void monitorTemperature(void)
{
    uint8_t running = (uint8_t)(protStatusByte >> 6) & 1u;

    if (!running) {
        /* Not running: check for OT warning via systemFlags bit5 */
        if (systemFlags & (1u << 5)) {
            restartFlags |= (1u << 2);
        }
    } else {
        /* Running: set restart latch if OT flag active */
        if (systemFlags & (1u << 5)) {
            restartFlags |= (1u << 2);
        }
    }

    /* Check temperature value */
    {
        uint16_t temp = tempAdcValue;

        if (temp > 0x0055u) {
            /* Temperature > 85 (ADC units) – clear OT flag */
            systemFlags &= ~(1u << 5);
            return;
        }

        if (!running) {
            /* Not running and temp <= 85 – check droopMode */
            uint16_t dm = droopMode;
            if ((dm - 3u) != 0)
                return;

            /* droopMode == 3: OT if temp > 102 */
            if (temp > 0x0066u) {
                pmbusAlertFlags |= (1u << 0);     /* PMBus alert */
                systemFlags |= (1u << 5);     /* OT warning flag */
            }
        }
    }
}

/* ============================================================================
 * checkFanControl — fan I2C address update
 * Assembly: 0x4FBC – 0x4FEC
 *
 * Reads three hardware configuration bits (PORTC/PORTD GPIO), assembles
 * a fan controller I2C address, and writes it to I2C2ADD.
 *
 * Bit mapping:
 *   Address bit1 ← PORTC high-byte bit6  (RC14)
 *   Address bit2 ← PORTC high-byte bit5  (RC13)
 *   Address bit3 ← PORTD high-byte bit2  (RD10)
 *
 * Final I2C address = (assembled_bits | 0xB0) >> 1
 * ============================================================================ */
void checkFanControl(void)
{
    uint16_t addr_bits;

    /* Start from existing 0x1E08, clear bits 1-3 */
    addr_bits = fanI2cAddr;

    /* Bit1 from PORTC bit14 (RC14) — was high byte of PORTC at 0x2D3, bit6 */
    addr_bits &= ~(1u << 1);
    addr_bits |= ((uint16_t)PORTCbits.RC14 << 1);

    /* Bit2 from PORTC bit13 (RC13) — was high byte of PORTC at 0x2D3, bit5 */
    addr_bits &= ~(1u << 2);
    addr_bits |= ((uint16_t)PORTCbits.RC13 << 2);

    /* Bit3 from PORTD bit10 (RD10) — was high byte of PORTD at 0x2DB, bit2 */
    addr_bits &= ~(1u << 3);
    addr_bits |= ((uint16_t)PORTDbits.RD10 << 3);

    fanI2cAddr = addr_bits;

    /* Build I2C address: keep bits 1-3, OR with 0xB0 (base 0x58), then >> 1 */
    {
        uint16_t a = (addr_bits & 0x000Eu) | 0x00B0u;
        a >>= 1;
        I2C2ADD = a;
    }
}

/* ============================================================================
 * checkStandbyRail — standby (12 V) rail and Vin/output relay monitor
 * Assembly: 0x4FEE – 0x508C
 *
 * Complex multi-condition function that monitors:
 *   - Whether the 12 V standby rail (flags_1BF2 bit3) is present
 *   - The current systemState (must be ACTIVE = 2)
 *   - controlStatus for fault bits
 *   - LATD bit3 (output relay drive signal)
 *   - ADCBUF5 (Vin sense ADC) against thresholds that vary with droopMode
 *
 * Based on the conditions the function either:
 *   - Sets LATD bit3 (output relay ON) and sets pwmRunning bit0 (PWM run)
 *   - Clears LATD bit3 (relay OFF) and clears pwmRunning bit0
 *   - Increments standbyDebounce debounce counter toward opening the relay
 *
 * standbyDebounce tracks how many ticks the Vin sense has been above threshold.
 * When standbyDebounce > 59 (0x3B) LATD bit3 is cleared (relay opens, PWM stops).
 *
 * If any of the guard conditions fail (no 12V, wrong state, fault bits, etc.)
 * the function forces LATD bit3 HIGH (relay on default) and standbyDebounce is set
 * appropriately.
 * ============================================================================ */
void checkStandbyRail(void)
{
    uint16_t f1bf2 = pwmRunning;

    /* Guard: require 12V present (bit3) */
    if (!(f1bf2 & (1u << 3))) {
        /* No 12V – force relay on and clear debounce */
        LATDbits.LATD3 = 1;
        standbyDebounce = 0;
        /* Set PWM running flag */
        pwmRunning &= ~(1u << 0);
        return;
    }

    /* Guard: must be in ACTIVE state (systemState == 2) */
    {
        uint16_t state = systemState;
        if (state != 2u) {
            goto force_relay_on;
        }

        /* Guard: controlStatus must not have OVP/fault bits (bit8) */
        {
            uint16_t cf = controlStatus;
            if (cf & (1u << 8)) {
                goto force_relay_on;
            }

            /* Check for OT/fault bits in systemFlags (bits 5 and 7 = 0xA0) */
            {
                uint16_t f1e1a  = systemFlags & 0x00A0u;  /* bits 5,7 */
                uint16_t dm1    = droopMode;

                if (f1e1a != 0) {
                    /* OT/fault active */
                    if ((dm1 - 4u) == 0) {
                        /* droopMode == 4: check 0x1E19 bit3 */
                        if (!(internalStatusFlags & (1u << 3)))
                            goto force_relay_on;
                    }
                }

                /* Check droopMode == 4 with controlStatus bit1 */
                {
                    uint16_t dm2 = droopMode;
                    if ((dm2 - 4u) == 0) {
                        if (cf & (1u << 1)) {
                            /* bit1 set → force relay on */
                            goto force_relay_on;
                        }
                    }
                }

                /* ---- Relay/Vin hysteresis logic ---- */
                {
                    uint16_t f1e1c = auxFlags;
                    uint16_t bit2  = f1e1c & (1u << 1);

                    if (bit2 == 0) {
                        /* bit1 clear: normal path */

                        if (LATDbits.LATD3) {
                            /* Relay currently ON: update debounce state */
                            standbyDebounce = 0;

                            /* Read vinSenseAdc and compare against droop-mode threshold */
                            {
                                uint16_t adcbuf5 = vinSenseAdc;
                                uint16_t thresh;

                                if (LATDbits.LATD4) {
                                    thresh = 0x0153u;   /* 339 – higher rail */
                                } else {
                                    thresh = 0x02D9u;   /* 729 – lower rail */
                                }

                                if (adcbuf5 > thresh) {
                                    /* Vin above threshold – relay stays on */
                                    goto relay_on_path;
                                }
                                /* Vin below threshold – turn relay ON (enable path) */
                                LATDbits.LATD3 = 1;
                                goto set_pwm_flag;
                            }
                        } else {
                            /* Relay currently OFF – check ADCBUF5 */
                            goto debounce_path;
                        }
                    } else {
                        /* bit1 set */
                        if (f1e1a == 0) {
                            /* No fault flags */
                            uint16_t dm3 = droopMode;
                            if ((dm3 - 4u) == 0) {
                                /* droopMode 4: check vinSenseAdc vs 0x179 */
                                uint16_t adcbuf5 = vinSenseAdc;
                                if (adcbuf5 <= 0x0179u) {
                                    goto relay_on_path;
                                }
                                goto debounce_path;
                            }
                            /* droopMode 3: check vinSenseAdc vs 0x2DF */
                            if ((dm3 - 3u) == 0) {
                                uint16_t adcbuf5 = vinSenseAdc;
                                if (adcbuf5 <= 0x02DFu) {
                                    goto debounce_path_slow;
                                }
                                goto debounce_path;
                            }
                            goto debounce_path;
                        } else {
                            goto relay_on_path;
                        }
                    }
                }
            }
        }
    }

debounce_path:
    /* Increment standbyDebounce debounce counter */
    {
        uint16_t cnt = standbyDebounce + 1u;
        standbyDebounce = cnt;
        if (cnt <= 0x003Bu) {   /* <= 59 */
            goto set_pwm_flag;
        }
        /* Counter expired: cap at 60, clear relay */
        standbyDebounce = 0x003Cu;
        LATDbits.LATD3 = 0;    /* relay OFF */
        goto set_pwm_flag;
    }

debounce_path_slow:
    standbyDebounce = 0;
    goto set_pwm_flag;

relay_on_path:
    LATDbits.LATD3 = 1;
    standbyDebounce = 0;
    goto set_pwm_flag;

force_relay_on:
    LATDbits.LATD3 = 1;
    standbyDebounce = 0;

set_pwm_flag:
    /* Reflect relay state into pwmRunning bit0 (PWM run) */
    if (LATDbits.LATD3) {
        /* Relay OFF → clear PWM run */
        pwmRunning &= ~(1u << 0);
    } else {
        /* Relay ON → set PWM run */
        pwmRunning |= (1u << 0);
    }
}

/* ============================================================================
 * uptimeCounter — 32-bit uptime counter with flash-write trigger
 * Assembly: 0x50D4 – 0x5106
 *
 * If fwUpdateFlags bit1 is clear the 32-bit counter at uptimeCounterLo:uptimeCounterHi is
 * zeroed, otherwise:
 *   - Increments uptimeCounterLo:uptimeCounterHi (32-bit add with carry)
 *   - If the counter exceeds 0x001B_773F (1,800,511) it clears bit1 of
 *     fwUpdateFlags, prepares flash-write parameters and calls two flash
 *     service routines (5A88 = flash write, 5AB8 = flash CRC), storing
 *     the result in flashUpdateResult.
 * ============================================================================ */
void uptimeCounter(void)
{
    if (!(fwUpdateFlags & (1u << 1))) {
        /* Counter not running – reset */
        uptimeCounterLo = 0;
        uptimeCounterHi = 0;
        return;
    }

    /* Increment 32-bit counter */
    {
        uint32_t cnt = ((uint32_t)uptimeCounterHi << 16) | uptimeCounterLo;
        cnt++;
        uptimeCounterLo = (uint16_t)(cnt & 0xFFFFu);
        uptimeCounterHi = (uint16_t)(cnt >> 16);

        /* Check against threshold 0x001B_773F */
        if (cnt > 0x001B773Fu) {
            fwUpdateFlags &= ~(1u << 1);    /* clear "counter running" flag */

            /* Prepare flash write: page data W2=0, W1=0xFF */
            {
                uint8_t  page_data = 0x00u;
                uint8_t  cmd       = 0xFFu;

                /* Call flash write (0x5A88) with W1=cmd, W0=page_data */
                (void)page_data;
                (void)cmd;
                eepromCfgUpdate(0xFF, 0xFF, 0xFF);
                flashUpdateResult = eepromCfgRead();
                /* Result in W0 → flashUpdateResult */
                /* (result is propagated through W0 by calling convention) */
            }
        }
    }
}

/* ============================================================================
 * checkEnablePin — hardware enable-pin debounce
 * Assembly: 0x5108 – 0x512A
 *
 * Reads the ENABLE hardware pin status from flags_1262 bit14.
 * If bit14 is set (enable requested):
 *   - If LATE bit6 is already set → return (already enabled)
 *   - Otherwise increment enableDebounce; if enableDebounce > 29 (0x1D):
 *       cap at 30 (0x1E), set LATE bit6 (enable output)
 * If bit14 is clear (enable not requested):
 *   - Clear LATE bit6
 *   - Reset enableDebounce to 0
 * ============================================================================ */
void checkEnablePin(void)
{
    uint16_t f1262 = statusFlags2;

    if (f1262 & (1u << 14)) {
        /* Enable pin high */
        if (LATEbits.LATE6) {
            return;   /* already enabled */
        }
        /* Debounce: increment counter */
        {
            uint16_t cnt = enableDebounce + 1u;
            enableDebounce = cnt;
            if (cnt <= 0x001Du) {   /* <= 29 */
                return;
            }
            /* Debounce expired */
            enableDebounce = 0x001Eu;    /* cap at 30 */
            LATEbits.LATE6 = 1;     /* assert enable output */
        }
    } else {
        /* Enable pin low – de-assert output and reset counter */
        LATEbits.LATE6 = 0;
        enableDebounce = 0;
    }
}

/* ============================================================================
 * vinUvpCheck — Vin under-voltage protection check
 * Assembly: 0x512C – 0x5148
 *
 * Reads vcal_b (var_125E = 0x125E, calibrated Vout ch B) and compares it
 * against fixed thresholds to determine the Vin UV status.
 *
 * If vcal_b > 0x13E (318):  set auxFlags bit5 (Vin OK) → return
 * If vcal_b <= 0xFF (255):  clear auxFlags bit5 (Vin UV)
 *   - If LATF bit1 is set: clear bit3 of i2cStatusByte (fan enable)
 * If vcal_b in (255, 318]:  no change (hysteresis band)
 * ============================================================================ */
void vinUvpCheck(void)
{
    int16_t vcalb = vcal_b;   /* 0x125E */

    if (vcalb > 0x013E) {
        /* Vin above upper threshold – OK */
        auxFlags |= (1u << 5);
        return;
    }

    if (vcalb > 0x00FF) {
        /* Hysteresis band – no change */
        return;
    }

    /* Vin below lower threshold – under-voltage */
    auxFlags &= ~(1u << 5);

    /* If output relay is on (LATF bit1), disable fan */
    if (LATFbits.LATF1) {
        i2cStatusByte &= ~(1u << 3);   /* clear fan enable bit */
    }
}

/* ============================================================================
 * powerGoodCheck — power-good signal debounce and control
 * Assembly: 0x514A – 0x518A
 *
 * Manages the PWRGD output signal.
 *
 * If controlStatus bit0 is set (fault present):
 *   - If auxFlags bit7 is set → return immediately
 *   - Otherwise increment pgoodAssertCnt (PWRGD assert counter)
 *   - If pgoodAssertCnt > 0x1F3F (7999): set flags_1262 bit0 and auxFlags bit7,
 *     return (PWRGD asserted)
 *
 * If controlStatus bit0 is clear:
 *   - Clear flags_1262 bit0
 *   - Reset pgoodAssertCnt to 0
 *   - If auxFlags bit7 is set:
 *       If LATD bit3 is set (relay on) → return
 *       Otherwise increment pgoodDebounce (PWRGD de-assert counter)
 *       If pgoodDebounce > 999: clear auxFlags bit7
 *   - Else (bit7 clear):
 *       Reset pgoodDebounce to 0
 * ============================================================================ */
void powerGoodCheck(void)
{
    uint16_t f1e1c = auxFlags;
    uint16_t cf    = controlStatus;
    uint16_t fault = cf & (1u << 0);  /* bit0 = fault present */

    if (fault) {
        /* Fault active: assert PWRGD after debounce */
        if (f1e1c & (1u << 7)) {
            /* PWRGD already asserted */
            return;
        }
        /* Increment assertion counter */
        {
            uint16_t cnt = pgoodAssertCnt + 1u;
            pgoodAssertCnt = cnt;
            if (cnt <= 0x1F3Fu) {
                return;
            }
            /* Counter expired – assert PWRGD */
            statusFlags2 |= (1u << 0);   /* OVP latch? repurposed as PWRGD */
            auxFlags |= (1u << 7);
        }
    } else {
        /* No fault: de-assert PWRGD logic */
        statusFlags2 &= ~(1u << 0);
        pgoodAssertCnt = 0;

        if (f1e1c & (1u << 7)) {
            /* PWRGD currently asserted – wait for relay-off then debounce */
            if (LATDbits.LATD3) {
                /* Relay still on – stay */
                return;
            }
            /* Relay off: increment de-assert counter */
            {
                uint16_t cnt = pgoodDebounce + 1u;
                pgoodDebounce = cnt;
                if (cnt <= 0x03E7u) {
                    return;
                }
                /* Counter expired – de-assert PWRGD */
                auxFlags &= ~(1u << 7);
            }
        } else {
            /* PWRGD not asserted: reset counter */
            pgoodDebounce = 0;
        }
    }
}

/* ============================================================================
 *  — ACTIVE state handler
 * Assembly: 0x518C – 0x51FC
 *
 * Main operating state.  Called every control tick while systemState == 2.
 *
 * 1. Checks controlStatus for shutdown/fault conditions:
 *    - controlStatus == 4 (state forced): check bit5 of 0x1BEA (droop enable);
 *      if set go to fault initiation.
 *    - controlStatus == 0: no shutdown needed → skip to step 2.
 *    - controlStatus has bits 1 or 3 set (OVP/OCP): arm restart latches.
 *    - In all non-zero cases: perform PWM shutdown and transition to FAULT (3)
 *      or STATE5 (5).
 *
 * 2. Calls checkCurrentLimit (0x4BD4):
 *    - If current limiting active → set bit4 of 0x1265 (run indicator);
 *      if auxFlags bit1 is clear, also shut down and go to state 5.
 *    - If not limiting → clear bit4 of 0x1265.
 *
 * 3. If LATF bit1 is clear → shut down and go to state 6.
 *
 * 4. Decrement faultResetTimer (fault soft-reset timer) and delayTimer if non-zero.
 * ============================================================================ */
void state2Active(void)
{
    uint16_t cf = controlStatus;  /* controlStatus */

    /* --- Step 1: check for fault/shutdown condition --- */
    if (cf == 4u) {
        /* controlStatus == 4: state-forced shutdown */
        if (droopEnableFlags & (1u << 5)) {
            goto do_fault;
        }
        goto no_fault;
    }

    if (cf == 0u) {
        goto no_fault;
    }

    /* controlStatus != 0 and != 4 */
    if (cf & 0x000Au) {
        /* Bits 1 or 3 set (OVP/OCP) – arm restart latches */
        restartFlags |= (1u << 0);
        restartFlags |= (1u << 1);
    }

do_fault:
    /* Shutdown PWM and transition to FAULT */
    statusFlags = 0;
    runtimeFlags = 0;
    pwmRunRequest  |= (1u << 0);    /* PWM run request */
    pwmRunning  &= ~(1u << 0);   /* clear PWM running */
    setFaultState();
    Nop();
    pmbusAlertFlags |= (1u << 0);     /* PMBus alert */

    pwmOverrideEnable();              /* RCALL 0x4B50 */
    systemState = ST_FAULT;               /* → FAULT */

no_fault:
    /* --- Step 2: current limit check --- */
    {
        /* checkCurrentLimit returns non-zero byte if limiting active */
        uint8_t cl = (uint8_t)checkCurrentLimit();  /* RCALL 0x4BD4 */

        if (cl != 0) {
            /* Current limiting is active */
            protStatusByte |= (1u << 4);

            /* If auxFlags bit1 is clear → transition to state 5 */
            if (!(auxFlags & (1u << 1))) {
                pwmRunning  &= ~(1u << 0);
                setFaultState();
                Nop();
                pmbusAlertFlags  |= (1u << 0);
                pwmOverrideEnable();     /* RCALL 0x4B50 */
                systemState = ST_HOLDOFF;      /* → STATE5 */
                protStatusByte &= ~(1u << 4);
            }
        } else {
            /* Not limiting */
            protStatusByte &= ~(1u << 4);
        }
    }

    /* --- Step 3: LATF bit1 asserted forces transition to state 6 --- */
    if (LATFbits.LATF1) {
        pwmOverrideEnable();
        systemState = ST_RELAY;   /* → STATE6 */
    }

    /* --- Step 4: decrement soft timers --- */
    {
        uint16_t t1 = faultResetTimer;
        if (t1 != 0) {
            faultResetTimer = t1 - 1u;
        }
    }
    {
        int16_t t2 = delayTimer;
        if (t2 != 0) {
            delayTimer = t2 - 1;
        }
    }
}
