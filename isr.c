/******************************************************************************* ******************************************************************************
MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any derivatives created by 
any person or entity by or on your behalf, exclusively with Microchip?s products.  Microchip and its licensors
 retain all ownership and intellectual property rights in the accompanying software and in all derivatives hereto.  
This software and any accompanying information is for suggestion only.  It does not modify Microchip?s standard warranty for its products.  
You agree that you are solely responsible for testing the software and determining its suitability.  Microchip has no obligation to modify, test, certify, 
or support the software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE 
APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP?S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN 
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
#include <libpic30.h>

#include "p33Fxxxx.h"
#include "define.h"
#include "dsp.h"

volatile unsigned int timerInterruptCount = 0;




void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{
  // timerInterruptCount ++; 	/* Increment interrupt counter */
  IFS0bits.T1IF = 0; 		/* Clear Interrupt Flag */
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt()
{
  // timerInterruptCount ++; 	/* Increment interrupt counter */
    
  // voltage_cal_and_ovp();

  
  IFS0bits.T2IF = 0; 		/* Clear Interrupt Flag */
  IFS3bits.PSEMIF         = 0;  
  IEC3bits.PSEMIE         = 1;   
  PTCONbits.SEIEN         = 1;   
}

void __attribute__((__interrupt__, no_auto_psv)) _SI2C2Interrupt()
{
  // timerInterruptCount ++; 	/* Increment interrupt counter */
  IFS3bits.SI2C2IF = 0; 		/* Clear Interrupt Flag */
}

void __attribute__((__interrupt__, no_auto_psv)) _PWMSpEventMatchInterrupt()
{
  timerInterruptCount ++; 	/* Increment interrupt counter */
  
  PTPER   = 0x05F9;        /* period register (PTPER-8)   */
  PDC1    = 0x0611;            /* primary high-side: 50% DC   */
  PDC2    = 0x0611;            /* primary low-side:  50% DC   */
  PDC3    = 0x36B;            /* SR duty cycle               */
  PHASE3  = 0;                /* SR phase offset             */
  DTR3    = 0x2F;             /* SR dead-time rising edge    */
  ALTDTR3 = 0x2F;             /* SR dead-time falling edge   */  
  
  PTCONbits.SEIEN         = 0;   /* 0x0400 bit11 - disable Special Event interrupt enable */
  IEC3bits.PSEMIE         = 0;   /* 0x009A bit10 - disable PSEM interrupt                */
  IFS3bits.PSEMIF         = 0;   /* 0x008A bit10 - clear PSEM interrupt flag             */ 
}

