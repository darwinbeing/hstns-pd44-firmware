/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 RAM Variable Address Map
 * Extracted by decompiling firmware assembly
 *
 * dsPIC33FJ64GS606 Memory Map:
 *   0x0000 - 0x07FE  SFR Space (peripheral registers)
 *   0x0800 - 0x17FE  X Data RAM (4 KB)
 *   0x1800 - 0x1FFE  Y Data RAM (2 KB)
 *   0x2000 - 0x27FE  DMA RAM (1 KB)
 *   0x2800 - 0x2BFE  Unimplemented
 *   0x2C00 - 0x8000  Optional program memory mapped space
 * ============================================================================ */

#ifndef RAM_MAP_H
#define RAM_MAP_H

#include <stdint.h>

/* ---- Type aliases ---- */
typedef int8_t   s8;
typedef uint8_t  u8;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int32_t  s32;
typedef uint32_t u32;

/* ============================================================================
 * Firmware mode selection (used by ISR dispatcher)
 * ============================================================================ */
extern volatile uint16_t fw_mode;            /* 0x27FE - firmware operating mode
                                              *   mode 2 = normal control mode (segment 1)
                                              *   mode 3 = backup/redundant mode (segment 2)
                                              *   other  = initial/degraded mode            */

/* ============================================================================
 * Product identification
 * ============================================================================ */
extern char model_id[5];                     /* 0x1D08..0x1D0C "H05WD" */

/* ============================================================================
 * System state machine states
 * ============================================================================ */
#define ST_IDLE      0
#define ST_STARTUP   1
#define ST_ACTIVE    2
#define ST_FAULT     3
#define ST_HOLDOFF   5
#define ST_RELAY     6

/* ============================================================================
 * State machine & control flags
 * ============================================================================ */
extern volatile uint16_t systemState;           /* 0x1E22 - main state machine
                                              *   0=IDLE, 1=STARTUP, 2=ACTIVE
                                              *   3=FAULT, 5=STATE5, 6=STATE6 */
extern volatile uint16_t statusFlags;         /* 0x125A - main control flags word
                                              *   bit4  = running
                                              *   bit5  = regulation active
                                              *   bit7  = 12V present
                                              *   bit9  = OVP flag
                                              *   bit10 = OCV flag
                                              *   bit11 = steady-state detect  */
extern volatile uint16_t protectionStatus;         /* 0x1264 - status flags */
extern volatile uint16_t faultFlags;         /* 0x126A - mode/status flags
                                              *   bit1  = start request
                                              *   bit2  = shutdown request
                                              *   bit7  = over-temperature flag
                                              *   bit8  = OVP debounce
                                              *   bit9  = UVP debounce         */
extern volatile uint16_t thermalFlags;         /* 0x1266 - bit7 = OT flag */
extern volatile uint16_t droopMode;         /* 0x1268 */

extern volatile uint16_t systemFlags;         /* 0x1E1A
                                              *   bit0  = I2C request pending
                                              *   bit6  = Flash erase/readback trigger
                                              *   bit15 = fault state               */
extern volatile uint16_t fwUpdateFlags;         /* 0x1E1B - bit0 = firmware update active */
extern volatile uint16_t auxFlags;         /* 0x1E1C - bit1, bit11 status bits */
extern volatile uint16_t startupFlags;         /* 0x1E1D - bit0 = startup tick */
extern volatile uint16_t currentLimitFlags;         /* 0x1E18 - bit0 = current limiting active */
extern volatile uint16_t controlStatus;      /* 0x1E20 - bit1 = fault present */
extern volatile uint8_t  protStatusByte;         /* 0x1265 - status flags (byte, odd addr) */
extern volatile uint8_t  i2cStatusByte;         /* 0x1BEB - I2C status flags (byte, odd addr) */

/* ---- Control variables ---- */
extern volatile int16_t voutTargetCode;         /* 0x1D4E - output voltage target code */
extern volatile int16_t ocpCurrentRef;          /* 0x1DD2 - OCP current reference setpoint */
extern volatile int16_t vrefModeSelect;         /* 0x1D44 - voltage reference mode selector */
extern volatile int16_t delayTimer;             /* 0x1D42 - delay/countdown timer */
extern volatile int16_t countdownTimer;         /* countdown/wait timer (T1 ISR) */

/* ============================================================================
 * Voltage/current limits (initialized by initVars, some overwritten at runtime)
 * ============================================================================ */
extern volatile int16_t trim_avg_adc11;      /* 0x1E48 - ADCBUF11 32-sample avg (init=193) */
extern volatile int16_t trim_avg_adc1;       /* 0x1E4A - ADCBUF1  32-sample avg */
extern volatile int16_t trim_avg_adc10;      /* 0x1E4C - ADCBUF10 32-sample avg (init=193) */
extern int16_t ioutLimitHi;               /* 0x1E16 = 25 */
extern int16_t ioutLimitLo;               /* 0x1E12 = 25 */
extern int16_t ioutDefault;                /* 0x1D18 = 25 */
extern int16_t marginThreshold;             /* 0x1E10 = 10 */
extern int16_t freqSetpoint;               /* 0x1D3E = 350 (0x15E) */
extern int16_t oc2rs_shadow;                /* 0x1E38 = 400 */

/* ============================================================================
 * ADC & voltage measurement
 * ============================================================================ */
extern volatile int16_t adc_an0;             /* ADCBUF0 - Vout path A */
extern volatile int16_t adc_an2;             /* ADCBUF2 - Vout path B */
extern volatile int16_t prev_vout_a;         /* 0x1D9A */
extern volatile int16_t prev_vout_b;         /* 0x1D96 */
extern volatile int16_t vout_sum;            /* 0x1DA0 = 2*AN0 + 2*AN2 */
extern volatile int16_t vout_avg_sum;        /* 0x1D9E - 4-point average sum */
extern volatile int16_t voutReference;            /* 0x1DA4 - LLC frequency target */
extern volatile int16_t voutSetpoint;         /* 0x1D4C - voltage setpoint */
extern volatile int16_t voutCalibrated;            /* 0x1D66 - AN5 calibrated voltage */
extern volatile int16_t outputVoltage;            /* 0x1BD4 - output voltage ADC */

/* Calibrated voltage channels (from 64-point average + calibration) */
extern volatile int16_t vcal_a;              /* 0x1260 - calibrated Vout ch A (AN5) */
extern volatile int16_t vcal_b;              /* 0x125E - calibrated Vout ch B (AN3) */
extern volatile int16_t vcal_diff;           /* 0x126C - vcal_a - vcal_b */
extern volatile int16_t vraw_sum_b;          /* 0x1D3C - raw 64-point sum ch B */

/* 64-point voltage circular buffers */
extern volatile uint16_t vbuf_a[64];        /* AN5 (ch A) 64-sample buffer */
extern volatile uint16_t vbuf_b[64];        /* AN3 (ch B) 64-sample buffer */

/* Calibration coefficients (loaded from flash buffer 0x15E6-0x15F8) */
extern volatile int16_t cal_vb;             /* 0x1D2A - Vout ch B gain */
extern volatile int16_t ofs_vb;             /* 0x1D28 - Vout ch B offset */
extern volatile int16_t cal_va;             /* 0x1D32 - Vout ch A gain */
extern volatile int16_t ofs_va;             /* 0x1D30 - Vout ch A offset */
extern volatile int16_t cal_va2;            /* 0x1D2E - Vout ch A2 gain (AN5) */
extern volatile int16_t ofs_va2;            /* 0x1D2C - Vout ch A2 offset */
extern volatile int16_t cal_a_gain;         /* 0x1D26 - current gain */
extern volatile int16_t cal_a_offset;       /* 0x1D24 - current offset */
extern volatile int16_t cal_pdc5;           /* 0x1E44 - PDC5 calibration */
extern volatile int16_t cal_var_1E42;       /* 0x1E42 - auxiliary calibration */

/* ============================================================================
 * Voltage loop 2P2Z / PID
 * ============================================================================ */
extern volatile int16_t voltageError;       /* 0x1DE0 - e[n] */
extern volatile int16_t prevError;          /* 0x1DDE - e[n-1] */
extern volatile int16_t prevPrevError;     /* 0x1DDC - e[n-2] */
extern volatile int16_t var_1D4E;            /* 0x1D4E */
extern volatile uint16_t steadyCount;       /* 0x1DD2 - steady-state counter */
extern volatile int16_t droopPeriod;        /* 0x1DD6 */
extern volatile int16_t var_1D36;            /* 0x1D36 */

/* Q15 fixed-point format */
#define Q15_SHIFT   15
#define Q15_SCALE   (1L << Q15_SHIFT)
#define Q15_FROM_FLOAT(x)  ((int32_t)((x) * Q15_SCALE))

/* 2P2Z compensator coefficients
 * Transfer function: H(z) = (n1 + n2*z^-1 + n3*z^-2) / (1 - d2*z^-1 - d3*z^-2)
 */
#define COMP_N1     Q15_FROM_FLOAT(-1.95197)    /* numerator coeff for e[n] */
#define COMP_N2     Q15_FROM_FLOAT( 2.424896)   /* numerator coeff for e[n-1] */
#define COMP_N3     Q15_FROM_FLOAT(-0.69126)    /* numerator coeff for e[n-2] */
#define COMP_D2     Q15_FROM_FLOAT( 0.25742)    /* denominator coeff for y[n-1] */
#define COMP_D3     Q15_FROM_FLOAT(-0.00742)    /* denominator coeff for y[n-2] */

extern volatile int32_t compN3;             /* 0x1D82:0x1D84 - init = COMP_N3 */
extern volatile int32_t compN2;             /* 0x1D86:0x1D88 - init = COMP_N2 */
extern volatile int32_t compN1;             /* 0x1D8A:0x1D8C - init = COMP_N1 */
extern volatile int16_t compD1;             /* 0x1D8E - init = COMP_D3 */
extern volatile int16_t compD0;             /* 0x1D90 - init = COMP_D2 */
extern volatile int16_t compSetpoint;       /* 0x1D92 - compensator reference */

/* 2P2Z compensator accumulators (32-bit intermediate results) */
extern volatile int32_t compDenomAccum;     /* 0x1DE6:0x1DE8 - denominator accumulator */
extern volatile int32_t compNumerAccum;     /* 0x1DEA:0x1DEC - numerator accumulator */

/* 2P2Z compensator history (32-bit state variables) */
extern volatile int32_t compY_n2;           /* 0x1D76:0x1D78 - y[n-2] history */
extern volatile int32_t compY_n1;           /* 0x1D7A:0x1D7C - y[n-1] history */
extern volatile int32_t compY_out;          /* 0x1D7E:0x1D80 - y[n] output */

/* ============================================================================
 * Current measurement (multi-stage filtering)
 * ============================================================================ */
extern volatile int16_t iout_avg;            /* 0x1D46 - 8-point average */
extern volatile int16_t Imeas;               /* 0x1D44 - calibrated current (OCP) */
extern volatile int16_t Imeas_scaled;        /* 0x1D42 = Imeas * 0x1E80 >> 10 */
extern volatile int16_t Imeas_cal_a;         /* 0x1E4E - calibrated path A */
extern volatile int16_t Imeas_longavg;       /* 0x1D1E - 1024-point long-term average */
extern volatile int16_t outputCurrent;            /* 0x1BB2 - output current ADC */

/* Current 64-sample ring buffer & 1024-sample accumulator */
extern volatile int16_t iout_ring_idx;       /* 0x1100 - ring index (0..1023) */
extern volatile int32_t iout_accum;          /* 0x1102:0x1104 - running 1024-sample sum */
extern volatile int16_t iout_cal_raw;        /* 0x1106 - secondary calibrated value */
extern volatile int16_t iout_64avg;          /* 0x1108 - 64-sample calibrated average */
extern volatile int32_t ioutSum64;           /* 0x1256 - 64-sample current sum accumulator */
extern volatile int32_t  iout_4buf[64];     /* 64-sample x 32-bit current circular buffer */
extern volatile int16_t  iout_ring[1024];   /* 1024-sample current ring buffer */

/* Droop/trim ADC accumulators (32-sample batch) */
extern volatile int16_t trim_count;          /* 0x120A - sample counter (0..31) */
extern volatile int16_t trim_accum_adc1;     /* 0x120C - ADCBUF1 accumulator */
extern volatile int16_t trim_accum_adc10;    /* 0x120E - ADCBUF10 accumulator */
extern volatile int16_t trim_accum_adc11;    /* 0x1210 - ADCBUF11 accumulator */
extern volatile int16_t droopAdj;           /* 0x1212 */
extern volatile int16_t droopResult;        /* 0x1214 - PID droop output */

/* ============================================================================
 * OCP foldback PI controller
 * ============================================================================ */
extern volatile int16_t ocpError;           /* 0x1D50 - OCP error signal */
extern volatile int16_t ocpProportional;            /* 0x1D54 - proportional term */
extern volatile int16_t ocpIntegralLo;        /* 0x1D56 - integral term lo */
extern volatile int16_t ocpIntegralHi;        /* 0x1D58 - integral term hi */
extern volatile int32_t ocpAccumulator;           /* 0x1216:0x1218 - PI accumulator */
extern volatile int16_t ocpOutput;          /* 0x121A - clamped PI output */
extern volatile int16_t ocpOutputRaw;      /* 0x121C - unclamped PI output */
extern volatile int16_t ocpKiInput;        /* 0x1D52 - anti-windup gain */
extern volatile uint16_t ssDebounce;       /* 0x121E - soft-start debounce counter */
extern volatile uint16_t ocpTimer;         /* 0x1220 - OCP detection timer */
extern volatile uint16_t foldbackTimer;    /* 0x1222 - current-foldback hold timer */
extern volatile uint16_t oeTimer;          /* 0x1224 - output-enable soft-start counter */
extern volatile uint16_t edgeCount;        /* 0x1226 - PORTD bit6 edge accumulator */
extern volatile uint16_t portd6Cur;        /* 0x1228 - current PORTD bit6 sample */
extern volatile uint16_t portd6Prev;       /* 0x122A - previous PORTD bit6 sample */
extern volatile uint16_t stableCount;      /* 0x122C - stable period accumulator */
extern volatile uint16_t sampleCount;      /* 0x122E - measurement window sample count */
extern volatile int16_t vrefOcpAdj;        /* 0x1D38 - OCP adjustment to Vref */
extern volatile int16_t vref_ls;             /* 0x1D3A - droop compensation */
extern volatile int16_t var_1D4A;            /* 0x1D4A */
extern volatile uint16_t voutLoThresh;     /* 0x1D5A - Vout lower threshold for OE hysteresis */

/* ============================================================================
 * PWM register shadows
 * ============================================================================ */
extern volatile int16_t dtr3_shadow;         /* 0x1D6A - DTR3 + ALTDTR3 shadow (same value for both) */
extern volatile int16_t pdc3;                /* 0x1D6C - PDC3 shadow */
extern volatile int16_t pdc2;                /* 0x1D6E - PDC2 shadow */
extern volatile int16_t pdc1;                /* 0x1D70 - PDC1 shadow */
extern volatile int16_t ptper;               /* 0x1D72 - used for PHASE3 clamp comparison */
extern volatile int16_t ptper_computed;      /* 0x1D74 - PTPER shadow (written to PTPER register) */
extern volatile int16_t phase3_target;       /* 0x1E56 - PHASE3 target with ramp-down clamp */

/* ============================================================================
 * I2C2 -> Flash command interface (PC/BMC sends commands via I2C2 PMBus)
 * ============================================================================ */
extern volatile uint16_t flashCmdFlags;    /* 0x1928 - Flash operation flag bits
                                              *   bit0 = flashPageReadWrite
                                              *   bit1 = flashPageProgramRead
                                              *   bit4 = flashReadPage6
                                              *   bit5 = flashProgramRead32
                                              *   bit6 = flashReadPage7        */
extern volatile uint8_t  i2cRxPageNum;       /* 0x1921 - page number from I2C2 host */
extern volatile uint8_t  i2cRxDataLo;        /* 0x1922 - data byte lo from I2C2 host */
extern volatile uint8_t  i2cRxDataHi;        /* 0x1923 - data byte hi from I2C2 host */
extern volatile uint16_t flash_write_offset; /* 0x191E */
extern volatile uint8_t  flash_write_page_hi;/* 0x191F */

/* ============================================================================
 * Flash buffers
 * ============================================================================ */
extern uint8_t flash_buf_256[256];           /* 0x1294 - 256-byte page buffer */
extern uint8_t flash_buf_181E[256];          /* 0x181E - 256-byte read buffer */
extern uint8_t flash_buf_171E[256];          /* 0x171E - 256-byte read buffer */
extern uint8_t flash_sector_buf_1498[256];   /* 0x1498 - sector buffer */
extern uint8_t flash_sector_buf_1598[24];    /* 0x1598 - sector buffer 2 */
extern uint8_t flash_read_buf_15B0[32];      /* 0x15B0 */
extern uint8_t flash_read_buf_15D0[32];      /* 0x15D0 */
extern uint8_t flash_read_buf_15E6[32];      /* 0x15E6 */
extern uint8_t flash_read_buf_160E[256];     /* 0x160E */
extern uint16_t flash_data_160A;             /* 0x160A */
extern uint16_t flash_data_160C;             /* 0x160C */
extern uint16_t flash_page_addr;             /* 0x1606 */
extern uint16_t flash_expected_crc_lo;       /* 0x1388 */
extern uint16_t flash_expected_crc_hi;       /* 0x138C */
extern uint16_t flash_page_count;            /* 0x1390 */
extern uint16_t flash_crc_accum;             /* 0x1396 */
extern uint16_t at45db_status;               /* 0x1292 */
extern uint8_t  flash_verify_flags[3];       /* 0x128A-0x128C */

/* ============================================================================
 * Timers & counters
 * ============================================================================ */
extern volatile uint32_t tick_counter;       /* 0x1252:0x1254 - T1 sub-divider (0-63) */
extern volatile int16_t  delay_timer_1D40;   /* 0x1D40 */
extern volatile int16_t  countdown_1D20;     /* 0x1D20 */

/* ============================================================================
 * EEPROM emulation
 * ============================================================================ */
extern uint16_t eeprom_crc_lo_saved;         /* 0x27F0 */
extern uint16_t eeprom_page_addr;            /* 0x27F2 */
extern uint16_t eeprom_crc_lo;               /* 0x27F4 */
extern uint16_t eeprom_cfg_reg;              /* 0x27FA - configuration register
                                              *   bit0    = enable
                                              *   bit5:4  = mode (2-bit)
                                              *   bit7:6  = status (2-bit) */
extern uint16_t eeprom_var_27FC;             /* 0x27FC */

/* ============================================================================
 * State handler variables
 * ============================================================================ */
extern volatile uint16_t pwmSoftStartCnt;    /* 0x1CBE - PWM soft-start counter */
extern volatile uint16_t vinCooldownTimer;   /* 0x1CC0 - Vin monitor cooldown timer */
extern volatile uint16_t voutOvpThreshold;   /* 0x1CC2 - Vout OVP/UVP threshold (fan droop) */
extern volatile uint16_t voutFanSetpoint;    /* 0x1CC4 - Vout fan speed setpoint */
extern volatile uint16_t voutWorkValue;      /* 0x1CC6 - Vout working value */
extern volatile uint8_t  hwConfigByte;       /* 0x1BCC - PSU type / hardware ID byte */
extern volatile uint8_t  ocTripLevel;        /* 0x1BCD - Over-current trip level index */
extern volatile uint16_t startupTickCnt;     /* 0x1E0A - Startup tick counter */
extern volatile uint16_t standbyDebounce;    /* 0x1E0C - Standby-rail debounce counter */
extern volatile uint16_t faultResetTimer;    /* 0x1E0E - Fault soft-reset timer */
extern volatile uint16_t enableDebounce;     /* 0x1E02 - Enable-pin debounce counter */
extern volatile uint16_t pgoodDebounce;      /* 0x1E04 - Power-good debounce counter */
extern volatile uint16_t pgoodAssertCnt;     /* 0x1E06 - Power-good assertion counter */
extern volatile uint16_t fanI2cAddr;         /* 0x1E08 - Fan I2C address work register */
extern volatile uint16_t vinOcpLimit;        /* 0x1E14 - Vin OCP limit */
extern volatile uint16_t internalStatusFlags; /* 0x1E19 - Internal status flags
                                              *   bit0 = state5 init done
                                              *   bit3 = droop mode 3
                                              *   bit6 = droop active
                                              *   bit7 = OT shutdown latch */
extern volatile uint16_t restartFlags;       /* 0x1E1E - Restart/re-enable flags
                                              *   bit0 = AC-cycle restart latch
                                              *   bit1 = soft-start restart latch
                                              *   bit2 = OCP restart latch */
extern volatile uint16_t ocpTripFlags;       /* 0x1E21 - OCP trip flags (bit2 = trip) */
extern volatile uint16_t secondaryOcSetpoint; /* 0x1E3A - Secondary OC setpoint */
extern volatile uint16_t droopEnableFlags;   /* 0x1BEA - bit5 = droop enable */
extern volatile uint16_t startupResetLatch;  /* 0x1BEC - startup reset latch */
extern volatile uint16_t startupResetLatch2; /* 0x1BEE - startup reset latch 2 */
extern volatile uint16_t pwmRunRequest;      /* 0x1BF0 - bit0 = PWM run request */
extern volatile uint16_t pwmRunning;         /* 0x1BF2 - bit0 = PWM running
                                              *   bit3 = Vin present flag */
extern volatile uint16_t flashUpdateResult;  /* 0x1BBC - Flash update result */
extern volatile uint16_t statusFlags2;       /* 0x1262 - Status flags:
                                              *   bit0  = OVP latch
                                              *   bit3  = Vin UV latch
                                              *   bit5  = OT warn latch
                                              *   bit6  = OT shutdown latch
                                              *   bit14 = enable-pin high */
extern volatile uint16_t ovpDebounceFlags;   /* 0x1263 - bit5 = OVP debounce, bit6 = active */
extern volatile uint16_t droopWorkA;         /* 0x126E - Droop work reg A */
extern volatile uint16_t droopWorkB;         /* 0x1270 - Droop work reg B */
extern volatile uint16_t pmbusAlertFlags;    /* 0x192A - bit0 = PMBus alert */
extern volatile uint16_t tempAdcValue;       /* 0x1D16 - temperature ADC value */
extern volatile uint16_t voutRefInitial;     /* 0x1DA2 - Vout ref initial */
extern volatile uint16_t unusedPad;          /* 0x1DDF - unused pad */
extern volatile uint16_t vinUvSubCounter;    /* 0x1DEE - Vin UV sub-counter (ms ticks) */
extern volatile uint16_t vinUvSecCounter;    /* 0x1DF0 - Vin UV seconds counter */
extern volatile uint16_t state5MsCounter;    /* 0x1DF2 - state5 ms tick counter */
extern volatile uint16_t state5SecCounter;   /* 0x1DF4 - state5 seconds counter */
extern volatile uint16_t ovpMultResultLo;    /* 0x1DF6 - Vout OVP multiply result lo */
extern volatile uint16_t ovpMultResultHi;    /* 0x1DF8 - Vout OVP multiply result hi */
extern volatile uint16_t ovpDebounceCnt;     /* 0x1DFA - Vout OVP debounce counter */
extern volatile uint16_t ovpTripCounter;     /* 0x1DFC - Vout OVP trip counter */
extern volatile uint16_t uptimeCounterLo;    /* 0x1DFE - Uptime counter lo */
extern volatile uint16_t uptimeCounterHi;    /* 0x1E00 - Uptime counter hi */

/* ============================================================================
 * T1 subsystem variables
 * ============================================================================ */
extern volatile uint16_t pdcShadowA;         /* 0x1BDE - PDC shadow A */
extern volatile uint16_t pdcShadowB;         /* 0x1BD0 - PDC shadow B */
extern volatile uint16_t oc1rsGateTiming;    /* 0x1926 - OC1RS / gate timing */
extern volatile uint16_t llcFreqPeriod;      /* 0x1BE0 - LLC frequency period */
extern volatile uint16_t pdc3Shadow;         /* 0x1BCE - PDC3 shadow */
extern volatile uint16_t oc2rsGateTiming;    /* 0x1924 - OC2RS / gate timing 2 */
extern volatile uint16_t ocpThresholdHw;     /* 0x1BB6 - OCP threshold (HW) */
extern volatile uint16_t llcHwFault;         /* 0x1BF1 - LLC HW fault - bit4/bit5 */
extern volatile uint16_t llcStatus;          /* 0x1BF4 - LLC status - bit8 = inhibit */
extern volatile uint8_t  fclSeqState;        /* 0x124A - FCL sequencer state byte */
extern volatile uint16_t ovpUvpLatchTarget;  /* 0x1248 - OVP/UVP latch clear target */
extern volatile uint16_t fanTickCounterLo;   /* 0x1274 - fan tick counter lo */
extern volatile uint16_t fanTickCounterHi;   /* 0x1276 - fan tick counter hi */
extern volatile uint16_t fanSpeedAccum;      /* 0x1272 - fan speed accumulator */
extern volatile uint16_t fanVerifyCountLo;   /* 0x128A - fan verify count lo */
extern volatile uint16_t fanVerifyCountHi;   /* 0x128C - fan verify count hi */
extern volatile uint16_t uartRxByteCount;    /* 0x1CF8 - UART1 RX byte counter */
extern volatile uint16_t softStartRampCnt;   /* 0x124E - soft-start ramp counter */
extern volatile uint16_t softStartDwellCnt;  /* 0x124C - soft-start dwell counter */
extern volatile uint16_t softStartPwmLimit;  /* 0x1D5C - current soft-start PWM limit */
extern volatile int16_t  adcAn0Raw;          /* 0x1D9C - AN0 raw save */
extern volatile int16_t  adcAn2Raw;          /* 0x1D98 - AN2 raw save */
extern volatile uint16_t droopBoostFlags;    /* 0x126B - droop boost flags */
extern volatile int16_t  initFreq;           /* 0x1D34 - initial frequency */

/* ============================================================================
 * ISR T2 variables
 * ============================================================================ */
extern volatile int16_t  droopKffFactor;     /* 0x1DA6 - Kff scaling factor */
extern volatile int16_t  freqTarget;         /* 0x1DAE - target frequency */
extern volatile int16_t  freqCoeffA;         /* 0x1DA8 - freq sweep coeff A */
extern volatile int16_t  freqCoeffB;         /* 0x1DAA - freq sweep coeff B */
extern volatile int16_t  freqCoeffC;         /* 0x1DAC - freq sweep coeff C */
extern volatile int32_t  softStartCounter;   /* 0x1DE2 - soft-start 32-bit counter */
extern volatile int16_t  uvpDebounce;        /* 0x1DD4 - UVP debounce counter */
extern volatile int16_t  hbSeqState;         /* 0x1DDA - half-bridge sequencing state */
extern volatile int16_t  ocpDetCount;        /* 0x1DD0 - OCP detection counter */
extern volatile int16_t  latfTimer;          /* 0x1DD8 - LATF off-timer */

/* ============================================================================
 * T2 subroutine variables
 * ============================================================================ */
extern volatile int16_t  debounceAdc12;      /* 0x1DB2 - ADCBUF12 debounce counter */
extern volatile int16_t  ioutRing8pt[8];     /* 0x1DB4 - 8-point current ring buffer */
extern volatile int16_t  ocRampCounter;      /* 0x1DB0 - OC ramp counter */
extern volatile int16_t  adcBuf4Raw;         /* 0x1D94 - ADCBUF4 raw save */
extern volatile int16_t  ioutRingIdx8pt;     /* 0x1DC4 - 8-point ring buffer index */
extern volatile int16_t  ioutRunSum;         /* 0x1D68 - running sum for 8-pt avg */
extern volatile int16_t  faultEdgeCnt;       /* 0x1DC6 - fault edge detection counter */
extern volatile int16_t  portdBit1State;     /* 0x1DC8 - PORTD bit1 state */
extern volatile int16_t  portdBit0State;     /* 0x1D1A - PORTD bit0 state */
extern volatile int16_t  ovpCounter;         /* 0x1DCA - OVP counter */
extern volatile int16_t  freqLimitActive;    /* 0x1DCE - frequency limit active flag */
extern volatile int16_t  pwm3PenhFlag;       /* 0x1DCC - PWM3 PENH flag */

/* ============================================================================
 * Peak tracking (stored max values, persisted to flash page 0)
 * peakTracking[0]=0x1278 .. peakTracking[8]=0x1288
 * Each slot compared against a live ADC telemetry register
 * ============================================================================ */
extern volatile uint16_t peakTracking[9];       /* 0x1278..0x1288 */

/* ============================================================================
 * Flash verify / control
 * ============================================================================ */
extern volatile uint16_t flashChecksumResult;   /* 0x128E - 13-bit page-6 checksum bitmask */
extern volatile uint16_t flashCtrlFlags;        /* 0x1290 - bit1=init guard, bit3=page-blank */

/* ============================================================================
 * Live ADC telemetry (compared against peakTracking[])
 * ============================================================================ */
extern volatile uint16_t adcLiveA;              /* 0x1BE8 - peak group A (vs peakTracking[8]) */
extern volatile uint16_t adcLiveA1;             /* 0x1BE6 - (vs peakTracking[7] indirectly) */
extern volatile uint16_t adcLiveA2;             /* 0x1BE4 - (vs peakTracking[6] indirectly) */
extern volatile uint16_t adcLive1;              /* 0x1BE2 - (vs peakTracking[5]) */
extern volatile uint16_t adcLive2;              /* 0x1BDC - (vs peakTracking[4]) */
extern volatile uint16_t adcLive3;              /* 0x1BDA - (vs peakTracking[3]) */
extern volatile uint16_t adcLive4;              /* 0x1BD8 - (vs peakTracking[2]) */
extern volatile uint16_t adcLive5;              /* 0x1BD6 - (vs peakTracking[1]) */
extern volatile uint16_t adcLiveB;              /* 0x1BD2 - (vs peakTracking[7]) */
extern volatile uint16_t adcLiveC;              /* 0x1BC8 - (vs peakTracking[6]) */

/* ============================================================================
 * I2C / PMBus operational variables
 * ============================================================================ */
extern volatile uint16_t ioutVoutTarget;        /* 0x1BB0 - Iout/Vout readback target */
extern volatile uint16_t ioutAdcRaw;            /* 0x1BB4 - Iout ADC raw value */
extern volatile uint16_t eepromPageShadow;      /* 0x1BB8 - shadow of eeprom_page_addr */
extern volatile uint16_t eepromCrcShadow;       /* 0x1BBA - shadow of eeprom CRC */
extern volatile uint16_t ioutScaleConst;        /* 0x1BC6 - Iout scale constant */
extern volatile uint16_t ioutCalFactor;         /* 0x1BCA - Iout cal factor (word) */
extern volatile uint16_t eepromSavedShadow;     /* 0x198E - shadow of eeprom_crc_lo_saved */

/* ============================================================================
 * UART1 subsystem
 * ============================================================================ */
extern volatile uint16_t uartRxBuf[13];         /* 0x1CC8 - RX data buffer (words) */
extern volatile uint16_t uartRxBufIdx;          /* 0x1CE0 - RX buffer fill index */
extern volatile uint16_t uartRxState;           /* 0x1CE2 - RX state machine */
extern volatile uint16_t uartTxPending;         /* 0x1CE4 - TX packet pending flag */
extern volatile uint16_t uartTxChecksum;        /* 0x1CE6 - TX checksum accumulator */
extern volatile uint16_t uartTxPacketLen;       /* 0x1CE8 - TX packet byte count */
extern volatile uint16_t uartTxPrescaler;       /* 0x1CEA - TX rate limiter counter */
extern volatile uint16_t uartTxByteIdx;         /* 0x1CEC - TX current byte index */
extern volatile uint8_t  uartTxBuf[9];          /* 0x1CEE - TX packet buffer */
extern volatile uint8_t  uartStreamSrc[7];      /* 0x1CF9 - streaming source buffer */
extern volatile uint16_t uartStatusWord;        /* 0x1D0E - status payload for TX */
extern volatile uint16_t uartCmdParam0;         /* 0x1D12 - parsed cmd parameter 0 */
extern volatile uint16_t uartCmdParam1;         /* 0x1D10 - parsed cmd parameter 1 */
extern volatile uint16_t uartCmdParamExt;       /* 0x1D14 - extended cmd parameter */

/* ============================================================================
 * OC2RS control
 * ============================================================================ */
extern volatile uint16_t oc2rsUpperLimit;       /* 0x1E3C - OC2RS hard upper clamp */
extern volatile uint16_t oc2rsUpdateCnt;        /* 0x1E52 - divide-by-10 rate limiter */
extern volatile uint16_t oc2rsTarget;           /* 0x1E54 - computed OC2RS target value */

/* ============================================================================
 * I2C2 protocol variables
 * ============================================================================ */
extern volatile uint16_t rxBufIndex;            /* 0x1E40 - PMBus Rx byte index */
extern volatile uint16_t rxPendFlag;            /* 0x1E3E - Rx pending flag */
extern volatile uint16_t rxEventFlags;          /* 0x1940 - Rx event flags (bit1-3) */
extern volatile uint8_t  rxPacketBuf[34];       /* 0x1958 - PMBus Rx packet buffer */
extern volatile uint8_t  rxAddrByte;            /* 0x1979 - received I2C address */
extern volatile uint16_t txCtrlWord;            /* 0x1950 - Tx control word */
extern volatile uint16_t txByteCount;           /* 0x192C - Tx remaining bytes */
extern volatile uint16_t txChecksumAccum;       /* 0x1956 - Tx checksum accumulator */
extern volatile uint16_t txByteCntPreset;       /* 0x1952 - Tx byte-count preset */
extern volatile uint16_t txSubReg;              /* 0x192E - Tx sub-register */
extern volatile uint16_t txBusStateFlags;       /* 0x194E - Tx-ready/bus-state flags */
extern volatile uint16_t rxAuxFlags;            /* 0x1948 - Rx auxiliary flags */
extern volatile uint16_t i2cPeriodCnt;          /* 0x197C - I2C2 period counter */
extern volatile uint16_t i2cTxCounter;          /* 0x19A4 - I2C2 transaction counter */
extern volatile uint16_t i2cTickCntLo;          /* 0x1936 - 32-bit tick counter lo */
extern volatile uint16_t i2cTickCntHi;          /* 0x1938 - 32-bit tick counter hi */
extern volatile uint16_t i2cBusLockCnt;         /* 0x193A - bus-lock error counter */
extern volatile uint16_t i2cAccumLo;            /* 0x194A - 32-bit accum lo */
extern volatile uint16_t i2cAccumHi;            /* 0x194C - 32-bit accum hi */
extern volatile uint16_t fanDroopStepCnt;       /* 0x193C - fan/droop step counter */
extern volatile uint16_t startupResetShadow;    /* 0x193E - shadow of startupResetLatch */
extern volatile uint8_t  eventCntBit4;          /* 0x1946 */
extern volatile uint8_t  eventCntBit5;          /* 0x1947 */
extern volatile uint8_t  eventCntBit6;          /* 0x1944 */
extern volatile uint8_t  eventCntBit7;          /* 0x1945 */
extern volatile uint8_t  eventCntBit8;          /* 0x1942 */

/* ============================================================================
 * PMBus calibration / config registers
 * ============================================================================ */
extern volatile uint16_t pmbusReadPtr0;         /* 0x19A0 */
extern volatile uint16_t pmbusReadPtr1;         /* 0x19A2 */
extern volatile uint16_t voutCalE;              /* 0x199E */
extern volatile uint16_t voutCalD;              /* 0x199C */
extern volatile uint16_t tempLinearFmt;         /* 0x199A */
extern volatile uint16_t voutCalC;              /* 0x1998 */
extern volatile uint16_t voutCalB;              /* 0x1996 */
extern volatile uint16_t voutCalA;              /* 0x1994 */
extern volatile uint16_t voutScaleA;            /* 0x1992 */
extern volatile uint16_t voutScaleB;            /* 0x1990 */
extern volatile uint16_t pmbusCfgReg0;          /* 0x197E */
extern volatile uint16_t pmbusCfgReg1;          /* 0x1982 */
extern volatile uint16_t pmbusCfgReg2;          /* 0x1984 */
extern volatile uint16_t pmbusCfgReg3;          /* 0x1986 */
extern volatile uint16_t pmbusCfgReg4;          /* 0x1988 */
extern volatile uint16_t pmbusCfgReg5;          /* 0x198A */
extern volatile uint16_t pmbusCfgReg6;          /* 0x198C */
extern volatile uint16_t pmbusDataReg0;         /* 0x19A6 */
extern volatile uint16_t pmbusDataReg1;         /* 0x19A8 */
extern volatile uint16_t pmbusDataReg2;         /* 0x19AA */
extern volatile uint16_t pmbusDataReg3;         /* 0x19AC */
extern volatile uint16_t pmbusDataReg4;         /* 0x19AE */
extern volatile uint16_t pmbusLiveBC0;          /* 0x1BC0 */
extern volatile uint16_t pmbusLive197A;         /* 0x197A */

/* ============================================================================
 * I2C2 pointer tables (hold RAM addresses for PMBus command dispatch)
 * ============================================================================ */
extern volatile uint16_t ptrTable19B0[3];
extern volatile uint16_t ptrTable19BA[3];
extern volatile uint16_t ptrTable19D0[8];
extern volatile uint16_t ptrTable19F0[8];
extern volatile uint16_t ptrTable1A10[30];
extern volatile uint16_t ptrTable1A70[5];
extern volatile uint16_t ptrTable1B90[7];

/* ============================================================================
 * T1 sub-protection variables (used by t1_sub_prot.c)
 * ============================================================================ */
extern volatile uint16_t protTempAdc;           /* 0x1BBE - temperature/current threshold comparison */
extern volatile uint16_t protCounter1230;       /* 0x1230 - ready-flag debounce counter */
extern volatile uint16_t protCounter1232;       /* 0x1232 - temp-limit debounce counter */
extern volatile uint16_t protCounter1234;       /* 0x1234 - standby entry counter */
extern volatile uint16_t protSetpoint1236;      /* 0x1236 - frequency ramp setpoint */
extern volatile uint16_t protVar1238;           /* 0x1238 - protection debounce counter */
extern volatile uint16_t protVar123A;           /* 0x123A - temperature debounce counter */
extern volatile uint16_t protVar123C;           /* 0x123C - current-limit debounce counter */
extern volatile uint16_t protVar123E;           /* 0x123E - clampToward target */
extern volatile uint16_t protVar1240;           /* 0x1240 - precise regulation ref */
extern volatile uint16_t protVar1242;           /* 0x1242 - droop offset */
extern volatile uint16_t protVar1244;           /* 0x1244 - voltage ramp value */
extern volatile uint16_t protVar1246;           /* 0x1246 - calibrated voltage intermediate */

/* ============================================================================
 * Constant definitions
 * ============================================================================ */
#define VREF_NOMINAL    0xC2F       /* 3119 */
#define VREF_MIN        0x7ED
#define VREF_MAX        0xF0F
#define PWM_CLK_80      0xB3FB00u   /* 11,796,224 */

/* Vin sense ADC shadow (was odd-address RAM read at 0x1A51) */
extern volatile uint16_t vinSenseAdc;

/* Temperature-to-current-limit lookup table (was at 0x1BF6, 198 entries) */
extern uint8_t tempLimitLut[198];

/* Library functions */
extern int32_t __mulsi3(int32_t a, int32_t b);

#endif /* RAM_MAP_H */
