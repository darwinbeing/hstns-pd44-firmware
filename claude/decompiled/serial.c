/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — serial.c
 * UART2 debug console — printf() redirected to UART2 115200 8N1
 *
 * External 16 MHz crystal, PLL -> Fcy = 50 MHz
 * BRG = FCY / (16 * 115200) - 1 = 26  (actual baud ≈ 115741, error 0.47%)
 *
 * printf() output chain:
 *   printf() -> C library formatter -> write(1, buf, len) -> U2TXREG
 *
 * XC16 C runtime calls write() for all stdout/stderr output. By defining
 * our own write(), we override the default stub in libc. The linker
 * resolves printf's internal call to our implementation automatically.
 *
 * Stack usage: printf with format specifiers (%d, %x, etc.) requires
 * ~300-500 bytes of stack. Linker option --stack must be large enough
 * to accommodate printf + ISR nesting. Original firmware used ~8KB stack
 * (SP=0x085C, SPLIM=0x27E0). Current build uses --stack=4096.
 * ============================================================================ */
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "variables.h"

#define FCY         50000000UL
#define BAUD        115200UL
#define U2BRG_VAL   ((FCY / (16UL * BAUD)) - 1)   /* 26 */

#define FLASH_FRAME_SYNC0  0xAAu
#define FLASH_FRAME_SYNC1  0x55u
#define FLASH_FRAME_PAGE_INDEX_BYTES 2u
#define FLASH_FRAME_DATA   256u
#define FLASH_ACK_OK       0x06u
#define FLASH_ACK_FAIL     0x15u
#define FLASH_ACK_BUSY     0x42u

extern void at45dbWaitReady(void);
extern void at45dbPageErase(uint16_t page_num);
extern void at45dbBufferWrite(const uint8_t *src, uint16_t len);
extern void at45dbBufferProgramToPage(uint16_t page_num);
void serialPutc(char c);

typedef enum {
    FLASH_RX_SYNC0 = 0,
    FLASH_RX_SYNC1 = 1,
    FLASH_RX_PAGE0 = 2,
    FLASH_RX_PAGE1 = 3,
    FLASH_RX_DATA  = 4,
    FLASH_RX_CRC0  = 5,
    FLASH_RX_CRC1  = 6
} flash_rx_state_t;

static volatile uint16_t flashUartWritePage = 0;
static volatile uint16_t flashUartFrameCount = 0;
static volatile uint16_t flashUartErrCount = 0;
static volatile uint16_t flashUartLastCrcRx = 0;
static volatile uint16_t flashUartLastCrcCalc = 0;

static flash_rx_state_t flashRxState = FLASH_RX_SYNC0;
static uint16_t flashRxIndex = 0;
static uint16_t flashRxCrc = 0;
static uint16_t flashRxPageIndex = 0;
static uint8_t flashRxPayload[FLASH_FRAME_DATA];

/* CRC-16/XMODEM (poly=0x1021, init=0x0000, no-reflect, no-xorout) */
static uint16_t crc16XmodemUpdate(uint16_t crc, uint8_t byte)
{
    crc ^= (uint16_t)byte << 8;
    for (uint8_t j = 0; j < 8; j++) {
        if (crc & 0x8000u) {
            crc = (uint16_t)((crc << 1) ^ 0x1021u);
        } else {
            crc = (uint16_t)(crc << 1);
        }
    }
    return crc;
}

static uint16_t crc16XmodemFrame(uint16_t page_index, const uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0;
    crc = crc16XmodemUpdate(crc, (uint8_t)(page_index & 0xFFu));
    crc = crc16XmodemUpdate(crc, (uint8_t)((page_index >> 8) & 0xFFu));

    for (uint16_t i = 0; i < len; i++) {
        crc = crc16XmodemUpdate(crc, buf[i]);
    }
    return crc;
}

static int at45dbWritePage256Binary(const uint8_t *buf, uint16_t page_idx)
{
    uint16_t page_addr = page_idx & 0x03FFu;
    at45dbPageErase(page_addr);
    at45dbBufferWrite(buf, FLASH_FRAME_DATA);
    at45dbBufferProgramToPage(page_addr);
    at45dbWaitReady();
    return 1;
}

static void flashRxResetToSync0(void)
{
    flashRxState = FLASH_RX_SYNC0;
}

static void flashRxResyncFromByte(uint8_t b)
{
    if (b == FLASH_FRAME_SYNC0) {
        flashRxState = FLASH_RX_SYNC1;
    } else {
        flashRxState = FLASH_RX_SYNC0;
    }
}

static void flashRxFailAndResync(uint8_t b)
{
    flashUartErrCount++;
    serialPutc((char)FLASH_ACK_FAIL);
    flashRxResyncFromByte(b);
}

static int flashRxValidatePageIndex(uint16_t page_index)
{
    if ((page_index & 0xFC00u) != 0u) {
        return 0;
    }
    return 1;
}

static void flashRxStartPayload(void)
{
    flashRxState = FLASH_RX_DATA;
    flashRxIndex = 0;
}

static void flashRxHandleFullFrame(uint8_t last_byte)
{
    uint16_t calc;

    if (!flashRxValidatePageIndex(flashRxPageIndex)) {
        flashRxFailAndResync(last_byte);
        return;
    }

    calc = crc16XmodemFrame(flashRxPageIndex, flashRxPayload, FLASH_FRAME_DATA);
    flashUartLastCrcCalc = calc;

    if (calc != flashRxCrc) {
        flashRxFailAndResync(last_byte);
        return;
    }

    if (LATFbits.LATF1) {
        /* PSU output active: block flash writes for safety. */
        serialPutc((char)FLASH_ACK_BUSY);
        flashRxResetToSync0();
        return;
    }

    if (!at45dbWritePage256Binary(flashRxPayload, flashRxPageIndex)) {
        flashRxFailAndResync(last_byte);
        return;
    }

    flashUartWritePage = flashRxPageIndex;
    flashUartFrameCount++;
    serialPutc((char)FLASH_ACK_OK);
    flashRxResetToSync0();
}

/* --------------------------------------------------------------------------
 * flashUart2LoaderService
 *
 * Frame format from PC:
 *   [0]   0xAA
 *   [1]   0x55
 *   [2]   page index low byte
 *   [3]   page index high byte
 *   [4..259]  256 bytes page payload
 *   [260] CRC16 low byte
 *   [261] CRC16 high byte
 *
 * CRC range:
 *   page index (2 bytes, little-endian) + 256-byte payload
 *
 * UART2 response:
 *   0x06 = success, 0x15 = CRC/program/page-index error, 0x42 = busy
 * -------------------------------------------------------------------------- */
void flashUart2LoaderService(void)
{
    if (U2STAbits.OERR) {
        U2STAbits.OERR = 0;
    }

    while (U2STAbits.URXDA) {
        uint8_t b = (uint8_t)U2RXREG;

        switch (flashRxState) {
        case FLASH_RX_SYNC0:
            if (b == FLASH_FRAME_SYNC0) {
                flashRxState = FLASH_RX_SYNC1;
            }
            break;

        case FLASH_RX_SYNC1:
            if (b == FLASH_FRAME_SYNC1) {
                flashRxState = FLASH_RX_PAGE0;
            } else if (b == FLASH_FRAME_SYNC0) {
                flashRxState = FLASH_RX_SYNC1;
            } else {
                flashRxResetToSync0();
            }
            break;

        case FLASH_RX_PAGE0:
            flashRxPageIndex = (uint16_t)b;
            flashRxState = FLASH_RX_PAGE1;
            break;

        case FLASH_RX_PAGE1:
            flashRxPageIndex |= (uint16_t)b << 8;
            flashRxStartPayload();
            break;

        case FLASH_RX_DATA:
            flashRxPayload[flashRxIndex++] = b;
            if (flashRxIndex >= FLASH_FRAME_DATA) {
                flashRxState = FLASH_RX_CRC0;
            }
            break;

        case FLASH_RX_CRC0:
            flashRxCrc = b;
            flashRxState = FLASH_RX_CRC1;
            break;

        case FLASH_RX_CRC1:
            flashRxCrc |= ((uint16_t)b << 8);
            flashUartLastCrcRx = flashRxCrc;
            flashRxHandleFullFrame(b);
            break;

        default:
            flashRxResyncFromByte(b);
            break;
        }
    }
}

/* --------------------------------------------------------------------------
 * serialInit — configure UART2 for 115200 8N1
 * -------------------------------------------------------------------------- */
void serialInit(void)
{
    U2MODE = 0x0000;            /* reset */
    U2STA  = 0x0000;

    U2BRG  = U2BRG_VAL;        /* 115200 baud */

    U2MODEbits.UARTEN = 1;     /* enable UART2 */
    U2STAbits.UTXEN   = 1;     /* enable transmitter */
}

/* --------------------------------------------------------------------------
 * serialPutc / serialPuts — direct UART2 output (bypass printf)
 * -------------------------------------------------------------------------- */
void serialPutc(char c)
{
    while (U2STAbits.UTXBF);
    U2TXREG = c;
}

void serialPuts(const char *s)
{
    while (*s) {
        while (U2STAbits.UTXBF);
        U2TXREG = *s++;
    }
}

/* --------------------------------------------------------------------------
 * write — XC16 libc stdout/stderr hook
 *
 * This function overrides the default write() weak symbol in the XC16 C
 * library. When printf() formats a string, it calls write(1, buf, len)
 * to emit the result. We redirect all output to UART2.
 *
 * Parameters:
 *   handle — file descriptor (1 = stdout, 2 = stderr)
 *   buf    — pointer to formatted output buffer from printf
 *   len    — number of bytes to write
 *
 * Returns: number of bytes written, or -1 if handle is not stdout/stderr
 * -------------------------------------------------------------------------- */
int write(int handle, void *buf, unsigned int len)
{
    unsigned int i;
    char *p = (char *)buf;

    if (handle != 1 && handle != 2)
        return -1;

    for (i = 0; i < len; i++) {
        while (U2STAbits.UTXBF)     /* wait until TX buffer not full */
            ;
        U2TXREG = *p++;
    }

    return (int)len;
}
