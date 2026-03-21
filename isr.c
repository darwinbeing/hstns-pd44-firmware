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

int16_t e_n = 0, e_n1 = 0, e_n2 = 0, e_n3;
int32_t y_n1 = 24000, y_n2 = 24000;
int32_t y_n = 24000;

int32_t n1 = Q15_FROM_FLOAT(-1.95197);
int32_t n2 = Q15_FROM_FLOAT(2.424896);
int32_t n3 = Q15_FROM_FLOAT(-0.69126);
int16_t d2 = Q15_FROM_FLOAT(0.25742);
int16_t d3 = Q15_FROM_FLOAT(-0.00742);

/* --- ADC inputs --- */
u16  adc_an0 = 0;    /* AN0 raw sample (Vout path A)   */
u16  adc_an2 = 0;    /* AN2 raw sample (Vout path B)   */
u16  adc_an0_prev = 0;    /* AN0 raw sample (Vout path A)   */
u16  adc_an2_prev = 0;    /* AN2 raw sample (Vout path B)   */
s16  vfb_sum2ch;      /* (AN0 + AN2) * 2                */
s16  vref = VREF;          /* output voltage set-point       */
s16  comp_2p2z_vref;

/* --- Frequency control word (after clamping) --- */
s16  u_exec = 0x5DC0;        /* clamped output -> PTPER calc   */

/* --- Gain scheduling (Kff) --- */
s16  kff_vout;      /* Vout-proportional Kff base     */
s16  kff_gain;      /* dynamic gain factor G(n)
                     * 1024 = steady state  (0x400)
                     * 1500 = transient boost (0x5DC) x1.465      */

/* --- Computed PWM values (written to registers each cycle) --- */
s16  ptper;         /* switching period register      */
s16  pdc1 = 0x05F9;     /* PTPER - 8  (fine adjustment)   */
s16  pdc2 = 0x05F9;     /* PTPER - 8  (fine adjustment)   */
s16  pdc3;          /* SR duty cycle                  */
s16  dtr;           /* dead-time  (symmetric)         */


extern  int16_t adc_4pt_sum;       // DAT_ram_1d9e, 4-point moving average


static __attribute__((always_inline)) int16_t util_divsd(int32_t dividend, int16_t divisor)
{
        register int32_t result asm("w0") = dividend;

        __asm__ volatile (
            "push w1\n\t"
            "repeat #17\n\t"
            "div.sd %0, %1\n\t"
            "pop w1\n\t"
            : "+r"(result)
            : "r"(divisor)
            : "cc"
        );
        return (int16_t)result;
}


void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt()
{
        // timerInterruptCount ++; 	/* Increment interrupt counter */
        llc_adc_current_sample();
        llc_droop_trim_calc();
        llc_ocp_foldback();
        IFS0bits.T1IF = 0; 		/* Clear Interrupt Flag */
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt()
{
        // timerInterruptCount ++; 	/* Increment interrupt counter */

        s16 e_slew;
        s16 delta;

        s32 tmp;
        s16 kff_vout;
        s16 kff;

        /* Stage 1: u_exec -> kff_vout */
        tmp      = (u_exec * KFF_COEFF) >> 15;
        kff_vout = tmp - KFF_OFFSET;

        if      (kff_vout > (s16)KFF_MAX) kff_vout = (s16)KFF_MAX;
        else if (kff_vout < (s16)KFF_MIN) kff_vout = (s16)KFF_MIN;

        /* Stage 2: apply transient gain factor
         *   steady-state:  kff_gain=1024, result = kff_vout (unity scale)
         *   transient:     kff_gain=1500, result = kff_vout * 1.465       */
        kff_gain = 1024;

        adc_an0 = ADCBUF0;
        adc_an2 = ADCBUF2;
        vfb_sum2ch = adc_an0 * 2 + adc_an2 * 2;
        adc_4pt_sum = adc_an0 + adc_an0_prev + adc_an2 + adc_an2_prev;
        adc_an0_prev = adc_an0;
        adc_an2_prev = adc_an2;

        e_n = comp_2p2z_vref -  vfb_sum2ch;	    /* Find error */

        delta = e_n - e_n1;
        if (delta > 100) {                      /* rate limit up */
                e_n = e_n1 + 50;
        } else if (delta < -100) {             /* rate limit down */
                e_n = e_n1 - 50;
        }

        if (e_n > 100) {
                e_n = 100;
        } else if (e_n < -100) {
                e_n = -100;
        }

        kff = ((s32)kff_vout * (s32)kff_gain) >> 10;

        /* ---- A terms: d2*u(n-1) + d3*u(n-2) ---- */
        int32_t A =  __mulsi3((int32_t)d2, (int32_t)y_n1)
                        + __mulsi3((int32_t)d3, (int32_t)y_n2);

        /* ---- B terms: n1*e(n) + n2*e(n-1) + n3*e(n-2) ---- */
        int32_t B = __mulsi3((int32_t)n1, (int32_t)e_n)
                        + __mulsi3((int32_t)n2, (int32_t)e_n1)
                        + __mulsi3((int32_t)n3, (int32_t)e_n2);

        /* ---- Final: [(B * Kff) >> 7 + A] >> 13 ---- */
        s32 BxKff = __mulsi3(B, (s32)kff) >> 7;
        y_n    = (BxKff + A) >> 13;

        if(y_n > 25000) {
                y_n = 25000;
        } else if(y_n < 2600) {
                y_n = 2600;
        }

        u_exec = (s16)y_n;

        /* Protect divider */
        if (u_exec < U_EXEC_MIN) u_exec = U_EXEC_MIN;
        if (u_exec > U_EXEC_MAX) u_exec = U_EXEC_MAX;


        /* Switching period */
        int16_t period     = ((s16)util_divsd((s32)PWM_CLK_80, (s16)u_exec)) >> 1;
        // int16_t period     = ((s16)__builtin_divsd((s32)PWM_CLK_80, (s16)u_exec)) >> 1;
        ptper = period - 8;
        pdc1 = period;
        pdc2 = period;

        e_n1 = e_n;		/* Update previous voltage error */
        e_n2 = e_n1;
        e_n3 = e_n2;

        /* Upadation of previous Compensator outputs */
        y_n1 = y_n;
        y_n2 = y_n1;

        llc_current_fast_avg();
        llc_voltage_cal_ovp();
        
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
        // timerInterruptCount ++;    /* Increment interrupt counter */

        PTPER   = ptper;           /* period register (PDC-8)   */
        PDC1    = pdc1;            /* primary high-side: 100% DC   */
        PDC2    = pdc2;            /* primary low-side:  100% DC   */
        PDC3    = 0x36B;           /* SR duty cycle               */
        PHASE3  = 0;               /* SR phase offset             */
        DTR3    = 0x2F;            /* SR dead-time rising edge    */
        ALTDTR3 = 0x2F;            /* SR dead-time falling edge   */

        PTCONbits.SEIEN         = 0;   /* 0x0400 bit11 - disable Special Event interrupt enable */
        IEC3bits.PSEMIE         = 0;   /* 0x009A bit10 - disable PSEM interrupt                */
        IFS3bits.PSEMIF         = 0;   /* 0x008A bit10 - clear PSEM interrupt flag             */
}
