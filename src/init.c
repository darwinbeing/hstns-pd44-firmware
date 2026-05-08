/******************************************************************************* ******************************************************************************
MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any derivatives created by
any person or entity by or on your behalf, exclusively with Microchip’s products.  Microchip and its licensors
 retain all ownership and intellectual property rights in the accompanying software and in all derivatives hereto.
This software and any accompanying information is for suggestion only.  It does not modify Microchip’s standard warranty for its products.
You agree that you are solely responsible for testing the software and determining its suitability.  Microchip has no obligation to modify, test, certify,
or support the software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,
BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP’S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN
ANY APPLICATION.

IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS,
DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON
ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP
FOR THIS SOFTWARE.
MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.

*************************************************************************************************************************************************************/
#include <xc.h>

#include "p33Fxxxx.h"
#include "define.h"

extern void delay_100us(unsigned int delay);
extern void faultLoop();
extern unsigned int inputVoltage, outputVoltage;
extern int modifier;
extern unsigned char index_SR, faultState;
extern unsigned int synchRectDutyCycle[126][2];
extern unsigned int timerInterruptCount;
extern int16_t e_n, e_n1, e_n2, e_n3;
extern int32_t y_n1, y_n2, y_n;
extern int32_t n1, n2, n3;
extern int16_t d2, d3;
extern u16 adc_an0, adc_an2, adc_an0_prev, adc_an2_prev;
extern s16 voutSetpoint;
extern s16 u_exec;
extern s16 pdc1, pdc2, pdc3;
extern int16_t cal_a_gain, cal_a_offset;
extern int32_t integrator;
extern int16_t Iref, vref_ocp_adj, vref_ls;
extern volatile uint16_t systemState;
extern volatile uint16_t statusFlags;
extern volatile uint16_t protectionStatus;
extern volatile uint16_t runtimeFlags;
extern volatile uint16_t thermalFlags;
extern volatile uint16_t droopMode;
extern volatile uint16_t systemFlags;
extern volatile uint16_t auxFlags;
extern volatile uint16_t currentLimitFlags;
extern volatile uint16_t controlStatus;
extern volatile uint16_t pwmRunRequest;
extern volatile uint16_t pwmRunning;
extern volatile uint16_t pmbusAlertFlags;
extern volatile uint16_t faultResetTimer;
extern volatile uint16_t fanI2cAddr;
extern volatile uint16_t adcLiveA;
extern volatile uint16_t tempAdcValue;
extern volatile uint16_t fanDroopStepCnt;
extern uint16_t ocp_shutdown_counter;
extern uint16_t ocp_foldback_counter;
extern uint16_t ioutAdcRaw;
extern uint16_t ovp_counter;
extern uint16_t ocp_latch_threshold;
extern uint16_t ocp_hard_threshold;
extern uint16_t ocp_latch_delay;
extern uint16_t ocp_foldback_delay;
extern uint16_t ovp_threshold_normal;
extern uint16_t ovp_threshold_mode4;
extern uint16_t ovp_freq_ctrl_min;
unsigned char softStartFlag;

void initVars(void)
{
  softStartFlag = 0;
  timerInterruptCount = 0;

  e_n = 0;
  e_n1 = 0;
  e_n2 = 0;
  e_n3 = 0;

  y_n1 = 24000;
  y_n2 = 24000;
  y_n = 24000;

  n1 = Q15_FROM_FLOAT(-1.95197);
  n2 = Q15_FROM_FLOAT(2.424896);
  n3 = Q15_FROM_FLOAT(-0.69126);
  d2 = Q15_FROM_FLOAT(0.25742);
  d3 = Q15_FROM_FLOAT(-0.00742);

  adc_an0 = 0;
  adc_an2 = 0;
  adc_an0_prev = 0;
  adc_an2_prev = 0;
  voutSetpoint = 0;
  u_exec = 0x5DC0;
  pdc1 = 0x05F9;
  pdc2 = 0x05F9;
  pdc3 = SR_FIXED_PDC3;

  systemState = 2;
  statusFlags = 0;
  protectionStatus = 0;
  runtimeFlags = 0;
  thermalFlags = 0;
  droopMode = 0;
  systemFlags = 0;
  auxFlags = 0;
  currentLimitFlags = 0;
  controlStatus = 0;
  pwmRunRequest = (1u << 0);
  pwmRunning = (1u << 0);
  pmbusAlertFlags = 0;
  faultResetTimer = 0;
  fanI2cAddr = 0;
  adcLiveA = 0;
  tempAdcValue = 0;
  fanDroopStepCnt = 0;
  ocp_shutdown_counter = 0;
  ocp_foldback_counter = 0;
  ioutAdcRaw = OCP_DROOP3_THRESHOLD;
  ovp_counter = 0;
  ocp_latch_threshold = OCP_LATCH_THRESHOLD;
  ocp_hard_threshold = OCP_HARD_THRESHOLD;
  ocp_latch_delay = OCP_LATCH_DELAY;
  ocp_foldback_delay = OCP_FOLDBACK_DELAY;
  ovp_threshold_normal = OVP_THRESHOLD_NORMAL;
  ovp_threshold_mode4 = OVP_THRESHOLD_MODE4;
  ovp_freq_ctrl_min = OVP_FREQ_CTRL_MIN;

  cal_a_gain = 0x2030;
  cal_a_offset = (int16_t)0xFFF8;
  integrator = 0;
  Iref = 0xA6;
  vref_ocp_adj = 0;
  vref_ls = 0;
}

void initClock(void)
{
  /* Because LLC Full-Bridge converter is frequency controlled an external oscillator
     is used to provide the optimum tolerance over the specified operating temperature range */

  /* Configure Oscillator to operate the device near 50Mhz
     Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
     Fosc= 16*(50)/(2*4) = 100Mhz for Fosc, Fcy = 50Mhz */

  /* Configure PLL prescaler, PLL postscaler, PLL divisor */
  PLLFBD = 48; 			  /* M = PLLFBD + 2 */
  CLKDIVbits.PLLPOST = 0;   /* N1 = 2 */
  CLKDIVbits.PLLPRE = 2;    /* N2 = 4 */

  // FWDTEN 1 = Watchdog Timer is always enabled (LPRC oscillator cannot bedisabled;
  // clearing the SWDTEN bit in the RCON register will have no effect
  // RCONbits.SWDTEN = 0;                       /* Disable Watch Dog Timer*/

  __builtin_write_OSCCONH(0x03);			/* New Oscillator External Crystal w/ PLL */
  __builtin_write_OSCCONL(0x01);  		/* Enable Switch */

  while(OSCCONbits.COSC != 0b011);		/* Wait for new Oscillator to become External Crystal w/ PLL */
  while(OSCCONbits.LOCK != 1);			/* Wait for Pll to Lock */

  /* Now setup the ADC and PWM clock for 120MHz
     ((Internal FRC OSC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz*/

  ACLKCONbits.ASRCSEL = 0;				/* External Oscillator provides the clock for APLL */
  ACLKCONbits.FRCSEL = 1;					/* Selects FRC clock for auxiliary PLL */
  ACLKCONbits.SELACLK = 1;				/* Auxiliary Oscillator provides clock source for PWM & ADC */
  ACLKCONbits.APSTSCLR = 7;				/* Divide Auxiliary clock by 1 */
  ACLKCONbits.ENAPLL = 1;					/* Enable Auxiliary PLL */

  while(ACLKCONbits.APLLCK != 1);                         /* Wait for Auxiliary PLL to Lock */

}

void initTIMER()
{
  TMR1 = 0;
  PR1 = T1_PER;
  IPC0bits.T1IP = 2;
  IFS0bits.T1IF = 0;
  IEC0bits.T1IE = 1;
  T1CONbits.TON = 1;

  TMR2 = 0;
  PR2 = T2_PER;
  IPC1bits.T2IP = 4;
  IFS0bits.T2IF = 0;
  IEC0bits.T2IE = 1;
  T2CONbits.TON = 1;

  TMR4 = 0;
  PR4 = T4_PER;
  IFS1bits.T4IF = 0;
  IEC1bits.T4IE = 0;
  T4CONbits.TON = 1;
}

void initIOPorts(void)
{
  TRISBbits.TRISB7 = 1;

  TRISCbits.TRISC12 = 1;
  TRISCbits.TRISC13 = 1;
  TRISCbits.TRISC14 = 1;
  TRISCbits.TRISC15 = 1;
  LATC = 0;

  TRISDbits.TRISD2 = 0;
  TRISDbits.TRISD3 = 0;
  TRISDbits.TRISD4 = 0;
  TRISDbits.TRISD5 = 0;
  LATDbits.LATD3 = 1;
  ODCDbits.ODCD11 = 1;
  ODCDbits.ODCD0 = 1;

  // LED_ON();

  TRISE = 0;
  LATEbits.LATE3 = 1;

  TRISFbits.TRISF0 = 0;
  TRISFbits.TRISF1 = 0;
  TRISFbits.TRISF6 = 0;
  LATFbits.LATF6 = 1;

  TRISGbits.TRISG9 = 0;
  LATGbits.LATG9 = 1;
}

void pwm_force_off(void)
{
  /* Override PWM1H/L - full-bridge leg 1 */
  IOCON1bits.OVRENH = 1;
  Nop();
  Nop();
  Nop();
  IOCON1bits.OVRENL = 1;
  Nop();
  Nop();
  Nop();
  /* Override PWM2H/L - full-bridge leg 2 */
  IOCON2bits.OVRENH = 1;
  Nop();
  Nop();
  Nop();
  IOCON2bits.OVRENL = 1;
  Nop();
  Nop();
  Nop();

  /* Override PWM3H/L - synchronous rectifier */
  IOCON3bits.OVRENH = 1;
  Nop();
  Nop();
  Nop();
  IOCON3bits.OVRENL = 1;
}

void initPWM(void)
{

  TMR3 = 0;
  PR3 = T3_PER;
  T3CONbits.TON = 1;

  OC2R = 0;
  OC2RS = 400;
  OC2CONbits.OCM = 6;
  OC2CONbits.OCTSEL = 1;

  PTCONbits.EIPU = 1;

  // enable SEVTCMP interrupt
  IPC14bits.PSEMIP = 5; // set interrupt priority
  IEC3bits.PSEMIE = 1;  // enable interrupt

  SEVTCMP = 0;
  PTCON2bits.PCLKDIV = 3; // Clock divider = 2^n (n=0,1,2,3,4,5,6) don't use 1,5 or 6, see errata
  PTPER = SOFTSTARTPERIOD;

  /* PWM 1 Configuration */
  IOCON1bits.PENH = 1;                  /* PWM1H is controlled by I/O module */
  IOCON1bits.PENL = 1;                  /* PWM1L is controlled by I/O module */
  IOCON1bits.PMOD = 2;                  /* Push-Pull Mode */

  PWMCON1bits.FLTIEN = 1;
  PWMCON1bits.IUE = 1;                  /* Disable Immediate duty cycle updates */

#ifdef OPEN_LOOP
  PDC1 = PTPER  - DEADTIME;
#else
  PDC1 = 294;
#endif

  FCLCON1bits.FLTMOD = 0;               /* Latched Fault Mode */
  FCLCON1bits.FLTSRC = 2;               /* Fault Source FLT1 - Current */
  FCLCON1bits.FLTPOL = 0;               /* Active High */

  PHASE1 = 0;
  DTR1 = 0x2A;
  ALTDTR1 = 0x2A;

  /* PWM 2 Configuration */
  IOCON2bits.PENH = 1;                  /* PWM1H is controlled by I/O module */
  IOCON2bits.PENL = 1;                  /* PWM1L is controlled by I/O module */
  IOCON2bits.PMOD = 2;                  /* Push-Pull Mode */

  PWMCON2bits.FLTIEN = 1;
  PWMCON2bits.IUE = 1;                  /* Disable Immediate duty cycle updates */

#ifdef OPEN_LOOP
  PDC2 = PTPER  - DEADTIME;
#else
  PDC2 = 294;
#endif

  FCLCON2bits.FLTMOD = 0;               /* Latched Fault Mode */
  FCLCON2bits.FLTSRC = 2;               /* Fault Source FLT1 - Current */
  FCLCON2bits.FLTPOL = 0;               /* Active High */

  PHASE2 = 0;
  DTR2 = 0x2A;
  ALTDTR2 = 0x2A;


  /* PWM 3 Configuration */
  IOCON3bits.PENH = 1;                  /* PWM1H is controlled by I/O module */
  IOCON3bits.PENL = 1;                  /* PWM1L is controlled by I/O module */
  IOCON3bits.PMOD = 2;                  /* Push-Pull Mode */

  PWMCON3bits.FLTIEN = 1;
  PWMCON3bits.IUE = 1;                  /* Disable Immediate duty cycle updates */

#ifdef OPEN_LOOP
  PDC3 = PTPER  - DEADTIME;
#else
  PDC3 = 294;
#endif

  FCLCON3bits.FLTMOD = 0;               /* Latched Fault Mode */
  FCLCON3bits.FLTSRC = 2;               /* Fault Source FLT1 - Current */
  FCLCON3bits.FLTPOL = 0;               /* Active High */

  PHASE3 = 0x82;

  DTR3 = 0x2F;
  ALTDTR3 = 0x2F;

  /* PWM 5 Configuration */
  PWMCON5bits.ITB = 1;
  PWMCON5bits.DTC = 0b10;
  IOCON5bits.PENH = 1;
  IOCON5bits.PENL = 0;
  IOCON5bits.PMOD = 3; // Output Mode: 0=Complementary, 1=Redundant, 2=Push-Pull, 3=Independent
  PDC5 = 0x372;
  PHASE5 = 0x49B;

  pwm_force_off();

  //PWM enable:
  PTCONbits.PTEN = 1; // Enable the PWM Module

}

void initADC(void)
{

  ADCONbits.FORM = 0;                               /* Integer data format */
  ADCONbits.EIE = 0;                                /* Early Interrupt disabled */
  ADCONbits.SEQSAMP = 0;                            /* Select simultaneous sampling */
  ADCONbits.SLOWCLK = 1;                            /* Slow clock must be set */
  ADCONbits.ASYNCSAMP = 0;                          /* Dedicated S&H starts sampling when trigger detected */
  ADCONbits.ADCS = 7;                               /* ADC clock = FADC/6 = ~ 120MHz / 6 = 20MHz, 14*Tad = 1.4 MSPS, two SARs = 2.8 MSPS */

  ADSTAT = 0;
  ADBASE = 0;

  ADCPC0bits.TRGSRC0 = 0x1F;
  ADCPC0bits.TRGSRC1 = 0x1F;

  ADCPC1bits.TRGSRC2 = 0x1F;

  ADCPC2bits.TRGSRC4 = 0x1F;
  ADCPC2bits.TRGSRC5 = 0x1F;

  ADCPC3bits.TRGSRC6 = 0x1F;
  ADCPC3bits.TRGSRC7 = 0x1F;

  ADPCFGbits.PCFG7 = 1;

  ADCONbits.ADON = 1;				  /* Enable ADC now to allow time to stabilize */

}


void initCMP4(void)
{
  // configure DAC
  CMPCON4bits.DACOE = 1; // DAC output enable
  CMPCON4bits.RANGE = 1; // use AVdd/2 as referance

  CMPDAC4 = 0;

  CMPCON4bits.CMPON = 1;

}

void initCMP3(void)
{

  // configure DAC
  CMPCON3bits.INSEL = 2; // DAC output enable
  CMPCON3bits.RANGE = 1; // use AVdd/2 as referance

  CMPDAC3 = 0x1A3;

  CMPCON3bits.CMPON = 1;

}

void initI2C2(void)
{
  I2C2CONbits.STREN = 1;
  I2C2ADD = 0x58;
  IPC12bits.SI2C2IP = 3;

  IFS3 = 0;
  IEC3bits.SI2C2IE = 1;
}

void initUART(void)
{

  /* U1BRG */
  U1BRG = 0x28A;                  /* 4800 baud rate prescaler */

  /* Clear UART1 interrupt flags */
  IFS0bits.U1TXIF = 0;
  IFS0bits.U1RXIF = 0;

  /* Disable UART1 interrupts */
  IEC0bits.U1TXIE = 0;
  IEC0bits.U1RXIE = 0;

  /* U1MODE = 0x8006 */
  U1MODEbits.UARTEN = 1;           /* enable UART1 */
  U1MODEbits.PDSEL  = 0;           /* 9N1 9-bit, no parity, 1 stop bit */
  U1MODEbits.STSEL  = 0;           /* 1 stop bit */

  /* U1STA = 0x0400 */
  U1STAbits.UTXEN   = 1;           /* enable transmitter */

  /* U2BRG */
  U2BRG = 0x1A;                  /* 115200 baud rate prescaler */

  /* Clear UART2 interrupt flags */
  IFS1bits.U2TXIF = 0;
  IFS1bits.U2RXIF = 0;

  /* Disable UART2 interrupts */
  IEC1bits.U2TXIE = 0;
  IEC1bits.U2RXIE = 0;

  /* U2MODE = 0x8000 */
  U2MODEbits.UARTEN = 1;           /* enable UART2 */
  U2MODEbits.PDSEL  = 0;           /* 8N1 8-bit, no parity, 1 stop bit */
  U2MODEbits.STSEL  = 0;           /* 1 stop bit */

  /* U2STA = 0x0400 */
  U2STAbits.UTXEN   = 1;           /* enable transmitter */
}

void initSPI2(void)
{
  SPI2CON1bits.PPRE1 = 1;
  SPI2CON1bits.MSTEN = 1;
  SPI2CON1bits.CKE = 1;
  SPI2CON2 = 0;

  SPI2STATbits.SPIEN = 1;
}


#define FCY 50000000UL
#include <libpic30.h>

void delay_us(uint16_t us)
{
  __delay_us(us);
}

void delay_ms(uint16_t ms)
{
  __delay_ms(ms);
}
