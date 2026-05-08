#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UNIT_TEST_MINIMAL 1

#include <xc.h>
#include "../../decompiled/variables.h"

/* ---- Minimal global definitions used by the tested decompiled functions ---- */
uint16_t ADCBUF0;
uint16_t ADCBUF1;
uint16_t ADCBUF2;
uint16_t ADCBUF3;
uint16_t ADCBUF4;
uint16_t ADCBUF5;
uint16_t ADCBUF10;
uint16_t ADCBUF11;
uint16_t ADCBUF12;
uint16_t ADCBUF13;
uint16_t ADCBUF14;
uint16_t ADCBUF15;
uint16_t PDC5;
uint16_t PORTD;
uint16_t PORTG;
uint16_t TRISF;
uint16_t I2C2CON;
uint16_t I2C2ADD;
uint16_t I2C2TRN;
uint16_t I2C2RCV;
uint16_t I2C2STAT;
uint16_t IPC12;
uint16_t IFS3;
uint16_t IEC3;
volatile portd_bits_t PORTDbits;
volatile latg_bits_t LATGbits;
volatile i2c2stat_bits_t I2C2STATbits;

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

volatile int16_t vcal_a;
volatile int16_t vcal_b;
volatile int16_t vcal_diff;
volatile int16_t vraw_sum_b;
volatile uint16_t vbuf_a[64];
volatile uint16_t vbuf_b[64];
volatile uint32_t tick_counter;

volatile int16_t Imeas;
volatile int16_t Imeas_scaled;
volatile int16_t Imeas_cal_a;
volatile int16_t Imeas_longavg;
volatile int16_t iout_ring_idx;
volatile int32_t iout_accum;
volatile int16_t iout_cal_raw;
volatile int16_t iout_64avg;
volatile int32_t ioutSum64;
volatile int32_t iout_4buf[64];
volatile int16_t iout_ring[1024];

volatile int16_t trim_count;
volatile int16_t trim_accum_adc1;
volatile int16_t trim_accum_adc10;
volatile int16_t trim_accum_adc11;
volatile int16_t trim_avg_adc1;
volatile int16_t trim_avg_adc10;
volatile int16_t trim_avg_adc11;
volatile int16_t droopAdj;
volatile int16_t droopResult;

volatile int16_t ocpCurrentRef;
volatile int16_t ocpError;
volatile int16_t ocpProportional;
volatile int16_t ocpIntegralLo;
volatile int32_t ocpAccumulator;
volatile int16_t ocpOutput;
volatile int16_t ocpOutputRaw;
volatile int16_t ocpKiInput;

volatile int16_t vout_sum;
volatile int16_t vrefModeSelect;
volatile int16_t vref_ls;
volatile int16_t vrefOcpAdj;

int8_t flash_read_buf_15E6[32];

volatile uint16_t pwmRunning;
volatile uint16_t thermalFlags;
volatile uint16_t systemState;
volatile uint16_t droopMode;
volatile uint16_t systemFlags;
volatile uint16_t pmbusAlertFlags;
volatile uint16_t statusFlags;
volatile uint16_t auxFlags;
volatile uint16_t currentLimitFlags;
volatile uint16_t controlStatus;

volatile int16_t debounceAdc12;
volatile int16_t ocRampCounter;
volatile int16_t voutSetpoint;
int16_t marginThreshold;

volatile int16_t adcBuf4Raw;
volatile int16_t ioutRing8pt[8];
volatile int16_t ioutRingIdx8pt;
volatile int16_t ioutRunSum;
volatile int16_t iout_avg;

volatile uint16_t ssDebounce;
int16_t freqSetpoint;

uint8_t flash_sector_buf_1598[24];
uint8_t flash_read_buf_15B0[32];
uint8_t flash_read_buf_15D0[32];
uint16_t eeprom_crc_lo_saved;
uint16_t eeprom_page_addr;
uint16_t eeprom_crc_lo;
uint16_t eeprom_cfg_reg;
volatile uint16_t flashCmdFlags;
uint8_t flash_buf_181E[256];
uint8_t flash_program_scratch[16];
uint8_t flash_buf_171E[256];
uint8_t flash_read_buf_160E[256];
uint16_t flashBlockByteCount;
uint16_t flash_data_160A;
uint16_t flash_data_160C;
uint16_t flash_page_addr;
volatile uint16_t flash_write_offset;
volatile uint16_t flashBlockPageWord;
volatile uint16_t flashBlockLastOffset;

volatile uint16_t llcStatus;
volatile uint16_t droopEnableFlags;
volatile uint16_t eepromCrcShadow;
volatile uint16_t flashUpdateResult;
volatile uint16_t ioutCalFactor;
volatile uint16_t ioutCalFactorShadow;
volatile uint16_t rxBufIndex;
volatile uint16_t rxPendFlag;
volatile uint16_t rxEventFlags;
volatile uint8_t rxPacketBuf[34];
volatile uint8_t rxAddrByte;
volatile uint16_t txCtrlWord;
volatile uint16_t txByteCount;
volatile uint16_t txChecksumAccum;
volatile uint16_t txByteCntPreset;
volatile uint16_t txSubReg;
volatile uint16_t txBusStateFlags;
volatile uint16_t i2cRxData;
volatile uint8_t pmbusStringBuf[9];
volatile uint8_t pmbusInfoBytes[10];

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
volatile uint16_t ioutScaleConst;
volatile uint16_t ocpThresholdHw;
volatile uint16_t ioutAdcRaw;
volatile int16_t outputCurrent;
const uint16_t crc16_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
};

static int g_pwm_override_calls;

void pwmOverrideEnable(void)
{
    g_pwm_override_calls++;
}

void shutdownPwm(void)
{
    g_pwm_override_calls++;
}

int32_t __mulsi3(int32_t a, int32_t b)
{
    return a * b;
}

#include "../../decompiled/voltage_loop.c"
#include "../../decompiled/pwm.c"
#include "../../decompiled/protection.c"
#include "../../decompiled/flash.c"
#include "../../decompiled/adc.c"

void eepromCfgUpdate(uint8_t mode, uint8_t status, uint8_t enable)
{
    eepromSetConfig(mode, status, enable);
}

uint16_t eepromCfgRead(void)
{
    return eeprom_cfg_reg;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wattributes"
/* Host GCC cannot compile dsPIC ISR attributes; the PMBus ISR itself is not exercised here. */
#define __attribute__(x)
#include "../../decompiled/pmbus.c"
#undef __attribute__
#pragma GCC diagnostic pop

/* ---- Tiny test framework ---- */
static int g_failures = 0;
static int g_tests_run = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("FAIL: %s (line %d): %s\n", __func__, __LINE__, msg); \
            g_failures++; \
            return; \
        } \
    } while (0)

#define ASSERT_EQ_I32(exp, got, msg) \
    do { \
        int32_t _e = (int32_t)(exp); \
        int32_t _g = (int32_t)(got); \
        if (_e != _g) { \
            printf("FAIL: %s (line %d): %s (exp=%ld got=%ld)\n", \
                   __func__, __LINE__, msg, (long)_e, (long)_g); \
            g_failures++; \
            return; \
        } \
    } while (0)

#define RUN_TEST(fn) \
    do { \
        int _before = g_failures; \
        fn(); \
        g_tests_run++; \
        if (g_failures == _before) { \
            printf("PASS: %s\n", #fn); \
        } \
    } while (0)

static void reset_state(void);

static int parse_int_arg(const char *flag, const char *raw, int *out)
{
    char *end = NULL;
    long v;
    if (raw == NULL || raw[0] == '\0') {
        fprintf(stderr, "missing value for %s\n", flag);
        return -1;
    }
    v = strtol(raw, &end, 0);
    if (end == NULL || *end != '\0') {
        fprintf(stderr, "invalid integer for %s: %s\n", flag, raw);
        return -1;
    }
    *out = (int)v;
    return 0;
}

static int run_ocp_sweep_mode(int argc, char **argv)
{
    int vout_start = 0;
    int vout_end = 7000;
    int vout_step = 20;
    int imeas = 500;
    int iref = 100;
    int ki_input = 30;
    int accum_init = 0;
    int settle = 1;
    int dynamic = 0;

    int i;
    for (i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--vout-start") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--vout-start", argv[++i], &vout_start) != 0) return 2;
        } else if (strcmp(argv[i], "--vout-end") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--vout-end", argv[++i], &vout_end) != 0) return 2;
        } else if (strcmp(argv[i], "--vout-step") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--vout-step", argv[++i], &vout_step) != 0) return 2;
        } else if (strcmp(argv[i], "--imeas") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--imeas", argv[++i], &imeas) != 0) return 2;
        } else if (strcmp(argv[i], "--iref") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--iref", argv[++i], &iref) != 0) return 2;
        } else if (strcmp(argv[i], "--ki-input") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--ki-input", argv[++i], &ki_input) != 0) return 2;
        } else if (strcmp(argv[i], "--accum-init") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--accum-init", argv[++i], &accum_init) != 0) return 2;
        } else if (strcmp(argv[i], "--settle") == 0 && (i + 1) < argc) {
            if (parse_int_arg("--settle", argv[++i], &settle) != 0) return 2;
        } else if (strcmp(argv[i], "--dynamic") == 0) {
            dynamic = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            fprintf(stderr,
                    "Usage: %s --ocp-sweep [options]\n"
                    "  --vout-start N   (default 0)\n"
                    "  --vout-end N     (default 7000)\n"
                    "  --vout-step N    (default 20)\n"
                    "  --imeas N        (default 500)\n"
                    "  --iref N         (default 100)\n"
                    "  --ki-input N     (default 30)\n"
                    "  --accum-init N   (default 0)\n"
                    "  --settle N       (default 1)\n"
                    "  --dynamic        (keep integrator state across sweep)\n",
                    argv[0]);
            return 0;
        } else {
            fprintf(stderr, "unknown argument: %s\n", argv[i]);
            return 2;
        }
    }

    if (vout_step <= 0 || settle <= 0 || vout_end < vout_start) {
        fprintf(stderr, "invalid sweep range/step\n");
        return 2;
    }

    reset_state();
    Imeas = (int16_t)imeas;
    ocpCurrentRef = (int16_t)iref;
    ocpKiInput = (int16_t)ki_input;
    ocpAccumulator = (int32_t)accum_init;

    printf("vout_sum,an0,an2,i_error,gain_scale,Kp,Ki,u_raw,vrefOcpAdj,ocpAccumulator\n");
    for (int v = vout_start; v <= vout_end; v += vout_step) {
        vout_sum = (int16_t)v;
        if (!dynamic) {
            ocpAccumulator = (int32_t)accum_init;
        }

        for (int n = 0; n < settle; n++) {
            ocpFoldback();
        }

        printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%ld\n",
               (int)vout_sum,
               (int)(vout_sum >> 2),
               (int)(vout_sum >> 2),
               (int)(Imeas - ocpCurrentRef),
               (int)ocpError,
               (int)ocpIntegralLo,
               (int)ocpProportional,
               (int)ocpOutputRaw,
               (int)vrefOcpAdj,
               (long)ocpAccumulator);
    }

    return 0;
}

static void reset_state(void)
{
    ADCBUF0 = ADCBUF1 = ADCBUF2 = ADCBUF3 = ADCBUF4 = ADCBUF5 = 0;
    ADCBUF10 = ADCBUF11 = ADCBUF12 = ADCBUF13 = ADCBUF14 = ADCBUF15 = 0;
    PDC5 = 0;
    PORTD = 0;
    PORTG = 0;
    TRISF = 0;
    I2C2CON = 0;
    I2C2ADD = 0;
    I2C2TRN = 0;
    I2C2RCV = 0;
    I2C2STAT = 0;
    IPC12 = 0;
    IFS3 = 0;
    IEC3 = 0;
    PORTDbits.RD0 = 0;
    PORTDbits.RD6 = 0;
    PORTDbits.RD11 = 0;
    I2C2STATbits.D_A = 0;
    tick_counter = 0;

    pwmRunning = 0;
    thermalFlags = 0;
    systemState = 0;
    droopMode = 0;
    systemFlags = 0;
    pmbusAlertFlags = 0;
    statusFlags = 0;
    auxFlags = 0;
    currentLimitFlags = 0;
    controlStatus = 0;

    debounceAdc12 = 0;
    ocRampCounter = 0;
    voutSetpoint = 0;
    marginThreshold = 0;

    adcBuf4Raw = 0;
    ioutRingIdx8pt = 0;
    ioutRunSum = 0;
    iout_avg = 0;
    memset((void *)ioutRing8pt, 0, sizeof(ioutRing8pt));

    ssDebounce = 0;
    freqSetpoint = 0;

    memset(flash_sector_buf_1598, 0, sizeof(flash_sector_buf_1598));
    memset(flash_read_buf_15B0, 0, sizeof(flash_read_buf_15B0));
    memset(flash_read_buf_15D0, 0, sizeof(flash_read_buf_15D0));
    memset(flash_buf_181E, 0, sizeof(flash_buf_181E));
    memset(flash_program_scratch, 0, sizeof(flash_program_scratch));
    memset(flash_buf_171E, 0, sizeof(flash_buf_171E));
    memset(flash_read_buf_160E, 0, sizeof(flash_read_buf_160E));
    memset((void *)rxPacketBuf, 0, sizeof(rxPacketBuf));
    memset((void *)pmbusStringBuf, 0, sizeof(pmbusStringBuf));
    memset((void *)pmbusInfoBytes, 0, sizeof(pmbusInfoBytes));
    flashCmdFlags = 0;
    flashBlockByteCount = 0;
    flash_data_160A = 0;
    flash_data_160C = 0;
    flash_page_addr = 0;
    flash_write_offset = 0;
    flashBlockPageWord = 0;
    flashBlockLastOffset = 0;
    llcStatus = 0;
    droopEnableFlags = 0;
    eepromCrcShadow = 0;
    flashUpdateResult = 0;
    ioutCalFactor = 0;
    ioutCalFactorShadow = 0;
    rxBufIndex = 0;
    rxPendFlag = 0;
    rxEventFlags = 0;
    rxAddrByte = 0;
    txCtrlWord = 0;
    txByteCount = 0;
    txChecksumAccum = 0;
    txByteCntPreset = 0;
    txSubReg = 0;
    txBusStateFlags = 0;
    i2cRxData = 0;
    pmbusReadPtr0 = 0;
    pmbusReadPtr1 = 0;
    voutCalE = 0;
    voutCalD = 0;
    tempLinearFmt = 0;
    voutCalC = 0;
    voutCalB = 0;
    voutCalA = 0;
    voutScaleA = 0;
    voutScaleB = 0;
    ioutScaleConst = 0;
    ocpThresholdHw = 0;
    ioutAdcRaw = 0;
    outputCurrent = 0;
    eeprom_crc_lo_saved = 0;
    eeprom_page_addr = 0;
    eeprom_crc_lo = 0;
    eeprom_cfg_reg = 0;

    g_pwm_override_calls = 0;
}

static void verify_signatures(void)
{
    /* Parameter count and types are validated by function-pointer compatibility. */
    uint16_t (*f1)(uint16_t, uint16_t, uint16_t, uint16_t) = thresholdCompare433C;
    uint16_t (*f2)(uint16_t, uint16_t, volatile int16_t *, uint16_t) = latchCounter430C;
    void (*f3)(void) = adcBuf12OvercurrentLatch;
    void (*f4)(void) = adcBuf4FastAverage;
    void (*f5)(void) = softStartRamp2;
    void (*f5a)(void) = softStartRamp;
    void (*f5b)(void) = stateControlMachine;
    uint16_t (*f6)(uint16_t, uint16_t, uint16_t, uint16_t) = thresholdCompare;
    uint16_t (*f7)(uint16_t, uint16_t, volatile uint16_t *, uint16_t) = debounceCounter;
    int16_t (*f8)(int16_t, int16_t) = clampToward;
    void (*f9)(uint8_t, uint16_t *) = crc16Update;
    void (*f10)(uint8_t *, uint16_t *, uint32_t) = crc16Block;
    void (*f10a)(uint16_t) = eepromSetCrcLo;
    void (*f10b)(uint16_t) = eepromSetPageAddr;
    uint16_t (*f10c)(void) = eepromGetCrcLo;
    void (*f11)(uint8_t, uint8_t, uint8_t) = eepromSetConfig;
    uint8_t (*f12)(uint16_t) = flash_lookup_15D0;
    uint8_t (*f13)(uint16_t) = flash_lookup_15B0;
    uint8_t (*f14)(uint16_t) = flash_lookup_1598;
    void (*f15)(void) = adcMiscSample;
    void (*f16)(void) = adcVoltageSample;
    void (*f17)(void) = adcCurrentSample;
    void (*f18)(void) = droopTrimCalc;
    void (*f19)(void) = pidControl;
    void (*f20)(void) = ocpFoldback;
    uint16_t (*f21)(uint8_t *, uint16_t) = pmbusChecksumVerify;
    uint16_t (*f22)(uint16_t) = pmbusEncodeResponse;
    void (*f23)(void) = pmbusRxPacketDecode;
    uint16_t (*f24)(uint16_t) = pmbusBlockCommandInvalid;
    (void)f1; (void)f2; (void)f3; (void)f4; (void)f5; (void)f5a; (void)f5b;
    (void)f6; (void)f7; (void)f8; (void)f9; (void)f10;
    (void)f10a; (void)f10b; (void)f10c;
    (void)f11; (void)f12; (void)f13; (void)f14;
    (void)f15; (void)f16; (void)f17; (void)f18; (void)f19; (void)f20;
    (void)f21; (void)f22; (void)f23; (void)f24;
}

/* ---- Simple tests ---- */
static void test_thresholdCompare433C_simple(void)
{
    ASSERT_EQ_I32(1, thresholdCompare433C(0x026C, 0x026C, 0x0136, 0), ">= high threshold should be 1");
    ASSERT_EQ_I32(0, thresholdCompare433C(0x0136, 0x026C, 0x0136, 1), "<= low threshold should be 0");
    ASSERT_EQ_I32(1, thresholdCompare433C(0x0200, 0x026C, 0x0136, 1), "between thresholds should keep prev=1");
    ASSERT_EQ_I32(0, thresholdCompare433C(0x0200, 0x026C, 0x0136, 0), "between thresholds should keep prev=0");
}

static void test_latchCounter430C_simple(void)
{
    int16_t c = 0;
    ASSERT_EQ_I32(0, latchCounter430C(1, 3, &c, 0), "first mismatch keeps prev");
    ASSERT_EQ_I32(1, c, "counter increments");
    ASSERT_EQ_I32(0, latchCounter430C(1, 3, &c, 0), "second mismatch keeps prev");
    ASSERT_EQ_I32(2, c, "counter increments to 2");
    ASSERT_EQ_I32(1, latchCounter430C(1, 3, &c, 0), "third mismatch accepts new level");
    ASSERT_EQ_I32(0, c, "counter resets when accepted");

    c = 9;
    ASSERT_EQ_I32(1, latchCounter430C(1, 3, &c, 1), "equal level should keep prev");
    ASSERT_EQ_I32(0, c, "equal level should clear counter");
}

/* ---- Medium tests ---- */
static void model_adcbuf4_step(
    int16_t sample, int16_t ring[8], int16_t *idx, int16_t *run_sum, int16_t *avg)
{
    int16_t cur_idx = *idx;
    ring[cur_idx] = sample;
    {
        int16_t sum = (int16_t)(*run_sum + sample);
        int16_t next = (int16_t)(cur_idx + 1);
        *avg = (int16_t)(sum >> 3);
        if (next > 7) {
            next = 0;
        }
        *run_sum = (int16_t)(sum - ring[next]);
        *idx = next;
    }
}

static void test_adcBuf4FastAverage_medium(void)
{
    int16_t model_ring[8] = {0};
    int16_t model_idx = 0;
    int16_t model_sum = 0;
    int16_t model_avg = 0;
    int16_t samples[] = {8, 16, 24, 32, 40, 48, 56, 64, 72, 80};
    size_t i;

    reset_state();

    for (i = 0; i < (sizeof(samples) / sizeof(samples[0])); i++) {
        ADCBUF4 = (uint16_t)samples[i];
        adcBuf4FastAverage();
        model_adcbuf4_step(samples[i], model_ring, &model_idx, &model_sum, &model_avg);
        ASSERT_EQ_I32(model_idx, ioutRingIdx8pt, "ring index mismatch");
        ASSERT_EQ_I32(model_sum, ioutRunSum, "running sum mismatch");
        ASSERT_EQ_I32(model_avg, iout_avg, "average mismatch");
        ASSERT_EQ_I32(samples[i], adcBuf4Raw, "raw sample mismatch");
    }
}

static void test_adcBuf4FastAverage_random_medium(void)
{
    int16_t model_ring[8] = {0};
    int16_t model_idx = 0;
    int16_t model_sum = 0;
    int16_t model_avg = 0;
    int i;

    reset_state();
    srand(1234);
    for (i = 0; i < 128; i++) {
        int16_t s = (int16_t)(rand() % 200);
        ADCBUF4 = (uint16_t)s;
        adcBuf4FastAverage();
        model_adcbuf4_step(s, model_ring, &model_idx, &model_sum, &model_avg);
        ASSERT_EQ_I32(model_idx, ioutRingIdx8pt, "random ring index mismatch");
        ASSERT_EQ_I32(model_sum, ioutRunSum, "random running sum mismatch");
        ASSERT_EQ_I32(model_avg, iout_avg, "random average mismatch");
    }
}

static void test_protection_helpers_medium(void)
{
    volatile uint16_t cnt = 0;
    int i;
    struct tc {
        uint16_t value;
        uint16_t low;
        uint16_t high;
        uint16_t prev;
    } cases[] = {
        /* Below/at/above primary threshold */
        {0,    100, 200, 0},
        {99,   100, 200, 1},
        {100,  100, 200, 0},
        {150,  100, 200, 1},
        {200,  100, 200, 0},
        {201,  100, 200, 1},
        {65535,100, 200, 0},
        /* Assembly call pattern where low > high (hysteresis window is (high, low)) */
        {233,  0x26C, 0x136, 0},
        {233,  0x26C, 0x136, 1},
        {309,  0x26C, 0x136, 0},   /* at high boundary */
        {620,  0x26C, 0x136, 1},   /* at low boundary */
        {621,  0x26C, 0x136, 0},   /* >= low -> 1 */
    };

    /* thresholdCompare (0x30E2) assembly-equivalent model:
     *   if value >= low      -> 1
     *   else if value <= high -> 0
     *   else                  -> prev
     */
    ASSERT_EQ_I32(0, thresholdCompare(90, 100, 200, 0), "thresholdCompare value<=high -> 0");
    ASSERT_EQ_I32(1, thresholdCompare(210, 100, 200, 1), "thresholdCompare value>=low -> 1");
    ASSERT_EQ_I32(1, thresholdCompare(210, 100, 200, 0), "thresholdCompare value>=low ignores prev");
    ASSERT_EQ_I32(1, thresholdCompare(150, 100, 200, 1), "thresholdCompare value>=low -> 1");

    for (i = 0; i < (int)(sizeof(cases) / sizeof(cases[0])); i++) {
        int32_t exp;
        uint16_t got = thresholdCompare(cases[i].value, cases[i].low, cases[i].high, cases[i].prev);
        if (cases[i].value >= cases[i].low) {
            exp = 1;
        } else if (cases[i].value <= cases[i].high) {
            exp = 0;
        } else {
            exp = (cases[i].prev & 0xFFu);
        }
        ASSERT_EQ_I32(exp, got, "thresholdCompare vector mismatch");
    }

    /* Hysteresis hold band check: high < value < low keeps prev */
    ASSERT_EQ_I32(0, thresholdCompare(400, 620, 309, 0), "thresholdCompare hold prev=0");
    ASSERT_EQ_I32(1, thresholdCompare(400, 620, 309, 1), "thresholdCompare hold prev=1");

    /* clampToward (0x3242): one-step toward target */
    ASSERT_EQ_I32(9, clampToward(10, 3), "clampToward down");
    ASSERT_EQ_I32(4, clampToward(3, 10), "clampToward up");
    ASSERT_EQ_I32(5, clampToward(5, 5), "clampToward equal");

    /* debounceCounter (0x31B2): assembly-faithful behavior */
    cnt = 0;
    ASSERT_EQ_I32(1, debounceCounter(1, 3, &cnt, 0), "debounceCounter mismatch returns new immediately");
    ASSERT_EQ_I32(0, cnt, "debounceCounter mismatch under-threshold clears counter");
    cnt = 2;
    ASSERT_EQ_I32(0, debounceCounter(1, 3, &cnt, 0), "debounceCounter at/above threshold keeps previous");
    ASSERT_EQ_I32(3, cnt, "debounceCounter keeps incremented counter when threshold reached");
    cnt = 9;
    ASSERT_EQ_I32(1, debounceCounter(1, 3, &cnt, 1), "debounceCounter equal-level keeps previous");
    ASSERT_EQ_I32(0, cnt, "debounceCounter equal-level clears counter");
}

static uint16_t thresholdCompare_inline_asm_wrapper(uint16_t value,
                                                    uint16_t low_thresh,
                                                    uint16_t high_thresh,
                                                    uint16_t prev_state)
{
#if defined(__XC16__)
    register uint16_t w0 __asm__("w0") = value;
    register uint16_t w1 __asm__("w1") = low_thresh;
    register uint16_t w2 __asm__("w2") = high_thresh;
    register uint16_t w3 __asm__("w3") = prev_state;

    /* Original 0x30E2..0x30F6 sequence, instruction-for-instruction. */
    __asm__ volatile(
        "mov w0, w4\n\t"
        "mov w3, w0\n\t"
        "sub w4, w1, [w15]\n\t"
        "bra nc, 1f\n\t"
        "mov #0x1, w0\n\t"
        "bra 2f\n\t"
        "1:\n\t"
        "sub w4, w2, [w15]\n\t"
        "bra gtu, 2f\n\t"
        "clr w0\n\t"
        "2:\n\t"
        "ze w0, w0\n\t"
        : "+r"(w0), "+r"(w1), "+r"(w2), "+r"(w3)
        :
        : "w4", "memory", "cc");

    return (uint8_t)w0;
#else
    /* Host unit-test fallback: same control flow as the original instructions. */
    uint16_t w0 = value;
    uint16_t w1 = low_thresh;
    uint16_t w2 = high_thresh;
    uint16_t w3 = prev_state;
    uint16_t w4;

    w4 = w0;
    w0 = w3;

    if (w4 >= w1) {
        w0 = 1;
    } else if (w4 <= w2) {
        w0 = 0;
    }
    return (uint8_t)w0;
#endif
}

static void test_thresholdCompare_inline_asm_agree(void)
{
    size_t i;
    uint16_t v;
    struct cfg {
        uint16_t low;
        uint16_t high;
    } cfgs[] = {
        {100, 200},
        {0x026C, 0x0136},
        {0, 0},
        {500, 500},
        {0xFFFF, 0x0000},
    };
    uint16_t prevs[] = {0, 1, 0x55, 0xFF};
    uint16_t edge_values[32];
    size_t edge_count = 0;

    for (i = 0; i < (sizeof(cfgs) / sizeof(cfgs[0])); i++) {
        uint16_t low = cfgs[i].low;
        uint16_t high = cfgs[i].high;
        size_t p;

        edge_count = 0;
        edge_values[edge_count++] = 0;
        edge_values[edge_count++] = 1;
        edge_values[edge_count++] = 0x00FF;
        edge_values[edge_count++] = 0x0100;
        edge_values[edge_count++] = 0x7FFF;
        edge_values[edge_count++] = 0xFFFE;
        edge_values[edge_count++] = 0xFFFF;
        edge_values[edge_count++] = low;
        edge_values[edge_count++] = (uint16_t)(low - 1u);
        edge_values[edge_count++] = (uint16_t)(low + 1u);
        edge_values[edge_count++] = high;
        edge_values[edge_count++] = (uint16_t)(high - 1u);
        edge_values[edge_count++] = (uint16_t)(high + 1u);

        for (p = 0; p < (sizeof(prevs) / sizeof(prevs[0])); p++) {
            uint16_t prev = prevs[p];

            for (v = 0; v < 1024; v++) {
                uint16_t got = thresholdCompare(v, low, high, prev);
                uint16_t ref = thresholdCompare_inline_asm_wrapper(v, low, high, prev);
                if (got != ref) {
                    printf("FAIL: %s mismatch low=0x%04X high=0x%04X prev=0x%04X value=0x%04X got=0x%04X ref=0x%04X\n",
                           __func__, low, high, prev, v, got, ref);
                    g_failures++;
                    return;
                }
            }

            for (v = 0; v < edge_count; v++) {
                uint16_t value = edge_values[v];
                uint16_t got = thresholdCompare(value, low, high, prev);
                uint16_t ref = thresholdCompare_inline_asm_wrapper(value, low, high, prev);
                if (got != ref) {
                    printf("FAIL: %s edge mismatch low=0x%04X high=0x%04X prev=0x%04X value=0x%04X got=0x%04X ref=0x%04X\n",
                           __func__, low, high, prev, value, got, ref);
                    g_failures++;
                    return;
                }
            }
        }
    }
}

static uint16_t crc16_ref_update(uint8_t byte, uint16_t crc)
{
    uint8_t idx = (uint8_t)(byte ^ (crc & 0xFFu));
    return (uint16_t)((crc >> 8) ^ crc16_table[idx]);
}

static void test_flash_helpers_medium(void)
{
    uint16_t crc = 0x1234;
    uint16_t ref = 0x1234;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0xAB};
    size_t i;

    for (i = 0; i < sizeof(data); i++) {
        crc16Update(data[i], &crc);
        ref = crc16_ref_update(data[i], ref);
    }
    ASSERT_EQ_I32(ref, crc, "crc16Update sequence should match reference");

    crc = 0xBEEF;
    ref = 0xBEEF;
    crc16Block(data, &crc, (uint32_t)sizeof(data));
    for (i = 0; i < sizeof(data); i++) {
        ref = crc16_ref_update(data[i], ref);
    }
    ASSERT_EQ_I32(ref, crc, "crc16Block should match iterative update");
}

static void test_eeprom_and_lookup_simple(void)
{
    reset_state();

    eeprom_cfg_reg = 0;
    eepromSetConfig(2, 1, 1);
    ASSERT_EQ_I32((2u << 4) | (1u << 6) | 1u, eeprom_cfg_reg, "eepromSetConfig field update");

    eepromSetConfig(0xFF, 3, 0xFF);
    ASSERT_EQ_I32((2u << 4) | (3u << 6) | 1u, eeprom_cfg_reg, "eepromSetConfig selective update");

    eeprom_crc_lo_saved = 0x55AA;
    ASSERT_EQ_I32(0x55AA, eepromGetCrcLo(), "eepromGetCrcLo value");
    eepromSetCrcLo(0x3333);
    ASSERT_EQ_I32(0x3333, eeprom_crc_lo, "eepromSetCrcLo value");
    eepromSetPageAddr(0x7777);
    ASSERT_EQ_I32(0x7777, eeprom_page_addr, "eepromSetPageAddr value");

    flash_read_buf_15D0[3] = 0xA3;
    flash_read_buf_15B0[4] = 0xB4;
    flash_sector_buf_1598[5] = 0xC5;
    ASSERT_EQ_I32(0xA3, flash_lookup_15D0(3), "flash_lookup_15D0");
    ASSERT_EQ_I32(0xB4, flash_lookup_15B0(4), "flash_lookup_15B0");
    ASSERT_EQ_I32(0xC5, flash_lookup_1598(5), "flash_lookup_1598");
}

static uint8_t pmbus_checksum_byte(const uint8_t *buf, uint16_t data_len)
{
    uint16_t sum = (uint16_t)(I2C2ADD + I2C2ADD);
    uint16_t i;

    for (i = 0; i < data_len; i++)
        sum = (uint16_t)(sum + buf[i]);

    return (uint8_t)(-(uint8_t)sum);
}

static void pmbus_feed_rx(uint8_t value)
{
    I2C2RCV = value;
    pmbusRxPacketDecode();
}

static void pmbus_set_tuple(uint16_t idx, uint8_t cmd, uint16_t word)
{
    rxPacketBuf[idx] = cmd;
    rxPacketBuf[idx + 1u] = (uint8_t)word;
    rxPacketBuf[idx + 2u] = (uint8_t)(word >> 8);
}

static void test_pmbus_checksum_encode_and_rx_medium(void)
{
    uint8_t pkt[] = {0x22, 0x10, 0x00};

    reset_state();
    I2C2ADD = 0x58;
    pkt[2] = pmbus_checksum_byte(pkt, 2);
    ASSERT_EQ_I32(0, pmbusChecksumVerify(pkt, sizeof(pkt)), "valid checksum should return zero");
    pkt[2] ^= 0x01u;
    ASSERT_EQ_I32(1, pmbusChecksumVerify(pkt, sizeof(pkt)), "invalid checksum should return one");
    ASSERT_EQ_I32(0, pmbusChecksumVerify(pkt, 0), "zero-length checksum is a no-op");

    I2C2TRN = 0x1234;
    ASSERT_EQ_I32(0, pmbusEncodeResponse(1), "response code 1 accepted");
    ASSERT_EQ_I32(0xFE, I2C2TRN, "response code 1");
    ASSERT_EQ_I32(0, pmbusEncodeResponse(2), "response code 2 accepted");
    ASSERT_EQ_I32(0xCE, I2C2TRN, "response code 2");
    ASSERT_EQ_I32(0, pmbusEncodeResponse(4), "response code 4 accepted");
    ASSERT_EQ_I32(0xFA, I2C2TRN, "response code 4");
    ASSERT_EQ_I32(0, pmbusEncodeResponse(8), "response code 8 accepted");
    ASSERT_EQ_I32(0x55, I2C2TRN, "response code 8");
    ASSERT_EQ_I32(0, pmbusEncodeResponse(0x10), "response code 0x10 accepted");
    ASSERT_EQ_I32(0xAA, I2C2TRN, "response code 0x10");
    I2C2TRN = 0x77;
    ASSERT_EQ_I32(1, pmbusEncodeResponse(0), "unsupported response code rejected");
    ASSERT_EQ_I32(0x77, I2C2TRN, "unsupported response should not touch TRN");
    pmbusUnsupportedCommand();
    ASSERT_EQ_I32(0x55, I2C2TRN, "unsupported command helper byte");

    reset_state();
    txChecksumAccum = 0x34;
    rxEventFlags = 0xFFFF;
    pmbusChecksumResponse();
    ASSERT_EQ_I32(1, (txCtrlWord >> 1) & 1, "checksum response sets end flag");
    ASSERT_EQ_I32(0, rxEventFlags, "checksum response clears events");
    ASSERT_EQ_I32(0xCC, I2C2TRN, "checksum response byte");

    reset_state();
    txChecksumAccum = 0x10;
    pmbusSendDataByte(0x22);
    ASSERT_EQ_I32(0x22, I2C2TRN, "send data byte loads TRN");
    ASSERT_EQ_I32(0x32, txChecksumAccum, "send data byte accumulates checksum");
    pmbusSendEndChecksum();
    ASSERT_EQ_I32(1, (txCtrlWord >> 1) & 1, "send end checksum sets end flag");
    ASSERT_EQ_I32(0xCE, I2C2TRN, "send end checksum byte");

    reset_state();
    pmbus_feed_rx(0x13);
    ASSERT_EQ_I32(1, rxBufIndex, "rx index after first byte");
    ASSERT_EQ_I32(0x13, rxPacketBuf[0], "rx byte stored at zero-based buffer index");
    ASSERT_EQ_I32(1, (rxEventFlags >> 1) & 1, "cmd 0x13 event bit");

    reset_state();
    rxBufIndex = 0x21;
    pmbus_feed_rx(0xB0);
    ASSERT_EQ_I32(0xB0, rxAddrByte, "address byte stored separately");
    ASSERT_EQ_I32(0x21, rxBufIndex, "address-byte path does not increment rx index");

    reset_state();
    pmbus_feed_rx(0x3B);
    pmbus_feed_rx(0x00);
    pmbus_feed_rx(0x12);
    pmbus_feed_rx(0x34);
    ASSERT_EQ_I32(0x1A34, i2cRxData, "cmd 0x3B adds flash page offset");
    ASSERT_EQ_I32(1, (flashCmdFlags >> 1) & 1, "cmd 0x3B sets flash command bit1");

    reset_state();
    pmbus_feed_rx(0x20);
    pmbus_feed_rx(2);
    pmbus_feed_rx(8);
    ASSERT_EQ_I32(0, flash_data_160A, "cmd 0x20 mode 2 page 8 offset");
    ASSERT_EQ_I32(2, flash_data_160C, "cmd 0x20 mode 2 page 8 bank");
    ASSERT_EQ_I32(1, (flashCmdFlags >> 5) & 1, "cmd 0x20 sets flash command bit5");
}

static void test_pmbus_block_validator_edges_medium(void)
{
    reset_state();

    flash_read_buf_15E6[0x10] = 1;
    pmbus_set_tuple(1, 0x31, 1);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x31 rejected when flash flag is set");

    flash_read_buf_15E6[0x10] = 0;
    droopEnableFlags = 0;
    pmbus_set_tuple(1, 0x31, 0);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x31 rejects zero without bit10");
    pmbus_set_tuple(1, 0x31, 1);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x31 accepts nonzero without bit10");

    droopEnableFlags = 0x0400;
    pmbus_set_tuple(1, 0x31, 0x00FF);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x31 accepts low update with bit10");
    pmbus_set_tuple(1, 0x31, 0x8000);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x31 accepts 0x8000 with bit10");
    pmbus_set_tuple(1, 0x31, 0x0100);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x31 rejects 0x0100 with bit10");

    pmbus_set_tuple(1, 0x32, 0x3000);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x32 lower bound");
    pmbus_set_tuple(1, 0x32, 0x2FFF);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x32 below lower bound");
    pmbus_set_tuple(1, 0x32, 0x3133);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x32 upper bound");
    pmbus_set_tuple(1, 0x32, 0x3134);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x32 above upper bound");

    pmbus_set_tuple(1, 0x33, 0x0040);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x33 lower bound");
    pmbus_set_tuple(1, 0x33, 0x003F);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x33 below lower bound");
    pmbus_set_tuple(1, 0x34, 0x1180);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x34 upper bound");
    pmbus_set_tuple(1, 0x34, 0x1181);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x34 above upper bound");

    pmbusReadPtr0 = 0x1000;
    pmbus_set_tuple(1, 0x35, 0x0F00);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x35 threshold equal");
    pmbus_set_tuple(1, 0x35, 0x0F01);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x35 threshold above");

    pmbusReadPtr1 = 0x1000;
    pmbus_set_tuple(1, 0x36, 0x10FF);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x36 threshold below");
    pmbus_set_tuple(1, 0x36, 0x1100);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x36 threshold equal");

    voutCalD = 0x2000;
    pmbus_set_tuple(1, 0x37, 0x1F00);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x37 threshold equal");
    pmbus_set_tuple(1, 0x37, 0x1F01);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x37 threshold above");

    voutCalE = 0x2000;
    pmbus_set_tuple(1, 0x38, 0x20FF);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x38 threshold below");
    pmbus_set_tuple(1, 0x38, 0x2100);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x38 threshold equal");

    pmbus_set_tuple(1, 0x3D, 0x0000);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x3D rejects low byte zero");
    pmbus_set_tuple(1, 0x3D, 0x00EF);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x3D accepts 0xEF");
    pmbus_set_tuple(1, 0x3D, 0x00F0);
    ASSERT_EQ_I32(1, pmbusBlockCommandInvalid(1), "cmd 0x3D rejects 0xF0");
    pmbus_set_tuple(1, 0x3D, 0x00FA);
    ASSERT_EQ_I32(0, pmbusBlockCommandInvalid(1), "cmd 0x3D accepts 0xFA");
}

static void test_pmbus_flash_and_programming_medium(void)
{
    uint16_t sum;

    reset_state();
    rxBufIndex = 3;
    rxPacketBuf[1] = 2;
    rxPendFlag = 1;
    txChecksumAccum = 0x10;
    flash_read_buf_160E[0x21] = 0xA5;
    pmbusFlashReadWindow();
    ASSERT_EQ_I32(0xA5, I2C2TRN, "flash read page window byte");
    ASSERT_EQ_I32(0xB5, txChecksumAccum, "flash read checksum accumulates byte");

    reset_state();
    rxBufIndex = 3;
    rxPacketBuf[1] = 2;
    rxPendFlag = 0x10;
    txChecksumAccum = 0x22;
    pmbusFlashReadWindow();
    ASSERT_EQ_I32(0xDE, I2C2TRN, "flash read page window checksum");
    ASSERT_EQ_I32(1, (txCtrlWord >> 1) & 1, "flash read page window sets end flag");

    reset_state();
    rxBufIndex = 5;
    rxPacketBuf[1] = 0xC8;
    rxPacketBuf[2] = 0x11;
    rxPacketBuf[3] = 0x22;
    pmbusFlashWriteWindow();
    ASSERT_EQ_I32(0x11, flash_read_buf_160E[0xC8], "flash write window byte 0");
    ASSERT_EQ_I32(0x22, flash_read_buf_160E[0xC9], "flash write window byte 1");
    ASSERT_EQ_I32(1, (flashCmdFlags >> 4) & 1, "flash write window changed bit");
    ASSERT_EQ_I32(0x31, I2C2TRN, "flash write window ack");

    reset_state();
    rxBufIndex = 4;
    rxPacketBuf[1] = 0xC7;
    rxPacketBuf[2] = 0x55;
    pmbusFlashWriteWindow();
    ASSERT_EQ_I32(0, flash_read_buf_160E[0xC7], "protected write window without LLC mode");
    ASSERT_EQ_I32(0xAA, I2C2TRN, "protected write window nack");

    reset_state();
    rxBufIndex = 8;
    eepromCrcShadow = 0x1234;
    rxPacketBuf[1] = 0x05;
    rxPacketBuf[2] = 0xA6;
    rxPacketBuf[3] = 0x02;
    rxPacketBuf[4] = 0x01;
    rxPacketBuf[5] = 0x12;
    rxPacketBuf[6] = 0x34;
    pmbusOperationUnlock();
    ASSERT_EQ_I32(1, (systemFlags >> 9) & 1, "unlock sets programming enable");
    ASSERT_EQ_I32(1, eeprom_cfg_reg & 1, "unlock persists enable bit");
    ASSERT_EQ_I32(0x31, I2C2TRN, "unlock ack");

    reset_state();
    systemFlags = 0x0200;
    rxBufIndex = 4;
    rxPacketBuf[1] = 'P';
    rxPacketBuf[2] = 'H';
    pmbusProgramHeader();
    ASSERT_EQ_I32(1, systemFlags & 1, "program header accepted");
    ASSERT_EQ_I32(0x31, I2C2TRN, "program header ack");

    reset_state();
    systemFlags = 0x0200;
    rxBufIndex = 8;
    rxPacketBuf[1] = 3;
    rxPacketBuf[2] = 0x00;
    rxPacketBuf[3] = 0x10;
    rxPacketBuf[4] = 0;
    rxPacketBuf[5] = 0xAA;
    rxPacketBuf[6] = 0xBB;
    rxPacketBuf[7] = 0xCC;
    sum = 0;
    for (uint16_t i = 1; i <= 7; i++)
        sum = (uint16_t)(sum + rxPacketBuf[i]);
    rxPacketBuf[8] = (uint8_t)(-(uint8_t)sum);
    pmbusProgramBlock();
    ASSERT_EQ_I32(0x0810, flashBlockPageWord, "program block page offset");
    ASSERT_EQ_I32(3, flashBlockByteCount, "program block byte count");
    ASSERT_EQ_I32(0xAA, flash_program_scratch[0], "program block scratch byte 0");
    ASSERT_EQ_I32(0xBB, flash_program_scratch[1], "program block scratch byte 1");
    ASSERT_EQ_I32(0xCC, flash_program_scratch[2], "program block scratch byte 2");
    ASSERT_EQ_I32(0xAA, flash_buf_171E[0x10], "program block page byte 0");
    ASSERT_EQ_I32(0xBB, flash_buf_171E[0x11], "program block page byte 1");
    ASSERT_EQ_I32(0xCC, flash_buf_171E[0x12], "program block page byte 2");
    ASSERT_EQ_I32(0, flash_buf_181E[0x10], "program block leaves readback buffer");
    ASSERT_EQ_I32(0x31, I2C2TRN, "program block ack");

    reset_state();
    llcStatus = 0x0100;
    rxBufIndex = 0x0B;
    for (uint16_t i = 0; i < 9; i++)
        rxPacketBuf[i + 1u] = (uint8_t)('A' + i);
    pmbusEnterProgramming();
    ASSERT_EQ_I32('A', pmbusStringBuf[0], "enter programming stores string byte 0");
    ASSERT_EQ_I32('I', pmbusStringBuf[8], "enter programming stores string byte 8");
    ASSERT_EQ_I32(1, (currentLimitFlags >> 1) & 1, "enter programming sets current flag");
    ASSERT_EQ_I32(0x31, I2C2TRN, "enter programming ack");

    reset_state();
    llcStatus = 0x0100;
    pmbusStringBuf[8] = '\n';
    pmbusInfoBytes[0] = '\r';
    rxPendFlag = 10;
    txChecksumAccum = 0x44;
    pmbusEnterProgramming();
    ASSERT_EQ_I32(0xBC, I2C2TRN, "enter programming terminator checksum");
    ASSERT_EQ_I32(1, (txCtrlWord >> 2) & 1, "enter programming terminator sets checksum flag");

    reset_state();
    llcStatus = 0x0100;
    rxBufIndex = 0x16;
    rxPacketBuf[1] = 0x11;
    rxPacketBuf[2] = 0x22;
    rxPacketBuf[3] = 0x33;
    rxPacketBuf[4] = 0x44;
    for (uint16_t i = 0; i < 16; i++)
        rxPacketBuf[i + 5u] = (uint8_t)(0x80u + i);
    pmbusStoreProgrammingData();
    ASSERT_EQ_I32(0x11, (uint8_t)flash_read_buf_15E6[0x11], "store programming metadata 0");
    ASSERT_EQ_I32(0x44, (uint8_t)flash_read_buf_15E6[0x14], "store programming metadata 3");
    ASSERT_EQ_I32(0x80, (uint8_t)flash_read_buf_15E6[0], "store programming data 0");
    ASSERT_EQ_I32(0x8F, (uint8_t)flash_read_buf_15E6[15], "store programming data 15");
    ASSERT_EQ_I32(1, (flashCmdFlags >> 6) & 1, "store programming sets flash bit6");
    ASSERT_EQ_I32(1, (statusFlags >> 6) & 1, "store programming sets status bit6");
    ASSERT_EQ_I32(0x31, I2C2TRN, "store programming ack");
}

static void test_pmbus_readback_and_mode_medium(void)
{
    reset_state();
    rxBufIndex = 4;
    rxPacketBuf[1] = 0;
    rxPacketBuf[2] = 0;
    rxPendFlag = 3;
    flash_read_buf_15D0[3] = 0xA3;
    pmbusBlockWriteModeSelect();
    ASSERT_EQ_I32(0xA3, I2C2TRN, "summary block byte");

    reset_state();
    rxBufIndex = 4;
    rxPacketBuf[1] = 2;
    rxPacketBuf[2] = 0x0A;
    rxPendFlag = 4;
    flash_read_buf_15B0[4] = 0xB4;
    pmbusBlockWriteModeSelect();
    ASSERT_EQ_I32(0xB4, I2C2TRN, "data block byte");

    reset_state();
    rxBufIndex = 4;
    rxPacketBuf[1] = 1;
    rxPacketBuf[2] = 0;
    rxPendFlag = 5;
    flash_sector_buf_1598[5] = 0xC5;
    pmbusBlockWriteModeSelect();
    ASSERT_EQ_I32(0xC5, I2C2TRN, "status block byte");

    reset_state();
    rxBufIndex = 3;
    rxPacketBuf[1] = 0xE0;
    rxPendFlag = 4;
    pmbusInfoBytes[4] = 0x44;
    pmbusReadDeviceInfo();
    ASSERT_EQ_I32(0x44, I2C2TRN, "device info byte");

    reset_state();
    rxBufIndex = 5;
    rxPacketBuf[1] = 3;
    rxPendFlag = 2;
    flash_buf_181E[2] = 0x76;
    pmbusReadbackBlock();
    ASSERT_EQ_I32(0x76, I2C2TRN, "readback block byte");

    reset_state();
    rxBufIndex = 5;
    rxPacketBuf[1] = 'P';
    rxPacketBuf[2] = 'H';
    rxPacketBuf[3] = 1;
    pmbusSetOperationMode();
    ASSERT_EQ_I32(1, (llcStatus >> 8) & 1, "operation mode enable");
    ASSERT_EQ_I32(0x31, I2C2TRN, "operation mode enable ack");

    reset_state();
    llcStatus = 0x0100;
    rxBufIndex = 3;
    rxPacketBuf[1] = 1;
    pmbusReadConfigCommand();
    ASSERT_EQ_I32(0x0048, systemFlags & 0x0048, "read config sets update flags");
    ASSERT_EQ_I32(0x31, I2C2TRN, "read config ack in LLC mode");

    reset_state();
    rxBufIndex = 3;
    rxPacketBuf[1] = 0xAA;
    ioutCalFactor = 0x1234;
    pmbusReadVoutCommand();
    ASSERT_EQ_I32(1, (currentLimitFlags >> 8) & 1, "read vout arms calibration");
    ASSERT_EQ_I32(0x1234, ioutCalFactorShadow, "read vout shadows calibration");
    ASSERT_EQ_I32(0x31, I2C2TRN, "read vout ack");

    reset_state();
    rxBufIndex = 2;
    pmbusStartFlashWrite();
    ASSERT_EQ_I32(1, (systemFlags >> 8) & 1, "start flash write sets pending bit");
    ASSERT_EQ_I32(0x0800, flash_write_offset, "start flash write offset");
    ASSERT_EQ_I32(0x31, I2C2TRN, "start flash write ack");
}

static void test_adc_misc_voltage_current_medium(void)
{
    int16_t expected_a;
    int16_t expected_b;

    reset_state();

    statusFlags |= (1u << 6);
    flash_read_buf_15E6[0x11] = 0x34;
    flash_read_buf_15E6[0x12] = 0x12; /* cal_vb = 0x1234 */
    flash_read_buf_15E6[0x13] = 0x00;
    flash_read_buf_15E6[0x14] = 0x01; /* ofs_vb = (0x0100>>4)+1 = 17 */

    flash_read_buf_15E6[0x00] = 0x78;
    flash_read_buf_15E6[0x01] = 0x56; /* cal_va = 0x5678 */
    flash_read_buf_15E6[0x02] = 0x10;
    flash_read_buf_15E6[0x03] = 0x00; /* ofs_va = (0x0010>>4)+1 = 2 */

    flash_read_buf_15E6[0x04] = 0x20;
    flash_read_buf_15E6[0x05] = 0x00; /* cal_va2 = 0x0020 */
    flash_read_buf_15E6[0x06] = 0x10;
    flash_read_buf_15E6[0x07] = 0x00; /* ofs_va2 = 2 */

    flash_read_buf_15E6[0x08] = 0x30;
    flash_read_buf_15E6[0x09] = 0x20; /* cal_a_gain = 0x2030 */
    flash_read_buf_15E6[0x0A] = 0xF8;
    flash_read_buf_15E6[0x0B] = 0xFF; /* raw=-8 => -8/122 +1 = 1 */
    flash_read_buf_15E6[0x0C] = 0x44;
    flash_read_buf_15E6[0x0D] = 0x03; /* cal_pdc5 = 0x0344 (in range) */
    flash_read_buf_15E6[0x0E] = 0xAA;
    flash_read_buf_15E6[0x0F] = 0x55; /* cal_var_1E42 = 0x55AA */

    adcMiscSample();
    ASSERT_EQ_I32(0, (statusFlags >> 6) & 1, "adcMiscSample should clear status bit6");
    ASSERT_EQ_I32(0x1234, (uint16_t)cal_vb, "cal_vb load");
    ASSERT_EQ_I32(17, ofs_vb, "ofs_vb decode");
    ASSERT_EQ_I32(0x5678, (uint16_t)cal_va, "cal_va load");
    ASSERT_EQ_I32(2, ofs_va, "ofs_va decode");
    ASSERT_EQ_I32(0x0020, (uint16_t)cal_va2, "cal_va2 load");
    ASSERT_EQ_I32(2, ofs_va2, "ofs_va2 decode");
    ASSERT_EQ_I32(0x2030, (uint16_t)cal_a_gain, "cal_a_gain load");
    ASSERT_EQ_I32(1, cal_a_offset, "cal_a_offset decode");
    ASSERT_EQ_I32(0x0344, (uint16_t)cal_pdc5, "cal_pdc5 load");
    ASSERT_EQ_I32(0x0344, PDC5, "PDC5 should update when in range");
    ASSERT_EQ_I32(0x55AA, (uint16_t)cal_var_1E42, "cal_var_1E42 load");

    ADCBUF5 = 1000;
    ADCBUF3 = 900;
    cal_va2 = 0x2000;
    ofs_va2 = 1;
    cal_vb = 0x1FB7;
    ofs_vb = -1;
    adcVoltageSample();
    expected_a = (int16_t)((((int32_t)(ADCBUF5 >> 6)) * (int32_t)cal_va2) >> 13) + ofs_va2;
    expected_b = (int16_t)((((int32_t)(ADCBUF3 >> 6)) * (int32_t)cal_vb) >> 13) + ofs_vb;
    ASSERT_EQ_I32(expected_a, vcal_a, "adcVoltageSample vcal_a");
    ASSERT_EQ_I32(expected_b, vcal_b, "adcVoltageSample vcal_b");
    ASSERT_EQ_I32((int16_t)(expected_a - expected_b), vcal_diff, "adcVoltageSample vcal_diff");

    ADCBUF4 = 120;
    cal_a_gain = 0x2030;
    cal_a_offset = 1;
    adcCurrentSample();
    ASSERT_EQ_I32(120, (int16_t)iout_4buf[0], "adcCurrentSample stores ADCBUF4 into 64-ring");
    ASSERT_EQ_I32(2, Imeas_cal_a, "adcCurrentSample Imeas_cal_a");
    ASSERT_EQ_I32(1, Imeas, "adcCurrentSample Imeas from iout_avg path");
    ASSERT_TRUE(Imeas >= 0, "adcCurrentSample Imeas non-negative");
    ASSERT_TRUE(Imeas_scaled >= 0, "adcCurrentSample Imeas_scaled non-negative");
}

static void test_adc_droop_pid_ocp_complex(void)
{
    reset_state();

    /* droopTrimCalc: after 32 samples, averages update and accumulators clear */
    ADCBUF11 = 128;
    ADCBUF10 = 256;
    ADCBUF1 = 64;
    for (int i = 0; i < 32; i++) {
        droopTrimCalc();
    }
    ASSERT_EQ_I32(32, trim_avg_adc11, "trim_avg_adc11");
    ASSERT_EQ_I32(64, trim_avg_adc10, "trim_avg_adc10");
    ASSERT_EQ_I32(16, trim_avg_adc1, "trim_avg_adc1");
    ASSERT_EQ_I32(0, trim_count, "trim_count reset");

    /* pidControl: clamp to 0x4D max and to 0 minimum */
    Imeas = 2000;
    droopMode = 0;
    vrefModeSelect = 0;
    pidControl();
    ASSERT_EQ_I32(0x4D, droopResult, "pidControl upper clamp");
    ASSERT_EQ_I32(0x4D, vref_ls, "pidControl writes vref_ls");

    Imeas = -2000;
    pidControl();
    ASSERT_EQ_I32(0, droopResult, "pidControl lower clamp");
    ASSERT_EQ_I32(0, vref_ls, "pidControl lower clamp writes vref_ls");

    /* ocpFoldback: output and integrator clamp boundaries */
    vout_sum = 0x1000;
    Imeas = 5000;
    ocpCurrentRef = 100;
    ocpKiInput = 30;
    ocpAccumulator = 0x01F30000;
    ocpFoldback();
    ASSERT_TRUE(vrefOcpAdj >= 0, "ocpFoldback vrefOcpAdj non-negative");
    ASSERT_TRUE(vrefOcpAdj <= 0x0FA0, "ocpFoldback output clamp <=0xFA0");
    ASSERT_TRUE(ocpAccumulator >= 0, "ocpFoldback integrator non-negative");
    ASSERT_TRUE(ocpAccumulator <= 0x01F40000L, "ocpFoldback integrator upper clamp");
}

/* ---- Complex tests ---- */
static void configure_oc_flag_zero_inputs(void)
{
    /* cmp -> 1, new_level -> 0, prev_bit3==0 => latched bit3 stays 0 */
    thermalFlags = (1u << 1);
    ADCBUF12 = 0x026C;
    pwmRunning &= (uint16_t)~(1u << 3);
}

static void configure_oc_flag_one_inputs(void)
{
    /* cmp -> 0, new_level -> 1, prev_bit3==1 => latched bit3 stays 1 */
    thermalFlags = 0;
    ADCBUF12 = 0x0136;
    pwmRunning |= (1u << 3);
}

static void test_softStartRamp2_complex(void)
{
    reset_state();

    /* mismatch, freqSetpoint > 0 => decrement */
    auxFlags = 0;
    PORTDbits.RD11 = 1;
    freqSetpoint = 10;
    softStartRamp2();
    ASSERT_EQ_I32(9, freqSetpoint, "freqSetpoint should decrement on mismatch");
    ASSERT_EQ_I32(0, (auxFlags >> 11) & 1, "bit11 should not change while decrementing");

    /* mismatch, freqSetpoint == 0 => copy RD11 to auxFlags bit11 */
    freqSetpoint = 0;
    auxFlags = 0;
    PORTDbits.RD11 = 1;
    softStartRamp2();
    ASSERT_EQ_I32(1, (auxFlags >> 11) & 1, "bit11 should copy RD11");

    /* match, bit11 clear => 0x15E */
    auxFlags &= (uint16_t)~(1u << 11);
    PORTDbits.RD11 = 0;
    freqSetpoint = 123;
    softStartRamp2();
    ASSERT_EQ_I32(0x015E, freqSetpoint, "match+bit11=0 should set nominal 0x15E");

    /* match, bit11 set => 3 */
    auxFlags |= (1u << 11);
    PORTDbits.RD11 = 1;
    freqSetpoint = 123;
    softStartRamp2();
    ASSERT_EQ_I32(3, freqSetpoint, "match+bit11=1 should set 3");
}

static void test_softStartRamp_complex(void)
{
    reset_state();

    /* Match path: debounce clears immediately. */
    auxFlags = 0;
    PORTDbits.RD0 = 0;
    ssDebounce = 5;
    softStartRamp();
    ASSERT_EQ_I32(0, ssDebounce, "match should clear debounce counter");

    /* Mismatch path: first two calls only count. */
    auxFlags = 0;
    PORTDbits.RD0 = 1;
    ssDebounce = 0;
    softStartRamp();
    ASSERT_EQ_I32(1, ssDebounce, "first mismatch increments");
    ASSERT_EQ_I32(0, (auxFlags >> 1) & 1, "aux bit1 unchanged at first mismatch");
    softStartRamp();
    ASSERT_EQ_I32(2, ssDebounce, "second mismatch increments");
    ASSERT_EQ_I32(0, (auxFlags >> 1) & 1, "aux bit1 unchanged at second mismatch");

    /* Third mismatch commits input state to auxFlags bit1 and clears counter. */
    softStartRamp();
    ASSERT_EQ_I32(0, ssDebounce, "third mismatch clears counter after commit");
    ASSERT_EQ_I32(1, (auxFlags >> 1) & 1, "third mismatch should commit bit1");
}

static void test_stateControlMachine_complex(void)
{
    reset_state();

    systemState = ST_ACTIVE;
    marginThreshold = 17;
    stateControlMachine();
    ASSERT_EQ_I32(17, marginThreshold, "non-IDLE should not decrement margin");

    systemState = ST_IDLE;
    marginThreshold = 0;
    stateControlMachine();
    ASSERT_EQ_I32(0, marginThreshold, "IDLE with zero margin should stay zero");

    marginThreshold = 9;
    stateControlMachine();
    ASSERT_EQ_I32(8, marginThreshold, "IDLE with positive margin should decrement");
}

static void test_adcBuf12OvercurrentLatch_complex(void)
{
    /* A) ACTIVE + oc_flag=0 + droopMode not in {3,4} => force IDLE path */
    reset_state();
    systemState = ST_ACTIVE;
    droopMode = 2;
    pwmRunning = 1u; /* bit0 set */
    configure_oc_flag_zero_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_IDLE, systemState, "invalid droop mode should force IDLE");
    ASSERT_EQ_I32(0x09C4, marginThreshold, "margin should be forced to 0x09C4");
    ASSERT_EQ_I32(1, pmbusAlertFlags & 1, "PMBus alert bit0 should be set");
    ASSERT_EQ_I32(0, pwmRunning & 1, "pwmRunning bit0 should be cleared");
    ASSERT_EQ_I32(1, g_pwm_override_calls, "pwmOverrideEnable should be called once");

    /* B) ACTIVE + oc_flag=0 + droopMode=3 + cnt<=0x4B => ramp only */
    reset_state();
    systemState = ST_ACTIVE;
    droopMode = 3;
    voutSetpoint = 0x1000;
    configure_oc_flag_zero_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_ACTIVE, systemState, "state should remain ACTIVE during ramp");
    ASSERT_EQ_I32(1, ocRampCounter, "ocRampCounter should increment");
    ASSERT_EQ_I32(0x0FFE, voutSetpoint, "voutSetpoint should subtract 2*cnt");
    ASSERT_EQ_I32(1, (statusFlags >> 13) & 1, "statusFlags bit13 should be set");
    ASSERT_EQ_I32(0, g_pwm_override_calls, "no override call in early ramp");

    /* C) ACTIVE + oc_flag=0 + cnt>0x4B => shutdown path */
    reset_state();
    systemState = ST_ACTIVE;
    droopMode = 3;
    ocRampCounter = 0x4B;
    voutSetpoint = 0x1200;
    statusFlags = (1u << 13);
    pwmRunning = 1u;
    configure_oc_flag_zero_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_IDLE, systemState, "overflow ramp should force IDLE");
    ASSERT_EQ_I32(0, ocRampCounter, "ocRampCounter should clear");
    ASSERT_EQ_I32(0, (statusFlags >> 13) & 1, "statusFlags bit13 should clear on shutdown");
    ASSERT_EQ_I32(0x1168, (uint16_t)voutSetpoint, "voutSetpoint should include final subtract");
    ASSERT_EQ_I32(1, g_pwm_override_calls, "shutdown should call override");

    /* D) ACTIVE + oc_flag=1 => clear ramp bit13/counter only */
    reset_state();
    systemState = ST_ACTIVE;
    droopMode = 3;
    statusFlags = (1u << 13);
    ocRampCounter = 5;
    configure_oc_flag_one_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_ACTIVE, systemState, "oc_flag=1 should stay ACTIVE");
    ASSERT_EQ_I32(0, (statusFlags >> 13) & 1, "bit13 should clear");
    ASSERT_EQ_I32(0, ocRampCounter, "ocRampCounter should clear");
    ASSERT_EQ_I32(0, g_pwm_override_calls, "no override call in this branch");

    /* E) non-ACTIVE + oc_flag=0 + state!=0 => force IDLE with alert */
    reset_state();
    systemState = ST_STARTUP;
    statusFlags = (1u << 13);
    ocRampCounter = 7;
    pwmRunning = 1u;
    configure_oc_flag_zero_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_IDLE, systemState, "non-active branch should force IDLE");
    ASSERT_EQ_I32(0, (statusFlags >> 13) & 1, "bit13 should clear");
    ASSERT_EQ_I32(0, ocRampCounter, "ocRampCounter should clear");
    ASSERT_EQ_I32(1, pmbusAlertFlags & 1, "PMBus alert should set");
    ASSERT_EQ_I32(0, pwmRunning & 1, "bit0 should clear");
    ASSERT_EQ_I32(1, g_pwm_override_calls, "override should be called");

    /* F) non-ACTIVE + oc_flag!=0 => clear bit13/counter and return without override */
    reset_state();
    systemState = ST_STARTUP;
    statusFlags = (1u << 13);
    ocRampCounter = 11;
    configure_oc_flag_one_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_STARTUP, systemState, "oc_flag!=0 should keep state in non-ACTIVE path");
    ASSERT_EQ_I32(0, (statusFlags >> 13) & 1, "non-ACTIVE path should clear bit13");
    ASSERT_EQ_I32(0, ocRampCounter, "non-ACTIVE path should clear ocRampCounter");
    ASSERT_EQ_I32(0, g_pwm_override_calls, "non-ACTIVE + oc_flag!=0 should not call override");

    /* G) non-ACTIVE + state==IDLE + oc_flag==0 => force override without alert set */
    reset_state();
    systemState = ST_IDLE;
    configure_oc_flag_zero_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(ST_IDLE, systemState, "state already IDLE should remain IDLE");
    ASSERT_EQ_I32(0, pmbusAlertFlags & 1, "IDLE path should not set alert bit0");
    ASSERT_EQ_I32(1, g_pwm_override_calls, "IDLE path should still call override");

    /* H) ACTIVE + oc_flag=1 + bit13 clear => no side effects */
    reset_state();
    systemState = ST_ACTIVE;
    droopMode = 3;
    ocRampCounter = 9;
    statusFlags = 0;
    configure_oc_flag_one_inputs();
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(9, ocRampCounter, "ACTIVE+oc_flag=1 with bit13 clear should keep ocRampCounter");
    ASSERT_EQ_I32(0, statusFlags, "ACTIVE+oc_flag=1 with bit13 clear should keep statusFlags");
    ASSERT_EQ_I32(0, g_pwm_override_calls, "ACTIVE+oc_flag=1 should not call override");
}

static void test_adcBuf12_bit3_debounce_complex(void)
{
    int i;

    reset_state();
    systemState = ST_STARTUP;
    thermalFlags = 0;
    ADCBUF12 = 0x0136; /* cmp=0 -> new_level=1 */

    for (i = 0; i < 2; i++) {
        adcBuf12OvercurrentLatch();
        ASSERT_EQ_I32(0, (pwmRunning >> 3) & 1, "bit3 should not latch before debounce limit");
    }
    adcBuf12OvercurrentLatch();
    ASSERT_EQ_I32(1, (pwmRunning >> 3) & 1, "bit3 should latch on third mismatch");
    ASSERT_EQ_I32(0, debounceAdc12, "counter should clear on latch transition");
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--ocp-sweep") == 0) {
        return run_ocp_sweep_mode(argc, argv);
    }

    verify_signatures();

    RUN_TEST(test_thresholdCompare433C_simple);
    RUN_TEST(test_latchCounter430C_simple);
    RUN_TEST(test_adcBuf4FastAverage_medium);
    RUN_TEST(test_adcBuf4FastAverage_random_medium);
    RUN_TEST(test_protection_helpers_medium);
    RUN_TEST(test_thresholdCompare_inline_asm_agree);
    RUN_TEST(test_flash_helpers_medium);
    RUN_TEST(test_eeprom_and_lookup_simple);
    RUN_TEST(test_pmbus_checksum_encode_and_rx_medium);
    RUN_TEST(test_pmbus_block_validator_edges_medium);
    RUN_TEST(test_pmbus_flash_and_programming_medium);
    RUN_TEST(test_pmbus_readback_and_mode_medium);
    RUN_TEST(test_adc_misc_voltage_current_medium);
    RUN_TEST(test_adc_droop_pid_ocp_complex);
    RUN_TEST(test_softStartRamp_complex);
    RUN_TEST(test_softStartRamp2_complex);
    RUN_TEST(test_stateControlMachine_complex);
    RUN_TEST(test_adcBuf12OvercurrentLatch_complex);
    RUN_TEST(test_adcBuf12_bit3_debounce_complex);

    if (g_failures != 0) {
        printf("C unittest failed: tests=%d failures=%d\n", g_tests_run, g_failures);
        return 1;
    }

    printf("C unittest passed: tests=%d failures=%d\n", g_tests_run, g_failures);
    return 0;
}
