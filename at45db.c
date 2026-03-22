
#include "xc.h"
#include "p33Fxxxx.h"
#include "define.h"

/* reads can bypass the buffers */
#define OP_READ_CONTINUOUS	0xE8
#define OP_READ_PAGE		0xD2

/* group B requests can run even while status reports "busy" */
#define OP_READ_STATUS		0xD7	/* group B */

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

int spi_at45db_read_page(uint8_t *buf, uint16_t len,
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
        spi_at45db_read_page(buf, 256, p, 0);

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
