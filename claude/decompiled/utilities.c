/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — utilities.c
 * Decompiled functions: I2C helpers, PMBus status, LED update, fan update,
 * flash calibration, voltage/current regulation, OC2RS computation,
 * LLC run modes, UART RX handler, protection check.
 * ============================================================================ */
#include <xc.h>
#include <string.h>
#define FCY 50000000UL
#include <libpic30.h>
#include "variables.h"

static inline void at45dbCsAssert(void)
{
    LATGbits.LATG9 = 0;
}

static inline void at45dbCsDeassert(void)
{
    LATGbits.LATG9 = 1;
}


/* ---- Forward declarations for flash_ops.c ---- */
extern void at45dbReadStatus(uint16_t *dest);
extern void at45dbPageErase(uint16_t page_num);
extern void at45dbBufferWrite(const uint8_t *src, uint16_t len);
extern void at45dbBufferProgramToPage(uint16_t page_num);
extern void at45dbPageRead(uint8_t *buf, uint16_t len,
                           uint16_t page_num, uint16_t byte_offset);

/* Forward declarations for functions in other files */
extern void t1IsrI2cBody(void);     /* 0x29FA in i2c_protocol.c */

/* Forward declarations for functions in this file */
void flashSaveStatusPage(void);

/* ============================================================================
 * flashVerifyChecksum (0x3B3A) — Verify additive checksum over buffer region
 * Returns 0 if checksum OK, 1 if mismatch
 * ============================================================================ */
static uint16_t flashVerifyChecksum(uint8_t *buf, uint16_t start,
                                      uint16_t end, uint8_t expected)
{
    uint16_t sum = 0;
    for (uint16_t i = start; i <= end; i++)
        sum += buf[i];
    return ((uint8_t)(sum + expected) == 0) ? 0 : 1;
}

/* ============================================================================
 * loadCalibrationFromFlash (0x3AE0) — Copy flash_read_buf_15D0 to
 * working registers (peak tracking, gate timing)
 * ============================================================================ */
static void loadCalibrationFromFlash(void)
{
    uint8_t *p = flash_read_buf_15D0;
    peakTracking[8] = p[0] | ((uint16_t)p[1] << 8);
    peakTracking[7] = p[2] | ((uint16_t)p[3] << 8);
    peakTracking[6] = p[4] | ((uint16_t)p[5] << 8);
    peakTracking[5] = p[6] | ((uint16_t)p[7] << 8);
    oc1rsGateTiming = p[8] | ((uint16_t)p[9] << 8);
    oc2rsGateTiming = p[10] | ((uint16_t)p[11] << 8);
    peakTracking[4] = p[12] | ((uint16_t)p[13] << 8);
    peakTracking[3] = p[14] | ((uint16_t)p[15] << 8);
    peakTracking[2] = p[16] | ((uint16_t)p[17] << 8);
    peakTracking[1] = p[18] | ((uint16_t)p[19] << 8);
    peakTracking[0] = p[20] | ((uint16_t)p[21] << 8);
}

/* ============================================================================
 * copyPeaksToFlashBuf (0x4068) — Copy peak registers to flash_read_buf_15D0
 * ============================================================================ */
static void copyPeaksToFlashBuf(void)
{
    uint8_t *p = flash_read_buf_15D0;
    p[0]  = (uint8_t)peakTracking[8];  p[1]  = (uint8_t)(peakTracking[8] >> 8);
    p[2]  = (uint8_t)peakTracking[7];  p[3]  = (uint8_t)(peakTracking[7] >> 8);
    p[4]  = (uint8_t)peakTracking[6];  p[5]  = (uint8_t)(peakTracking[6] >> 8);
    p[6]  = (uint8_t)peakTracking[5];  p[7]  = (uint8_t)(peakTracking[5] >> 8);
    p[8]  = (uint8_t)oc1rsGateTiming; p[9]  = (uint8_t)(oc1rsGateTiming >> 8);
    p[10] = (uint8_t)oc2rsGateTiming; p[11] = (uint8_t)(oc2rsGateTiming >> 8);
    p[12] = (uint8_t)peakTracking[4];  p[13] = (uint8_t)(peakTracking[4] >> 8);
    p[14] = (uint8_t)peakTracking[3];  p[15] = (uint8_t)(peakTracking[3] >> 8);
    p[16] = (uint8_t)peakTracking[2];  p[17] = (uint8_t)(peakTracking[2] >> 8);
    p[18] = (uint8_t)peakTracking[1];  p[19] = (uint8_t)(peakTracking[1] >> 8);
    p[20] = (uint8_t)peakTracking[0];  p[21] = (uint8_t)(peakTracking[0] >> 8);
}

/* ============================================================================
 * at45dbSetBinaryPage256() — 0x38DC
 * AT45DB buffer/page size configuration to 256-byte binary mode:
 *   0x3D, 0x2A, 0x80, 0xA6
 * Note: this is NOT the AT45DB chip-erase command.
 * ============================================================================ */
static void at45dbSetBinaryPage256(void)
{
    at45dbReadStatus(&at45db_status);
    while (!(at45db_status & 0x80))
        at45dbReadStatus(&at45db_status);

    at45dbCsAssert();
    (void)SPI2BUF;
    SPI2BUF = 0x3D; while (!SPI2STATbits.SPIRBF); (void)SPI2BUF;
    SPI2BUF = 0x2A; while (!SPI2STATbits.SPIRBF); (void)SPI2BUF;
    SPI2BUF = 0x80; while (!SPI2STATbits.SPIRBF); (void)SPI2BUF;
    SPI2BUF = 0xA6; while (!SPI2STATbits.SPIRBF);
    at45dbCsDeassert();
}

/* ============================================================================
 * flashClearCalibrationPages (0x3ABA) — Page refresh loop over pages 0..7
 * (called after chip erase in flashCalibrationLoad fallback path)
 * ============================================================================ */
static void flashClearCalibrationPages(void)
{
    memset(flash_sector_buf_1498, 0, 256);
    for (uint16_t page = 0; page < 8; page++) {
        at45dbPageErase(page);
        at45dbBufferWrite(flash_sector_buf_1498, 256);
        at45dbBufferProgramToPage(page);
    }
}


/* ============================================================================
 * computeFaultBit() — 0x4DA4
 * Compute fault bit from protectionStatus/systemFlags
 * ============================================================================ */
void computeFaultBit(void) {
    uint16_t w0 = (protectionStatus >> 14) & 1;
    uint16_t w1 = systemFlags >> 15;
    w0 = (w0 | w1) << 1;
    uint16_t cs = controlStatus;
    cs = (cs & ~(1u << 1)) | (w0 & 2);
    controlStatus = cs;
}

/* ============================================================================
 * adcProcess (0x29FA) — alias for t1IsrI2cBody in i2c_protocol.c
 * ============================================================================ */
void adcProcess(void) {
    t1IsrI2cBody();
}

/* ============================================================================
 * eepromCfgUpdate (0x5A88) — Update eeprom_cfg_reg bit fields
 *   mode   (W0): 2-bit for bits[5:4], 0xFF = no change
 *   status (W1): 2-bit for bits[7:6], 0xFF = no change
 *   enable (W2): 1-bit for bit[0],    0xFF = no change
 * ============================================================================ */
void eepromCfgUpdate(uint8_t mode, uint8_t status, uint8_t enable)
{
    uint16_t reg = eeprom_cfg_reg;
    reg &= 0x00F1;     /* preserve bits 7-4 and bit0, clear bits 3-1 */

    if ((uint8_t)(mode + 1) != 0) {         /* mode != 0xFF */
        uint16_t val = (mode & 0x3) << 4;
        reg &= 0x00C1;                       /* clear bits 5:4 too */
        reg |= val;
    }
    if ((uint8_t)(status + 1) != 0) {       /* status != 0xFF */
        uint16_t val = (status & 0x3) << 6;
        reg &= ~0x00C0;                     /* clear bits 7:6 */
        reg |= val;
    }
    if ((uint8_t)(enable + 1) != 0) {       /* enable != 0xFF */
        reg = (reg & ~1u) | (enable & 1);
    }
    eeprom_cfg_reg = reg;
}

/* ============================================================================
 * eeprom accessors (0x5AB4..0x5AE2)
 * ============================================================================ */
uint16_t eepromCfgRead(void) { return eeprom_cfg_reg; }
uint16_t eepromPageAddrRead(void) { return eeprom_page_addr; }
uint16_t eepromCrcRead(void) { return eeprom_crc_lo; }
uint16_t eepromSavedRead(void) { return eeprom_crc_lo_saved; }

/* ============================================================================
 * i2cProcessCommand (0x5AE4) — Save fw_mode and RESET
 * ============================================================================ */
void i2cProcessCommand(void) {
    eeprom_var_27FC = fw_mode;
    fw_mode = 1;
    eepromCfgUpdate(0xFF, 0xFF, 0);       /* disable enable bit */
    asm("RESET");
}

/* ============================================================================
 * shutdownPwm (0x4B50) — Alias for pwmOverrideEnable() in t2_subroutines.c
 *
 * The assembly at 0x4B50 sets OVRENH/OVRENL on IOCON1/2/3 via BSET on high bytes
 * (0x423/0x443/0x463 bit1/bit0). This is the same function as pwmOverrideEnable().
 * Called from fault handlers to force PWM outputs low via override.
 * ============================================================================ */
extern void pwmOverrideEnable(void);
void shutdownPwm(void) { pwmOverrideEnable(); }

/* ============================================================================
 * pwmDisable (0x4B7C) — Sequenced PWM re-enable with step counter
 * ============================================================================ */
void pwmDisable(void) {
    uint16_t step = pwmSoftStartCnt + 1;
    pwmSoftStartCnt = step;

    if (step == 1) {
        IOCON1 &= ~(1u << 0);
        asm("NOP"); asm("NOP"); asm("NOP");
        IOCON2 &= ~(1u << 1);
        return;
    }
    if (step == 3) {
        IOCON1 &= ~(1u << 1);
        asm("NOP"); asm("NOP"); asm("NOP");
        IOCON2 &= ~(1u << 0);
        asm("NOP"); asm("NOP"); asm("NOP");
        IOCON3 &= ~(1u << 1);
        asm("NOP"); asm("NOP"); asm("NOP");
        IOCON3 &= ~(1u << 0);
        asm("NOP"); asm("NOP"); asm("NOP");
        pwmSoftStartCnt = 4;
        statusFlags2 |= (1u << 13);
    }
}

/* ============================================================================
 * setFaultState (0x1FD6) — Build status word from controlStatus bits
 * ============================================================================ */
static void setFaultStateImpl(void) {
    uint16_t w3 = controlStatus;
    uint16_t f = pwmRunRequest;
    f = (f & ~(1u << 1)) | (((w3 >> 3) & 2));
    f = (f & ~(1u << 2)) | (((w3 & 1) << 2));
    uint16_t b3 = ((w3 >> 3) | (w3 >> (w3 & 0xF))) & 1;
    f = (f & ~(1u << 3)) | (b3 << 3);
    pwmRunRequest = f;

    if ((w3 & (1u << 2)) && (LATD & (1u << 5))) {
        startupResetLatch |= (1u << 8);
        if (droopEnableFlags & (1u << 5)) {
            pwmRunRequest |= (1u << 6);
        }
    } else {
        startupResetLatch &= ~(1u << 8);
    }

    if (auxFlags & (1u << 1)) {
        pwmRunning |= (1u << 1);
        pwmRunRequest &= ~(1u << 4);
    } else {
        pwmRunning &= ~(1u << 1);
        if (!(droopEnableFlags & (1u << 11))) {
            pwmRunRequest |= (1u << 4);
        }
    }

    /* Call status conversion accessors and store results */
    flashUpdateResult = eepromCfgRead();
    eepromCrcShadow = eepromCrcRead();
    eepromPageShadow = eepromPageAddrRead();
    eepromSavedShadow = eepromSavedRead();
}

void setFaultState(void) { setFaultStateImpl(); }

/* ============================================================================
 * updateLatf6Gate (0x37CA)
 *
 * Assembly-equivalent behavior:
 *   - Set LATF bit6 when any of:
 *       1) llcPeriodCmd < 0x1F3F AND runtimeFlags bit5 == 0
 *       2) adcBuf4Raw < 0x13B
 *       3) voutTargetCode != voutRefTarget
 *   - Otherwise, clear LATF bit6 only when (uint16_t)Imeas_cal_a >= 0x129.
 * ============================================================================ */
void updateLatf6Gate(void)
{
    int16_t period_cmd = (int16_t)llcPeriodCmd; /* 0x1DA4 */

    if (period_cmd < 0x1F3F) {
        if (!(runtimeFlags & (1u << 5))) {
            LATF |= (1u << 6);
            return;
        }
    }

    if ((int16_t)adcBuf4Raw < 0x13B) {          /* 0x1D94 */
        LATF |= (1u << 6);
        return;
    }

    if (voutTargetCode != voutRefTarget) {      /* 0x1D4E vs 0x1D4A */
        LATF |= (1u << 6);
        return;
    }

    if ((uint16_t)Imeas_cal_a >= 0x0129u) {     /* 0x1E4E, unsigned compare */
        LATF &= ~(1u << 6);
    }
}

/* ============================================================================
 * checkOtp (0x4D94) — Over-temperature protection LED control
 * ============================================================================ */
void checkOtp(void) {
    if ((droopEnableFlags & (1u << 11)) && (thermalFlags & (1u << 7))) {
        LATF |= (1u << 1);
    } else {
        LATF &= ~(1u << 1);
    }
}

/* ============================================================================
 * updatePmbusStatus (0x546C) — PMBus status and droop output control
 * ============================================================================ */
void updatePmbusStatus(void) {
    if (!(currentLimitFlags & 1))
        currentLimitFlags &= ~(1u << 13);

    uint16_t latd = LATD;
    int do_fan = 0;

    if ((latd & (1u << 4)) && !(statusFlags2 & (1u << 5))
        && (vrefModeSelect == 0xC4D) && (systemFlags & 0xA0)) {
        do_fan = 1;
    }

    if (!do_fan) {
        if (runtimeFlags & (1u << 5)) goto end_section;
        if (flash_read_buf_15E6[0x10] != 0) goto end_section;
        if (currentLimitFlags & (1u << 13)) {
            if ((int16_t)ioutDefault > 0x3E) {
                if ((int16_t)ioutDefault > 0x56) goto turn_on;
                goto end_section;
            }
        }
        LATD &= ~(1u << 5);
        goto check_droop;
    }

turn_on:
    LATD |= (1u << 5);
    goto check_droop;

end_section:
    LATD &= ~(1u << 5);

check_droop:
    if (!(LATD & (1u << 5))) {
        if (droopMode == 4) {
            oc2rs_shadow = 0x190;
            return;
        }
        oc2rs_shadow = 0x64;
    }
}

/* ============================================================================
 * computeTargetOc2rs (0x54C2) — OC2RS target computation
 * Sub-functions: scaleImeasCal (0x534A), classifyDelta (0x5364),
 *   table_lookup (0x5398), compute_droop_adj (0x53D2),
 *    (0x540A)
 * ============================================================================ */

/* 0x534A: scale Imeas_cal_a * 11 / 559, clamp to 10 */
static int16_t scaleImeasCal(void) {
    int32_t prod = (int32_t)Imeas_cal_a * 11;
    uint16_t rem = (uint16_t)(prod % 559);
    if (rem > 10) rem = 10;
    return (int16_t)rem;
}

/* 0x5364: map (val - ref) to zone 0..4 */
static int16_t classifyDelta(int16_t val, int16_t ref) {
    int16_t diff = val - ref;
    if (diff > 5)  return 0;
    if (diff > 1)  return 1;
    if (diff < -5) return 4;
    if (diff < -1) return 3;
    return 2;
}

/* ============================================================================
 * oc2rsLinearAdjustment() — 0x540A
 * Piecewise linear OC2RS adjustment based on setpoint vs output voltage
 * ============================================================================ */
static void oc2rsLinearAdjustment(void) {
    uint16_t sp = secondaryOcSetpoint;
    uint16_t vout = outputVoltage;
    int16_t adj = 0;
    int do_adj = 1;

    if (sp > vout + 3000)       adj = 50;
    else if (sp > vout + 100)   adj = 25;
    else if (sp > vout - 100)   adj = 12;
    else if (vout > sp + 3000)  adj = -50;
    else if (vout > sp + 1200)  adj = -12;
    else if (vout > sp + 300)   adj = -6;
    else do_adj = 0;

    if (do_adj)
        oc2rs_shadow += adj;

    /* Clamp to [199 .. 1999] */
    if ((int16_t)oc2rs_shadow > 0x7CF)
        oc2rs_shadow = 0x7CF;
    else if ((int16_t)oc2rs_shadow <= 0xC6)
        oc2rs_shadow = 0xC7;
}

void computeTargetOc2rs(void) {
    uint16_t cnt = oc2rsUpdateCnt + 1;
    oc2rsUpdateCnt = cnt;
    if ((int16_t)cnt <= 9) return;
    oc2rsUpdateCnt = 0;

    int16_t imeas_zone = scaleImeasCal();
    int16_t zone_lo = classifyDelta(ioutLimitLo, 0x4D);
    int16_t zone_def = classifyDelta(ioutDefault, 0x4D);
    if (zone_lo > zone_def) zone_lo = zone_def;

    /* 2D table lookup at PSV address 0x934A */
    /* uint16_t table_idx = (uint16_t)imeas_zone * 5 + zone_lo; */
    /* int16_t table_adj = psv_table[table_idx]; */
    /* secondaryOcSetpoint += table_adj; */

    /* Droop adjustment: check ioutLimitLo and ioutDefault vs 0x57 */
    if (ioutLimitLo > 0x57 && ioutDefault > 0x57) {
        oc2rs_shadow = 0x7CF;
        return;
    }

    /* Clamp secondaryOcSetpoint to [0x5DC .. 0x54C4] */
    if (secondaryOcSetpoint > 0x54C4)
        secondaryOcSetpoint = 0x54C4;
    if ((int16_t)secondaryOcSetpoint <= 0x5DB)
        secondaryOcSetpoint = 0x5DC;
}

/* ============================================================================
 * checkCurrentLimit (0x4BD4) — Current limit timer
 * ============================================================================ */
void checkCurrentLimit(void) {
    uint16_t flags = currentLimitFlags;
    if (!(flags & 0x100)) {
        vinUvSubCounter = 0;
        vinUvSecCounter = 0;
        return;
    }
    uint16_t ticks = vinUvSubCounter + 1;
    vinUvSubCounter = ticks;
    if (ticks > 0x3E7) {
        vinUvSubCounter = 0;
        vinUvSecCounter++;
    }
}

/* ============================================================================
 * updateStatusLeds (0x571E) — Build 8-byte LED status packet, TX via UART1
 *
 * Packet types:
 *   Header 0x58 = status (from system state)
 *   Header 0x60 = streaming (from UART RX buffer)
 *
 * Status packet layout:
 *   [0]=0x58, [1]=fault_byte, [2:3]=voutScaledQ2(BE),
 *   [4:5]=RAM16(0x1D0E)(BE), [6:7]=cal_var_1E42(BE), [8]=checksum
 * ============================================================================ */
void updateStatusLeds(void) {
    uint16_t tx_idx = uartTxByteIdx;

    if (tx_idx != 0)
        goto transmit_phase;

    /* Streaming mode check */
    if ((llcStatus & 0x0100) && (currentLimitFlags & 0x02)) {
        uint8_t *pkt = uartTxBuf;
        pkt[0] = 0x60;
        uint16_t checksum = 0;
        uint8_t *src = uartStreamSrc;
        for (uint16_t i = 1; i <= 7; i++) {
            pkt[i] = src[i - 1];
            checksum += pkt[i];
        }
        uartTxChecksum = checksum;
        uartTxPacketLen = 8;
        uartTxBuf[8] = (uint8_t)checksum;
        goto transmit_phase;
    }

    /* Build new status packet */
    if (uartTxPending != 0)
        goto transmit_phase;

    {
        uint8_t *pkt = uartTxBuf;
        uint16_t checksum = 0;
        uint16_t freq = voutScaledQ2;
        uint16_t val_1D0E = uartStatusWord;
        uint16_t cal = cal_var_1E42;

        pkt[0] = 0x58;
        pkt[1] = ((protectionStatus & 0x1000) || systemState == ST_HOLDOFF) ? 0x02 : 0x00;
        pkt[2] = (uint8_t)(freq >> 8);
        pkt[3] = (uint8_t)(freq);
        pkt[4] = (uint8_t)(val_1D0E >> 8);
        pkt[5] = (uint8_t)(val_1D0E);
        pkt[6] = (uint8_t)(cal >> 8);
        pkt[7] = (uint8_t)(cal);

        for (uint16_t i = 0; i < 8; i++)
            checksum += pkt[i];

        uartTxChecksum = checksum;
        uartTxPacketLen = 8;
        uartTxBuf[8] = (uint8_t)checksum;
        uartTxPending = 1;
    }

transmit_phase:
    /* Check U1STA.TRMT (bit8): TX shift register empty */
    if (!(U1STA & 0x0100))
        return;

    /* Prescaler: send 1 byte per 10 calls */
    {
        uint16_t prescaler = uartTxPrescaler + 1;
        uartTxPrescaler = prescaler;
        if (prescaler <= 9) return;
        uartTxPrescaler = 0;
    }

    /* Send one byte */
    tx_idx = uartTxByteIdx;
    U1TXREG = (uint16_t)uartTxBuf[tx_idx];
    tx_idx++;
    uartTxByteIdx = tx_idx;

    if (tx_idx <= 8) return;

    /* Packet fully sent */
    if (uartTxBuf[0] == 0x60) {
        if (currentLimitFlags & 0x02) {
            currentLimitFlags &= ~0x02;
            IFS0 |= 0x1000;        /* set U1TXIF */
        }
    }

    uartTxByteIdx = 0;
    uartTxPending = 0;
}

/* ============================================================================
 * flashCollectStatusData (0x3E48) — Read flash page sector data into status buffer
 *
 * Assembly 0x3E48 – 0x3EAA:
 *   - Copies flash_verify_flags[0..2] and flash_crc_accum to 1598[0..3]
 *   - Loop 1 (W9=1..7): reads page 1, byte_offset = W9*32 (sectors 1-7),
 *     extracts local_buf[8:9] into 1598[4..17]
 *   - Loop 2 (W8=0..2): reads page 2, byte_offset = W8*32 (sectors 0-2),
 *     extracts local_buf[8:9] into 1598[18..23]
 * ============================================================================ */
static void flashCollectStatusData(void) {
    flash_sector_buf_1598[0] = flash_verify_flags[0];
    flash_sector_buf_1598[1] = flash_verify_flags[1];
    flash_sector_buf_1598[2] = flash_verify_flags[2];
    flash_sector_buf_1598[3] = (uint8_t)flash_crc_accum;

    uint8_t local_buf[32];

    /* Read 7 sectors from page 1 (byte offsets 32,64,...,224), extract bytes 8-9 */
    uint16_t idx = 0;
    for (uint16_t i = 1; i < 8; i++) {
        at45dbPageRead(local_buf, 32, 1, i * 32);
        flash_sector_buf_1598[idx + 4] = local_buf[8];
        flash_sector_buf_1598[idx + 5] = local_buf[9];
        idx += 2;
    }

    /* Read 3 sectors from page 2 (byte offsets 0,32,64), extract bytes 8-9 */
    for (uint16_t i = 0; i < 3; i++) {
        at45dbPageRead(local_buf, 32, 2, i * 32);
        flash_sector_buf_1598[i * 2 + 18] = local_buf[8];
        flash_sector_buf_1598[i * 2 + 19] = local_buf[9];
    }
}

/* ============================================================================
 * flashSaveStatusPage (0x3EAC) — Read fan data, write to flash page 5
 * ============================================================================ */
void flashSaveStatusPage(void) {
    flashCollectStatusData();
    at45dbPageErase(5);
    at45dbBufferWrite(flash_sector_buf_1598, 24);
    at45dbBufferProgramToPage(5);
}

/* ============================================================================
 * flashPeriodicSave (0x3EBC) — Fan tick counter and periodic flash update
 * ============================================================================ */
void flashPeriodicSave(void) {
    if (!(pwmRunning & 0x01)) return;
    if (llcStatus & 0x100) return;
    if (LATF & (1u << 1)) return;

    fanTickCounter++;

    if (fanTickCounter <= 0x000493DFUL) return;    /* ~300,000 = ~5 min at 1ms tick */

    fanTickCounter = 0;

    if (fanVerifyCount <= 0x00FFFFFEu)
        fanVerifyCount++;

    fanSpeedAccum++;
    if (fanSpeedAccum > 0x59F) {
        fanSpeedAccum = 0;
        flashSaveStatusPage();
    }
}

/* ============================================================================
 * flashCalibrationLoad (0x3B5E) — Flash calibration loader / initialization
 * ============================================================================ */
void flashCalibrationLoad(void) {
    if (flashCtrlFlags & 0x02) return;
    flashCtrlFlags |= 0x02;

    at45dbReadStatus(&at45db_status);

    /* Read page 6 (256 bytes) -> flash_read_buf_160E */
    at45dbPageRead(flash_read_buf_160E, 0x100, 6, 0);

    /* Check if page is blank (0xFF, 0xFF) */
    if (flash_read_buf_160E[0] == 0xFF && flash_read_buf_160E[1] == 0xFF)
        flashCtrlFlags |= 0x08;
    else
        flashCtrlFlags &= ~0x08;

    /* Flash present check (status bit0) */
    if (!(at45db_status & 0x01))
        goto flash_not_present;

    if (flashCtrlFlags & 0x08)
        goto flash_not_present;

    /* Read calibration pages */
    at45dbPageRead(flash_read_buf_15D0, 0x16, 0, 0);
    at45dbPageRead(flash_sector_buf_1598, 0x18, 5, 0);
    at45dbPageRead((uint8_t *)(void *)flash_read_buf_15E6, 0x20, 7, 0);

    /* Copy verify flags */
    flash_verify_flags[0] = flash_sector_buf_1598[0];
    flash_verify_flags[1] = flash_sector_buf_1598[1];
    flash_verify_flags[2] = flash_sector_buf_1598[2];
    flash_crc_accum = (uint16_t)flash_sector_buf_1598[3];

    /* Check calibration presence */
    if (flash_read_buf_15E6[0] == 0 && flash_read_buf_15E6[1] == 0)
        statusFlags &= ~(1u << 6);
    else
        statusFlags |= (1u << 6);

    /* Special current mode (0x15F6 = flash_read_buf_15E6[0x10]) */
    if (flash_read_buf_15E6[0x10] == 0x01) {
        outputCurrent = (int16_t)0x8000;
        currentLimitFlags |= (1u << 0);
        droopEnableFlags |= (1u << 10);
        droopEnableFlags |= (1u << 3);
    }

    /* Verify 13 checksummed regions in flash_read_buf_160E */
    {
        uint8_t retry = 0;
        do {
            uint16_t vr = 0;
            uint16_t c;

            c = flashVerifyChecksum(flash_read_buf_160E, 0x00, 0x06, flash_read_buf_160E[0x07]);
            vr = (vr & ~1u) | (c & 1);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x08, 0x26, flash_read_buf_160E[0x27]);
            vr = (vr & ~2u) | ((c & 1) << 1);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x28, 0x6E, flash_read_buf_160E[0x6F]);
            vr = (vr & ~4u) | ((c & 1) << 2);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x70, 0x73, flash_read_buf_160E[0x74]);
            vr = (vr & ~8u) | ((c & 1) << 3);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x75, 0x8C, flash_read_buf_160E[0x73]);
            vr = (vr & ~0x10u) | ((c & 1) << 4);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x8D, 0x90, flash_read_buf_160E[0x91]);
            vr = (vr & ~0x20u) | ((c & 1) << 5);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x92, 0x9E, flash_read_buf_160E[0x90]);
            vr = (vr & ~0x40u) | ((c & 1) << 6);

            c = flashVerifyChecksum(flash_read_buf_160E, 0x9F, 0xA2, flash_read_buf_160E[0xA3]);
            vr = (vr & ~0x80u) | ((c & 1) << 7);

            c = flashVerifyChecksum(flash_read_buf_160E, 0xA4, 0xB0, flash_read_buf_160E[0xA2]);
            vr = (vr & ~0x100u) | ((c & 1) << 8);

            c = flashVerifyChecksum(flash_read_buf_160E, 0xB1, 0xB4, flash_read_buf_160E[0xB5]);
            vr = (vr & ~0x200u) | ((c & 1) << 9);

            c = flashVerifyChecksum(flash_read_buf_160E, 0xB6, 0xC7, flash_read_buf_160E[0xB4]);
            vr = (vr & ~0x400u) | ((c & 1) << 10);

            c = flashVerifyChecksum(flash_read_buf_160E, 0xC8, 0xCB, flash_read_buf_160E[0xCC]);
            vr = (vr & ~0x800u) | ((c & 1) << 11);

            c = flashVerifyChecksum(flash_read_buf_160E, 0xCD, 0xFF, flash_read_buf_160E[0xCB]);
            vr = (vr & ~0x1000u) | ((c & 1) << 12);

            flashChecksumResult = vr;

            if (vr == 0) break;   /* all 13 checksums passed => done */

            at45dbPageRead(flash_read_buf_160E, 0x100, 6, 0);
            retry++;
        } while (retry < 3);
    }

    goto epilogue;

flash_not_present:
    //at45dbSetBinaryPage256();       /* 0x38DC */
    //flashClearCalibrationPages();         /* 0x3ABA */

    flash_verify_flags[0] = 0;
    flash_verify_flags[1] = 0;
    flash_verify_flags[2] = 0;
    flash_crc_accum = 0;

    memset(flash_read_buf_160E, 0, 0x100);
    memset(flash_read_buf_15D0, 0, 0x16);
    memset(flash_sector_buf_1598, 0, 0x18);

epilogue:
    loadCalibrationFromFlash();
}

/* ============================================================================
 * voltageErrorTracking (0x40C2) — Peak voltage/current tracking
 * ============================================================================ */
void voltageErrorTracking(void) {
    if (LATF & (1u << 1)) return;

    if (systemFlags & (1u << 3)) {
        systemFlags &= ~(1u << 3);

        memset(flash_read_buf_15D0, 0, 0x16);

        loadCalibrationFromFlash();
        /* at45dbPageErase(0); */
        /* at45dbBufferWrite(flash_read_buf_15D0, 0x16); */
        /* at45dbBufferProgramToPage(0); */
        return;
    }

    if (llcStatus & 0x0100) return;

    uint16_t groupA = auxFlags & 0x2;
    uint16_t groupB = 0;
    uint8_t updated = 0;

    /* Group A: instantaneous ADC peaks */
    if (groupA) {
        if (adcLiveA > peakTracking[8]) { peakTracking[8] = adcLiveA; updated = 1; }
        if (adcLiveB > peakTracking[7]) { peakTracking[7] = adcLiveB; updated = 1; }
        if (adcLiveC > peakTracking[6]) { peakTracking[6] = adcLiveC; updated = 1; }
    }

    /* Group B: PWM shadow peaks */
    if (pwmRunning & 0x1) {
        groupB = 1;
        if (adcLive1 > peakTracking[5]) { peakTracking[5] = adcLive1; updated = 1; }
        if (pdcShadowB > oc1rsGateTiming) { oc1rsGateTiming = pdcShadowB; updated = 1; }
        if (pdc3Shadow > oc2rsGateTiming) { oc2rsGateTiming = pdc3Shadow; updated = 1; }
    }

    /* Group C: slow telemetry peaks */
    if (!groupA && !groupB) goto done;

    if (adcLive2 > peakTracking[4]) { peakTracking[4] = adcLive2; updated = 1; }
    if (adcLive3 > peakTracking[3]) { peakTracking[3] = adcLive3; updated = 1; }
    if (adcLive4 > peakTracking[2]) { peakTracking[2] = adcLive4; updated = 1; }
    if (adcLive5 > peakTracking[1]) { peakTracking[1] = adcLive5; updated = 1; }
    {
        uint16_t v = outputVoltage;
        if (v > peakTracking[0] && v > 0xC7) { peakTracking[0] = v; updated = 1; }
    }

done:
    if (updated) {
        statusFlags2 &= ~(1u << 12);
        copyPeaksToFlashBuf();
        /* at45dbPageErase(0); */
        /* at45dbBufferWrite(flash_read_buf_15D0, 0x16); */
        /* at45dbBufferProgramToPage(0); */
    }
}

/* ============================================================================
 * Snapshot helpers for currentRegulation
 * ============================================================================ */
static void snapshotStatusToSectorBuf(void) {
    flash_sector_buf_1498[0] = (uint8_t)flash_crc_accum;
    flash_sector_buf_1498[1] = flash_verify_flags[0];
    flash_sector_buf_1498[2] = flash_verify_flags[1];
    flash_sector_buf_1498[3] = flash_verify_flags[2];
    flash_sector_buf_1498[4] = (uint8_t)llcStatus;
    flash_sector_buf_1498[5] = (uint8_t)(llcStatus >> 8);
    flash_sector_buf_1498[6] = (uint8_t)pwmRunning;
    flash_sector_buf_1498[7] = (uint8_t)(pwmRunning >> 8);
    flash_sector_buf_1498[8] = (uint8_t)startupResetLatch2;
    flash_sector_buf_1498[9] = (uint8_t)(startupResetLatch2 >> 8);
    flash_sector_buf_1498[10] = (uint8_t)startupResetLatch;
    flash_sector_buf_1498[11] = (uint8_t)(startupResetLatch >> 8);
    flash_sector_buf_1498[12] = (uint8_t)flashUpdateResult;
    flash_sector_buf_1498[13] = (uint8_t)(flashUpdateResult >> 8);
    flash_sector_buf_1498[14] = (uint8_t)eepromCrcShadow;
    flash_sector_buf_1498[15] = (uint8_t)(eepromCrcShadow >> 8);
    flash_sector_buf_1498[16] = (uint8_t)eepromPageShadow;
    flash_sector_buf_1498[17] = (uint8_t)(eepromPageShadow >> 8);
    flash_sector_buf_1498[18] = (uint8_t)droopEnableFlags;
    flash_sector_buf_1498[19] = (uint8_t)(droopEnableFlags >> 8);
    flash_sector_buf_1498[20] = (uint8_t)ioutScaleConst;
    flash_sector_buf_1498[21] = (uint8_t)(ioutScaleConst >> 8);
    flash_sector_buf_1498[22] = (uint8_t)ocpThresholdHw;
    flash_sector_buf_1498[23] = (uint8_t)(ocpThresholdHw >> 8);
    flash_sector_buf_1498[24] = (uint8_t)ioutAdcRaw;
    flash_sector_buf_1498[25] = (uint8_t)(ioutAdcRaw >> 8);
    flash_sector_buf_1498[26] = (uint8_t)ioutCalFactor;
    flash_sector_buf_1498[27] = (uint8_t)(ioutCalFactor >> 8);
}

static void snapshotTelemetryToSectorBuf(void) {
    flash_sector_buf_1498[0] = (uint8_t)flash_crc_accum;
    flash_sector_buf_1498[1] = (uint8_t)adcLiveA;
    flash_sector_buf_1498[2] = (uint8_t)(adcLiveA >> 8);
    flash_sector_buf_1498[3] = (uint8_t)adcLiveA1;
    flash_sector_buf_1498[4] = (uint8_t)(adcLiveA1 >> 8);
    flash_sector_buf_1498[5] = (uint8_t)adcLiveA2;
    flash_sector_buf_1498[6] = (uint8_t)(adcLiveA2 >> 8);
    flash_sector_buf_1498[7] = (uint8_t)adcLive1;
    flash_sector_buf_1498[8] = (uint8_t)(adcLive1 >> 8);
    flash_sector_buf_1498[9] = (uint8_t)pdcShadowA;
    flash_sector_buf_1498[10] = (uint8_t)(pdcShadowA >> 8);
    flash_sector_buf_1498[11] = (uint8_t)adcLiveB;
    flash_sector_buf_1498[12] = (uint8_t)(adcLiveB >> 8);
    flash_sector_buf_1498[13] = (uint8_t)adcLiveC;
    flash_sector_buf_1498[14] = (uint8_t)(adcLiveC >> 8);
    flash_sector_buf_1498[15] = (uint8_t)pdcShadowB;
    flash_sector_buf_1498[16] = (uint8_t)(pdcShadowB >> 8);
    flash_sector_buf_1498[17] = (uint8_t)adcLive2;
    flash_sector_buf_1498[18] = (uint8_t)(adcLive2 >> 8);
    flash_sector_buf_1498[19] = (uint8_t)adcLive3;
    flash_sector_buf_1498[20] = (uint8_t)(adcLive3 >> 8);
    flash_sector_buf_1498[21] = (uint8_t)adcLive4;
    flash_sector_buf_1498[22] = (uint8_t)(adcLive4 >> 8);
    flash_sector_buf_1498[23] = (uint8_t)adcLive5;
    flash_sector_buf_1498[24] = (uint8_t)(adcLive5 >> 8);
    flash_sector_buf_1498[25] = (uint8_t)outputCurrent;
    flash_sector_buf_1498[26] = (uint8_t)(outputCurrent >> 8);
    flash_sector_buf_1498[27] = (uint8_t)outputVoltage;
    flash_sector_buf_1498[28] = (uint8_t)(outputVoltage >> 8);
}

/* ============================================================================
 * currentRegulation (0x3F0E) — Flash sector management / telemetry snapshot
 * ============================================================================ */
void currentRegulation(void) {
    if (!(pmbusAlertFlags & 1u)) return;
    if (llcStatus & 0x100) return;
    if (LATF & (1u << 1)) return;
    if (startupResetLatch2 == 0) return;

    pmbusAlertFlags &= ~1u;

    /* Read sector buffer from flash page 1 */
    at45dbPageRead(flash_sector_buf_1498, 256, 1, 0);

    if ((auxFlags & 2) && (pwmRunning & 8) && (systemState != 5)) {
        /* Active mode */
        flash_crc_accum++;

        uint8_t local_frame[64];
        at45dbPageRead(local_frame, 64, 2, 0);

        /* Clear sector padding */
        for (uint16_t a = 0; a < 256; a++)
            flash_sector_buf_1498[a] = 0;

        snapshotStatusToSectorBuf();

        /* Write to flash page 1 */
        at45dbPageErase(1);
        at45dbBufferWrite(flash_sector_buf_1498, 256);
        at45dbBufferProgramToPage(1);

        /* Read page 3 */
        at45dbPageRead(flash_sector_buf_1498, 256, 3, 0);

        for (uint16_t a = 0; a < 256; a++)
            flash_sector_buf_1498[a] = 0;

        snapshotTelemetryToSectorBuf();
    } else {
        /* Idle mode */
        snapshotStatusToSectorBuf();

        at45dbPageErase(1);
        at45dbBufferWrite(flash_sector_buf_1498, 256);
        at45dbBufferProgramToPage(1);

        at45dbPageRead(flash_sector_buf_1498, 256, 3, 0);
        snapshotTelemetryToSectorBuf();
    }

    /* Common: write to flash page 3, then update fan */
    at45dbPageErase(3);
    at45dbBufferWrite(flash_sector_buf_1498, 256);
    at45dbBufferProgramToPage(3);
    flashSaveStatusPage();
}

/* ============================================================================
 * flashReadbackHandler (0x42C8) — Flash readback/erase handler
 * ============================================================================ */
void flashReadbackHandler(void) {
    if (!(systemFlags & (1u << 6))) return;
    if (LATF & (1u << 1)) return;

    systemFlags &= ~(1u << 6);

    memset(flash_sector_buf_1498, 0, 256);

    for (uint8_t page = 1; page < 6; page++) {
        at45dbPageErase(page);
        at45dbBufferWrite(flash_sector_buf_1498, 256);
        at45dbBufferProgramToPage(page);
    }

    flash_verify_flags[0] = 0;
    flash_verify_flags[1] = 0;
    flash_verify_flags[2] = 0;
    flash_crc_accum = 0;

    memset(flash_sector_buf_1598, 0, 24);
}

/* ============================================================================
 * runNormalOc2rsUpdate5522 (0x5522) — LLC normal run: compute OC2RS, write HW register
 * ============================================================================ */
static void runNormalOc2rsUpdate5522(void) {
    if (!(auxFlags & (1u << 8))) return;
    auxFlags &= ~(1u << 8);

    uint16_t vout = outputVoltage;
    if (vout <= 0xC7) {
        /* Voltage too low: slow ramp */
        OC2RS += 20;
        if (OC2RS > 0x18F)
            OC2RS = 0x190;
        return;
    }

    computeTargetOc2rs();
    uint16_t target = oc2rs_shadow;
    oc2rsTarget = target;

    uint16_t limit = oc2rsUpperLimit;
    if (limit < target) target = limit;

    if (currentLimitFlags & 1) {
        uint32_t prod = (uint32_t)outputCurrent * 0x7D1;
        uint16_t scaled = (uint16_t)(prod >> 8);
        if (target > scaled) target = scaled;
    }

    OC2RS = target;
}

/* ============================================================================
 * droopMode0Handler() — 0x546C
 * Droop mode 0: LATD.5 and oc2rs_shadow control
 * (Wrapper that calls updatePmbusStatus)
 * ============================================================================ */
void droopMode0Handler(void) {
    updatePmbusStatus();
}

/* ============================================================================
 * droopMode3Handler()
 * Droop mode 3 path (table-lookup-based compensation)
 * Integrated into computeTargetOc2rs via compute_droop_adj
 * ============================================================================ */
void droopMode3Handler(void) {
    /* Droop mode 3 logic is handled inside computeTargetOc2rs.
     * This entry point triggers the full OC2RS recomputation. */
    computeTargetOc2rs();
}

/* ============================================================================
 * uart1RxHandler (0x5582) — UART1 RX state machine
 * ============================================================================ */
void uart1RxHandler(void) {
    if (!(U1STA & 1)) return;
    if (llcStatus & 0x100) return;

    uint16_t rx = U1RXREG;
    uartRxByteCount = 0;
    uint16_t state = uartRxState;

    if (state == 0) {
        if (rx == 0x49)      uartRxState = 1;
        else if (rx == 0x56) uartRxState = 8;
        else return;
        uartRxBufIdx = 0;
        return;
    }
    if (state == 1) {
        if (rx != 0x34) { uartRxState = 0; return; }
        uartRxState = 2;
        return;
    }
    if (state == 2) {
        uint16_t idx = uartRxBufIdx;
        uartRxBuf[idx] = rx;
        idx++;
        uartRxBufIdx = idx;
        if (idx != 5) return;
        uint16_t sum = uartRxBuf[0] + uartRxBuf[1] +
                       uartRxBuf[2] + uartRxBuf[3];
        if ((sum & 0xFF) != rx) { uartRxState = 0; return; }
        uartCmdParam0 = (uartRxBuf[1] << 8) + uartRxBuf[0];
        uartCmdParam1 = (uartRxBuf[3] << 8) + uartRxBuf[2];
        return;
    }
    if (state == 8) {
        if (rx == 0x34)      { auxFlags |= (1u << 6); uartRxState = 9; }
        else if (rx == 0x33) { auxFlags &= ~(1u << 6); uartRxState = 9; }
        else                 uartRxState = 0;
        return;
    }
    if (state == 9) {
        uint16_t idx = uartRxBufIdx;
        uartRxBuf[idx] = rx;
        idx++;
        uartRxBufIdx = idx;
        if (idx != 13) return;
        /* Extended command: checksum verify + parameter extract */
        {
            uint16_t sum = 0;
            for (uint16_t i = 0; i < 12; i++)
                sum += uartRxBuf[i];
            if ((sum & 0xFF) == uartRxBuf[12]) {
                /* Valid: extract parameters */
                uartCmdParamExt = (uartRxBuf[1] << 8) + uartRxBuf[0];
            }
        }
        uartRxState = 0;
        return;
    }
    uartRxState = 0;
}

/* ---- I2C2 / PMBus comm sub-routines (stubs, addresses noted) ---- */
void i2cEncodeResponse(void) { }         /* 0x1616 */
void i2cThresholdMonitor(void) { }       /* 0x215A */
void i2cFlagUpdate(void) { }             /* 0x21FE */
void i2cChecksumValidate(void) { }       /* 0x2298 */
void i2cIoutUpdatePath(void) { }         /* 0x230C */
void i2cIoutScaling(void) { }            /* 0x231C */
void i2cTempBandSelect(void) { }         /* 0x2378 */
void i2cTempHysteresis(void) { }         /* 0x2382 */
void i2cPeakCurrentTrack(void) { }       /* 0x2466 */
void statusByteSync249c(void) { }        /* 0x249C */
void i2cStatusFlagSync(void) { }         /* 0x255A */
void i2cPowerEnableCtrl(void) { }        /* 0x25D4 */
void i2cTargetPeriodUpdate(void) { }     /* 0x25FC */
void i2cOvThresholdCheck(void) { }       /* 0x26A0 */

/* ---- Helper functions referenced by t1_sub_prot.c ---- */
uint16_t thresholdCompare(int16_t adc_val, int16_t upper_thresh,
                           int16_t lower_thresh, int16_t prev_state)
{
    uint16_t result = (uint8_t)prev_state;

    /* 0x30E2..0x30F6 — hysteresis comparator:
     *   value >= upper  -> 1 (above upper threshold)
     *   value <= lower  -> 0 (below lower threshold)
     *   in between      -> prev_state (hysteresis band)
     */
    if ((uint16_t)adc_val >= (uint16_t)upper_thresh) {
        result = 1u;
    } else if ((uint16_t)adc_val <= (uint16_t)lower_thresh) {
        result = 0u;
    }

    return (uint16_t)((uint8_t)result);
}

uint16_t debounceCounter(int16_t new_val, int16_t limit,
                          volatile int16_t *cnt, int16_t prev_val)
{
    if (new_val != prev_val) {
        int16_t next = (int16_t)(*cnt + 1);
        *cnt = next;

        /* 0x31B2..0x31CA:
         *   cnt < limit  -> clear counter and latch new value immediately
         *   cnt >= limit -> keep previous value and keep counter
         */
        if (next < limit) {
            *cnt = 0;
            prev_val = new_val;
        }
    } else {
        *cnt = 0;
        prev_val = new_val;
    }

    return (uint16_t)((uint8_t)prev_val);
}

/* ---- CRC16 lookup table (256 entries, used by flash_ops.c) ---- */
const uint16_t crc16_table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};

/* ============================================================================
 * delay_us / delay_ms — blocking delays (Fcy = 50 MHz)
 * ============================================================================ */
void delay_us(uint16_t us)
{
    while (us--)
        __delay_us(1);
}

void delay_ms(uint16_t ms)
{
    while (ms--)
        __delay_ms(1);
}
