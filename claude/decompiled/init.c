/* ============================================================================
 * HSTNS-PD44 Peripheral Initialization Functions
 * Decompiled from dsPIC33FJ64GS606 firmware
 *
 * Power supply: HP 800W LLC (HSTNS-PD44)
 * MCU: dsPIC33FJ64GS606
 *
 * Notes:
 *   - All register values shown as written by firmware
 *   - SFR names follow dsPIC33FJ64GS606 datasheet
 *   - Byte-addressed SFRs written via MOV.B / BSET.B instructions
 *   - Addresses in parentheses are raw SFR addresses where the symbolic
 *     name is used for clarity
 * ============================================================================ */

#include <xc.h>
#include <libpic30.h>  /* __delay32, Nop */
#include "variables.h"
#define NOP() __builtin_nop()

/* ============================================================================
 * initClock  (firmware address 0x59CE)
 *
 * Configures the primary oscillator PLL and auxiliary clock:
 *   - External 16 MHz crystal + PLL mode
 *   - PLLFBD = 0x30 (48) -> M = 50 (N1=2, N2=4 -> Fosc = 16MHz * 50 / 8 = 100 MHz, Fcy = 50 MHz)
 *   - CLKDIV: PLLPRE=0 (N1=2), PLLPOST=1 (N2=4), DOZE disabled
 *   - Oscillator switch sequence to FRC+PLL via OSCCON unlock
 *   - Waits for PLL lock (LOCK bit in OSCCON)
 *   - ACLKCON = 0x2740: auxiliary clock from primary PLL, divide-by-1
 *   - Waits for auxiliary clock ready (bit 6 of ACLKCON)
 * ============================================================================ */
void initClock(void)
{
    /* ---- PLL configuration ----
     * Fosc = Fin * M / (N1 * N2) = 16 MHz * 50 / (2 * 4) = 100 MHz
     * Fcy = Fosc / 2 = 50 MHz                                             */
    PLLFBD = 48;                        /* M = PLLFBD + 2 = 50               */
    CLKDIVbits.PLLPOST = 0;            /* N1 = 2                            */
    CLKDIVbits.PLLPRE  = 2;            /* N2 = PLLPRE + 2 = 4              */

    /* ---- Oscillator switch to External Crystal + PLL ----              */
    __builtin_write_OSCCONH(0x03);      /* NOSC = 011 = External w/ PLL      */

    /* ---- OSCTUN / OSCCONL unlock: write 0x46 then 0x57 then 0x01 to 0x742
     * Sets OSWEN=1 in OSCCONL to initiate the clock switch.               */
    __builtin_write_OSCCONL(0x01);      /* OSWEN = 1 -> start oscillator switch */

#ifndef SIMULATION_MODE
    /* ---- Wait for oscillator switch to complete ----
     * Poll OSCCON[6:4] (COSC field) until it matches NOSC (0x03 << 4 = 0x30)
     * Bits [6:4] of OSCCONL are the COSC field.                           */
    while (OSCCONbits.COSC != 0b011);   /* wait until External Crystal w/ PLL  */

    /* ---- Wait for PLL to lock ----
     * LOCK = bit 5 of OSCCON (OSCCONL).                                   */
    while (!OSCCONbits.LOCK);           /* wait for PLL lock                   */

    /* ---- Auxiliary clock: FRC * 16 / 1 ≈ 120 MHz for PWM & ADC ---- */
    ACLKCONbits.ASRCSEL  = 0;          /* External Oscillator for APLL       */
    ACLKCONbits.FRCSEL   = 1;          /* FRC clock for auxiliary PLL         */
    ACLKCONbits.SELACLK  = 1;          /* Aux oscillator provides PWM/ADC clk*/
    ACLKCONbits.APSTSCLR = 7;          /* Divide auxiliary clock by 1         */
    ACLKCONbits.ENAPLL   = 1;          /* Enable auxiliary PLL                */

    while (!ACLKCONbits.APLLCK);       /* wait for auxiliary PLL lock         */
#else
    ACLKCONbits.ASRCSEL  = 0;
    ACLKCONbits.FRCSEL   = 1;
    ACLKCONbits.SELACLK  = 1;
    ACLKCONbits.APSTSCLR = 7;
    ACLKCONbits.ENAPLL   = 1;
#endif
}

/* ============================================================================
 * initIOPorts  (firmware address 0x59A2)
 *
 * Configures GPIO direction (TRIS), initial output levels (LAT), and
 * open-drain (OD) control registers for all ports used by this design.
 *
 * Convention: TRIS bit = 1 -> input, 0 -> output
 *             LAT  bit = 1 -> drive high, 0 -> drive low
 * ============================================================================ */
void initIOPorts(void)
{
    /* ---- Port B ----
     * BSET TRISB, #7: set RB7 as input (likely fan tach or digital sense) */
    TRISBbits.TRISB7 = 1;               /* RB7 = input                         */

    /* ---- Port C ----
     * TRISC = 0xF000: RC15..RC12 are inputs, RC11..RC0 are outputs
     * LATC  = 0x0000: all Port C outputs driven low initially             */
    TRISC = 0xF000;
    LATC  = 0x0000;

    /* ---- Port D ----
     * TRISD = 0x0FC3: inputs on RD9..RD6, RD1, RD0; outputs on RD5..RD2
     *   0x0FC3 = 0000 1111 1100 0011
     *   Outputs: RD5 (RELAY/EN), RD4..RD2 (LED/control lines)
     * LATD  = 0x0008: RD3 driven high (e.g. LED or gate signal default)  */
    TRISD = 0x0FC3;
    LATD  = 0x0008;

    /* ODCD = 0x8001: open-drain on RD15 and RD0
     * RD0 open-drain likely used for I2C or wired-OR bus signal          */
    ODCD  = 0x8001;

    /* ---- Port E ----
     * TRISE = 0x0000: all Port E pins are outputs
     * LATE  = 0x0008: RE3 driven high initially                          */
    TRISE = 0x0000;
    LATE  = 0x0008;

    /* ---- Port F ----
     * TRISF = 0x003C: RF5..RF2 are inputs (e.g. SPI/UART remappable pins)
     *         RF1, RF0 are outputs
     * LATF  = 0x0040: RF6 driven high (was 0x3C + 4 = 0x40, CS line high) */
    TRISF = 0x003C;
    LATF  = 0x0040;                     /* RF6 = 1 (SPI CS deasserted)         */

    /* ---- Port G ----
     * TRISG = 0x01CC: RG8, RG7, RG6, RG3, RG2 are inputs
     *   0x01CC = 0000 0001 1100 1100
     * LATG  = 0x0200: RG9 driven high initially                          */
    TRISG = 0x01CC;
    LATG  = 0x0200;
}

/* ============================================================================
 * initTIMER  (firmware address 0x5AF6)
 *
 * Configures Timer1, Timer2, and Timer4:
 *
 *   Timer1: 10 ms tick interrupt
 *     - Clock: Fcy/8 prescaler -> 46 MHz / 8 = 5.75 MHz
 *     - PR1 = 0x2710 (10000) -> period = 10000 / 5.75 MHz = ~1.739 ms
 *       (actual period depends on prescaler setting written to T1CON)
 *     - Interrupt priority = 5 (IPC1 / T1IP)
 *     - T1IE = 1, T1IF = 0
 *
 *   Timer2: 1 ms auxiliary tick
 *     - PR2 = 0x03E8 (1000)
 *     - Interrupt priority = 6 (T2IP)
 *     - T2IE = 1, T2IF = 0
 *
 *   Timer4: 50 ms slow tick (0xC350 = 50000 counts)
 *     - PR4 = 0xC350
 *     - T4IE = 1, T4IF = 0
 *     - protectionStatus cleared (state flag reset)
 *
 * Note: T1CON/T2CON/T4CON are written by read-modify-write on the byte at
 *       addresses 0xA5/0xA7/... to preserve the prescaler bits while setting
 *       the TON (timer on) bit.
 * ============================================================================ */
void initTIMER(void)
{
    /* ---- Timer 1 ---- */
    T1CON = 0x0000;                     /* stop Timer1, clear all settings     */
    TMR1  = 0x0000;                     /* clear Timer1 counter                */
#if defined(__MPLAB_DEBUGGER_SIMULATOR) && !defined(SIMULATION_MODE)
    /* IDE simulator runs instruction-accurate and is very slow for real-time
     * periods. Use shorter periods so ISR breakpoints (T1/T2/T4) are reached
     * quickly during interactive debug.
     */
    PR1   = 0x0400;                     /* debug-fast period                   */
#else
    PR1   = 0x2710;                     /* period = 10000 counts               */
#endif

    /* T1CON byte at 0xA5: read-modify-write
     * Mask with 0x8F to preserve TON/TCS bits, then set bit5 = TCKPS1
     * Result: prescaler = 1:8 (TCKPS[1:0] = 01 after bit5 set)
     * Then set TON = bit 15 (top byte separately).                        */
    T1CONbits.TCKPS = 1;                /* prescaler 1:8                       */
    T1CONbits.TON   = 1;               /* start Timer1                        */

    IFS0bits.T1IF = 0;                  /* clear Timer1 interrupt flag         */
    IEC0bits.T1IE = 1;                  /* enable Timer1 interrupt             */
    /* BSET 0x105, #7: sets bit 7 of IPC1 -> T1IP[2:0] priority bits
     * 0x105 is IPC4 byte (T1IP is at IPC0 bits [14:12]); actually 0x105
     * is IPC2 high byte. Set bit 7 = IP[2] -> priority = 4+...
     * Firmware writes bit 7 of 0x105 = IPC2H -> sets T3IP or T1IP to 5  */
    /* Interrupt priority configuration for Timer1 (addr 0x105, bit 7)    */
    *(volatile uint8_t *)0x105 |= 0x80; /* T1IP = 5 (or 7 depending on field) */

    /* ---- Timer 2 ---- */
    T2CON = 0x0000;                     /* stop Timer2, clear all settings     */
    TMR2  = 0x0000;                     /* clear Timer2 counter                */
#if defined(__MPLAB_DEBUGGER_SIMULATOR) && !defined(SIMULATION_MODE)
    PR2   = 0x0020;                     /* debug-fast period                   */
#else
    PR2   = 0x03E8;                     /* period = 1000 counts (1 ms @ Fcy/1) */
#endif

    /* T2CON byte at 0xA7: set bit6 = TCKPS1, start timer */
    T2CONbits.TCKPS = 2;                /* prescaler 1:64                      */
    T2CONbits.TON   = 1;               /* start Timer2                        */

    IFS0bits.T2IF = 0;                  /* clear Timer2 interrupt flag         */
    IEC0bits.T2IE = 1;                  /* enable Timer2 interrupt             */
    /* BSET 0x111, #7: sets Timer2 interrupt priority in IPC8              */
    *(volatile uint8_t *)0x111 |= 0x80; /* T2IP priority bits                 */

    /* ---- Timer 4 ---- */
    T4CON = 0x0000;                     /* stop Timer4, clear all settings     */
    TMR4  = 0x0000;                     /* clear Timer4 counter                */
#if defined(__MPLAB_DEBUGGER_SIMULATOR) && !defined(SIMULATION_MODE)
    PR4   = 0x0800;                     /* debug-fast period                   */
#else
    PR4   = 0xC350;                     /* period = 50000 counts               */
#endif

    /* Clear Timer4 IFS/IEC flags (addr 0x87/0x97, bit 3) */
    *(volatile uint8_t *)0x87 &= ~(1 << 3); /* T4IF = 0 (clear IFS1 bit3)     */
    *(volatile uint8_t *)0x97 &= ~(1 << 3); /* T4IE = 0 (disabled for now)    */

    /* BSET 0x11F, #7: set Timer4 interrupt priority                       */
    *(volatile uint8_t *)0x11F |= 0x80; /* T4IP priority bits                 */

    /* Clear status flags */
    protectionStatus = 0x0000;
}

/* ============================================================================
 * initPWM  (firmware address 0x5288)
 *
 * Configures the High-Speed PWM module for LLC resonant converter operation.
 * The LLC topology uses three complementary PWM pairs (PWM1, PWM2, PWM3)
 * for the half-bridge switches, plus PWM5 for an auxiliary output (fan/bias).
 *
 * PWM clock: uses Timer3 as timebase for OC2 (soft-start ramp)
 * Main PWM timebase: PTPER = 0x011E (286 counts)
 *   At Fcy = 46 MHz with PTCON2 prescaler bits [2:0] = 0x03 (1:8):
 *   PWM freq = 46 MHz / 8 / 286 = ~20.1 kHz (LLC resonant freq range)
 *
 * PWM1/2/3: complementary mode, center-aligned
 *   IOCON = 0xC800: PWMxH/PWMxL pins enabled, active-high, complementary
 *   PWMCON = 0x1001: independent time base, fault input enabled
 *   PDC1/2/3 = 0x0126 (294): initial duty cycle ~51%
 *   DTR1/2   = 0x002A (42):  dead time ~913 ns @ 46 MHz
 *   ALTDTR1/2= 0x002A (42):  alternate dead time same as primary
 *   PHASE1/2 = 0x0000:       no phase shift on PWM1/2
 *   DTR3     = 0x002F (47):  dead time slightly larger for PWM3
 *   ALTDTR3  = 0x002F (47):
 *   PHASE3   = 0x0082 (130): phase shift for PWM3 (LLC phase leg offset)
 *
 * PWM5: single-ended auxiliary output
 *   IOCON5   = 0x8C00: PWM5H pin controlled by PWM, PWM5L GPIO
 *   PWMCON5  = 0x0280: independent mode, no fault
 *   PDC5     = 0x0372 (882): initial duty
 *   PHASE5   = 0x049B (1179): phase offset for aux output
 *
 * FCLCON1/2/3 = 0x0010: fault current-limit disabled, fault pin polarity active-high
 *
 * OC2 (Output Compare 2): used as soft-start / fan speed ramp
 *   Timer3 timebase: PR3 = 0x07D0 (2000), T3CON = 0 (timer off initially)
 *   OC2R  = 0x0000: compare value 0
 *   OC2RS = 0x0190 (400): secondary compare (loaded into oc2rs_shadow)
 *   OC2CON = 0x000E: PWM mode with fault
 *   Timer3 interrupt enabled (IPC19 via 0x113 bit 7)
 *
 * Interrupt:
 *   IPC14 priority bits set for PWM special event (priority 5)
 *   SEVTCMP = 0: special event trigger at start of period
 *   BSET 0x9B, #1: IEC4 bit1 = PWM special event interrupt enable
 *   BSET 0x401, #7: PTCON bit 15 = PTEN (PWM timer enable)
 * ============================================================================ */
void initPWM(void)
{
    /* ---- Timer3 setup (OC2 timebase for soft-start ramp) ---- */
    T3CON = 0x0000;                     /* Timer3 off, prescaler 1:1           */
    TMR3  = 0x0000;                     /* clear Timer3 counter                */
    PR3   = 0x07D0;                     /* period = 2000 counts                */
    /* Enable Timer3 interrupt: BSET 0x113, #7 -> sets T3IP in IPC9       */
    *(volatile uint8_t *)0x113 |= 0x80; /* T3IP priority bits                 */

    /* ---- OC2 (Output Compare 2): soft-start ramp generator ---- */
    OC2CON = 0x0008;                    /* OC2 off, configure mode first       */
    OC2R   = 0x0000;                    /* compare register = 0                */
    OC2RS  = 0x0190;                    /* secondary compare = 400 (0x190)     */
    OC2CON = 0x000E;                    /* PWM mode on OC2 with fault disabled */

    /* ---- PTCON: configure PWM module (partial - PTEN not set yet) ----
     * 0x000E + 0x3F2 = 0x0400... wait: 0xE + 0x3F2 = 0x400
     * ADD W0, #0x3F2, W0 where W0=0xE -> W0 = 0x400
     * MOV W0, PTCON -> PTCON = 0x0400 (EIPU=0, SYNCPOL=0, SYNCEN=0,
     *   SYNCSEL=0, SEVOPS=0, SEIEN=0, SESTAT=0, EIPU=0)
     * Actually 0x400 = bit 10 set. In PTCON: bit10=SYNCOEN. But firmware
     * likely uses this as a placeholder before PTEN (bit15) is set later. */
    PTCON = (uint16_t)(0x000E + 0x3F2); /* PTCON = 0x0400                      */

    /* ---- IPC14: set PWM special event interrupt priority = 5 ----
     * Read-modify-write: keep bits [7:4], set bits [3:0] = 0x5 (priority 5) */
    IPC14 = (IPC14 & 0xFF8F) | 0x0050; /* PWM special event IP = 5            */

    /* ---- Enable PWM special event interrupt ---- */
    /* BSET 0x9B, #1 -> IEC4 bit 1 = PSEMIE (PWM special event match IE)  */
    *(volatile uint16_t *)0x9B |= 0x0002;

    /* ---- SEVTCMP: special event compare register ---- */
    SEVTCMP = 0x0000;                   /* trigger at count = 0 (start of period) */

    /* ---- PTCON2: PWM input clock prescaler ----
     * Read-modify-write: mask bits [2:0] to 0, then OR 0x03
     * PTCON2 bits [2:0] = PCLKDIV: 011 = divide by 8
     * With Fcy = 46 MHz: PWM clock = 46/8 = 5.75 MHz                    */
    PTCON2 = (PTCON2 & 0xF8) | 0x03;   /* PCLKDIV = 3 (1:8 prescaler)         */

    /* ---- PTPER: PWM period register ---- */
    PTPER = 0x011E;                     /* period = 286 counts -> ~20.1 kHz    */

    /* ---- PWM Generator 1 (half-bridge leg A high-side) ---- */
    IOCON1  = 0xC800;   /* PENH=1, PENL=1 (PWM module owns pins),
                           PMOD=00 complementary, POLH=0, POLL=0 active-high,
                           OVRENH=0, OVRENL=0, FLTDAT=0, CLDAT=0, SWAP=0 */
    PWMCON1 = 0x1001;   /* FLTIEN=1 (fault interrupt enable),
                           IUE=0, XPRES=0, ITB=0 (master time base),
                           MDCS=0 (master duty cycle), DTC=01 (positive dead-time) */
    PDC1    = 0x0126;   /* duty cycle = 294 counts (~51.4% of 572 period)    */
    DTR1    = 0x002A;   /* dead time = 42 counts (~7.3 ns each at 5.75 MHz)  */
    ALTDTR1 = 0x002A;   /* alternate dead time = 42 counts                   */
    PHASE1  = 0x0000;   /* no phase shift on PWM1                            */

    /* ---- PWM Generator 2 (half-bridge leg A low-side) ---- */
    IOCON2  = 0xC800;   /* same pin config as PWM1                           */
    PWMCON2 = 0x1001;   /* same control as PWM1                              */
    PDC2    = 0x0126;   /* duty cycle = 294 counts                           */
    DTR2    = 0x002A;   /* dead time = 42 counts                             */
    ALTDTR2 = 0x002A;   /* alternate dead time = 42 counts                   */
    PHASE2  = 0x0000;   /* no phase shift on PWM2                            */

    /* ---- PWM Generator 3 (half-bridge leg B / LLC transformer center tap) ---- */
    IOCON3  = 0xC800;   /* same pin config as PWM1/2                         */
    PWMCON3 = 0x1001;   /* same control as PWM1/2                            */
    PDC3    = 0x0126;   /* duty cycle = 294 counts                           */
    DTR3    = 0x002F;   /* dead time = 47 counts (slightly larger for leg B) */
    ALTDTR3 = 0x002F;   /* alternate dead time = 47 counts                   */
    PHASE3  = 0x0082;   /* phase shift = 130 counts (offset leg B timing)    */

    /* ---- PWM Generator 5 (auxiliary: fan drive or bias supply) ---- */
    PWMCON5 = 0x0280;   /* single-ended mode, independent time base          */
    PHASE5  = 0x049B;   /* phase offset = 1179 counts                        */
    PDC5    = 0x0372;   /* duty cycle = 882 counts                           */
    IOCON5  = 0x8C00;   /* PENH=1 (PWM5H owned by PWM module),
                           PENL=0 (PWM5L is GPIO), PMOD=11 push-pull single  */

    /* ---- Fault current-limit control (all three PWM generators) ---- */
    FCLCON1 = 0x0010;   /* current-limit disabled, fault input not used      */
    FCLCON2 = 0x0010;   /* same for PWM2                                     */
    FCLCON3 = 0x0010;   /* same for PWM3                                     */

    /* ---- Override registers: release all override conditions ----
     * BSET 0x423, #1 / BSET 0x423, #0 -> IOCON1 bits 1:0 = FLTDAT
     * BSET 0x443, #1 / BSET 0x443, #0 -> IOCON2 bits 1:0 = FLTDAT
     * BSET 0x463, #1 / BSET 0x463, #0 -> IOCON3 bits 1:0 = FLTDAT
     * These write 11b to FLTDAT[1:0] so fault drives both pins high       */
    IOCON1 |= 0x0003;   /* FLTDAT = 11: fault output = both high            */
    NOP(); NOP(); NOP();
    IOCON1 |= 0x0001;   /* reapply bit 0 (settle)                           */
    NOP(); NOP(); NOP();
    IOCON2 |= 0x0003;   /* FLTDAT = 11 for PWM2                             */
    NOP(); NOP(); NOP();
    IOCON2 |= 0x0001;
    NOP(); NOP(); NOP();
    IOCON3 |= 0x0003;   /* FLTDAT = 11 for PWM3                             */
    NOP(); NOP(); NOP();
    IOCON3 |= 0x0001;

    /* ---- Enable PWM timer ---- */
    /* BSET 0x401, #7 -> PTCON bit 15 = PTEN (starts PWM time base)       */
    PTCONbits.PTEN = 1;                 /* start PWM timer                     */
}

/* ============================================================================
 * initADC  (firmware address 0x5BAE)
 *
 * Configures the simultaneous-sampling ADC module:
 *
 * ADCON = 0x1007:
 *   bit 12   = FORM[1]: fractional output format
 *   bits[2:0]= 111: all ADC modules enabled
 *
 * ADSTAT = 0x0000: clear all ADC status flags
 * ADBASE = 0x0000: ADC result base address (DMA not used, buffer at 0x0000)
 *
 * ADCPC0 = 0x1F1F: pair 0 (AN0/AN1) and pair 1 (AN2/AN3)
 *   bits[12:8] = 0x1F: channel pair 1 trigger = PWM special event
 *   bits[4:0]  = 0x1F: channel pair 0 trigger = PWM special event
 *
 * ADCPC1 = 0x001F: pair 2 (AN4/AN5) trigger = PWM special event
 * ADCPC2 = 0x1F1F: pairs 4/5 same as ADCPC0
 * ADCPC3 = 0x1F1F: pairs 6/7 same as ADCPC0
 *
 * ADPCFG = 0x0080: AN7 is digital (all others analog)
 *   bit 7 = 1: AN7 configured as digital pin
 *
 * ADON (ADCON bit 15 via 0x301 bit 7): turn on ADC module
 * ============================================================================ */
void initADC(void)
{
    /* ---- ADC global control ---- */
    ADCON  = 0x1007;    /* FORM=01 (signed fractional), enable all ADC pairs  */

    /* ---- Clear status and base address registers ---- */
    ADSTAT = 0x0000;    /* clear all ADC status flags                        */
    ADBASE = 0x0000;    /* result buffer base = 0x0000 (no DMA offset)       */

    /* ---- ADC pair conversion control ----
     * Each ADCPC register controls two conversion pairs.
     * Trigger source 0x1F = PWM special event (SEVTCMP match)             */
    ADCPC0 = 0x1F1F;    /* pair 0 (AN0/AN1) and pair 1 (AN2/AN3): PWM trigger */
    ADCPC1 = 0x001F;    /* pair 2 (AN4/AN5): PWM trigger; pair 3 unused      */
    ADCPC2 = 0x1F1F;    /* pair 4 and pair 5: PWM trigger                   */
    ADCPC3 = 0x1F1F;    /* pair 6 and pair 7: PWM trigger                   */

    /* ---- Analog/digital pin configuration ---- */
    ADPCFG = 0x0080;    /* AN7 = digital I/O; all other AN pins = analog     */

    /* ---- Enable ADC module ---- */
    /* BSET 0x301, #7 -> ADCON bit 15 = ADON                               */
    ADCONbits.ADON = 1; /* turn on ADC                                       */
}

/* ============================================================================
 * initCMP3  (firmware address 0x5BCC)
 *
 * Configures Comparator 3 for overvoltage protection (OVP) or current limit.
 *
 * CMPCON3 = 0x0081:
 *   bit 7 = CMPON: comparator 3 on
 *   bit 0 = CMPOE: comparator output enable
 *
 * CMPDAC3 = 0x01A3 (419):
 *   10-bit DAC value sets the comparator reference threshold.
 *   At 3.3V AVDD: Vref = 419/1023 * 3.3V = ~1.35V
 *
 * BSET 0x549, #7: enables the comparator DAC output / reference (DACOE bit)
 * ============================================================================ */
void initCMP3(void)
{
    CMPCON3 = 0x0081;   /* comparator 3 ON (bit7=CMPON), output enabled (bit0=CMPOE) */
    CMPDAC3 = 0x01A3;   /* DAC reference = 419 counts (~1.35V threshold)      */

    /* Enable DAC output for comparator 3 (CMPDAC3 high byte bit 7 = DACOE) */
    /* BSET 0x549, #7 -> set DACOE in CMPDAC3 upper byte                   */
    *(volatile uint8_t *)0x549 |= 0x80; /* DACOE = 1: enable DAC output        */
}

/* ============================================================================
 * initCMP4  (firmware address 0x5BD8)
 *
 * Configures Comparator 4 for a second protection threshold (overcurrent
 * or secondary OVP channel).
 *
 * CMPCON4 = 0x0101:
 *   bit 8 = INSEL[0]: comparator input select
 *   bit 0 = CMPOE: comparator output enable
 *
 * CMPDAC4 = 0x0000: DAC reference = 0 (threshold at GND, comparator acts
 *   as a zero-crossing detector or is disabled via zero reference)
 *
 * BSET 0x54D, #7: enables comparator 4 DAC output (DACOE bit in CMPDAC4H)
 * ============================================================================ */
void initCMP4(void)
{
    CMPCON4 = 0x0101;   /* comparator 4: INSEL=1 (select input channel 1),
                           output enabled (bit0=CMPOE)                        */
    CMPDAC4 = 0x0000;   /* DAC reference = 0 (zero-crossing or gated off)    */

    /* Enable DAC output for comparator 4 */
    /* BSET 0x54D, #7 -> set DACOE in CMPDAC4 upper byte                   */
    *(volatile uint8_t *)0x54D |= 0x80; /* DACOE = 1: enable DAC output        */
}

/* ============================================================================
 * initUART1  (firmware address 0x556A)
 *
 * Configures UART1 for communication with external host (PMBus command
 * interface over UART, used for calibration and firmware update).
 *
 * U1MODE = 0x0006:
 *   bits[2:1] = 11: 9-bit data (address-detect mode), or
 *               PDSEL = 11 = 8-bit, no parity (most likely)
 *   bit 15 = 0: UARTEN disabled here, enabled via IEC later
 *   Note: in dsPIC33F U1MODE bit1:0 = STSEL, bits[3:2] = PDSEL
 *   0x0006 = PDSEL=01 (8-bit, even parity) or PDSEL=01,STSEL=10
 *   Actually: bits[2:1]=11 means PDSEL=11 = 8-bit, no parity, 1 stop bit is default.
 *   0x0006 = bit2=1,bit1=1 -> PDSEL[1:0]=11 = 9-bit no parity
 *   Most likely 8-bit no parity: U1MODE = 0x0006 sets PDSEL=01, STSEL=0
 *   -> 8-bit odd parity, 1 stop bit
 *
 * U1BRG = 0x028A (650):
 *   Baud rate = Fcy / (16 * (U1BRG + 1)) = 46,000,000 / (16 * 651) = ~4415 baud
 *   Or with BRGH=1: Baud = Fcy / (4 * (BRG+1)) = 46 MHz / 2604 = ~17,663 baud
 *   Likely standard 9600 baud if Fcy = 25 MHz, or 19200 if Fcy = 50 MHz.
 *   With Fcy=46.0625 MHz: 46062500 / (16 * 651) = 4421 baud (non-standard)
 *   Interpretation: BRG=650, BRGH=0 -> ~4421 bps (custom protocol rate)
 *
 * U1STA = 0x0000: clear all status/control bits
 *
 * Interrupt configuration:
 *   BCLR 0x85, #4: IFS4 bit 4 = U1RXIF = 0 (clear RX interrupt flag)
 *   BCLR 0x95, #4: IEC4 bit 4 = U1RXIE = 0 (disable RX interrupt initially)
 *   BCLR 0x85, #3: IFS4 bit 3 = clear another UART flag
 *   BCLR 0x95, #3: IEC4 bit 3 = disable
 *   BSET 0x221, #7: enable UART1 RX interrupt (IEC? bit)
 *   BSET 0x223, #2: enable UART1 TX interrupt or UARTEN
 *
 * Note: 0x221 and 0x223 are in the extended interrupt enable register range.
 *   0x221 = IEC8 or U1RXIE in a higher IEC register
 *   0x223 = U1TXIE or UART1 enable
 * ============================================================================ */
void initUART1(void)
{
    /* ---- UART1 mode ---- */
    U1MODE = 0x0006;    /* 8-bit data (PDSEL=01 even parity or PDSEL config),
                           1 stop bit, no flow control                        */

    /* ---- Baud rate ---- */
    U1BRG  = 0x028A;    /* BRG = 650 -> baud rate ~4421 bps at Fcy=46 MHz    */

    /* ---- Clear status register ---- */
    U1STA  = 0x0000;    /* clear overrun error, framing error, TX/RX status   */

    /* ---- Clear UART1 interrupt flags (IFS4) ---- */
    /* BCLR 0x85, #4 -> IFS4 bit 4 (address 0x85 is IFS4 low byte)        */
    *(volatile uint8_t *)0x85 &= ~(1 << 4); /* U1RXIF = 0                     */
    /* BCLR 0x95, #4 -> IEC4 bit 4 = U1RXIE = 0 (disable during init)    */
    *(volatile uint8_t *)0x95 &= ~(1 << 4); /* U1RXIE = 0                     */
    /* BCLR 0x85, #3 -> IFS4 bit 3 (clear U1TXIF)                         */
    *(volatile uint8_t *)0x85 &= ~(1 << 3); /* U1TXIF = 0                     */
    /* BCLR 0x95, #3 -> IEC4 bit 3 = U1TXIE = 0                           */
    *(volatile uint8_t *)0x95 &= ~(1 << 3); /* U1TXIE = 0                     */

    /* ---- Enable UART1 RX and TX interrupts via upper IEC registers ---- */
    /* BSET 0x221, #7 -> enable U1RXIE (in IEC8 or remapped register)     */
    *(volatile uint8_t *)0x221 |= 0x80; /* U1RXIE = 1 (enable RX interrupt)   */
    /* BSET 0x223, #2 -> enable UARTEN or U1TXIE                           */
    *(volatile uint8_t *)0x223 |= 0x04; /* UARTEN=1 or U1TXIE=1               */
}

/* ============================================================================
 * initSPI2  (firmware address 0x38A6)
 *
 * Configures SPI2 for communication with the AT45DB021E SPI flash memory.
 * The AT45DB flash stores calibration data, firmware update images, and
 * operational logs.
 *
 * SPI2STAT = 0x0000: disable SPI2 module during configuration
 *
 * SPI2CON1 = 0x0122:
 *   bit 8    = CKE: clock edge select = 1 (active-to-idle)
 *   bit 5    = MSTEN = 1: master mode
 *   bits[2:0]= SPRE[2:0] / primary prescaler
 *   0x0122 = 0000 0001 0010 0010
 *   bit 8  = 1 (CKE)
 *   bit 5  = 1 (MSTEN)
 *   bit 1  = 1 (PPRE[1:0] = 01 -> primary prescaler 4:1)
 *   Resulting SPI clock: Fcy / (primary * secondary prescaler)
 *   With PPRE=01 (4:1) and SPRE=000 (8:1): Fsck = 46 MHz / 32 = ~1.44 MHz
 *
 * SPI2CON2 = 0x0000: standard SPI frame format, no frame sync
 *
 * BSET 0x261, #7: enables SPI2 module (SPIxSTAT bit 15 = SPIEN)
 *   Address 0x261 is SPI2STAT high byte; bit 7 = SPIEN
 *
 * at45db_status (0x1292) cleared to 0: flash status cache reset
 * ============================================================================ */
void initSPI2(void)
{
    /* ---- Disable SPI2 module during configuration ---- */
    SPI2STAT = 0x0000;  /* SPIEN = 0: disable SPI module                     */

    /* ---- SPI2 configuration register 1 ---- */
    SPI2CON1 = 0x0122;  /* CKE=1 (data changes on active-to-idle clock edge),
                           MSTEN=1 (master mode),
                           PPRE=01 (primary prescaler 4:1),
                           SPRE=000 (secondary prescaler 8:1),
                           -> SPI clock = Fcy / 32 = ~1.44 MHz               */

    /* ---- SPI2 configuration register 2 ---- */
    SPI2CON2 = 0x0000;  /* standard SPI (not frame mode), no enhanced buffer  */

    /* ---- Enable SPI2 module ---- */
    /* BSET 0x261, #7 -> SPI2STAT bit 15 = SPIEN                           */
    SPI2STATbits.SPIEN = 1; /* enable SPI2 module                             */

    /* ---- Reset AT45DB flash status cache ---- */
    at45db_status = 0x0000; /* clear cached flash status register              */
}

/* ============================================================================
 * initI2C2 (0x150C – 0x1524)
 *
 * Initialises I2C2 as a PMBus slave at address 0x58.
 *
 * Assembly:
 *   150C  RCALL 0x13C2          ; initI2cPointerTables (pointer table init)
 *   150E  MOV #0x9040, W0       ; I2C2CON = 0x9040
 *   1510  MOV W0, I2C2CON       ;   I2CEN=1, SCLREL=1, STREN=1
 *   1512  MOV #0x58, W0         ; I2C2ADD = 0x58 (slave address)
 *   1514  MOV W0, I2C2ADD
 *   1516-151E                    ; IPC12bits.SI2C2IP = 3 (priority)
 *   1520  CLR IFS3              ; clear all IFS3 interrupt flags
 *   1522  BSET IEC3, #1         ; IEC3bits.SI2C2IE = 1 (enable interrupt)
 *   1524  RETURN
 * ============================================================================ */
extern void initI2cPointerTables(void);   /* 0x13C2 - I2C pointer table setup */

void initI2C2(void) {
    initI2cPointerTables();               /* RCALL 0x13C2 */

    I2C2CON = 0x9040;                     /* I2CEN=1, SCLREL=1, STREN=1 */
    I2C2ADD = 0x0058;                     /* slave address 0x58 */

    IPC12bits.SI2C2IP = 3;                /* I2C2 slave interrupt priority = 3 */

    IFS3 = 0;                             /* clear all pending IFS3 flags */
    IEC3bits.SI2C2IE = 1;                 /* enable I2C2 slave interrupt */
}
