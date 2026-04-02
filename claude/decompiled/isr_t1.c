/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — isr_t1.c
 * Timer1 ISR (5kHz current control loop)
 *
 * Actual handler address: 0x37F6 (via ISR dispatcher 0x0D1C -> 0x1010 -> 0x37F6)
 * End: RETFIE @ 0x38A4
 *
 * This is the firmware's core "fast loop":
 *   - ADC sampling (voltage + current)
 *   - Droop compensation
 *   - OCP overcurrent foldback (PI)
 *   - Soft-start control
 *   - I2C/EEPROM handling
 *   - Fault protection
 *   - System monitoring (temperature, input voltage, fan, etc.)
 *
 * 27 subroutine calls, all completed within the 5kHz interrupt (200us period)
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* ---- Subroutine declarations (in call order) ---- */
extern void adcMiscSample(void);      /* 0x2A30 - auxiliary ADC channel sampling */
extern void adcVoltageSample(void);   /* 0x2AFC - output voltage dual-channel 64-point sampling */
extern void adcCurrentSample(void);   /* 0x2B70 - output current 64-point + 1024-point sampling */
extern void droopTrimCalc(void);      /* 0x2C32 - droop compensation calculation */
extern void pidControl(void);          /* 0x2C78 - PID control / droop adjustment */
extern void ocpFoldback(void);         /* 0x2CD6 - OCP overcurrent foldback (PI) */
extern void softStartRamp(void);      /* 0x2DC6 - soft-start voltage ramp */
extern void softStartRamp2(void);     /* 0x2DF6 - soft-start ramp phase 2 */
extern void modeCheck(void);           /* 0x32E0 - operating mode check */
extern void fanControl(void);              /* 0x3EBC - fan control */
extern void stateControlMachine(void);        /* 0x2E2C - LLC sub-state machine */
extern void protectionCheck(void);     /* 0x30F8 - protection check */
extern void pwmUpdate(void);           /* 0x33F4 - PWM update */
extern void stateInit(void);           /* 0x2E3C - state initialization */
extern void voltageRegulation(void);   /* 0x2E76 - voltage regulation */
extern void currentLimit(void);        /* 0x31CC - current limiting */
extern void faultHandler(void);        /* 0x3386 - fault handling */
extern void statusUpdate(void);        /* 0x3322 - status update */
extern void outputEnable(void);        /* 0x2EAC - output enable control */
extern void loadDetect(void);          /* 0x2F20 - load detection */
extern void watchdogService(void);     /* 0x37AA - watchdog service */
extern void vinCheck(void);            /* 0x3254 - input voltage check */
extern void portdSample(void);         /* 0x2F48 - PORTD sampling */
extern void i2cBusStuckHandler(void);     /* 0x2506 - I2C bus stuck detection */
extern void i2cTxAccumulate(void);        /* 0x24B2 - I2C transmit accumulation */
extern void flagProcess(void);         /* 0x2FA6 - flag processing */
extern void standbyCheck(void);        /* 0x3052 - standby check */
extern void eepromHandler(void);           /* 0x26D6 - EEPROM handling */
extern void tempSample(void);          /* 0x316E - temperature sampling */
extern void diagCheck(void);           /* 0x30AA - diagnostics check */
extern void uartRxTickService(void);      /* 0x583E - UART RX tick service */

/* ============================================================================
 * _T1Interrupt — Timer1 ISR, 5kHz
 *
 * Assembly 0x37F6 - 0x38A4:
 *
 *   37F6  PUSH RCOUNT
 *   37F8  MOV.D W0, [W15++]       ; save W0-W7
 *   37FA  MOV.D W2, [W15++]
 *   37FC  MOV.D W4, [W15++]
 *   37FE  MOV.D W6, [W15++]
 *
 *   ; --- 32-bit tick counter, wraps at 63 ---
 *   3800  MOV 0x1252, W2           ; tick_lo
 *   3802  MOV 0x1254, W3           ; tick_hi
 *   3804  ADD W2, #0x1, W0         ; tick++
 *   3806  ADDC W3, #0x0, W1
 *   3808  MOV W0, 0x1252
 *   380A  MOV W1, 0x1254
 *   380C  MOV #0x3F, W3            ; 63
 *   380E  SUB W0, W3, [W15]
 *   3810  SUBB W1, #0x0, [W15]
 *   3812  BRA LEU, 0x3818          ; if tick <= 63, skip
 *   3814  CLR 0x1252               ; else reset tick = 0
 *   3816  CLR 0x1254
 *
 *   ; --- 27 subroutine calls ---
 *   3818  RCALL 0x2A30             ; adcMiscSample
 *   381A  RCALL 0x2AFC             ; adcVoltageSample
 *   381C  RCALL 0x2B70             ; adcCurrentSample
 *   381E  RCALL 0x2C32             ; droopTrimCalc
 *
 *   ; --- Over-temperature detection ADCBUF14 ---
 *   3820  MOV ADCBUF14, W1
 *   3822  MOV #0xF8, W0            ; threshold = 248
 *   3824  SUB W1, W0, [W15]
 *   3826  BRA GTU, 0x382C          ; if ADCBUF14 > 248
 *   3828  BSET 0x1266, #7          ;   set OT flag
 *   382A  BRA 0x382E
 *   382C  BCLR 0x1266, #7          ;   clear OT flag
 *
 *   382E  RCALL 0x2C78             ; pidControl
 *   3830  RCALL 0x2CD6             ; ocpFoldback
 *   3832  RCALL 0x2DC6             ; softStartRamp
 *   3834  RCALL 0x2DF6             ; softStartRamp2
 *   3836  RCALL 0x32E0             ; modeCheck
 *   3838  CALL  0x3EBC             ; fanControl
 *   383C  RCALL 0x2E2C             ; stateControlMachine
 *   3840  CALL  0x2506             ; 
 *   3844  RCALL 0x30F8             ; protectionCheck
 *
 *   ; --- PWM output gate check ---
 *   3846  MOV.B 0x423, WREG        ; IOCON1L
 *   3848  BTST.Z W0, #1            ; test OVRENH (bit9)
 *   384A  BRA Z, 0x385C
 *   384C  MOV.B 0x423, WREG
 *   384E  BTST.Z W0, #0            ; test OVRENL (bit8)
 *   3850  BRA Z, 0x385C
 *   3852  MOV.B 0x443, WREG        ; IOCON2L
 *   3854  BTST.Z W0, #1
 *   3856  BRA Z, 0x385C
 *   3858  MOV.B 0x443, WREG
 *   385A  BTST.Z W0, #0
 *   385C  BRA NZ, 0x385E           ; skip if any PWM not enabled
 *   385E  RCALL 0x33F4             ; pwmUpdate
 *
 *   ; --- Delay timer ---
 *   3860  CP0 0x1D40               ; if delay_timer == 0
 *   3862  BRA NZ, 0x3868
 *   3864  RCALL 0x2E3C             ;   stateInit()
 *   3866  MOV #0x30F, W0           ;   reload = 783
 *   3868  MOV W0, 0x1D36
 *
 *   386A  RCALL 0x2E76             ; voltageRegulation
 *   386C  RCALL 0x31CC             ; currentLimit
 *   386E  RCALL 0x3386             ; faultHandler
 *   3870  RCALL 0x3322             ; statusUpdate
 *   3872  RCALL 0x2EAC             ; outputEnable
 *   3874  RCALL 0x2F20             ; loadDetect
 *
 *   ; --- Countdown timer 0x1D20 ---
 *   3876  MOV 0x1D20, W0
 *   3878  CP0 W0
 *   387A  BRA Z, 0x387E
 *   387C  DEC W0, W0               ; countdown--
 *   387E  MOV W0, 0x1D20
 *
 *   3880  RCALL 0x37AA             ; watchdogService
 *   3882  RCALL 0x3254             ; vinCheck
 *   3884  RCALL 0x2F48             ; portdSample
 *   3886  CALL  0x24B2             ; 
 *   388A  RCALL 0x2FA6             ; flagProcess
 *   388C  RCALL 0x3052             ; standbyCheck
 *   388E  CALL  0x26D6             ; eepromHandler
 *   3892  RCALL 0x316E             ; tempSample
 *   3894  RCALL 0x30AA             ; diagCheck
 *   3896  CALL  0x583E             ; 
 *
 *   ; --- Clear interrupt, restore context ---
 *   3898  BCLR IFS0, #3            ; IFS0bits.T1IF = 0
 *   389A  MOV.D [--W15], W6       ; restore W0-W7
 *   389C  MOV.D [--W15], W4
 *   389E  MOV.D [--W15], W2
 *   38A0  MOV.D [--W15], W0
 *   38A2  POP RCOUNT
 *   38A4  RETFIE
 * ============================================================================ */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    /* 32-bit tick counter (sub-divider, wraps at 0-63) */
    tick_counter++;
    if (tick_counter > 63)
        tick_counter = 0;

    /* ---- Main ADC sampling chain ---- */
    adcMiscSample();            /* 0x2A30 */
    adcVoltageSample();         /* 0x2AFC */
    adcCurrentSample();         /* 0x2B70 */
    droopTrimCalc();            /* 0x2C32 */

    /* ---- Over-temperature detection (ADCBUF14, threshold 248) ---- */
    if (ADCBUF14 <= 0xF8)
        thermalFlags |= (1u << 7);     /* set OT flag */
    else
        thermalFlags &= ~(1u << 7);    /* clear OT flag */

    /* ---- Control loop core ---- */
    pidControl();                /* 0x2C78 */
    ocpFoldback();               /* 0x2CD6 */
    softStartRamp();            /* 0x2DC6 */
    softStartRamp2();           /* 0x2DF6 */
    modeCheck();                 /* 0x32E0 */
    fanControl();                    /* 0x3EBC */

    /* ---- State machine & I2C ---- */
    stateControlMachine();              /* 0x2E2C */
#ifndef SIMULATION_MODE
    i2cBusStuckHandler();               /* 0x2506 */
#endif
    protectionCheck();                  /* 0x30F8 */

    /* ---- PWM update gate (0x3844..0x385C) ----
     * Skip pwmUpdate only if all four OVREN bits are set (all outputs overridden).
     * Assembly reads IOCON1H (0x423) and IOCON2H (0x443) byte-addressable high bytes.
     */
    if (!(IOCON1bits.OVRENH && IOCON1bits.OVRENL &&
          IOCON2bits.OVRENH && IOCON2bits.OVRENL)) {
        pwmUpdate();                 /* 0x33F4 */
    }

    /* ---- Delay-triggered state initialization ---- */
    if (delayTimer == 0) {
        stateInit();             /* 0x2E3C */
        ocpCurrentRef = 0x30F;            /* reload: 783 */
    }

    /* ---- Regulation and protection ---- */
    voltageRegulation();         /* 0x2E76 */
    currentLimit();              /* 0x31CC */
    faultHandler();              /* 0x3386 */
    statusUpdate();              /* 0x3322 */
    outputEnable();              /* 0x2EAC */
    loadDetect();                /* 0x2F20 */

    /* ---- Countdown timer ---- */
    if (countdownTimer != 0)
        countdownTimer--;

    /* ---- System management ---- */
    watchdogService();                  /* 0x37AA */
    vinCheck();                         /* 0x3254 */
    portdSample();                      /* 0x2F48 */
#ifndef SIMULATION_MODE
    i2cTxAccumulate();                  /* 0x24B2 */
#endif
    flagProcess();                      /* 0x2FA6 */
    standbyCheck();              /* 0x3052 */
#ifndef SIMULATION_MODE
    eepromHandler();                 /* 0x26D6 */
#endif
    tempSample();                /* 0x316E */
    diagCheck();                 /* 0x30AA */
#ifndef SIMULATION_MODE
    uartRxTickService();             /* 0x583E */
#endif

    IFS0bits.T1IF = 0;               /* clear T1 interrupt flag */
}
