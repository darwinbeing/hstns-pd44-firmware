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


#include "p33Fxxxx.h"

#include <stdint.h>
#include <stdbool.h>

/* ============================================================
 * Type aliases
 * ============================================================ */

typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int64_t  s64;


#define TRUE              1
#define FALSE             0
#define ON                1
#define OFF               0

// If OPEN_LOOP is defined then the frequency and duty cycle will be fixed. The period is defined according to
// SOFTSTARTPERIOD below. Faults will be disabled. When in open loop there is not a soft-start routine so do not start
// the converter with much load

//#define OPEN_LOOP                             1


// This #define allows the use of two arrays and various variables for dmci debugging/viewing in the ADCP0 interrupt service routine.

#define dmciVariables                           1


#define LED_FAULT           			LATBbits.LATB12
#define LED_1         		 		LATDbits.LATB2
#define LED_2         		 		LATBbits.LATB11
#define AUXILIARY_START                         LATBbits.LATB15

#define LED_ON()     do { LATDbits.LATD2 = 1; } while(0)
#define LED_OFF()    do { LATDbits.LATD2 = 0; } while(0)
#define LED_TOGGLE() do { LATDbits.LATD2 ^= 1; } while(0)

#define FAULT_INPUTVOLTAGE			1                                           // Input under or over voltage
#define FAULT_OVERCURRENT			2                                           // Output over current
#define FAULT_OUTPUTOVERVOLTAGE                 3                                           // 12V output over voltage
#define FAULT_OUTPUTUNDERVOLTAGE		4                                           // 12V output under voltage
#define FAULT_OVERTEMP    			5                                           // Over temperature fault
#define FAULT_SOFTSTART				6                                           // Failed Soft-start


#define PCBTEMP_MAX 	  			375                                         // PCB Temperature Check
										  	    // Declare Fault if PCB temperature is greater than 55C
											    // Vout = (Ta * Tc) + V0C, where Ta is the ambient temperature
										 	    // Tc is the temperature coefficent and V0C is the voltage at 0C.
											    // Vout = (70C * 10mV/C) + .5V = 1.2V, Convert to ADC reading
											    // (1.2 * 1023) / 3.3 = 372. Add some factor


#define MINPERIOD_NOLOAD			2850                                        // Use switching frequency to determine min/max input voltage
#define MINPERIOD_HALFLOAD          		2920
#define MINPERIOD_LOAD				3100
#define MAXPERIOD_NOLOAD			2000
#define MAXPERIOD_HALFLOAD          		2150
#define MAXPERIOD_LOAD				2240
#define OUTPUTVOLTAGE_MIN			24074                                         // Output voltage check
                                                                                              // [1.5k / (1.5k + 4.99k)] * 10.5
											      // Convert to ADC reading and Q15 format

#define OUTPUTVOLTAGE_MAX			30952                                         // Output voltage check
                                                                                              // [1.5k / (1.5k + 4.99k)] * 13.5
                                                                                              // Convert to ADC reading and Q15 format

#ifdef OPEN_LOOP

#define SOFTSTARTPERIOD                         2350
												//2940		~160kHz
												//2850		~165kHz
												//2760		~170kHz
												//2690		~175kHz
												//2620		~180kHz
												//2550		~185kHz
												//2480		~190kHz
												//2400		~195kHz
												//2350		~200kHz
												//2290		~205kHz
												//2230		~210kHz
												//2180		~215kHz
												//2130		~220kHz
												//2080		~225kHz
												//2040		~230kHz
												//2000		~235kHz
												//1960		~240kHz
												//1920		~245kHz
												//1880		~250kHz
												//1840		~255kHz

#else

#define SOFTSTARTPERIOD                         286                    // PTPER = ((1 / 300kHz) / 1.07ns) = 3115 / 2 (push-pull) = 1558 where 300kHz
								        //  is the desired initial switching frequency for the softstart.
                                                                        //  The Gate Drive Transformers maximum rated switching frequency (300kHz)


#endif

#define MAXDUTYCYCLE				1300                    // Soft start period minus dead time

#define RESONANTPERIOD				2232                    // Resonant Period

#define MINPERIOD				1870                    // MINPERIOD = PTPER = ((1 / 250kHz) / 1.07ns) / 2 = 1870, where 250kHz
									//	is the maximum operating range

#define MAXPERIOD                               3222                    // MAXPERIOD = PTPER = ((1 / 145kHz) / 1.07ns) / 2 = 3222, where 145kHz
									// is the minimum allowed as defined by the gain of the converter and input
									// voltage operating range (ensure inductive region)

#define DEADTIME				250                     // Dead-time of Halh-Bridge Converter
#define SRDEADTIME				350			// Dead-time for Synchronous Rectifier

#define SOFTSTARTMINVOLTAGE			14000                   // The minimum output voltage at startup before changing the frequency
									// is greater than 8V (@350V input full load) 300kHz switching frequency)

#define SOFTSTARTMAXVOLTAGE			26366                   // Correspondes to 11.5V output

#define VREF                                    3149                   // Reference voltage is from resistor divider circuit
                                                                        // (1.5kOhm / (1.5kOhm + 4.99kOhm)) * 12V = 2.774V
                                                                        // Now calculate expected ADC value (2.774V * 1023)/3.3V = 860
								    	// Then left shift by 5 for Q15 format (860 * 32) = 27520 = 0x6B80


#define BASEPERIOD                              2200                     // Model uses Push-Pull mode so BASE PERIOD is chosen close to resonant frequency of the LLC
#define NOMINALPERIOD                           5140                     // 180kHz

//Coefficient for Digital(3P3Z)Compensator
//#define n1 Q15(0.2711)
//#define n2 Q15(0.178)
//#define n3 Q15(0.1828)
//#define n4 Q15(0.2663)
//#define d2 Q15(0.6791)
//#define d3 Q15(0.7342)
//#define d4 Q15( 0.4133)

#define Q15_SHIFT 15
#define Q15_SCALE (1L << Q15_SHIFT)
#define Q15_FROM_FLOAT(x) ((int32_t)((x) * Q15_SCALE))

// #define n1 (int32_t)Q15_FROM_FLOAT(-1.95197)
// #define n2 (int32_t)Q15_FROM_FLOAT(2.424896)
// #define n3 (int32_t)Q15_FROM_FLOAT(-0.69126)
// #define d2 (int16_t)Q15_FROM_FLOAT(0.25742)
// #define d3 (int16_t)Q15_FROM_FLOAT(-0.00742)

#define SPPIDOutputCLAMP (32767)
#define PRESCALER 1


#define T1_PER 10000
#define T2_PER 1000
#define T3_PER 2000
#define T4_PER 50000



/* ============================================================
 * Constants
 * ============================================================ */

/* Error limiting */
#define ERR_SLEW_LIMIT      100     /* max |e(n) - e(n-1)| per cycle       */
#define ERR_MAX             100     /* hard upper clamp on error            */
#define ERR_MIN            (-100)   /* hard lower clamp on error            */

/* Frequency control word operating range */
#define U_EXEC_MIN         2600     /* lowest  allowed value  (~208 kHz)    */
#define U_EXEC_MAX        24000     /* highest allowed value  (safety)      */

/* Voltage-mode lower clamp values for u_exec */
#define U_TARGET_BURST     3000     /* burst mode  (reduced set-point)      */
#define U_TARGET_LIGHT     3400     /* no-load     (~272 kHz)               */
#define U_TARGET_NORMAL    3200     /* full load   (~256 kHz)               */
#define U_TARGET_LIGHT_THR 0x340    /* vin_adc threshold: light vs normal   */

/* Gain scheduling (Kff) parameters */
#define KFF_COEFF          1101     /* 0x44D - numerator slope              */
#define KFF_OFFSET           74     /* 0x4A  - intercept subtracted         */
#define KFF_MIN              44     /* 0x2C  - lower clamp on kff_vout      */
#define KFF_MAX             128     /* 0x80  - upper clamp on kff_vout      */
#define KFF_GAIN_NORMAL    1024     /* 0x400 - steady-state gain factor     */
#define KFF_GAIN_BOOST     1500     /* 0x5DC - transient boost  (~x1.465)   */

/* Transient gain boost */
#define TRANS_THR_POS        20     /* positive transient arm threshold     */
#define TRANS_THR_NEG      (-25)    /* negative transient arm threshold     */
#define TRANS_DURATION       19     /* boost holds for 19 cycles (~760 us)  */

/* Synchronous rectifier */
#define SR_DISABLE_THR     6799     /* u_exec <= this -> SR off             */
#define SR_FIXED_PDC3      0x36B    /* SR duty cycle at the disable boundary*/
#define PHASE3_SLEW          10     /* max PHASE3 decrease per cycle        */

/* Over-current protection */
#define OCP_THRESHOLD      0x800    /* vout_sum OCP level                   */
#define OCP_DURATION      25000     /* OCP must persist this many cycles    */

/* Frequency calculation constant
 *   0xB3FB00 = 11,796,224 = PWM_CLK_HZ / scaling_factor
 *   PTPER = FREQ_CONST / u_exec / 2                                        */
#define PWM_CLK_80      0xB3FB00u


#define VREF_NOMINAL  0xC2F     
#define VREF_MIN      0x7ED     
#define VREF_MAX      0xF0F     

int32_t __mulsi3(int32_t A, int32_t B);
int16_t __builtin_divsd(const int32_t num, const int16_t den);
