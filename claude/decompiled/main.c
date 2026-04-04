/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — main.c
 * Firmware entry point: 0x5A1C
 * Reconstructed by decompiling assembly
 * ============================================================================ */
#include <xc.h>
#include "variables.h"

/* Configuration Bits */

// Device: dsPIC33FJ64GS606
// Config bits extracted from HSTNS-PD44 hardware via PICkit 5 read

// FBS - Boot Segment
#pragma config BWRP = WRPROTECT_OFF
#pragma config BSS  = NO_FLASH

// FGS - General Code Segment
#pragma config GWRP = OFF
#pragma config GSS  = OFF

// FOSCSEL - Oscillator Source Selection
#pragma config FNOSC = PRIPLL
#pragma config IESO  = ON

// FOSC - Oscillator Configuration
#pragma config POSCMD  = XT
#pragma config OSCIOFNC = OFF
#pragma config FCKSM   = CSECMD

// FWDT - Watchdog Timer
#pragma config WDTPOST = PS2048
#pragma config WDTPRE  = PR32
#pragma config WINDIS  = OFF
#pragma config FWDTEN  = OFF

// FPOR - Power-on Reset
#pragma config FPWRT  = PWR32
#pragma config ALTSS1 = ON
#pragma config ALTQIO = OFF

// FICD - In-Circuit Debugger
#pragma config ICS    = PGD2
#pragma config JTAGEN = OFF

// FCMP - Comparator
#pragma config HYST0   = HYST45
#pragma config CMPPOL0 = POL_FALL
#pragma config HYST1   = HYST45
#pragma config CMPPOL1 = POL_FALL

/* ---- Function declarations ---- */
extern void initClock(void);        /* 0x59CE */
extern void initCMP4(void);         /* 0x5BD8 */
extern void initCMP3(void);         /* 0x5BCC */
extern void initIOPorts(void);      /* 0x59A2 */
extern void initADC(void);          /* 0x5BAE */
extern void initTIMER(void);        /* 0x5AF6 */
extern void initPWM(void);          /* 0x5288 */
extern void initI2C2(void);         /* 0x150C */
extern void initUART1(void);        /* 0x556A */
extern void initSPI2(void);         /* 0x38A6 */
extern void serialInit(void);       /* UART2 debug/loader port */
extern void flashUart2LoaderService(void);   /* UART2 AT45DB frame loader */

extern void i2cService(void);              /* 0x50C2 */
extern void startupControl(void);             /* 0x5522 */
extern void mainStateDispatch(void);       /* 0x51FE */
extern void flashPageProgramRead(void);     /* 0x41A2: Flash page program + read 256B */
extern void flashPageReadWrite(void);       /* 0x41BE: Flash page read + writeback */
extern void fwUpdateWriteVerify(void);      /* 0x425E: FW update 256B write + CRC verify */
extern void flashReadPage6(void);           /* 0x4260: Read Flash page 6 (config) */
extern void flashReadPage7(void);           /* 0x427E: Read Flash page 7 (calibration) */
extern void flashProgramRead32(void);       /* 0x429C: Flash program + read 32B */

/* ============================================================================
 * initVars() — 0x508E
 * RAM variable initialization, product ID "H05WD"
 * ============================================================================ */
void initVars(void)
{
    /* Product model ID "H05WD" (written byte by byte with computed values) */
    model_id[0] = 'H';         /* 0x48 */
    model_id[1] = '0';         /* 0x48 - 0x18 = 0x30 */
    model_id[2] = '5';         /* 0x30 + 0x05 = 0x35 */
    model_id[3] = 'W';         /* 0x57 */
    model_id[4] = 'D';         /* 0x57 - 0x13 = 0x44 */

    /* Voltage output limits */
    trim_avg_adc10 = 193;      /* 0x1E4C = 0xC1 */
    trim_avg_adc11 = 193;      /* 0x1E48 = 0xC1 */

    /* Current output limits */
    ioutLimitHi = 25;        /* 0x1E16 */
    ioutLimitLo = 25;        /* 0x1E12 */
    ioutDefault  = 25;        /* 0x1D18 */

    /* Margin threshold */
    marginThreshold = 10;     /* 0x1E10 */

    /* Frequency setpoint */
    freqSetpoint = 350;       /* 0x1D3E = 0x15E */

    /* Initialize state machine to IDLE */
    systemState = ST_IDLE;              /* 0x1E22 */

    /* statusUpdate/currentLimit hysteresis seed (read as thermalFlags bit15 in asm). */
    thermalFlags = 0x8000;              /* keep high-bit set until explicit updates */

    /* Output Compare 2 */
    OC2RS = 400;               /* 0x190 */
    oc2rs_shadow = 400;        /* 0x1E38 */
}

/* ============================================================================
 * main() — 0x5A1C
 *
 * Assembly:
 *   5A1C  CLRWDT
 *   5A1E  MOV.B #0xE0, W0       ; IPL=7 disable interrupts
 *   5A20  IOR.B SR
 *   5A22  CLR 0x126A
 *   5A24  CLR 0x125A
 *   5A26  RCALL 0x59CE          ; initClock()
 *   5A28  CALL  0x5BD8          ; initCMP4()
 *   5A2C  CALL  0x5BCC          ; initCMP3()
 *   5A30  RCALL 0x59A2          ; initIOPorts()
 *   5A32  CALL  0x5BAE          ; initADC()
 *   5A36  CALL  0x5AF6          ; initTIMER()
 *   5A3A  CALL  0x5288          ; initPWM()
 *   5A3E  CALL  0x150C          ; initI2C2()
 *   5A42  CALL  0x556A          ; initUART1()
 *   5A46  CALL  0x38A6          ; initSPI2()
 *   5A4A  CALL  0x508E          ; initVars()
 *   5A4E  MOV.B SR, WREG        ; IPL=0 enable interrupts
 *   5A50  AND.B W0, #0x1F, W0
 *   5A52  MOV.B WREG, SR
 *   5A54  CLRWDT                ; ── main loop ──
 *   5A56  CALL  0x50C2          ; i2cService()
 *   5A5A  CALL  0x5522          ; startupControl()
 *   5A5E  CALL  0x51FE          ; mainStateDispatch()
 *   5A62  CALL  0x41A2          ; ()
 *   5A66  CALL  0x41BE          ; ()
 *   5A6A  CALL  0x425E          ; ()
 *   5A6E  CALL  0x4260          ; ()
 *   5A72  CALL  0x427E          ; ()
 *   5A76  CALL  0x429C          ; ()
 *   5A78  BRA   0x5A54          ; back to main loop
 * ============================================================================ */
int main(void)
{
    ClrWdt();

    /* Disable all interrupts (IPL=7) */
    SRbits.IPL = 7;

    /* Clear flag words */
    runtimeFlags = 0;            /* CLR 0x126A */
    statusFlags = 0;            /* CLR 0x125A */

    /* ---- Peripheral initialization sequence (while interrupts disabled) ---- */
    /* Firmware init sequence from assembly at 0x5A26..0x5A4A. */
    initClock();               /* 0x59CE: PLL Fosc=100MHz, APLL ~120MHz */
    initCMP4();                /* 0x5BD8: CMPCON4=0x101, CMPDAC4=0 */
    initCMP3();                /* 0x5BCC: CMPCON3=0x81, CMPDAC3=0x1A3 */
    initIOPorts();             /* 0x59A2: GPIO TRIS/LAT/ODC */
    initADC();                 /* 0x5BAE: ADCON=0x1007, trigger source */
    initTIMER();               /* 0x5AF6: T1/T2/T4 */
    initPWM();                 /* 0x5288: PWM1/2/3 full-bridge+SR, PWM5 */
    initI2C2();                /* 0x150C: I2C2 slave address 0x58 */
    initUART1();               /* 0x556A: 9N1 4800baud */
    initSPI2();                /* 0x38A6: Master ~1.56MHz */
    serialInit();              /* UART2 115200 for flash frame loader */
    initVars();                /* 0x508E: RAM variable initialization */

    /* Enable interrupts (IPL=0) */
    SRbits.IPL = 0;

    /* ---- Main loop ---- */
    while (1) {
        ClrWdt();                      /* 0x5A54 */
        dbg_main_loop_calls++;
        dbg_main_stage = 1;
        i2cService();                 /* 0x50C2: check I2C2 status */
        dbg_main_stage = 2;
        startupControl();                /* 0x5522: startup/soft-start control */
        dbg_main_stage = 3;
        mainStateDispatch();          /* 0x51FE: T1-driven main state machine */
        dbg_main_stage = 4;
        //flashPageProgramRead();         /* 0x41A2: Flash page program + read 256B */
        //flashPageReadWrite();           /* 0x41BE: Flash page read + writeback */
        //fwUpdateWriteVerify();          /* 0x425E: FW update 256B write + CRC verify */
        //flashReadPage6();               /* 0x4260: Read Flash page 6 (config) */
        //flashReadPage7();               /* 0x427E: Read Flash page 7 (calibration) */
        //flashProgramRead32();           /* 0x429C: Flash program + read 32B */
        //flashUart2LoaderService();      /* UART2: AA55+PageIndex+256B+CRC -> AT45DB page write */
    }
}
