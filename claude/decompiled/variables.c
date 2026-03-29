/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — variables.c
 * Storage definitions for all extern variables declared in ram_map.h
 * ============================================================================ */
#include "variables.h"

/* Firmware mode */
volatile uint16_t fw_mode;

/* Product identification */
char model_id[5];

/* State machine & control flags */
volatile uint16_t systemState;
volatile uint16_t statusFlags;
volatile uint16_t protectionStatus;
volatile uint16_t faultFlags;
volatile uint16_t thermalFlags;
volatile uint16_t droopMode;
volatile uint16_t systemFlags;
volatile uint16_t fwUpdateFlags;
volatile uint16_t auxFlags;
volatile uint16_t startupFlags;
volatile uint16_t currentLimitFlags;
volatile uint16_t controlStatus;
volatile uint8_t  protStatusByte;
volatile uint8_t  i2cStatusByte;

/* Voltage/current limits */
volatile int16_t trim_avg_adc11;
volatile int16_t trim_avg_adc1;
volatile int16_t trim_avg_adc10;
int16_t ioutLimitHi;
int16_t ioutLimitLo;
int16_t ioutDefault;
int16_t marginThreshold;
int16_t freqSetpoint;
int16_t oc2rs_shadow;

/* ADC & voltage measurement */
volatile int16_t adc_an0;
volatile int16_t adc_an2;
volatile int16_t prev_vout_a;
volatile int16_t prev_vout_b;
volatile int16_t vout_sum;
volatile int16_t vout_avg_sum;
volatile int16_t llcPeriodCmd;
volatile int16_t voutSetpoint;
volatile int16_t voutCalibrated;
volatile int16_t outputVoltage;
volatile int16_t vcal_a;
volatile int16_t vcal_b;
volatile int16_t vcal_diff;
volatile int16_t vraw_sum_b;

/* Calibration coefficients */
volatile int16_t cal_vb;
volatile int16_t ofs_vb;
volatile int16_t cal_va;
volatile int16_t ofs_va;
volatile int16_t cal_va2;
volatile int16_t ofs_va2;
volatile int16_t cal_a_gain;
volatile int16_t cal_a_offset;
volatile int16_t cal_pdc5;
volatile int16_t cal_var_1E42;

/* Voltage loop 2P2Z / PID */
volatile int16_t voltageError;
volatile int16_t prevError;
volatile int16_t prevPrevError;
volatile int16_t voutTargetCode;
volatile uint16_t steadyCount;
volatile int16_t droopPeriod;
volatile int16_t ocpCurrentRef;

/* 2P2Z compensator coefficients */
volatile int32_t compN3;            /* n3 = Q15(-0.69126) */
volatile int32_t compN2;            /* n2 = Q15(2.424896) */
volatile int32_t compN1;            /* n1 = Q15(-1.95197) */
volatile int16_t compD1;            /* d3 = Q15(-0.00742) */
volatile int16_t compD0;            /* d2 = Q15(0.25742) */
volatile int16_t compSetpoint;      /* compensator reference */

/* 2P2Z compensator accumulators */
volatile int32_t compDenomAccum;
volatile int32_t compNumerAccum;

/* 2P2Z compensator history (32-bit) */
volatile int32_t compY_n2;
volatile int32_t compY_n1;
volatile int32_t compY_out;

/* Current measurement */
volatile int16_t iout_avg;
volatile int16_t Imeas;
volatile int16_t Imeas_scaled;
volatile int16_t Imeas_cal_a;
volatile int16_t Imeas_longavg;
volatile int16_t outputCurrent;
volatile int16_t iout_ring_idx;
volatile int32_t iout_accum;
volatile int16_t iout_cal_raw;
volatile int16_t iout_64avg;
volatile int32_t ioutSum64;
volatile int32_t  iout_4buf[64];
volatile int16_t  iout_ring[1024];
volatile uint16_t vbuf_a[64];
volatile uint16_t vbuf_b[64];

/* Droop/trim ADC accumulators */
volatile int16_t trim_count;
volatile int16_t trim_accum_adc1;
volatile int16_t trim_accum_adc10;
volatile int16_t trim_accum_adc11;
volatile int16_t droopAdj;
volatile int16_t droopResult;

/* OCP foldback PI controller */
volatile int16_t ocpError;
volatile int16_t ocpProportional;
volatile int16_t ocpIntegralLo;
volatile int16_t ocpIntegralHi;
volatile int32_t ocpAccumulator;
volatile int16_t ocpOutput;
volatile int16_t ocpOutputRaw;
volatile int16_t ocpKiInput;
volatile uint16_t ssDebounce;
volatile uint16_t ocpTimer;
volatile uint16_t foldbackTimer;
volatile uint16_t oeTimer;
volatile uint16_t edgeCount;
volatile uint16_t portd6Cur;
volatile uint16_t portd6Prev;
volatile uint16_t stableCount;
volatile uint16_t sampleCount;
volatile int16_t vrefOcpAdj;
volatile uint16_t voutLoThresh;
volatile int16_t vref_ls;
volatile int16_t vrefModeSelect;

/* PWM register shadows */
volatile int16_t dtr3_shadow;
volatile int16_t pdc3;
volatile int16_t pdc2;
volatile int16_t pdc1;
volatile int16_t ptper;
volatile int16_t ptperCommand;
volatile int16_t phase3_target;

/* I2C2 -> Flash command interface */
volatile uint16_t flashCmdFlags;
volatile uint8_t  i2cRxPageNum;
volatile uint8_t  i2cRxDataLo;
volatile uint8_t  i2cRxDataHi;
volatile uint16_t flash_write_offset;
volatile uint8_t  flash_write_page_hi;

/* Flash buffers */
uint8_t flash_buf_256[256];
uint8_t flash_buf_181E[256];
uint8_t flash_buf_171E[256];
uint8_t flash_sector_buf_1498[256];
uint8_t flash_sector_buf_1598[24];
uint8_t flash_read_buf_15B0[32];
uint8_t flash_read_buf_15D0[32];
uint8_t flash_read_buf_15E6[32];
uint8_t flash_read_buf_160E[256];
uint16_t flash_data_160A;
uint16_t flash_data_160C;
uint16_t flash_page_addr;
uint16_t flash_expected_crc_lo;
uint16_t flash_expected_crc_hi;
uint16_t flash_page_count;
uint16_t flash_crc_accum;
uint16_t at45db_status;
uint8_t  flash_verify_flags[3];

/* Timers & counters */
volatile uint32_t tick_counter;
volatile int16_t  delayTimer;
volatile int16_t  countdownTimer;
volatile int16_t  delay_timer_1D40;
volatile int16_t  countdown_1D20;
volatile int16_t  voutRefTarget;

/* State handler variables */
volatile uint16_t pwmSoftStartCnt;
volatile uint16_t vinCooldownTimer;
volatile uint16_t voutOvpThreshold;
volatile uint16_t voutFanSetpoint;
volatile uint16_t voutWorkValue;
volatile uint8_t  hwConfigByte;
volatile uint8_t  ocTripLevel;
volatile uint16_t startupTickCnt;
volatile uint16_t standbyDebounce;
volatile uint16_t faultResetTimer;
volatile uint16_t enableDebounce;
volatile uint16_t pgoodDebounce;
volatile uint16_t pgoodAssertCnt;
volatile uint16_t fanI2cAddr;
volatile uint16_t vinOcpLimit;
volatile uint16_t internalStatusFlags;
volatile uint16_t restartFlags;
volatile uint16_t ocpTripFlags;
volatile uint16_t secondaryOcSetpoint;
volatile uint16_t droopEnableFlags;
volatile uint16_t startupResetLatch;
volatile uint16_t startupResetLatch2;
volatile uint16_t pwmRunRequest;
volatile uint16_t pwmRunning;
volatile uint16_t flashUpdateResult;
volatile uint16_t statusFlags2;
volatile uint16_t ovpDebounceFlags;
volatile uint16_t droopWorkA;
volatile uint16_t droopWorkB;
volatile uint16_t pmbusAlertFlags;
volatile uint16_t tempAdcValue;
volatile uint16_t voutRefInitial;
volatile uint16_t unusedPad;
volatile uint16_t vinUvSubCounter;
volatile uint16_t vinUvSecCounter;
volatile uint16_t state5MsCounter;
volatile uint16_t state5SecCounter;
volatile uint16_t ovpMultResultLo;
volatile uint16_t ovpMultResultHi;
volatile uint16_t ovpDebounceCnt;
volatile uint16_t ovpTripCounter;
volatile uint16_t uptimeCounterLo;
volatile uint16_t uptimeCounterHi;

/* T1 subsystem variables */
volatile uint16_t pdcShadowA;
volatile uint16_t pdcShadowB;
volatile uint16_t oc1rsGateTiming;
volatile uint16_t llcFreqPeriod;
volatile uint16_t pdc3Shadow;
volatile uint16_t oc2rsGateTiming;
volatile uint16_t ocpThresholdHw;
volatile uint16_t llcHwFault;
volatile uint16_t llcStatus;
volatile uint8_t  fclSeqState;
volatile uint16_t ovpUvpLatchTarget;
volatile uint16_t fanTickCounterLo;
volatile uint16_t fanTickCounterHi;
volatile uint16_t fanSpeedAccum;
volatile uint16_t fanVerifyCountLo;
volatile uint16_t fanVerifyCountHi;
volatile uint16_t uartRxByteCount;
volatile uint16_t softStartRampCnt;
volatile uint16_t softStartDwellCnt;
volatile uint16_t softStartPwmLimit;
volatile int16_t  adcAn0Raw;
volatile int16_t  adcAn2Raw;
volatile uint16_t droopBoostFlags;
volatile int16_t  initFreq;

/* ISR T2 variables */
volatile int16_t  droopKffFactor;
volatile int16_t  freqTarget;
volatile int16_t  freqCoeffA;
volatile int16_t  freqCoeffB;
volatile int16_t  freqCoeffC;
volatile int32_t  softStartCounter;
volatile int16_t  uvpDebounce;
volatile int16_t  hbSeqState;
volatile int16_t  ocpDetCount;
volatile int16_t  latfTimer;

/* T2 subroutine variables */
volatile int16_t  debounceAdc12;
volatile int16_t  ioutRing8pt[8];
volatile int16_t  ocRampCounter;
volatile int16_t  adcBuf4Raw;
volatile int16_t  ioutRingIdx8pt;
volatile int16_t  ioutRunSum;
volatile int16_t  faultEdgeCnt;
volatile int16_t  portdBit1State;
volatile int16_t  portdBit0State;
volatile int16_t  ovpCounter;
volatile int16_t  freqLimitActive;
volatile int16_t  pwm3PenhFlag;

/* Peak tracking */
volatile uint16_t peakTracking[9];

/* Flash verify/control */
volatile uint16_t flashChecksumResult;
volatile uint16_t flashCtrlFlags;

/* Live ADC telemetry */
volatile uint16_t adcLiveA;
volatile uint16_t adcLiveA1;
volatile uint16_t adcLiveA2;
volatile uint16_t adcLive1;
volatile uint16_t adcLive2;
volatile uint16_t adcLive3;
volatile uint16_t adcLive4;
volatile uint16_t adcLive5;
volatile uint16_t adcLiveB;
volatile uint16_t adcLiveC;

/* I2C/PMBus operational */
volatile uint16_t ioutVoutTarget;
volatile uint16_t ioutAdcRaw;
volatile uint16_t eepromPageShadow;
volatile uint16_t eepromCrcShadow;
volatile uint16_t ioutScaleConst;
volatile uint16_t ioutCalFactor;
volatile uint16_t eepromSavedShadow;

/* UART1 subsystem */
volatile uint16_t uartRxBuf[13];
volatile uint16_t uartRxBufIdx;
volatile uint16_t uartRxState;
volatile uint16_t uartTxPending;
volatile uint16_t uartTxChecksum;
volatile uint16_t uartTxPacketLen;
volatile uint16_t uartTxPrescaler;
volatile uint16_t uartTxByteIdx;
volatile uint8_t  uartTxBuf[9];
volatile uint8_t  uartStreamSrc[7];
volatile uint16_t uartStatusWord;
volatile uint16_t uartCmdParam0;
volatile uint16_t uartCmdParam1;
volatile uint16_t uartCmdParamExt;

/* OC2RS control */
volatile uint16_t oc2rsUpperLimit;
volatile uint16_t oc2rsUpdateCnt;
volatile uint16_t oc2rsTarget;

/* I2C2 protocol */
volatile uint16_t rxBufIndex;
volatile uint16_t rxPendFlag;
volatile uint16_t rxEventFlags;
volatile uint8_t  rxPacketBuf[34];
volatile uint8_t  rxAddrByte;
volatile uint16_t txCtrlWord;
volatile uint16_t txByteCount;
volatile uint16_t txChecksumAccum;
volatile uint16_t txByteCntPreset;
volatile uint16_t txSubReg;
volatile uint16_t txBusStateFlags;
volatile uint16_t rxAuxFlags;
volatile uint16_t i2cPeriodCnt;
volatile uint16_t i2cTxCounter;
volatile uint16_t i2cTickCntLo;
volatile uint16_t i2cTickCntHi;
volatile uint16_t i2cBusLockCnt;
volatile uint16_t i2cAccumLo;
volatile uint16_t i2cAccumHi;
volatile uint16_t fanDroopStepCnt;
volatile uint16_t startupResetShadow;
volatile uint8_t  eventCntBit4;
volatile uint8_t  eventCntBit5;
volatile uint8_t  eventCntBit6;
volatile uint8_t  eventCntBit7;
volatile uint8_t  eventCntBit8;

/* PMBus calibration/config */
volatile uint16_t pmbusReadPtr0;
volatile uint16_t pmbusReadPtr1;
volatile uint16_t voutCalE;
volatile uint16_t voutCalD;
volatile uint16_t tempLinearFmt;
volatile uint16_t voutCalC;
volatile uint16_t voutCalB;
volatile uint16_t voutCalA;
volatile uint16_t voutScaleA;
volatile uint16_t voutScaleB;
volatile uint16_t pmbusCfgReg0;
volatile uint16_t pmbusCfgReg1;
volatile uint16_t pmbusCfgReg2;
volatile uint16_t pmbusCfgReg3;
volatile uint16_t pmbusCfgReg4;
volatile uint16_t pmbusCfgReg5;
volatile uint16_t pmbusCfgReg6;
volatile uint16_t pmbusDataReg0;
volatile uint16_t pmbusDataReg1;
volatile uint16_t pmbusDataReg2;
volatile uint16_t pmbusDataReg3;
volatile uint16_t pmbusDataReg4;
volatile uint16_t pmbusLiveBC0;
volatile uint16_t pmbusLive197A;

/* I2C2 pointer tables */
volatile uint16_t ptrTable19B0[3];
volatile uint16_t ptrTable19BA[3];
volatile uint16_t ptrTable19D0[8];
volatile uint16_t ptrTable19F0[8];
volatile uint16_t ptrTable1A10[30];
volatile uint16_t ptrTable1A70[5];
volatile uint16_t ptrTable1B90[7];

/* T1 sub-protection variables */
volatile uint16_t protTempAdc;
volatile uint16_t protCounter1230;
volatile uint16_t protCounter1232;
volatile uint16_t protCounter1234;
volatile uint16_t protSetpoint1236;
volatile uint16_t protVar1238;
volatile uint16_t protVar123A;
volatile uint16_t protVar123C;
volatile uint16_t protVar123E;
volatile uint16_t protVar1240;
volatile uint16_t protVar1242;
volatile uint16_t protVar1244;
volatile uint16_t protVar1246;

/* EEPROM emulation */
uint16_t eeprom_crc_lo_saved;
uint16_t eeprom_page_addr;
uint16_t eeprom_crc_lo;
uint16_t eeprom_cfg_reg;
uint16_t eeprom_var_27FC;

/* Vin sense ADC shadow (was odd-address RAM read at 0x1A51) */
volatile uint16_t vinSenseAdc;

/* Temperature-to-current-limit lookup table (was at 0x1BF6, 198 entries) */
uint8_t tempLimitLut[198];
