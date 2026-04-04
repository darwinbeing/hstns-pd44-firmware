/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — uart_cmd.c
 * UART1 command handler + AT45DB021E SPI Flash operations
 *
 * Functions:
 *   flashPageProgramRead()  — 0x41A2 (bit1: Flash write 2 bytes + read 256 bytes)
 *   flashPageReadWrite()  — 0x41BE (bit0: read/write Flash page)
 *   fwUpdateWriteVerify()  — 0x425E -> 0x41E0 (firmware update + CRC verification)
 *   flashReadPage6()  — 0x4260 (bit4: read Flash page 6, 256 bytes)
 *   flashReadPage7()  — 0x427E (bit6: read Flash page 7, 32 bytes)
 *   flashProgramRead32()  — 0x429C (bit5: Flash 32-byte read)
 *
 * SPI Flash low-level:
 *   at45dbReadStatus()       — 0x38B6
 *   at45dbWaitReady()       — ready polling helper
 *   at45dbPageErase()       — 0x3922 (opcode 0x81)
 *   at45dbBufferWrite()     — 0x396E (opcode 0x84)
 *   at45dbBufferProgramToPage() — 0x39D0 (opcode 0x88)
 *   at45dbPageRead()        — 0x3A1C (opcode 0xD2)
 *   at45dbReadDeviceId()    — JEDEC ID read (opcode 0x9F)
 *
 * CRC:
 *   crc16Update()  — 0x5B74
 *   crc16Block()   — 0x5B86
 *
 * EEPROM emulation:
 *   eepromSetCrcLo()     — 0x5ACC
 *   eepromSetPageAddr()  — 0x5AD4
 *   eepromGetCrcLo()     — 0x5AE0
 *   eepromSetConfig()     — 0x5A88
 *
 * Safety interlock: LATF bit1 = PSU output active, skip Flash ops when active
 * ============================================================================ */
#include <xc.h>
#include <stdint.h>
#include "variables.h"

#ifndef AT45DB_READY_DELAY_NOPS
#define AT45DB_READY_DELAY_NOPS 0u
#endif

static inline void at45dbCsAssert(void)
{
    LATGbits.LATG9 = 0;
}

static inline void at45dbCsDeassert(void)
{
    LATGbits.LATG9 = 1;
}

/* ---- SPI Flash low-level functions ---- */

#ifndef UNIT_TEST_MINIMAL

/* 0x38B6: Read AT45DB021E status register (opcode 0xD7) */
void at45dbReadStatus(uint16_t *dest)
{
    at45dbCsAssert();                      /* CS low (LATG9, 0x2F4 bit9) */
    (void)SPI2BUF;                          /* flush */
    SPI2BUF = 0xD7;                         /* status read command */
    while (!(SPI2STATbits.SPIRBF));         /* wait TX complete */
    (void)SPI2BUF;
    SPI2BUF = 0x00;                         /* clock out status byte */
    while (!(SPI2STATbits.SPIRBF));
    *dest = SPI2BUF;
    at45dbCsDeassert();                    /* CS high */
}

void at45dbWaitReady(void)
{
    for (;;) {
        at45dbReadStatus(&at45db_status);
        if (at45db_status & (1u << 7)) {
            return;
        }
#if AT45DB_READY_DELAY_NOPS > 0
        for (uint16_t i = 0; i < (uint16_t)AT45DB_READY_DELAY_NOPS; i++) {
            Nop();
        }
#endif
    }
}

/* 0x3922: Main Memory Page Erase (opcode 0x81) */
void at45dbPageErase(uint16_t page_num)
{
//    at45dbWaitReady();
//    at45dbCsAssert();
//    (void)SPI2BUF;
//    SPI2BUF = 0x81;
//    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
//    SPI2BUF = (page_num >> 8) & 0x03;      /* page[9:8] */
//    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
//    SPI2BUF = page_num & 0xFF;             /* page[7:0] */
//    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
//    SPI2BUF = 0x00;                         /* byte offset = 0 */
//    while (!(SPI2STATbits.SPIRBF));
//    at45dbCsDeassert();
}

/* 0x396E: Buffer1 Write (opcode 0x84) */
void at45dbBufferWrite(const uint8_t *src, uint16_t len)
{
    at45dbWaitReady();
    at45dbCsAssert();
    (void)SPI2BUF;
    SPI2BUF = 0x84;                         /* Buffer 1 Write */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;                         /* 3-byte buffer address */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF));

    for (uint16_t i = 0; i < len; i++) {
        (void)SPI2BUF;
        SPI2BUF = src[i];                   /* write payload byte */
        while (!(SPI2STATbits.SPIRBF));
    }
    at45dbCsDeassert();
}

/* 0x39D0: Buffer1 to Main Memory Page Program (opcode 0x88, no built-in erase) */
void at45dbBufferProgramToPage(uint16_t page_num)
{
    at45dbWaitReady();
    at45dbCsAssert();
    (void)SPI2BUF;
    SPI2BUF = 0x88;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = (page_num >> 8) & 0x03;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_num & 0xFF;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF));
    at45dbCsDeassert();
}

/* 0x3A1C: Main Memory Page Read (opcode 0xD2). */
void at45dbPageRead(uint8_t *buf, uint16_t len,
                    uint16_t page_addr, uint16_t byte_offset)
{
    at45dbWaitReady();
    at45dbCsAssert();
    (void)SPI2BUF;
    SPI2BUF = 0xD2u;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = (page_addr >> 8) & 0x03;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_addr & 0xFF;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = byte_offset & 0xFF;
    while (!(SPI2STATbits.SPIRBF));

    for (uint8_t i = 0; i < 4u; i++) {
        (void)SPI2BUF;
        SPI2BUF = 0x00;
        while (!(SPI2STATbits.SPIRBF));
    }

    for (uint16_t i = 0; i < len; i++) {
        (void)SPI2BUF;
        SPI2BUF = 0x00;
        while (!(SPI2STATbits.SPIRBF));
        buf[i] = (uint8_t)SPI2BUF;
    }
    at45dbCsDeassert();
}

/* JEDEC Device ID Read (opcode 0x9F), len = bytes requested by caller. */
void at45dbReadDeviceId(uint8_t *buf, uint16_t len)
{
    at45dbWaitReady();
    at45dbCsAssert();
    (void)SPI2BUF;
    SPI2BUF = 0x9Fu;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;

    for (uint16_t i = 0; i < len; i++) {
        (void)SPI2BUF;
        SPI2BUF = 0x00;
        while (!(SPI2STATbits.SPIRBF));
        buf[i] = (uint8_t)SPI2BUF;
    }
    at45dbCsDeassert();
}

#endif /* UNIT_TEST_MINIMAL */

/* ---- CRC-16 ---- */

/* 0x5B74: CRC-16 single-byte update (table lookup, table at ROM 0x914A) */
extern const uint16_t crc16_table[256];     /* program address 0x914A */

void crc16Update(uint8_t byte, uint16_t *crc)
{
    uint8_t index = byte ^ (*crc & 0xFF);
    *crc = (*crc >> 8) ^ crc16_table[index];
}

/* 0x5B86: CRC-16 block computation */
void crc16Block(uint8_t *data, uint16_t *crc_ptr, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
        crc16Update(data[i], crc_ptr);
}

/* ---- EEPROM emulation ---- */

void eepromSetCrcLo(uint16_t val)    { eeprom_crc_lo = val; }     /* 0x5ACC -> 0x27F4 */
void eepromSetPageAddr(uint16_t val) { eeprom_page_addr = val; }  /* 0x5AD4 -> 0x27F2 */
uint16_t eepromGetCrcLo(void)        { return eeprom_crc_lo_saved; } /* 0x5AE0 -> 0x27F0 */

/* 0x5A88: Set EEPROM config register fields (0xFF = no change) */
void eepromSetConfig(uint8_t mode, uint8_t status, uint8_t enable)
{
    uint16_t cfg = eeprom_cfg_reg;
    if (mode != 0xFF) {
        cfg &= 0xFFC1;
        cfg |= (mode & 0x03) << 4;
    }
    if (status != 0xFF) {
        cfg &= 0xFF3F;
        cfg |= (status & 0x03) << 6;
    }
    if (enable != 0xFF) {
        cfg &= ~0x0001;
        cfg |= (enable & 0x01);
    }
    eeprom_cfg_reg = cfg;
}

/* ---- Byte lookup tables ---- */
uint8_t flash_lookup_15D0(uint16_t idx) { return flash_read_buf_15D0[idx]; }  /* 0x42B0 */
uint8_t flash_lookup_15B0(uint16_t idx) { return flash_read_buf_15B0[idx]; }  /* 0x42B8 */
uint8_t flash_lookup_1598(uint16_t idx) { return flash_sector_buf_1598[idx]; } /* 0x42C0 */

#ifndef UNIT_TEST_MINIMAL

/* ============================================================================
 * flashPageProgramRead() — 0x41A2
 * Trigger: flashCmdFlags bit1
 * Function: Write 2 bytes to Flash, read back 256 bytes
 *
 * Assembly:
 *   41A2  BTST 0x1928, #1
 *   41A4  BRA Z, 0x41BC
 *   41A6  MOV #0x1923, W2          ; data_hi
 *   41A8  MOV.B [W2], W2
 *   41AC  MOV #0x1922, W3          ; data_lo
 *   41AE  MOV.B [W3], W3
 *   41B2  MOV #0x100, W1           ; len=256
 *   41B4  MOV #0x181E, W0          ; dest
 *   41B6  RCALL 0x3A1C             ; at45dbPageRead
 *   41B8  BSET 0x217, #4           ; I2C2CON.SCLREL=1 (release SCL clock stretch)
 *   41BA  BCLR 0x1928, #1
 *   41BC  RETURN
 * ============================================================================ */
void flashPageProgramRead(void)
{
    if (!(flashCmdFlags & (1u << 1))) return;

    at45dbPageRead(flash_buf_181E, 256,
                   (uint8_t)(i2cRxData >> 8), (uint8_t)i2cRxData);

    I2C2CONbits.SCLREL = 1;
    flashCmdFlags &= ~(1u << 1);
}

/* ============================================================================
 * flashPageReadWrite() — 0x41BE
 * Trigger: flashCmdFlags bit0
 * Function: Read Flash page to RAM, write back (safety interlock: LATF.1)
 * ============================================================================ */
void flashPageReadWrite(void)
{
    if (!(flashCmdFlags & (1u << 0))) return;

    if (!(LATF & (1u << 1))) {              /* Only operate when output inactive */
        uint8_t page = i2cRxPageNum;
        at45dbPageErase(page);
        at45dbBufferWrite(flash_buf_171E, 256);
        at45dbBufferProgramToPage(page);
    }

    I2C2CONbits.SCLREL = 1;
    flashCmdFlags &= ~(1u << 0);
}

/* ============================================================================
 * fwUpdateWriteVerify() — 0x425E -> 0x41E0
 * Firmware update handler (256-byte page write + CRC-16 verification)
 *
 * Write phase: flash_write_offset <= 0x9FFF (40KB)
 *   - Write flash_buf_256 to Flash
 *   - CRC16 accumulate
 *   - offset += 256
 *
 * Verify phase: offset > 0x9FFF
 *   - Compare computed CRC vs stored CRC
 *   - PASS: eeprom status = 3
 *   - FAIL: eeprom status = 2
 * ============================================================================ */
void fwUpdateWriteVerify(void)
{
    if (!(systemFlags & (1u << 8))) return;

    if (flash_write_offset <= 0x9FFF) {
        /* ---- Write phase ---- */
        at45dbPageRead(flash_buf_256, 256,
                       (uint8_t)(flash_write_offset >> 8), 0);

        uint16_t new_offset = flash_write_offset + 256;
        uint16_t crc_len = (new_offset <= 0x9FFF) ? 256 : 252;

        crc16Block(flash_buf_256, &flash_page_addr, crc_len);
        flash_write_offset += 256;
    } else {
        /* ---- Verify phase ---- */
        uint16_t computed_crc = flash_page_addr;

        if (computed_crc == flash_page_count &&
            flash_expected_crc_lo == eepromGetCrcLo()) {
            /* CRC passed */
            eepromSetCrcLo(flash_expected_crc_hi);
            eepromSetPageAddr(flash_page_addr);
            eepromSetConfig(0xFF, 3, 0xFF);   /* status = PASS */
        } else {
            /* CRC failed */
            eepromSetCrcLo(0);
            eepromSetPageAddr(flash_page_addr);
            eepromSetConfig(0xFF, 2, 0xFF);   /* status = FAIL */
        }

        systemFlags &= ~(1u << 8);
    }
}

/* ============================================================================
 * flashReadPage6() — 0x4260
 * Trigger: flashCmdFlags bit4
 * Function: Read Flash page 6, 256 bytes -> flash_read_buf_160E
 * ============================================================================ */
void flashReadPage6(void)
{
    if (!(flashCmdFlags & (1u << 4))) return;

    if (!(LATF & (1u << 1))) {
        at45dbPageErase(6);
        at45dbBufferWrite(flash_read_buf_160E, 256);
        at45dbBufferProgramToPage(6);
    }

    I2C2CONbits.SCLREL = 1;
    flashCmdFlags &= ~(1u << 4);
}

/* ============================================================================
 * flashReadPage7() — 0x427E
 * Trigger: flashCmdFlags bit6
 * Function: Read Flash page 7, 32 bytes -> flash_read_buf_15E6
 * ============================================================================ */
void flashReadPage7(void)
{
    if (!(flashCmdFlags & (1u << 6))) return;

    if (!(LATF & (1u << 1))) {
        at45dbPageErase(7);
        at45dbBufferWrite((uint8_t *)(void *)flash_read_buf_15E6, 32);
        at45dbBufferProgramToPage(7);
    }

    I2C2CONbits.SCLREL = 1;
    flashCmdFlags &= ~(1u << 6);
}

/* ============================================================================
 * flashProgramRead32() — 0x429C
 * Trigger: flashCmdFlags bit5
 * Function: Flash program + read 32 bytes (address from 0x160C:0x160A)
 * ============================================================================ */
void flashProgramRead32(void)
{
    if (!(flashCmdFlags & (1u << 5))) return;

    at45dbPageRead(flash_read_buf_15B0, 32,
                   flash_data_160C, flash_data_160A);

    I2C2CONbits.SCLREL = 1;
    flashCmdFlags &= ~(1u << 5);
}

/* ============================================================================
 * flashPageReadVerify() — 0x42C8
 * Trigger: systemFlags bit6
 * Function: Clear sector buffer, read back Flash pages 1-5, reset CRC/verify state
 * ============================================================================ */
void flashPageReadVerify(void)
{
    if (!(systemFlags & (1u << 6))) return;
    if (LATF & (1u << 1)) return;           /* safety interlock */

    systemFlags &= ~(1u << 6);

    /* Clear sector buffer 1 (256 bytes) */
    for (int i = 0; i < 256; i++)
        flash_sector_buf_1498[i] = 0;

    /* Read back Flash pages 1-5 */
    for (uint8_t page = 1; page < 6; page++) {
        at45dbPageErase(page);
        at45dbBufferWrite(flash_sector_buf_1498, 256);
        at45dbBufferProgramToPage(page);
    }

    /* Clear verify flags */
    flash_verify_flags[0] = 0;
    flash_verify_flags[1] = 0;
    flash_verify_flags[2] = 0;
    flash_crc_accum = 0;

    /* Clear sector buffer 2 (24 bytes) */
    for (int i = 0; i < 24; i++)
        flash_sector_buf_1598[i] = 0;
}

#endif /* UNIT_TEST_MINIMAL */
