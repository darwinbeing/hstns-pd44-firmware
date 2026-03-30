/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — isr.c
 * ISR dispatcher architecture + actual interrupt handler functions
 *
 * Firmware uses a dual-segment architecture:
 *   Segment 1: 0x0000-0x5BFE (main code)
 *   Segment 2: 0x5C00-0xA7E2 (mirror/backup, offset 0x4C00)
 *
 * ISR dispatch mechanism:
 *   IVT -> stub (0x0D00-0x0DE4) -> check fw_mode (0x27FE)
 *     mode==2 -> jump table (0x1000) -> segment 1 actual handler
 *     mode==3 -> segment 2 mirror handler
 *     default -> degraded/initial handler
 *
 * Jump table (0x1000):
 *   0x100C -> GOTO 0x5B38  (PSEM handler)
 *   0x1010 -> GOTO 0x37F6  (T1 handler, 5kHz current loop)
 *   0x1014 -> GOTO 0x456E  (T2 handler, voltage loop/frequency control)
 *   0x1024 -> GOTO 0x29B0  (SI2C2 handler)
 *   others -> GOTO 0x5BE2  (BREAK+RESET, not implemented)
 *
 * Note: one timer vector stub is empty (0x0DE0, direct RETURN).
 *       The voltage loop/frequency control path is at 0x0D40 -> 0x1014 -> 0x456E.
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* ---- Segment 1 actual handler declarations ---- */
extern void t1HandlerSeg1(void);          /* 0x37F6 */
extern void t3HandlerSeg1(void);          /* 0x456E */
extern void psemHandlerSeg1(void);        /* 0x5B38 */
extern void si2c2HandlerSeg1(void);       /* 0x29B0 */

/* ============================================================================
 * ISR classification:
 *
 * 1. Empty ISR (direct RETURN, 0x060000):
 *    - 0x0D00 INT0   External interrupt 0
 *    - 0x0D02 ADCP2  ADC pair 2
 *    - 0x0D88 AC1    Analog comparator 1
 *    - 0x0DAE MI2C2  I2C2 master
 *    - 0x0DB0 U1RX   UART1 receive
 *    - 0x0DB2 AC2    Analog comparator 2
 *    - 0x0DB4 IC2    Input capture 2
 *    - 0x0DB6 NVM    NVM write complete
 *    - 0x0DB8 INT2   External interrupt 2
 *    - 0x0DBA U1TX   UART1 transmit
 *    - 0x0DBC ADCP0  ADC pair 0
 *    - 0x0DBE ADCP4  ADC pair 4
 *    - 0x0DC0 ADCP5  ADC pair 5
 *    - 0x0DDA IC1    Input capture 1
 *    - 0x0DDC OC2    Output compare 2
 *    - 0x0DDE OC1    Output compare 1
 *    - 0x0DE0 T2     Timer2 (empty!)
 *    - 0x0DE2 CN     Change notification
 *    - 0x0DE4 IRQ103 (to be confirmed)
 *
 * 2. Mode-dispatched ISR (checks fw_mode to dispatch):
 *    - 0x0D04 PSEM   PWM special event -> mode2: 0x5B38, else: 0x5C0C
 *    - 0x0D1C T1     Timer1            -> mode2: 0x37F6, mode3: 0x5C10, else: 0x0AC0
 *    - 0x0D40 T2     Timer2            -> mode2: 0x456E, else: 0x5C14
 *    - 0x0D58 SPI1E  SPI1 error        -> mode2: 0x5BE2(BREAK), else: 0x5C18
 *    - 0x0D70 MI2C1  I2C1 master       -> mode2: 0x5BE2(BREAK), else: 0x5C1C
 *    - 0x0D8A SI2C2  I2C2 slave        -> mode2: 0x29B0, mode3: 0x5C24, else: 0x0526
 *    - 0x0DC2 ADCP1  ADC pair 1        -> mode2: 0x5BE2(BREAK), else: 0x5C50
 * ============================================================================ */

/* ============================================================================
 * ISR dispatcher pseudocode (T1 as example, 0x0D1C):
 *
 * Assembly:
 *   0D1C  MOV W0, [W15++]         ; save W0 to stack
 *   0D1E  MOV 0x27FE, W0          ; read fw_mode
 *   0D20  SUB W0, #0x2, [W15]     ; compare mode == 2?
 *   0D22  BRA NZ, 0xD2C           ; not equal, continue checking
 *   0D24  MOV [--W15], W0         ; restore W0
 *   0D26  GOTO 0x1010             ; -> jump table -> GOTO 0x37F6
 *   ...
 *   0D2C  SUB W0, #0x3, [W15]     ; compare mode == 3?
 *   0D2E  BRA NZ, 0xD38           ; not equal, take default
 *   0D30  MOV [--W15], W0
 *   0D32  GOTO 0x5C10             ; -> segment 2 handler
 *   ...
 *   0D38  MOV [--W15], W0
 *   0D3A  GOTO 0x0AC0             ; -> degraded handler
 * ============================================================================ */

/* ============================================================================
 * PSEM ISR dispatcher — 0x0D04
 *
 * Assembly:
 *   0D04  MOV W0, [W15++]
 *   0D06  MOV 0x27FE, W0
 *   0D08  SUB W0, #0x2, [W15]
 *   0D0A  BRA NZ, 0xD14
 *   0D0C  MOV [--W15], W0
 *   0D0E  GOTO 0x100C             ; -> GOTO 0x5B38
 *   0D12  RETURN
 *   0D14  MOV [--W15], W0
 *   0D16  GOTO 0x5C0C             ; -> segment 2 PSEM handler
 * ============================================================================ */

/* ============================================================================
 * PSEM actual handler — 0x5B38 (mode 2)
 *
 * PHASE3 ramp-down clamp:
 *   If (phase3_target - ptper) > 10, apply phase3_target -= 10 (gradual ramp)
 *   Else clamp PHASE3 = ptper (reached target)
 *
 * Assembly:
 *   5B38  MOV.D W0, [W15++]       ; save W0-W1
 *   5B3A  MOV   W2, [W15++]       ; save W2
 *   5B3C  MOV   0x1E56, W2        ; W2 = phase3_target
 *   5B3E  MOV   0x1D72, W1        ; W1 = ptper
 *   5B40  SUB   W2, W1, W0        ; W0 = phase3_target - ptper
 *   5B42  SUB   W0, #0xA, [W15]   ; compare diff vs 10
 *   5B44  BRA   LE, 0x5B4E        ; if diff <= 10, clamp
 *   5B46  SUB   W2, #0xA, W0      ; W0 = phase3_target - 10
 *   5B48  MOV   W0, 0x1E56        ; phase3_target -= 10
 *   5B4A  MOV   W0, PHASE3        ; PHASE3 = phase3_target - 10
 *   5B4C  BRA   0x5B52            ; skip clamp
 *   5B4E  MOV   W1, PHASE3        ; PHASE3 = ptper (clamped)
 *   5B50  MOV   W1, 0x1E56        ; phase3_target = ptper
 *   5B52  MOV   0x1D74, W0        ; ptperCommand
 *   5B54  MOV   W0, PTPER         ; write PTPER
 *   5B56  MOV   0x1D70, W0        ; pdc1
 *   5B58  MOV   W0, PDC1          ; write PDC1
 *   5B5A  MOV   0x1D6E, W0        ; pdc2
 *   5B5C  MOV   W0, PDC2          ; write PDC2
 *   5B5E  MOV   0x1D6C, W0        ; pdc3
 *   5B60  MOV   W0, PDC3          ; write PDC3
 *   5B62  MOV   0x1D6A, W0        ; dtr3_shadow
 *   5B64  MOV   W0, DTR3          ; write DTR3
 *   5B66  MOV   W0, ALTDTR3       ; write ALTDTR3 (same value as DTR3)
 *   5B68  BCLR  0x401, #3         ; PTCONbits.SEIEN = 0
 *   5B6A  BCLR  0x9B, #1          ; IEC3bits.PSEMIE = 0
 *   5B6C  BCLR  0x8B, #1          ; IFS3bits.PSEMIF = 0
 *   5B6E  MOV   [--W15], W2       ; restore W2
 *   5B70  MOV.D [--W15], W0       ; restore W0-W1
 *   5B72  RETFIE
 * ============================================================================ */
void __attribute__((interrupt, no_auto_psv)) _PWMSpEventMatchInterrupt(void)
{
    /* ---- PHASE3 ramp-down clamp ----
     * Gradually reduce PHASE3 toward ptper in steps of 10.
     * This prevents abrupt phase-shift changes that could cause
     * current spikes in the LLC converter.
     */
    int16_t diff = phase3_target - ptper;
    if (diff > 10) {
        /* Still ramping: decrement by 10 */
        phase3_target -= 10;
        PHASE3 = phase3_target;
    } else {
        /* Reached target: clamp to ptper */
        PHASE3 = ptper;
        phase3_target = ptper;
    }

    /* ---- Update remaining PWM hardware from shadow registers ---- */
    PTPER   = ptperCommand;   /* 0x1D74 -> PTPER */
    PDC1    = pdc1;             /* 0x1D70 -> PDC1 */
    PDC2    = pdc2;             /* 0x1D6E -> PDC2 */
    PDC3    = pdc3;             /* 0x1D6C -> PDC3 */
    DTR3    = dtr3_shadow;      /* 0x1D6A -> DTR3 */
    ALTDTR3 = dtr3_shadow;      /* 0x1D6A -> ALTDTR3 (same value as DTR3) */

    /* ---- Disable PSEM interrupt (one-shot update) ---- */
    PTCONbits.SEIEN  = 0;      /* disable special event interrupt trigger */
    IEC3bits.PSEMIE  = 0;      /* disable PSEM interrupt */
    IFS3bits.PSEMIF  = 0;      /* clear PSEM interrupt flag */
}

/* ============================================================================
 * SI2C2 ISR dispatcher — 0x0D8A
 *
 * Three-way dispatch:
 *   mode == 2 -> GOTO 0x1024 -> GOTO 0x29B0 (slave protocol handling)
 *   mode == 3 -> GOTO 0x5C24 (segment 2 slave handling)
 *   default   -> GOTO 0x0526 (degraded/initialization slave)
 *
 * 0x29B0 handler: full I2C2 slave protocol handling
 *   (see i2c_slave.c, pending decompilation)
 *
 * Full implementation in i2c_protocol.c
 * ============================================================================ */
