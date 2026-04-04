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

extern void pwmOverrideEnable(void);   /* 0x4B50 */

enum {
    TIMER1_PERIOD_COUNTS = 0x2710,
    TIMER2_PERIOD_COUNTS = 0x03E8,
    TIMER4_PERIOD_COUNTS = 0xC350,
    TIMER3_PERIOD_COUNTS = 0x07D0,

    PWM_PTCON_INIT       = 0x0400,
    PWM_PTPER_INIT       = 0x011E,
    PWM_IOCON_MAIN_INIT  = 0xC800,
    PWM_PWMCON_MAIN_INIT = 0x1001,
    PWM_DUTY_MAIN_INIT   = 0x0126,
    PWM_DTR12_INIT       = 0x002A,
    PWM_DTR3_INIT        = 0x002F,
    PWM_PHASE3_INIT      = 0x0082,
    PWM_PWMCON5_INIT     = 0x0280,
    PWM_PHASE5_INIT      = 0x049B,
    PWM_DUTY5_INIT       = 0x0372,
    PWM_IOCON5_INIT      = 0x8C00,
    PWM_FCLCON_INIT      = 0x0010,

    OC2CON_IDLE_MODE     = 0x0008,
    OC2CON_PWM_MODE      = 0x000E,
    OC2RS_INIT           = 0x0190,

    ADCON_INIT           = 0x1007,
    ADCPC_COMMON_INIT    = 0x1F1F,
    ADCPC1_INIT          = 0x001F,
    ADPCFG_INIT          = 0x0080,

    CMPCON3_INIT         = 0x0081,
    CMPDAC3_INIT         = 0x01A3,
    CMPCON4_INIT         = 0x0101,
    CMPDAC4_INIT         = 0x0000,

    UART1_MODE_INIT      = 0x0006,
    UART1_BRG_INIT       = 0x028A,
    SPI2CON1_INIT        = 0x0122,
    I2C2CON_INIT         = 0x9040,
};

/* ============================================================================
 * initClock  (firmware address 0x59CE)
 *
 * Configures oscillator registers exactly as in firmware image:
 *   - PLLFBD / CLKDIV programmed
 *   - clock switch requested via OSCCON unlock sequence
 *   - waits for COSC/LOCK in hardware mode
 *   - enables and waits auxiliary PLL (ACLKCON)
 * ============================================================================ */
void initClock(void)
{
    /* ---- PLL configuration (exact register values from image) ---- */
    PLLFBD = 48;                        /* M = PLLFBD + 2 = 50               */
    CLKDIVbits.PLLPOST = 0;            /* N1 = 2                            */
    CLKDIVbits.PLLPRE  = 2;            /* N2 = PLLPRE + 2 = 4              */

    /* ---- Oscillator switch to External Crystal + PLL ----              */
    __builtin_write_OSCCONH(0x03);      /* NOSC = 011 = External w/ PLL      */

    /* ---- OSCTUN / OSCCONL unlock: write 0x46 then 0x57 then 0x01 to 0x742
     * Sets OSWEN=1 in OSCCONL to initiate the clock switch.               */
    __builtin_write_OSCCONL(0x01);      /* OSWEN = 1 -> start oscillator switch */

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

    /* ODCD = 0x0801: open-drain on RD11 and RD0
     * RD0 open-drain likely used for I2C or wired-OR bus signal          */
    ODCD  = 0x0801;

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
 *   Timer1: 5 kHz interrupt
 *     - Clock: Fcy, no prescaler (TCKPS=00)
 *     - PR1 = 0x2710 (10000) -> 5 kHz @ Fcy=50 MHz
 *     - Interrupt priority configured via IPC0H (addr 0xA5)
 *     - T1IE = 1, T1IF = 0
 *
 *   Timer2: 50 kHz interrupt
 *     - Clock: Fcy, no prescaler (TCKPS=00)
 *     - PR2 = 0x03E8 (1000) -> 50 kHz @ Fcy=50 MHz
 *     - Interrupt priority configured via IPC1H (addr 0xA7)
 *     - T2IE = 1, T2IF = 0
 *
 *   Timer4: 1 ms tick (0xC350 = 50000 counts @ Fcy=50 MHz)
 *     - PR4 = 0xC350 (1 kHz)
 *     - T4IE = 0, T4IF = 0
 *     - protectionStatus cleared (state flag reset)
 *
 * Note:
 *   - T1IP/T2IP are configured in IPC0bits/IPC1bits.
 *   - Timer run bits are T1CONbits.TON / T2CONbits.TON / T4CONbits.TON.
 * ============================================================================ */
void initTIMER(void)
{
    /* ---- Timer 1 ---- */
    T1CON = 0x0000;                     /* stop Timer1, clear all settings     */
    TMR1  = 0x0000;                     /* clear Timer1 counter                */
    PR1   = TIMER1_PERIOD_COUNTS;

    /* 5B00..5B08: IPC0H = (IPC0H & 0x8F) | 0x20 -> T1IP = 2 */
    IPC0bits.T1IP = 2;

    IFS0bits.T1IF = 0;                  /* clear Timer1 interrupt flag         */
    IEC0bits.T1IE = 1;                  /* enable Timer1 interrupt             */
    /* 5B0E: BSET 0x105,#7 -> T1CONbits.TON = 1 */
    T1CONbits.TON = 1;                  /* start Timer1 */

    /* ---- Timer 2 ---- */
    T2CON = 0x0000;                     /* stop Timer2, clear all settings     */
    TMR2  = 0x0000;                     /* clear Timer2 counter                */
    PR2   = TIMER2_PERIOD_COUNTS;

    /* 5B18..5B1E: IPC1H = (IPC1H & 0x8F) | 0x40 -> T2IP = 4 */
    IPC1bits.T2IP = 4;

    IFS0bits.T2IF = 0;                  /* clear Timer2 interrupt flag         */
    IEC0bits.T2IE = 1;                  /* enable Timer2 interrupt             */
    /* 5B24: BSET 0x111,#7 -> T2CONbits.TON = 1 */
    T2CONbits.TON = 1;                  /* start Timer2 */

    /* ---- Timer 4 ---- */
    T4CON = 0x0000;                     /* stop Timer4, clear all settings     */
    TMR4  = 0x0000;                     /* clear Timer4 counter                */
    PR4   = TIMER4_PERIOD_COUNTS;

    /* 5B2E/5B30: clear T4 interrupt flag and keep T4 interrupt disabled */
    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 0;
    /* 5B32: BSET 0x11F,#7 -> T4CONbits.TON = 1 */
    T4CONbits.TON = 1;

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
 *   Timer3 timebase: PR3 = 0x07D0 (2000), then T3CON.TON = 1
 *   OC2R  = 0x0000: compare value 0
 *   OC2RS = 0x0190 (400): secondary compare (loaded into oc2rs_shadow)
 *   OC2CON = 0x000E: PWM mode with fault
 *   Timer3 started by setting T3CON.TON (0x113 bit7 in byte form)
 *
 * Interrupt:
 *   IPC14 priority bits set for PWM special event (priority 5)
 *   SEVTCMP = 0: special event trigger at start of period
 *   IEC3bits.PSEMIE = 1: PWM special event interrupt enable
 *   BSET 0x401, #7: PTCON bit 15 = PTEN (PWM timer enable)
 * ============================================================================ */
void initPWM(void)
{
    /* ---- Timer3 setup (OC2 timebase for soft-start ramp) ---- */
    T3CON = 0x0000;                     /* Timer3 off, prescaler 1:1           */
    TMR3  = 0x0000;                     /* clear Timer3 counter                */
    PR3   = TIMER3_PERIOD_COUNTS;
    T3CONbits.TON = 1;                  /* BSET 0x113,#7 in original image     */

    /* ---- OC2 (Output Compare 2): soft-start ramp generator ---- */
    OC2CON = OC2CON_IDLE_MODE;          /* OC2 off, configure mode first       */
    OC2R   = 0x0000;                    /* compare register = 0                */
    OC2RS  = OC2RS_INIT;                /* secondary compare = 400             */
    OC2CON = OC2CON_PWM_MODE;           /* PWM mode on OC2 with fault disabled */

    /* 52A2..52A4 computes and writes PTCON = 0x0400 */
    PTCON = PWM_PTCON_INIT;

    /* ---- IPC14: set PWM special event interrupt priority = 5 ----
     * Read-modify-write: keep bits [7:4], set bits [3:0] = 0x5 (priority 5) */
    IPC14bits.PSEMIP = 5;

    /* ---- Enable PWM special event interrupt ---- */
    IEC3bits.PSEMIE = 1;

    /* ---- SEVTCMP: special event compare register ---- */
    SEVTCMP = 0x0000;

    /* ---- PTCON2: PWM input clock prescaler ----
     * Read-modify-write: mask bits [2:0] to 0, then OR 0x03
     * PTCON2 bits [2:0] = PCLKDIV: 011 = divide by 8
     * With Fcy = 46 MHz: PWM clock = 46/8 = 5.75 MHz                    */
    PTCON2bits.PCLKDIV = 3;             /* PCLKDIV = 1:8                        */

    /* ---- PTPER: PWM period register ---- */
    PTPER = PWM_PTPER_INIT;             /* 286 counts                            */

    /* ---- PWM Generator 1 (half-bridge leg A high-side) ---- */
    IOCON1  = PWM_IOCON_MAIN_INIT;   /* PENH=1, PENL=1 (PWM module owns pins),
                           PMOD=00 complementary, POLH=0, POLL=0 active-high,
                           OVRENH=0, OVRENL=0, FLTDAT=0, CLDAT=0, SWAP=0 */
    PWMCON1 = PWM_PWMCON_MAIN_INIT;   /* FLTIEN=1 (fault interrupt enable),
                           IUE=0, XPRES=0, ITB=0 (master time base),
                           MDCS=0 (master duty cycle), DTC=01 (positive dead-time) */
    PDC1    = PWM_DUTY_MAIN_INIT;
    DTR1    = PWM_DTR12_INIT;
    ALTDTR1 = PWM_DTR12_INIT;
    PHASE1  = 0x0000;   /* no phase shift on PWM1                            */

    /* ---- PWM Generator 2 (half-bridge leg A low-side) ---- */
    IOCON2  = PWM_IOCON_MAIN_INIT;
    PWMCON2 = PWM_PWMCON_MAIN_INIT;
    PDC2    = PWM_DUTY_MAIN_INIT;
    DTR2    = PWM_DTR12_INIT;
    ALTDTR2 = PWM_DTR12_INIT;
    PHASE2  = 0x0000;   /* no phase shift on PWM2                            */

    /* ---- PWM Generator 3 (half-bridge leg B / LLC transformer center tap) ---- */
    IOCON3  = PWM_IOCON_MAIN_INIT;
    PWMCON3 = PWM_PWMCON_MAIN_INIT;
    PDC3    = PWM_DUTY_MAIN_INIT;
    DTR3    = PWM_DTR3_INIT;
    ALTDTR3 = PWM_DTR3_INIT;
    PHASE3  = PWM_PHASE3_INIT;

    /* ---- PWM Generator 5 (auxiliary: fan drive or bias supply) ---- */
    PWMCON5 = PWM_PWMCON5_INIT;
    PHASE5  = PWM_PHASE5_INIT;
    PDC5    = PWM_DUTY5_INIT;
    IOCON5  = PWM_IOCON5_INIT;   /* PENH=1 (PWM5H owned by PWM module),
                           PENL=0 (PWM5L is GPIO), PMOD=11 push-pull single  */

    /* ---- Fault current-limit control (all three PWM generators) ---- */
    FCLCON1 = PWM_FCLCON_INIT;
    FCLCON2 = PWM_FCLCON_INIT;
    FCLCON3 = PWM_FCLCON_INIT;

    /* ---- Assert override outputs on PWM1/2/3 ----
     * Assembly calls 0x4B50 here (BSET 0x423/0x443/0x463 bit1 then bit0).
     * Note these are IOCONx high-byte bit1/bit0 => word bit9/bit8:
     * OVRENH / OVRENL (not PEN, not FLTDAT).                              */
    pwmOverrideEnable();

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
    ADCON  = ADCON_INIT;

    /* ---- Clear status and base address registers ---- */
    ADSTAT = 0x0000;    /* clear all ADC status flags                        */
    ADBASE = 0x0000;    /* result buffer base = 0x0000 (no DMA offset)       */

    /* ---- ADC pair conversion control ----
     * Each ADCPC register controls two conversion pairs.
     * Trigger source 0x1F = PWM special event (SEVTCMP match)             */
    ADCPC0 = ADCPC_COMMON_INIT;
    ADCPC1 = ADCPC1_INIT;
    ADCPC2 = ADCPC_COMMON_INIT;
    ADCPC3 = ADCPC_COMMON_INIT;

    /* ---- Analog/digital pin configuration ---- */
    ADPCFG = ADPCFG_INIT;

    /* ---- Enable ADC module ---- */
    /* BSET 0x301, #7 -> ADCON bit 15 = ADON                               */
    ADCONbits.ADON = 1; /* turn on ADC                                       */
}

/* ============================================================================
 * initCMP3  (firmware address 0x5BCC)
 *
 * Configures Comparator 3 for overvoltage protection (OVP) or current limit.
 *
 * CMPCON3/CMPDAC3 are loaded from image constants.
 * Final step sets CMPCON3bits.CMPON=1 (BSET 0x549,#7).
 *
 * CMPDAC3 = 0x01A3 (419):
 *   10-bit DAC value sets the comparator reference threshold.
 *   At 3.3V AVDD: Vref = 419/1023 * 3.3V = ~1.35V
 *
 * Note: BSET 0x549,#7 is CMPCON3.CMPON, not DACOE.
 * ============================================================================ */
void initCMP3(void)
{
    CMPCON3 = CMPCON3_INIT;
    CMPDAC3 = CMPDAC3_INIT;

    CMPCON3bits.CMPON = 1;
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
 * Final step sets CMPCON4bits.CMPON=1 (BSET 0x54D,#7).
 * ============================================================================ */
void initCMP4(void)
{
    CMPCON4 = CMPCON4_INIT;   /* comparator 4: INSEL=1 (select input channel 1),
                           output enabled (bit0=CMPOE)                        */
    CMPDAC4 = CMPDAC4_INIT;

    CMPCON4bits.CMPON = 1;
}

/* ============================================================================
 * initUART1  (firmware address 0x556A)
 *
 * Assembly sequence (0x556A..0x5580):
 *   U1MODE = 0x0006
 *   U1BRG  = 0x028A
 *   U1STA  = 0x0000
 *   clear U1RXIF/U1TXIF, keep U1RXIE/U1TXIE disabled
 *   set U1MODE.UARTEN and U1STA.UTXEN
 *
 * Important:
 *   - BSET 0x221,#7 maps to U1MODEbits.UARTEN (not IEC).
 *   - BSET 0x223,#2 maps to U1STAbits.UTXEN (not IEC).
 * ============================================================================ */
void initUART1(void)
{
    U1MODE = UART1_MODE_INIT;      /* exact image value from firmware */
    U1BRG  = UART1_BRG_INIT;       /* exact image value from firmware */
    U1STA  = 0x0000;               /* clear status/control before enable */

    /* clear UART1 interrupt flags and keep IRQ disabled (assembly BCLRs) */
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 0;

    /* final enable sequence from 0x557C/0x557E */
    U1MODEbits.UARTEN = 1;    /* module enable */
    U1STAbits.UTXEN   = 1;    /* transmitter enable */
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
    SPI2CON1 = SPI2CON1_INIT;  /* CKE=1 (data changes on active-to-idle clock edge),
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

    I2C2CON = I2C2CON_INIT;               /* I2CEN=1, SCLREL=1, STREN=1 */
    I2C2ADD = 0x0058;                     /* slave address 0x58 */

    IPC12bits.SI2C2IP = 3;                /* I2C2 slave interrupt priority = 3 */

    IFS3 = 0;                             /* clear all pending IFS3 flags */
    IEC3bits.SI2C2IE = 1;                 /* enable I2C2 slave interrupt */
}
