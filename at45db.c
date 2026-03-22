
#include <stddef.h>    
#include "xc.h"
#include "p33Fxxxx.h"
#include "define.h"

/* reads can bypass the buffers */
#define OP_READ_CONTINUOUS	0xE8
#define OP_READ_PAGE		0xD2

/* group B requests can run even while status reports "busy" */
#define OP_READ_STATUS		0xD7	/* group B */

#define OP_DISABLE_PROTECT  0x3D

/* move data between host and buffer */
#define OP_READ_BUFFER1		0xD4	/* group B */
#define OP_WRITE_BUFFER1	0x84	/* group B */

/* erasing flash */
#define OP_ERASE_PAGE		0x81

/* move data between buffer and flash */
#define OP_TRANSFER_BUF1	0x53
#define OP_TRANSFER_BUF2	0x55
#define OP_MREAD_BUFFER1	0xD4
#define OP_MREAD_BUFFER2	0xD6
#define OP_MWERASE_BUFFER1	0x83
#define OP_MWERASE_BUFFER2	0x86
#define OP_MWRITE_BUFFER1	0x88	/* sector must be pre-erased */
#define OP_MWRITE_BUFFER2	0x89	/* sector must be pre-erased */

/* write to buffer, then write-erase to flash */
#define OP_PROGRAM_VIA_BUF1	0x82
#define OP_PROGRAM_VIA_BUF2	0x85


/* newer chips report JEDEC manufacturer and device IDs; chip
 * serial number and OTP bits; and per-sector writeprotect.
 */
#define OP_READ_ID		0x9F

uint16_t spi_flash_status;  /* bit7 = RDY/BUSY             */
uint8_t at45db_device_id[8];

static inline void spi_cs_assert(void)
{
    LATGbits.LATG9 = 0;    // CS low = active
}

static inline void spi_cs_deassert(void)
{
    LATGbits.LATG9 = 1;    // CS high = inactive
}

// SPI2 interface for AT45DB021E flash
// dsPIC33F SPI: write to SPI2BUF triggers 8 SCK clocks,
// simultaneously transmits and receives one byte.

static uint8_t spi_xfer(uint8_t data)
{
    if (SPI2STATbits.SPIROV)            // RX overflow occurred
        SPI2STATbits.SPIROV = 0;        // clear to resume

    while (SPI2STATbits.SPITBF);        // wait TX buffer empty
    SPI2BUF = data;                     // trigger 8 SCK clocks
    while (!SPI2STATbits.SPIRBF);       // wait RX complete
    return SPI2BUF;                     // read clears SPIRBF
}

static void spi_write(uint8_t data)
{
    (void)spi_xfer(data);               // discard received byte
}

static uint8_t spi_read(void)
{
    return spi_xfer(0xFF);              // send dummy, return received
}

static int spi_write_then_read(const void *txbuf, unsigned n_tx,
                                void *rxbuf, unsigned n_rx)
{
    const uint8_t *tx = (const uint8_t *)txbuf;
    uint8_t *rx = (uint8_t *)rxbuf;

    spi_cs_assert();

    for (unsigned i = 0; i < n_tx; i++)
        spi_write(tx[i]);

    for (unsigned i = 0; i < n_rx; i++)
        rx[i] = spi_read();

    spi_cs_deassert();

    return 0;
}

static void spi_at45db_read_status(uint16_t *status)
{
    uint8_t cmd = OP_READ_STATUS;
    uint8_t val;

    spi_write_then_read(&cmd, 1, &val, 1);

    *status = (uint16_t)val;
}

static void spi_at45db_wait_ready(void)
{
    uint16_t status;
    do {
        spi_at45db_read_status(&status);
    } while (!(status & 0x80));
}

void spi_at45db_read_id(void)
{
	u8 code = OP_READ_ID;

    // 1Fh 23h 00h 01h 00h
    // JEDEC code: 0001 1111 (1Fh for Adesto)
    // Family code: 001 (AT45Dxxx Family)
    // 2-Mbit DataFlash 
	spi_write_then_read(&code, 1, at45db_device_id, 5);
}

int spi_at45db_page_read(uint8_t *buf, uint16_t len,
                                 uint16_t page, uint8_t byte_offset)
{
    spi_at45db_wait_ready();
    
    uint8_t tx[8] = {
        0xD2,                           // opcode
        (page >> 8) & 0x03,             // A17-A16
        page & 0xFF,                    // A15-A8
        byte_offset,                    // A7-A0
        0x00, 0x00, 0x00, 0x00          // 4 dummy bytes
    };

    return spi_write_then_read(tx, 8, buf, len);
}

// CRC-16/XMODEM
static uint16_t crc16(const uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)buf[i] << 8;
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

// read flash page, send raw bytes via UART
void spi_flash_dump_raw(uint16_t start_page, uint16_t num_pages)
{
    uint8_t buf[256];

    for (uint16_t p = start_page; p < start_page + num_pages; p++) {
        spi_at45db_page_read(buf, 256, p, 0);

        // MCU: send sync header before each page
        while (U2STAbits.UTXBF); U2TXREG = 0xAA;  // sync byte 1
        while (U2STAbits.UTXBF); U2TXREG = 0x55;  // sync byte 2
        
        // then 256 data + 2 CRC
        for (uint16_t i = 0; i < 256; i++) {
            while (U2STAbits.UTXBF);
            U2TXREG = buf[i];
        }
        
        // send 2 bytes CRC (big-endian)
        uint16_t crc = crc16(buf, 256);
        while (U2STAbits.UTXBF);
        U2TXREG = (crc >> 8) & 0xFF;
        while (U2STAbits.UTXBF);
        U2TXREG = crc & 0xFF;
        
        LED_TOGGLE();
        ClrWdt();
    }
}

// Configure AT45DB021E page size (one-time nonvolatile setting)
// mode: 0 = binary 256 bytes, 1 = standard 264 bytes

static int spi_at45db_config_pagesize(uint8_t mode)
{
    uint8_t tx[4] = {
        0x3D, 0x2A, 0x80,
        (mode == 0) ? 0xA6 : 0xA7
    };

    spi_write_then_read(tx, 4, NULL, 0);

    spi_at45db_wait_ready();            // wait tEP (~20ms)

    return 0;
}

void spi_at45db_page_erase(uint16_t page)
{
    spi_at45db_wait_ready();

    uint8_t tx[4] = {
        OP_ERASE_PAGE,                       // page erase
        (page >> 8) & 0x03,        // A17-A16
        page & 0xFF,                // A15-A8
        0x00                        // don't care
    };

    spi_write_then_read(tx, 4, (void *)0, 0);
}

//static void spi_at45db_page_write(const uint8_t *buf, uint16_t page)
//{
//    spi_at45db_wait_ready();
//
//    // Step 1: write data into Buffer 1 (0x84 + 3 addr + 256 data)
//    uint8_t hdr1[4] = { 0x84, 0x00, 0x00, 0x00 };
//    spi_cs_assert();
//    for (int i = 0; i < 4; i++)
//        spi_write(hdr1[i]);
//    for (int i = 0; i < 256; i++)
//        spi_write(buf[i]);
//    spi_cs_deassert();
//
//    spi_at45db_wait_ready();
//
//    // Step 2: program Buffer 1 to page (0x88 + 3 addr)
//    uint8_t hdr2[4] = {
//        0x88,
//        (page >> 8) & 0x03,
//        page & 0xFF,
//        0x00
//    };
//    spi_write_then_read(hdr2, 4, (void *)0, 0);
//
//    // internal program ~35ms, call wait_ready before next operation
//}

static int spi_at45db_config_binary_pagesize(void)
{
    uint16_t status;

    // check current page size from status register bit 0
    // bit 0 = 1: already binary (256 bytes)
    // bit 0 = 0: standard (264 bytes), need to configure
    spi_at45db_read_status(&status);

    if (status & 0x01)
        return 0;                           // already 256-byte mode

    // wait ready
    spi_at45db_wait_ready();
    
    // configure binary page size (one-time nonvolatile)
    uint8_t tx[4] = { 0x3D, 0x2A, 0x80, 0xA6 };
    spi_write_then_read(tx, 4, (void *)0, 0);

    spi_at45db_wait_ready();                // wait tEP (~20ms)

    // verify
    spi_at45db_read_status(&status);

    return (status & 0x01) ? 0 : -1;       // 0=success, -1=failed
}

// 0x84 + 0x88: buffer then program, NO erase (caller must erase first)
static void spi_at45db_buf_write(const uint8_t *buf, uint16_t page)
{
    spi_at45db_wait_ready();

    // 0x84: write data into Buffer 1
    spi_cs_assert();
    spi_write(0x84);
    spi_write(0x00);
    spi_write(0x00);
    spi_write(0x00);
    for (uint16_t i = 0; i < 256; i++)
        spi_write(buf[i]);
    spi_cs_deassert();

    // 0x88: Buffer 1 to Page Program (no erase)
    uint8_t cmd[4] = {
        0x88,
        (page >> 8) & 0x03,
        page & 0xFF,
        0x00
    };
    spi_write_then_read(cmd, 4, (void *)0, 0);
}


// 0x84 + 0x83: buffer then program with erase
void spi_at45db_buf_program(const uint8_t *buf, uint16_t page)
{
    spi_at45db_wait_ready();

    spi_cs_assert();

    // 0x84: Buffer 1 Write + data
    spi_write(0x84);
    spi_write(0x00);
    spi_write(0x00);
    spi_write(0x00);

    for (uint16_t i = 0; i < 256; i++)
        spi_write(buf[i]);

    spi_cs_deassert();

    // 0x83: Buffer 1 to Page Program with built-in erase
    uint8_t cmd[4] = {
        0x83,
        (page >> 8) & 0x03,
        page & 0xFF,
        0x00
    };
    spi_write_then_read(cmd, 4, (void *)0, 0);

    // internal erase + program ~20ms
    // call wait_ready before next operation
}

int spi_at45db_page_write_safe(const uint8_t *buf, uint16_t page)
{
    uint8_t backup[256];
    uint8_t verify[256];

    // step 1: backup
    spi_at45db_page_read(backup, 256, page, 0);

    // step 2: erase
    spi_at45db_page_erase(page);

    // step 3: write
    spi_at45db_buf_write(buf, page);

    // step 4: verify
    spi_at45db_page_read(verify, 256, page, 0);

    for (uint16_t i = 0; i < 256; i++) {
        if (verify[i] != buf[i]) {
            // restore backup
            spi_at45db_page_erase(page);
            spi_at45db_buf_write(backup, page);
            return -1;
        }
    }

    return 0;
}


int spi_at45db_init(void)
{     
    // ensure binary page size (256 bytes)
    // return spi_at45db_config_binary_pagesize();
}


// 0x82: auto erase + program, one step
int spi_at45db_page_program(const uint8_t *buf, uint16_t page)
{
    spi_at45db_wait_ready();

    // 0x82: stream data ? buffer ? auto erase + program
    spi_cs_assert();

    spi_write(0x82);                    // opcode
    spi_write((page >> 8) & 0x03);      // A17-A16
    spi_write(page & 0xFF);             // A15-A8
    spi_write(0x00);                    // byte offset = 0

    for (uint16_t i = 0; i < 256; i++)
        spi_write(buf[i]);

    spi_cs_deassert();                  // triggers erase + program (~20ms)
    
    spi_at45db_wait_ready();

    uint16_t status;
    spi_at45db_read_status(&status);
    if (status & 0x20)
        return -1;

    return 0;
}