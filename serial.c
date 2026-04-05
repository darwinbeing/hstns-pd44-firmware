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
#include <xc.h>

#include "p33Fxxxx.h"
#include "define.h"


void uart1_putc(char c)
{
  while (U1STAbits.UTXBF);   /* wait if TX buffer full */
  U1TXREG = c;
}

void uart1_puts(const char *str)
{
  while (*str)
  {
    uart1_putc(*str++);
  }
}

void uart1_puts_ln(const char *str)
{
  uart1_puts(str);
  uart1_putc('\r');
  uart1_putc('\n');
}

void uart2_putc(char c)
{
  while (U2STAbits.UTXBF);   /* wait if TX buffer full */
  U2TXREG = c;
}

void uart2_puts(const char *str)
{
  while (*str)
  {
    uart2_putc(*str++);
  }
}

void uart2_puts_ln(const char *str)
{
  uart2_puts(str);
  uart2_putc('\r');
  uart2_putc('\n');
}

void _mon_putc(char c)
{
  while (U2STAbits.UTXBF);
  U2TXREG = c;
}

extern uint8_t spi_rx_buf[256];
extern uint32_t spi_flash_id;
extern int16_t vout_cal;
extern s16  ptper_adj;
extern s16  u_exec;
extern s16 comp_2p2z_vref;
extern int16_t Imeas;
extern int16_t vref_ocp_adj;
extern int16_t  vref_ls;
extern uint8_t at45db_device_id[8];
extern uint16_t spi_flash_status;

void uart2_transmit_frame(void)
{

  uint8_t  *src   = (uint8_t *)&spi_flash_status;  /* source data buffer */
  uint16_t  count = 0x20;               /* 32 bytes to transmit */

  /* Send preamble byte 0x55 (frame sync / auto-baud detect) */
  while (U2STAbits.UTXBF);
  U2TXREG = 0x55;

  /* Transmit data buffer byte by byte */
  do
  {
    while (U2STAbits.UTXBF);
    U2TXREG = *src++;
    count--;
  } while (count != 0);

  /* Send end-of-frame byte 0xAA */
  while (U2STAbits.UTXBF);
  U2TXREG = 0xAA;
}

/* --------------------------------------------------------------------------
 * write ? low-level syscall used by printf via stdout
 *
 * XC16 C runtime calls write(1, buf, len) for stdout output.
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


#define FLASH_FRAME_SYNC0  0xAAu
#define FLASH_FRAME_SYNC1  0x55u
#define FLASH_FRAME_PAGE_INDEX_BYTES 2u
#define FLASH_FRAME_DATA   256u
#define FLASH_ACK_OK       0x06u
#define FLASH_ACK_FAIL     0x15u
#define FLASH_ACK_BUSY     0x42u

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

/* --------------------------------------------------------------------------
 * serialPutc / serialPuts ? direct UART2 output (bypass printf)
 * -------------------------------------------------------------------------- */
void serialPutc(char c)
{
  while (U2STAbits.UTXBF);
  U2TXREG = c;
}

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

//  if (LATFbits.LATF1) {
//    /* PSU output active: block flash writes for safety. */
//    serialPutc((char)FLASH_ACK_BUSY);
//    flashRxResetToSync0();
//    return;
//  }

  // spi_at45db_page_program(flashRxPayload, flashRxPageIndex);

  if((flashUartFrameCount % 8) == 0) {
    LED_TOGGLE();
  }

  //
  //    if (!at45dbWritePage256Binary(flashRxPayload, flashRxPageIndex)) {
  //        flashRxFailAndResync(last_byte);
  //        return;
  //    }

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
  __builtin_disable_interrupts();   // mask all interrupts (IPL=7)

  for(;;) {
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

  __builtin_enable_interrupts();    // unmask interrupts (IPL=0)

}
