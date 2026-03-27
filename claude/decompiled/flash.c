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
 *   at45dbStartPageRead()   — 0x3922 (opcode 0x81)
 *   at45dbReadFromBuffer()  — 0x396E (opcode 0x84)
 *   at45dbWriteBufferToPage() — 0x39D0 (opcode 0x88)
 *   at45dbPageProgramRead() — 0x3A1C (opcode 0xD2)
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
#include "variables.h"

/* ---- SPI Flash low-level functions ---- */

/* 0x38B6: Read AT45DB021E status register (opcode 0xD7) */
void at45dbReadStatus(uint16_t *dest)
{
    LATGbits.LATG9 = 0;                    /* CS low (LATG9, 0x2F4 bit9) */
    (void)SPI2BUF;                          /* flush */
    SPI2BUF = 0xD7;                         /* status read command */
    while (!(SPI2STATbits.SPIRBF));         /* wait TX complete */
    (void)SPI2BUF;
    SPI2BUF = 0x00;                         /* clock out status byte */
    while (!(SPI2STATbits.SPIRBF));
    *dest = SPI2BUF;
    LATGbits.LATG9 = 1;                    /* CS high */
}

static inline int at45dbIsReady(void)
{
    at45dbReadStatus(&at45db_status);
    return (at45db_status & (1u << 7)) != 0;
}

/* 0x3922: Page-to-buffer1 transfer (opcode 0x81) */
void at45dbStartPageRead(uint16_t page_num)
{
    while (!at45dbIsReady());
    LATGbits.LATG9 = 0;
    (void)SPI2BUF;
    SPI2BUF = 0x81;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = (page_num >> 8) & 0x03;      /* page[9:8] */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_num & 0xFF;             /* page[7:0] */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;                         /* byte offset = 0 */
    while (!(SPI2STATbits.SPIRBF));
    LATGbits.LATG9 = 1;
}

/* 0x396E: Read from buffer1 (opcode 0x84) */
void at45dbReadFromBuffer(uint8_t *dest, uint16_t len)
{
    while (!at45dbIsReady());
    LATGbits.LATG9 = 0;
    (void)SPI2BUF;
    SPI2BUF = 0x84;                         /* Buffer 1 Read */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;                         /* 3 dummy addr bytes */
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF));

    for (uint16_t i = 0; i < len; i++) {
        (void)SPI2BUF;
        SPI2BUF = dest[i];                 /* clock in data */
        while (!(SPI2STATbits.SPIRBF));
    }
    LATGbits.LATG9 = 1;
}

/* 0x39D0: Buffer1 write-back to page (opcode 0x88, with built-in erase) */
void at45dbWriteBufferToPage(uint16_t page_num)
{
    while (!at45dbIsReady());
    LATGbits.LATG9 = 0;
    (void)SPI2BUF;
    SPI2BUF = 0x88;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = (page_num >> 8) & 0x03;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_num & 0xFF;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF));
    LATGbits.LATG9 = 1;
}

/* 0x3A1C: Buffer1 page program (opcode 0xD2) + readback */
void at45dbPageProgramRead(uint8_t *buf, uint16_t len,
                               uint16_t page_hi, uint16_t page_lo)
{
    while (!at45dbIsReady());
    LATGbits.LATG9 = 0;
    (void)SPI2BUF;
    SPI2BUF = 0xD2;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_hi & 0x03;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = page_lo & 0xFF;
    while (!(SPI2STATbits.SPIRBF)); (void)SPI2BUF;
    SPI2BUF = 0x00;
    while (!(SPI2STATbits.SPIRBF));

    /* 4 dummy bytes for pipeline delay */
    for (int i = 0; i < 4; i++) {
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
    LATGbits.LATG9 = 1;
}

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
 *   41B6  RCALL 0x3A1C             ; at45dbPageProgramRead
 *   41B8  BSET 0x217, #4           ; U1STA.UTXEN=1
 *   41BA  BCLR 0x1928, #1
 *   41BC  RETURN
 * ============================================================================ */
void flashPageProgramRead(void)
{
    if (!(flashCmdFlags & (1u << 1))) return;

    at45dbPageProgramRead(flash_buf_181E, 256,
                              i2cRxDataHi, i2cRxDataLo);

    U1STAbits.UTXEN = 1;
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
        at45dbStartPageRead(page);
        at45dbReadFromBuffer(flash_buf_171E, 256);
        at45dbWriteBufferToPage(page);
    }

    U1STAbits.UTXEN = 1;
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
    if (!(fwUpdateFlags & (1u << 0))) return;

    if (flash_write_offset <= 0x9FFF) {
        /* ---- Write phase ---- */
        at45dbPageProgramRead(flash_buf_256, 256,
                                  flash_write_page_hi, 0);

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

        fwUpdateFlags &= ~(1u << 0);
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
        at45dbStartPageRead(6);
        at45dbReadFromBuffer(flash_read_buf_160E, 256);
        at45dbWriteBufferToPage(6);
    }

    U1STAbits.UTXEN = 1;
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
        at45dbStartPageRead(7);
        at45dbReadFromBuffer(flash_read_buf_15E6, 32);
        at45dbWriteBufferToPage(7);
    }

    U1STAbits.UTXEN = 1;
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

    at45dbPageProgramRead(flash_read_buf_15B0, 32,
                              flash_data_160C, flash_data_160A);

    U1STAbits.UTXEN = 1;
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
        at45dbStartPageRead(page);
        at45dbReadFromBuffer(flash_sector_buf_1498, 256);
        at45dbWriteBufferToPage(page);
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
