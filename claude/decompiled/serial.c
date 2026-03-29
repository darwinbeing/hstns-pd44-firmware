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

#define FCY         50000000UL
#define BAUD        115200UL
#define U2BRG_VAL   ((FCY / (16UL * BAUD)) - 1)   /* 26 */

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
