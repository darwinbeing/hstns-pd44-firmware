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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ *
** ADDITIONAL NOTES:       LLC SeriesResonant converter without output Inductor
*************************************************************************************************************************************************************/
#include <xc.h>
#include "p33Fxxxx.h"
#include "define.h"

/* Configuration Bits */

// Device: dsPIC33FJ64GS606
// Config bits extracted from HSTNS-PD44 hardware via PICkit 5 read

// FBS - Boot Segment
#pragma config BWRP = WRPROTECT_OFF
#pragma config BSS  = NO_FLASH

// FGS - General Code Segment
#pragma config GWRP = OFF
#pragma config GSS  = OFF

// FOSCSEL - Oscillator Source Selection
#pragma config FNOSC = PRIPLL
#pragma config IESO  = ON

// FOSC - Oscillator Configuration
#pragma config POSCMD  = XT
#pragma config OSCIOFNC = OFF
#pragma config FCKSM   = CSECMD

// FWDT - Watchdog Timer
#pragma config WDTPOST = PS2048
#pragma config WDTPRE  = PR32
#pragma config WINDIS  = OFF
#pragma config FWDTEN  = ON
// #pragma config FWDTEN  = OFF

// FPOR - Power-on Reset
#pragma config FPWRT  = PWR32
#pragma config ALTSS1 = ON
#pragma config ALTQIO = OFF

// FICD - In-Circuit Debugger
#pragma config ICS    = PGD2
#pragma config JTAGEN = OFF

// FCMP - Comparator
#pragma config HYST0   = HYST45
#pragma config CMPPOL0 = POL_FALL
#pragma config HYST1   = HYST45
#pragma config CMPPOL1 = POL_FALL

extern void initClock(void);
extern void initIOPorts(void);
extern void initPWM(void);
extern void initADC(void);
void initI2C2(void);
void initSPI2(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void initTIMER();
void initCMP4(void);
void initCMP3(void);
void initUART(void);

extern s16  vref;

int main(void)
{

        initClock();                            				/* Initialize Primary and Auxiliary oscillators */

        initCMP4();
        initCMP3();

	initIOPorts();								/* Setup LEDs and other I/O Ports */

	initADC();								/* Setup ADC module and ADC triggering */

        initTIMER();

        initPWM();								/* Initialize Half-bridge and synchronous rectification PWMs */

        initI2C2();

        initUART();

        initSPI2();

        SR &= 0x1F;

        LATDbits.LATD3 = 0;
        LATDbits.LATD5 = 1;
        LATFbits.LATF6 = 0;
        LATFbits.LATF0 = 0;

        LED_ON();
        /* Override PWM1H/L - full-bridge leg 1 */
        IOCON1bits.OVRENH = 0;
        Nop();
        Nop();
        Nop();
        IOCON1bits.OVRENL = 0;
        Nop();
        Nop();
        Nop();
        /* Override PWM2H/L - full-bridge leg 2 */
        IOCON2bits.OVRENH = 0;
        Nop();
        Nop();
        Nop();
        IOCON2bits.OVRENL = 0;
        Nop();
        Nop();
        Nop();

        /* Override PWM3H/L - synchronous rectifier */
        IOCON3bits.OVRENH = 1;
        Nop();
        Nop();
        Nop();
        IOCON3bits.OVRENL = 0;

        vref = 0xFFAF;

        while(1) {

                if(vref < (VREF - 30)) vref += 10;

                ClrWdt();
                Nop();
                Nop();
                Nop();
                Nop();
                Nop();
	}

}
