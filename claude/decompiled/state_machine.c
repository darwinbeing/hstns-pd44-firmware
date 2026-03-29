/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — state_machine.c
 * Main state machine + I2C service + startup control
 *
 * Functions:
 *   i2cService()         — 0x50C2
 *   startupControl()        — 0x5522
 *   mainStateDispatch()  — 0x51FE
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* ---- External functions ---- */
extern void i2cProcessCommand(void);       /* 0x5AE4 */
extern uint16_t computeTargetOc2rs(void);  /* 0x54C2 */

/* ---- State handler functions ---- */
extern void state0Idle(void);               /* 0x4C04 */
extern void state1Startup(void);            /* 0x4C72 */
extern void state2Active(void);             /* 0x518C */
extern void state3Fault(void);              /* 0x4CA6 */
extern void state5Handler(void);            /* 0x4D3A */
extern void state6Handler(void);            /* 0x4D86 */

/* ---- Common service functions ---- */
extern void monitorVin(void);               /* 0x4DBC */
extern void monitorVout(void);              /* 0x4DEA */
extern void monitorIout(void);              /* 0x4EFC */
extern void monitorTemperature(void);       /* 0x4F92 */
extern void updateStatusLeds(void);         /* 0x571E */
extern void t1IsrI2cBody(void);             /* 0x29FA */
extern void checkFanControl(void);          /* 0x4FBC */
extern void checkStandbyRail(void);         /* 0x4FEE */
extern void checkEnablePin(void);           /* 0x5108 */
extern void updatePmbusStatus(void);        /* 0x546C */
extern void checkOtp(void);                 /* 0x4D94 */
extern void flashCalibrationLoad(void);     /* 0x3B5E */
extern void voltageErrorTracking(void);     /* 0x40C2 */
extern void currentRegulation(void);        /* 0x3F0E */
extern void flashReadbackHandler(void);     /* 0x42C8 */
extern void uptimeCounter(void);            /* 0x50D4 */
extern void vinUvpCheck(void);              /* 0x512C */
extern void powerGoodCheck(void);           /* 0x514A */

/* ============================================================================
 * i2cService() — 0x50C2
 *
 * Assembly:
 *   50C2  BTST 0x1E1A, #0          ; test i2c_request_pending
 *   50C4  BRA Z, 0x50D2            ; no request, return
 *   50C6  MOV.B I2C2STAT, WREG     ; read I2C2 status
 *   50C8  BTST.Z W0, #4            ; test P bit (Stop condition)
 *   50CA  BRA Z, 0x50D2            ; Stop not detected, return
 *   50CC  BCLR 0x1E1A, #0          ; clear pending flag
 *   50CE  CALL 0x5AE4              ; i2cProcessCommand()
 *   50D2  RETURN
 * ============================================================================ */
void i2cService(void)
{
    if (!(systemFlags & 0x0001))
        return;

    if (!(I2C2STAT & (1u << 4)))     /* P bit: Stop condition detected */
        return;

    systemFlags &= ~0x0001;
    i2cProcessCommand();           /* 0x5AE4 */
}

/* ============================================================================
 * startupControl() — 0x5522
 *
 * Two-phase startup control:
 *   1) Vout <= 199: soft-start ramp (OC2RS += 20, capped at 400)
 *   2) Vout > 199:  regulation mode (compute target OC2RS, current limiting)
 *
 * Assembly:
 *   5522  BTST 0x1E1D, #0          ; startup tick?
 *   5524  BRA Z, 0x5568            ; no tick, return
 *   5526  BCLR 0x1E1D, #0          ; clear tick flag
 *   5528  MOV 0x1BD4, W1           ; outputVoltage
 *   552A  MOV #0xC7, W0            ; threshold = 199
 *   552C  SUB W1, W0, [W15]
 *   552E  BRA LEU, 0x5558          ; Vout <= 199: soft-start
 *   5530  RCALL 0x54C2             ; computeTargetOc2rs()
 *   5532  MOV W0, 0x1E54           ; target_oc2rs = result
 *   5534  MOV 0x1E3C, W3           ; oc2rs_limit
 *   5536-553A                      ; final = max(oc2rs_limit, computed)
 *   553C  BTST 0x1E18, #0          ; current limiting active?
 *   553E  BRA Z, 0x5554
 *   5540  MOV 0x1BB2, W0           ; outputCurrent
 *   5542  MOV #0x7D1, W2           ; 2001
 *   5544  MUL.UU W0, W2, W0        ; product = iout * 2001
 *   5546-554C                      ; scaled = product >> 8
 *   554E-5552                      ; final = max(final, scaled)
 *   5554  MOV W3, OC2RS            ; apply
 *   5556  RETURN
 *   5558  MOV #0x14, W0            ; soft-start: OC2RS += 20
 *   555A  ADD OC2RS
 *   555C  MOV OC2RS, W1
 *   555E  MOV #0x18F, W0           ; upper limit 399
 *   5560  SUB W1, W0, [W15]
 *   5562  BRA LEU, 0x5568          ; OC2RS <= 399: continue
 *   5564  INC W0, W0               ; OC2RS = 400
 *   5566  MOV W0, OC2RS
 *   5568  RETURN
 * ============================================================================ */
void startupControl(void)
{
    if (!(startupFlags & 0x0001))
        return;

    startupFlags &= ~0x0001;         /* clear tick flag */

    uint16_t vout = outputVoltage;       /* 0x1BD4 */

    if (vout <= 199) {
        /* ---- Soft-start: OC2RS ramp ---- */
        OC2RS += 20;
        if (OC2RS > 399)
            OC2RS = 400;
        return;
    }

    /* ---- Regulation mode ---- */
    uint16_t computed = computeTargetOc2rs();  /* 0x54C2 */
    oc2rsTarget = computed;

    uint16_t final_val = oc2rsUpperLimit;
    if (final_val < computed)
        final_val = computed;       /* max(limit, computed) */

    if (currentLimitFlags & 0x0001) {
        /* Current limiting active: current foldback */
        uint32_t product = (uint32_t)outputCurrent * 2001u;  /* * 0x7D1 */
        uint16_t scaled = (uint16_t)(product >> 8);
        if (final_val < scaled)
            final_val = scaled;
    }

    OC2RS = final_val;
}

/* ============================================================================
 * mainStateDispatch() — 0x51FE
 *
 * Timer interrupt flag (IFS0 bit3) driven main state machine:
 *   - 6-state dispatch (IDLE/STARTUP/ACTIVE/FAULT/5/6)
 *   - 15 common service functions (monitoring/regulation/protection/comms)
 *
 * Assembly:
 *   51FE  MOV.B IFS0, WREG         ; read IFS0
 *   5200  BTST.Z W0, #3            ; test T1IF (Timer4 flag? actually T4IF at bit3)
 *   5202  BRA Z, 0x5286            ; not set, return
 *   5204  MOV 0x1E22, W0           ; systemState
 *   5206-5222                      ; switch(systemState) dispatch
 *   5224  RCALL 0x4C04             ; state0: idle
 *   5228  RCALL 0x4C72             ; state1: startup
 *   522C  RCALL 0x518C             ; state2: active
 *   5230  RCALL 0x4CA6             ; state3: fault
 *   5234  RCALL 0x4D3A             ; state5
 *   5238  RCALL 0x4D86             ; state6
 *   523A  RCALL 0x4DBC             ; monitorVin
 *   523C  RCALL 0x4DEA             ; monitorVout
 *   523E  RCALL 0x4EFC             ; monitorIout
 *   5240  RCALL 0x4F92             ; monitorTemperature
 *   5242-5256                      ; controlStatus bit1 computation
 *   5258  CALL 0x571E              ; updateStatusLeds
 *   525C  CALL 0x29FA              ; adcProcess
 *   5260  RCALL 0x4FBC             ; checkFanControl
 *   5262  RCALL 0x4FEE             ; checkStandbyRail
 *   5264  RCALL 0x5108             ; checkEnablePin
 *   5266  CALL 0x546C              ; updatePmbusStatus
 *   526A  RCALL 0x4D94             ; checkOtp
 *   526C  CALL 0x3B5E              ; pidControl
 *   5270  CALL 0x40C2              ; voltageRegulation
 *   5274  CALL 0x3F0E              ; currentRegulation
 *   5278  CALL 0x42C8              ; protectionCheck
 *   527C  RCALL 0x50D4             ; uptimeCounter
 *   527E  RCALL 0x512C             ; vinUvpCheck
 *   5280  RCALL 0x514A             ; powerGoodCheck
 *   5282  CLRWDT                   ; clear watchdog
 *   5284  BCLR 0x87, #3            ; clear IFS1bits.T4IF (0x87 = IFS1 high byte, bit3 = T4IF)
 *   5286  RETURN
 * ============================================================================ */
void mainStateDispatch(void)
{
    /* Check T4 timer trigger flag (BCLR/BTST 0x87 bit3 = IFS1 high byte bit3 = T4IF) */
    if (!IFS1bits.T4IF)
        return;

    /* ---- State dispatch ---- */
    switch (systemState) {
        case ST_IDLE:    state0Idle();      break;  /* 0x4C04 */
        case ST_STARTUP: state1Startup();   break;  /* 0x4C72 */
        case ST_ACTIVE:  state2Active();    break;  /* 0x518C */
        case ST_FAULT:   state3Fault();     break;  /* 0x4CA6 */
        case ST_HOLDOFF: state5Handler();   break;  /* 0x4D3A */
        case ST_RELAY:   state6Handler();   break;  /* 0x4D86 */
        default: break;
    }

    /* ---- Common monitoring ---- */
    monitorVin();                   /* 0x4DBC */
    monitorVout();                  /* 0x4DEA */
    monitorIout();                  /* 0x4EFC */
    monitorTemperature();           /* 0x4F92 */

    /* Compute "fault present" bit -> controlStatus bit1 */
    uint16_t fault_bit = ((protectionStatus >> 14) & 1) | (systemFlags >> 15);
    controlStatus = (controlStatus & ~0x0002) | ((fault_bit << 1) & 0x0002);

    /* ---- Common services ---- */
#ifndef SIMULATION_MODE
    updateStatusLeds();             /* 0x571E */
    t1IsrI2cBody();                 /* 0x29FA */
    checkFanControl();              /* 0x4FBC */
    checkStandbyRail();             /* 0x4FEE */
    checkEnablePin();               /* 0x5108 */
    updatePmbusStatus();            /* 0x546C */
    checkOtp();                     /* 0x4D94 */
#ifndef SIMULATION_MODE
    flashCalibrationLoad();         /* 0x3B5E */
#endif
    voltageErrorTracking();         /* 0x40C2 */
    currentRegulation();            /* 0x3F0E */
#ifndef SIMULATION_MODE
    flashReadbackHandler();         /* 0x42C8 */
#endif
    uptimeCounter();                /* 0x50D4 */
    vinUvpCheck();                 /* 0x512C */
    powerGoodCheck();              /* 0x514A */
#else
    uptimeCounter();                /* keep basic timebase alive */
    powerGoodCheck();               /* keep power-good evaluation */
#endif

    ClrWdt();
    IFS1bits.T4IF = 0;              /* clear T4 trigger flag */
}
