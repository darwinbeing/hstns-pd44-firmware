/* ============================================================================
 * HSTNS-PD44 dsPIC33FJ64GS606 — I2C2 Communication Protocol
 * Decompiled from firmware assembly
 *
 * Functions covered:
 *       0x13C2 – 0x150A  Initialize I2C2 parameter/data tables
 *             0x150C – 0x1524  Configure I2C2 peripheral (calls above)
 *       0x1526 – 0x15E8  PMBus receive-data dispatcher
 *         0x15EA – 0x1614  Packet checksum helper
 *             0x1616 – 0x164A  Encode one response byte into I2C2TRN
 *        0x164C – 0x1650  Transmit fixed 0x55 byte
 *         0x1652 – 0x16BA  Prepare Tx: set byte count, load pointer
 *          0x16BC – 0x1614  Build multi-byte Tx payload
 *     0x16BC – 0x1848  PMBus command read-handler dispatch table
 *     0x24B2 – 0x2504  T1-ISR: I2C2 timing / watchdog
 *           0x2506 – 0x2558  T1-ISR: I2C2 bus-lock detector / recovery
 *   eepromHandler     0x26D6 – 0x2744  T1-ISR: determine STATUS_WORD / CML byte
 *   si2c2_handler      0x29B0 – 0x29F8  I2C2 slave interrupt service routine
 *   t1IsrI2cBody    0x29FA – 0x2A2E  Body called from T1 ISR for I2C work
 * ============================================================================ */

#include <stdint.h>
#include <xc.h>
#include "variables.h"

/* --------------------------------------------------------------------------
 * Register / SFR addresses used in this module
 * (xc.h provides I2C2CON, I2C2ADD, I2C2TRN, I2C2RCV, I2C2STAT,
 *  IPC12, IFS3, IEC3, TRISF, LATE, LATD, PORTG by name)
 * -------------------------------------------------------------------------- */

/* SFR access for addresses not named by xc.h */
#define SFR16(a)   (*(volatile uint16_t *)(a))

/* I2C2 slave address used by this supply */
#define I2C2_SLAVE_ADDR     0x58        /* 7-bit address 0x58 (0xB0 on wire) */

/* I2C2CON init value: I2C slave, GCEN=0, STREN=0, A10M=0, SCLREL=1 */
#define I2C2CON_INIT        0x9040

/* ============================================================================
 * RAM layout for the I2C parameter / data tables
 *
 * The firmware keeps three parallel arrays all indexed by PMBus command code:
 *
 *   ptrTable19B0[]  (0x19B0..0x19B6)  – read-pointer table, 3 entries (cmd 0..2)
 *   ptrTable19BA[]  (0x19BA..0x19BE)  – pointer table group B (cmd 0..2)
 *   ptrTable19D0[]  (0x19D0..0x19DE)  – pointer table group C, 8 entries
 *   ptrTable19F0[]  (0x19F0..0x19FE)  – pointer table group D, 8 entries
 *   ptrTable1A10[]  (0x1A10..0x1A4A)  – pointer table group E, 30 entries
 *   ptrTable1A70[]  (0x1A70..0x1A78)  – pointer table group F, 5 entries
 *   ptrTable1B90[]  (0x1B90..0x1B9C)  – pointer table group G, 7 entries
 *
 * rxPacketBuf[]     (0x1958..0x197A)  – incoming PMBus packet buffer
 *   rxPacketBuf[0]  = command byte
 *   rxPacketBuf[1]  = byte count (for block writes)
 *   rxPacketBuf[2]  = data[0]
 *   ...
 *   rxPacketBuf[0x21] = address byte (written when rxBufIndex==0x21)
 *
 * rxAddrByte          – I2C address byte (stored separately)
 * rxBufIndex  (0x1E40)    – running index into rxPacketBuf (incremented each byte)
 * rxPendFlag (0x1E3E)    – pending-Rx flag (cleared by rx_handler)
 * rxEventFlags          – receive event flags
 *   bit1 = cmd 0x13 received
 *   bit2 = cmd 0x14 received
 *   bit3 = cmd 0x15 received
 *
 * txCtrlWord (0x1950)    – transmit control word
 *   bit0 = "odd byte" flag (affects  encoding)
 *   bit1 = "multi-byte" flag
 *   bit2 = checksum-included flag
 * txByteCount  (0x192C)    – remaining Tx bytes
 * txChecksumAccum  (0x1956)    – running Tx sequence / checksum accumulator
 * txByteCntPreset  (0x1952)    – Tx byte-count preset
 *
 * i2cPeriodCnt (0x197C)– I2C2 timeout/period counter (init 0x5DC = 1500)
 * i2cTxCounter   (0x19A4) – transaction counter
 * i2cTickCntLo:Hi (0x1936/0x1938) – 32-bit tick counter for slave handler
 * i2cBusLockCnt (0x193A)     – bus-lock error counter (resets I2C2 if > 9999)
 *
 * startupResetLatch2 high byte (0x1BEF) – STATUS_BYTE response value
 * startupResetLatch2 low byte  (0x1BEE) – STATUS_WORD low byte response value
 * pwmRunRequest  (0x1BF0) – I2C2 internal state flags
 *   bit0 = VOUT_OV flag (affects STATUS_BYTE)
 *   bit1 = in write-protect / fault mode
 *   bit6 = fan-fault pending
 *   bit8 = OC flag
 *   bit9 = OT flag
 *   bit12= input UV
 *   bit13= input OV
 * pwmRunning (0x1BF2) – secondary flags
 *   bit1 = I2C active
 * droopEnableFlags  (0x1BEA) – status flags
 *   bit0..3 various condition bits
 * startupResetLatch  (0x1BEC) – mode flags used in status reporting
 * startupResetShadow  (0x193E) – shadow of startupResetLatch for change detection
 * txBusStateFlags  (0x194E) – Tx-ready / bus-state flags
 *   bit7 = I2C2 master ACK expected
 * ioutVoutTarget (0x1BB0) – Iout/Vout read-back target word
 * outputCurrent  (0x1BB2) – Iout/Vout limit word
 * ============================================================================ */

/* ============================================================================
 * Forward declarations
 * ============================================================================ */
static void pmbusInitPointerTable(void);
static void pmbusI2c2Configure(void);
static void pmbusRxPacketDecode(void);
static uint16_t pmbusChecksumVerify(uint8_t *buf, uint16_t len);
static uint16_t pmbusEncodeResponse(uint16_t code);
static void pmbusUnsupportedCommand(void);
static void pmbusMultiByteTxSetup(void);
static void pmbusWriteCommand(uint8_t cmd, uint16_t word_val, uint16_t byte_cnt);
static uint16_t pmbusValidateCommand(uint16_t cmd_idx);
void i2cTxAccumulate(void);
void i2cBusStuckHandler(void);
void eepromHandler(void);
/* si2c2_handler is the ISR — declared below */


/* ============================================================================
 *   (0x13C2 – 0x150A)
 *
 * Initializes the parallel pointer tables that map PMBus command codes to
 * their backing RAM variables, then zeros every variable those tables point
 * to, and finally writes hard-coded default values into several registers.
 *
 * The three "group" tables (19B0, 19BA/19BC/19BE, 19D0..19FA, 1A10..1A4A,
 * 1A70..1A78, 1B90..1B9C) each hold RAM addresses; the init loop clears
 * the variable at each table entry.
 * ============================================================================ */
static void pmbusInitPointerTable(void)
{
    /* --- Group 19B0: 3 command read-pointer entries (indices 0x00..0x02) --- */
    ptrTable19B0[0] = (uint16_t)&llcStatus;           /* was 0x1BF4 */
    ptrTable19B0[1] = (uint16_t)&pwmRunning;           /* was 0x1BF2 */
    ptrTable19B0[2] = (uint16_t)&startupResetLatch2;   /* was 0x1BEE */

    /* --- Group 19BA: 3 entries (indices 0x00..0x02, alternate set) --- */
    ptrTable19BA[0] = (uint16_t)&eventCntBit4;  /* was 0x1946 */
    ptrTable19BA[1] = (uint16_t)&eventCntBit6;  /* was 0x1944 */
    ptrTable19BA[2] = (uint16_t)&eventCntBit8;  /* was 0x1942 */

    /* --- Group 19D0: 8 entries (PMBus commands 0x03 group) --- */
    ptrTable19D0[0] = (uint16_t)&adcLiveA;      /* was 0x1BE8 */
    ptrTable19D0[1] = (uint16_t)&adcLiveA1;     /* was 0x1BE6 */
    ptrTable19D0[2] = (uint16_t)&adcLiveA2;     /* was 0x1BE4 */
    ptrTable19D0[3] = (uint16_t)&i2cTxCounter;  /* was 0x19A4 */
    ptrTable19D0[4] = (uint16_t)&i2cAccumHi;    /* was 0x194C */
    ptrTable19D0[5] = (uint16_t)&i2cAccumLo;    /* was 0x194A */
    ptrTable19D0[6] = (uint16_t)&adcLiveB;      /* was 0x1BD2 */
    ptrTable19D0[7] = (uint16_t)&adcLiveC;      /* was 0x1BC8 */

    /* --- Group 19F0: 8 entries --- */
    ptrTable19F0[0] = (uint16_t)&adcLive1;      /* was 0x1BE2 */
    ptrTable19F0[1] = (uint16_t)&pdcShadowA;    /* was 0x1BDE */
    ptrTable19F0[2] = (uint16_t)&pmbusLiveBC0;  /* was 0x1BC0 */
    ptrTable19F0[3] = (uint16_t)&pdcShadowB;    /* was 0x1BD0 */
    ptrTable19F0[4] = (uint16_t)&pdc3Shadow;    /* was 0x1BCE */
    ptrTable19F0[5] = (uint16_t)&pmbusLive197A; /* was 0x197A */
    /* (0x19FA left as-is in source — not written by param_init) */

    /* --- Group 1A10: 30 entries (PMBus READ_xxx commands) --- */
    ptrTable1A10[0] = (uint16_t)&droopEnableFlags;  /* was 0x1BEA */
    ptrTable1A10[1] = (uint16_t)&outputCurrent;      /* was 0x1BB2 */
    ptrTable1A10[2] = (uint16_t)&ioutScaleConst;    /* was 0x1BC6 */
    ptrTable1A10[3] = (uint16_t)&ocpThresholdHw;    /* was 0x1BB6 */
    ptrTable1A10[4] = (uint16_t)&ioutAdcRaw;        /* was 0x1BB4 */
    ptrTable1A10[5] = (uint16_t)&pmbusReadPtr1;     /* was 0x19A2 */
    ptrTable1A10[6] = (uint16_t)&pmbusReadPtr0;     /* was 0x19A0 */
    ptrTable1A10[7] = (uint16_t)&voutCalE;          /* was 0x199E */
    ptrTable1A10[8] = (uint16_t)&voutCalD;          /* was 0x199C */
    ptrTable1A10[9] = (uint16_t)&tempLinearFmt;     /* was 0x199A */
    ptrTable1A10[10]= (uint16_t)&voutCalC;          /* was 0x1998 */
    ptrTable1A10[11]= (uint16_t)&voutCalB;          /* was 0x1996 */
    ptrTable1A10[12]= (uint16_t)&voutCalA;          /* was 0x1994 */
    ptrTable1A10[13]= (uint16_t)&ioutCalFactor;     /* was 0x1BCA */
    /* entries 14..15 skipped (gap) */
    ptrTable1A10[16]= (uint16_t)&outputVoltage;     /* was 0x1BD4 */
    ptrTable1A10[17]= (uint16_t)&pmbusDataReg0;     /* was 0x19A6 */
    ptrTable1A10[18]= (uint16_t)&adcLive2;          /* was 0x1BDC */
    ptrTable1A10[19]= (uint16_t)&pmbusDataReg4;     /* was 0x19AE */
    ptrTable1A10[20]= (uint16_t)&adcLive3;          /* was 0x1BDA */
    ptrTable1A10[21]= (uint16_t)&pmbusDataReg3;     /* was 0x19AC */
    ptrTable1A10[22]= (uint16_t)&adcLive4;          /* was 0x1BD8 */
    ptrTable1A10[23]= (uint16_t)&pmbusDataReg2;     /* was 0x19AA */
    /* 0x1A40..0x1A48 are ptrTable1A10[24..28] */
    ptrTable1A10[24]= (uint16_t)&adcLive5;          /* was 0x1BD6 */
    ptrTable1A10[25]= (uint16_t)&pmbusDataReg1;     /* was 0x19A8 */
    /* skip [26] */
    ptrTable1A10[27]= (uint16_t)&voutScaleA;        /* was 0x1992 */
    ptrTable1A10[28]= (uint16_t)&voutScaleB;        /* was 0x1990 */

    /* --- Group 1A70: 5 entries --- */
    ptrTable1A70[0] = (uint16_t)&flashUpdateResult; /* was 0x1BBC */
    ptrTable1A70[1] = (uint16_t)&eepromCrcShadow;   /* was 0x1BBA */
    ptrTable1A70[2] = (uint16_t)&eepromPageShadow;  /* was 0x1BB8 */
    ptrTable1A70[3] = (uint16_t)&eepromSavedShadow; /* was 0x198E */
    ptrTable1A70[4] = (uint16_t)&pmbusCfgReg6;      /* was 0x198C */

    /* --- Group 1B90: 7 entries --- */
    ptrTable1B90[0] = (uint16_t)&pmbusCfgReg5;      /* was 0x198A */
    ptrTable1B90[1] = (uint16_t)&pmbusCfgReg4;      /* was 0x1988 */
    ptrTable1B90[2] = (uint16_t)&pmbusCfgReg3;      /* was 0x1986 */
    ptrTable1B90[3] = (uint16_t)&pmbusCfgReg2;      /* was 0x1984 */
    ptrTable1B90[4] = (uint16_t)&pmbusCfgReg1;      /* was 0x1982 */
    /* gap */
    ptrTable1B90[6] = (uint16_t)&pmbusCfgReg0;      /* was 0x197E */

    /* --- Zero every RAM location referenced by the tables ---
     * Tables are compiler-placed and may not be contiguous, so
     * iterate each table explicitly.                            */
    for (int i = 0; i < 3; i++) *((volatile uint16_t *)ptrTable19B0[i]) = 0;
    for (int i = 0; i < 3; i++) *((volatile uint16_t *)ptrTable19BA[i]) = 0;
    for (int i = 0; i < 8; i++) *((volatile uint16_t *)ptrTable19D0[i]) = 0;
    for (int i = 0; i < 8; i++) *((volatile uint16_t *)ptrTable19F0[i]) = 0;
    for (int i = 0; i < 30; i++) { if (ptrTable1A10[i]) *((volatile uint16_t *)ptrTable1A10[i]) = 0; }
    for (int i = 0; i < 5; i++) *((volatile uint16_t *)ptrTable1A70[i]) = 0;
    for (int i = 0; i < 7; i++) { if (ptrTable1B90[i]) *((volatile uint16_t *)ptrTable1B90[i]) = 0; }

    /* --- Hard-coded defaults (0x14B6 onward) --- */
    llcStatus    = 1;              /* 0x1BF4 = 1 : mark init done           */
    i2cPeriodCnt = 0x05DC;         /* 0x197C = 1500 : period preset          */

    /* PMBus read-pointer defaults */
    pmbusReadPtr1 = 0x15C0;
    pmbusReadPtr0 = 0x21C0;

    /* Vout read-back scaling parameters */
    voutCalE = 0x0B00;
    voutCalD = 0x0B00 + 0x200;   /* = 0x0D00 */
    voutCalC = 0x0D00 + 0x0C0;   /* = 0x0DC0 */
    voutCalB = 0x15C0;
    voutCalA = 0x15C0;

    /* Iout calibration defaults */
    ioutCalFactor  = 0x0108;
    ioutScaleConst = 0x3133;
    ocpThresholdHw = 0x0640;
    ioutAdcRaw     = 0x1180;

    /* Linear-format exponent / mantissa preset values */
    voutScaleA  = 0x0135;
    voutScaleB  = 0x0135 + 6;       /* = 0x013B */
    pmbusCfgReg5 = 0x00EF;
    pmbusCfgReg4 = 0x00EF + 0x10;    /* = 0x00FF */
    pmbusCfgReg3 = 0x001F;
    pmbusCfgReg2 = 0x3FFF;
    pmbusCfgReg1 = 0x3FFF;
    pmbusCfgReg0 = 0x001F;

    /* READ_TEMPERATURE_1 preset */
    tempLinearFmt = 0x10AB;

    /* Clear event flags */
    txBusStateFlags = 0;
    rxAuxFlags = 0;
}

/* ============================================================================
 *   (0x150C – 0x1524)
 *
 * Configures the I2C2 peripheral as slave and enables
 * the SI2C2 (slave) interrupt.
 * ============================================================================ */
static void pmbusI2c2Configure(void)
{

    /* Configure I2C2: enable as slave, address = 0x58 */
    I2C2CON = I2C2CON_INIT;
    I2C2ADD  = I2C2_SLAVE_ADDR;

    /* Set SI2C2 interrupt priority to 3 in IPC12 (bits [5:4]):
     *   IPC12 = (IPC12 & 0x8F) | 0x30                                      */
    IPC12 = (IPC12 & 0x8Fu) | 0x30u;

    /* Clear all I2C2 interrupt flags, then enable SI2C2 interrupt (IEC3<1>) */
    IFS3  = 0;
    IEC3 |= (1u << 1);   /* BSET IEC3, #1 */
}

/* ============================================================================
 *   (0x1526 – 0x15E8)
 *
 * Called from the SI2C2 ISR when a data byte has been clocked in.
 * Dispatches the receive byte into rxPacketBuf[] and interprets complete packets.
 *
 * Packet layout (PMBus block write to this device):
 *   rxPacketBuf[0] = command code  (first byte written, when rxBufIndex was 0x21 -> stored
 *                               separately in rxAddrByte)
 *   rxPacketBuf[1] = byte count
 *   rxPacketBuf[2..n] = data bytes
 *
 * When rxBufIndex == 0x21 (first byte = address byte):
 *   store I2C2RCV -> rxAddrByte, do NOT increment rxPacketBuf.
 * Otherwise:
 *   store I2C2RCV -> rxPacketBuf[rxBufIndex - 0x1958], then increment rxBufIndex.
 *
 * After each byte the function checks rxPacketBuf[0] (command code) against
 * known commands and sets bits in rxEventFlags.
 *
 * When rxBufIndex reaches 4 (full 3-data-byte packet):
 *   Command 0x3B (PAGE command): reconstruct 16-bit value from rxPacketBuf[2:3],
 *     clamp/wrap to [0x0800 .. 0xF7FE], store in i2cRxDataLo/Hi (0x1922),
 *     set flashCmdFlags bit1 (cmd_1).
 *   Command 0x20 (VOUT_COMMAND): validate rxPacketBuf[2] (page <= 0x0A),
 *     interpret rxPacketBuf[1] to pick a sub-function (mode 2 or 3) and encode
 *     a page/selector value into flash_data_160A/160C, set flashCmdFlags bit5.
 * ============================================================================ */
static void pmbusRxPacketDecode(void)
{
    uint16_t idx = rxBufIndex;  /* current rx byte index (0x1E40) */

    /* First data byte: treat as address byte, store separately */
    if (idx == 0x21) {
        uint8_t b = (uint8_t)I2C2RCV;
        rxAddrByte = b;
        /* fall through — do NOT increment idx */
    } else {
        /* Normal data byte: store into rxPacketBuf[idx - base] where base = 0x1958 */
        uint8_t b = (uint8_t)I2C2RCV;
        rxPacketBuf[idx - 0x1958u + 0] = b;   /* [W2+W1] where W2=idx, W1=0x1958 */
        idx++;
        rxBufIndex = idx;
    }

    /* Acknowledge receipt: clear the pending flag */
    rxPendFlag = 0;

    /* Check command byte (rxPacketBuf[0]) for known event codes */
    {
        uint8_t cmd = rxPacketBuf[0];

        if (cmd == 0) {
            /* No flags to set for cmd 0 */
        } else if (idx > 1) {
            /* Only evaluate flags when we have at least one data byte */
        }

        /* Check specific command codes and set event flags */
        if (cmd == 0x13) {
            rxEventFlags |= (1u << 1);   /* BSET rxEventFlags, #1 */
        }
        /* Read again (I2C2RCV is a FIFO — already consumed above; re-read
         * is actually a separate check on the already-buffered value)      */
        if (cmd == 0x14) {
            rxEventFlags |= (1u << 2);   /* BSET rxEventFlags, #2 */
        }
        if (cmd == 0x15) {
            rxEventFlags |= (1u << 3);   /* BSET rxEventFlags, #3 */
            goto rx_done;
        }
    }
    goto rx_check_complete;

rx_done:
    /* No further checks */
    goto rx_check_complete;

rx_check_complete:
    /* --- Check whether a complete packet has arrived (idx == 4) --- */
    idx = rxBufIndex;

    if (idx == 4) {
        /* Full 3-byte packet received.  Command is rxPacketBuf[0]. */
        uint8_t cmd = rxPacketBuf[0];

        if (cmd == 0x3B) {
            /* PAGE command: build a 16-bit target voltage / page value.
             * rxPacketBuf[2] = high byte, rxPacketBuf[3] = low byte.   */
            uint16_t val = ((uint16_t)rxPacketBuf[2] << 8)
                         | (uint16_t)(uint8_t)rxPacketBuf[3];

            /* Store in i2cRxDataLo/Hi (0x1922) */
            i2cRxDataLo = (uint8_t)val;
            i2cRxDataHi = (uint8_t)(val >> 8);

            /* Clamp: if val > 0xF7FE -> val = 0xF7FF (saturate high) */
            if (val > 0xF7FEu) {
                val = 0xF7FF;
            } else {
                /* Wrap-around add: val += 0x0800 */
                val += 0x0800u;
            }
            i2cRxDataLo = (uint8_t)val;
            i2cRxDataHi = (uint8_t)(val >> 8);

            /* Signal cmd_1 pending (BSET flashCmdFlags, #1) */
            flashCmdFlags |= (1u << 1);
            return;
        }

        /* idx == 3: partial packet? — fall through to next check */
    }

    if (idx == 3) {
        /* 2-byte data packet received */
        uint8_t cmd  = rxPacketBuf[0];
        uint8_t bcnt = rxPacketBuf[1];   /* byte count / sub-command */

        if (cmd != 0x20) {
            return;   /* only handle VOUT_COMMAND (0x20) */
        }

        /* Validate page index: rxPacketBuf[2] must be <= 0x0A */
        uint8_t page = rxPacketBuf[2];
        if (page > 0x0A) {
            return;
        }

        /* Interpret bcnt to select sub-function (mode 2 = normal, 3 = alt) */
        if (bcnt == 2) {
            /* Mode 2: page 0..7 -> selector = page << 5  (bank A)
             *         page 8..  -> selector = (page-8) << 5 (bank B) + code=2 */
            if (page <= 7) {
                flash_data_160C = 1;
                flash_data_160A = (uint16_t)page << 5;
            } else {
                flash_data_160C = 2;
                flash_data_160A = (uint16_t)(page - 8) << 5;
            }
            flashCmdFlags |= (1u << 5);   /* BSET flashCmdFlags, #5 */
            return;
        }

        if (bcnt == 3) {
            /* Mode 3: same logic with different bank codes (2 or 4) */
            if (page <= 7) {
                flash_data_160C = (uint16_t)2u;
                flash_data_160A = (uint16_t)page << 5;
            } else {
                flash_data_160C = 4;
                flash_data_160A = (uint16_t)(page - 8) << 5;
            }
            flashCmdFlags |= (1u << 5);
            return;
        }
    }

    /* Default: no action */
    return;
}

/* ============================================================================
 *   (0x15EA – 0x1614)
 *
 * Computes a simple 8-bit additive checksum over a byte buffer and compares
 * it against the last byte of the buffer (the expected checksum byte).
 *
 * Entry: W0 = command code (unused by checksum but may be used by caller),
 *        W1 = buffer length (number of data bytes, not including checksum),
 *        W5 = pointer to start of buffer (set by caller at 0x15EB).
 * Exit:  W0 = 1 if checksum MATCHES, 0 if mismatch.
 *        W3 (low byte) = toggled result flag.
 *
 * Algorithm:
 *   sum = 2 * I2C2ADD   (device address counted twice)
 *   for i in 0 .. len-1:
 *       sum += buf[i]
 *   expected = buf[len]   (byte immediately after the payload)
 *   match = (expected == (uint8_t)(-sum))
 *   return match ? 1 : 0  (actually returns ZE(toggle), see assembly)
 * ============================================================================ */
static uint16_t pmbusChecksumVerify(uint8_t *buf, uint16_t len)
{
    /* W5 = buf, W1 = len — mirror assembly register use */
    uint8_t  *ptr  = buf;
    uint16_t  addr = (uint16_t)I2C2ADD;
    uint16_t  sum  = addr + addr;   /* 2 * slave address */
    uint16_t  i;

    for (i = 0; i < len; i++) {
        sum += (uint16_t)(uint8_t)ptr[i];
    }

    /* Compare last byte (ptr[len]) against two's-complement of sum */
    uint8_t expected = ptr[len];       /* byte at offset len = checksum byte */
    uint8_t computed = (uint8_t)(-(uint8_t)sum);

    uint8_t match = 0;
    if (expected == computed) {
        match = 1;
    }

    /* Assembly toggles bit0 of W3 (BTG.B W3, #0) and zero-extends.
     * This effectively XORs the match flag.  Start value of W3 is 0.     */
    match ^= 0;   /* initial W3 = 0, after toggle W3 = match */

    return (uint16_t)match;
}

/* ============================================================================
 *   (0x1616 – 0x164A)
 *
 * Encode one PMBus response code into I2C2TRN (the transmit register).
 * Called 17+ times throughout the Tx handlers.
 *
 * Entry: W0 = command code (one of: 1, 2, 4, 8, 0x10, or other)
 * Exit:  W0 = 0 if a byte was loaded into I2C2TRN; non-zero = "no-op / error"
 *        (return value is zero-extended W0, i.e. the result of CLR.B / ZE)
 *
 * Encoding table:
 *   code == 1    -> I2C2TRN = 0xFE  (READ_BYTE response)
 *   code == 2    -> I2C2TRN = 0xCE
 *   code == 4    -> I2C2TRN = 0xFA  (READ_WORD response start)
 *   code == 8    -> I2C2TRN = 0x55
 *   code == 0x10 -> I2C2TRN = 0xAA
 *   other        -> I2C2TRN = 0x01  (error / unsupported)
 * After writing I2C2TRN the low byte of the return value is cleared -> W0 = 0.
 * ============================================================================ */
static uint16_t pmbusEncodeResponse(uint16_t code)
{
    uint8_t tx_byte;

    if (code == 4) {
        tx_byte = 0xFA;
    } else if (code > 4) {
        if (code == 8) {
            tx_byte = 0x55;
        } else if (code == 0x10) {
            tx_byte = 0xAA;
        } else {
            /* Unsupported code */
            I2C2TRN = 0x01;
            return 0;   /* CLR.B W0 -> ZE -> 0 */
        }
    } else if (code == 1) {
        tx_byte = 0xFE;
    } else if (code == 2) {
        tx_byte = 0xCE;
    } else {
        /* code == 0 or unknown */
        I2C2TRN = 0x01;
        return 0;
    }

    I2C2TRN = tx_byte;
    return 0;   /* CLR.B W0 -> ZE(W0) = 0 */
}

/* ============================================================================
 *   (0x164C – 0x1650)
 *
 * Unconditionally loads 0x55 ("UNSUPPORTED COMMAND" filler) into I2C2TRN.
 * ============================================================================ */
static void pmbusUnsupportedCommand(void)
{
    I2C2TRN = 0x55;
}

/* ============================================================================
 *   (0x1652 – 0x16BA)
 *
 * Prepares the transmit state machine for the next read transaction:
 *   1. Reads rxBufIndex (0x1E40) to determine how many bytes were received.
 *   2. Computes txByteCount = rxBufIndex - 2 (skip address + command bytes).
 *   3. If txByteCount <= 0x0F sets flag bit0 in txCtrlWord (0x1950).
 *   4. If txByteCount > 0x0F sets flag bit1 in txCtrlWord.
 *   5. Calls (txCtrlWord) to write the first response header byte.
 *   6. If  returned non-zero (byte was sent):
 *      a. Shifts rxPendFlag right by W0 positions to get bit offset.
 *      b. Computes index W2 = txByteCount (adjusted); looks up rxPacketBuf[W2] to
 *         find the command code for the pointed-to register.
 *      c. Loads ptrTable19B0[cmd_idx] to get the data pointer.
 *      d. Reads either the low byte or the high byte of *ptr depending on
 *         the "odd byte" flag (txCtrlWord bit0).
 *      e. Writes the data byte to I2C2TRN and reads it back (pipeline flush).
 *      f. Increments txChecksumAccum.
 * ============================================================================ */
static void pmbusMultiByteTxSetup(void)
{
    uint16_t idx = rxBufIndex;   /* 0x1E40 */

    if (idx <= 2) {
        /* Nothing useful received yet */
        if (idx == 0) {
            /* CP0 W0: zero — set bit0 */
            txCtrlWord |= (1u << 0);
        }
        /* else idx in [1,2]: set bit0 */
        txCtrlWord |= (1u << 0);
        goto tx_setup_send;
    }

    /* txByteCount = idx - 2 */
    {
        uint16_t cnt = idx - 2u;
        txByteCount = cnt;

        if (cnt <= 0x0F) {
            /* Short transaction: set bit1 */
            txCtrlWord |= (1u << 1);
        } else {
            /* Long transaction: set bit1 */
            txCtrlWord |= (1u << 1);
        }
    }

tx_setup_send:
    {
        uint16_t ctrl = txCtrlWord;
        uint16_t result = (ctrl);

        if (result == 0) {
            /*  did not put a valid byte in TRN — done */
            return;
        }

        /* result != 0: load the actual data byte based on command index */
        uint16_t rx_pend_val = rxPendFlag;   /* 0x1E3E */
        uint16_t shifted     = rx_pend_val >> result;   /* LSR W1, W0 */
        uint16_t w2          = shifted + 1u;             /* INC W0, W2 */

        /* SL 0x192C (txByteCount shadow), WREG — double txByteCount */
        uint16_t cnt_dbl = txByteCount << 1;

        if (cnt_dbl > rx_pend_val) {
            /* Within range: select byte from ptr table */
            if (rx_pend_val & 1u) {
                /* Odd: check if result == 1 */
                if (result != 1) {
                    return;
                }
                /* Load low byte of *ptr */
                goto load_low_byte;
            } else {
                /* Even: look up command byte at rxPacketBuf[w2] */
                goto load_via_ptr;
            }
        } else {
            /* Out of range: set multi-byte flag and clear event flags */
            txCtrlWord  |= (1u << 1);
            rxEventFlags = 0;
            /* Negate txChecksumAccum (two's complement) and mask to low byte */
            uint8_t neg_seq = (uint8_t)(-(int8_t)(uint8_t)txChecksumAccum);
            I2C2TRN = neg_seq;
            return;
        }

load_via_ptr:
        {
            /* Look up command index from receive buffer, use as pointer index */
            uint16_t cmd_idx = (uint16_t)(uint8_t)rxPacketBuf[w2];
            cmd_idx *= 2u;   /* word index */
            uint16_t data_ptr = ptrTable19B0[cmd_idx / 2u];
            /* Read low byte of the pointed-to register */
            uint8_t  data_lo  = (uint8_t)(*(volatile uint16_t *)data_ptr);
            uint16_t data_val = (uint16_t)data_lo;
            I2C2TRN = data_val;
            /* Pipeline: read I2C2TRN back */
            (void)I2C2TRN;
            txChecksumAccum++;
            return;
        }

load_low_byte:
        {
            uint16_t cmd_idx = (uint16_t)(uint8_t)rxPacketBuf[w2];
            cmd_idx *= 2u;
            uint16_t data_ptr = ptrTable19B0[cmd_idx / 2u];
            /* Read HIGH byte (shift right 8) */
            uint8_t  data_hi  = (uint8_t)((*(volatile uint16_t *)data_ptr) >> 8);
            uint16_t data_val = (uint16_t)data_hi;
            I2C2TRN = data_val;
            (void)I2C2TRN;
            txChecksumAccum++;
            return;
        }
    }
}

/* ============================================================================
 *  post-tx  (0x16B0 – 0x16BA)
 *
 * Called when the Tx pointer is out of range (cnt_dbl > rxPendFlag).
 * Sets multi-byte flag, clears event flags, writes two's-complement of
 * txChecksumAccum into I2C2TRN as a PEC/checksum byte.
 * ============================================================================ */
static void pmbusChecksumResponse(void)
{
    txCtrlWord  |= (1u << 1);   /* BSET 0x1950, #1 */
    rxEventFlags = 0;           /* CLR  0x1940     */

    /* Write negated txChecksumAccum low-byte as PEC byte */
    uint8_t pec = (uint8_t)(-(int8_t)(uint8_t)txChecksumAccum) & 0xFFu;
    I2C2TRN = pec;
}

/* ============================================================================
 *   (0x16BC – 0x1848)
 *
 * Builds a multi-byte Tx payload for a PMBus command.
 * The PMBus command byte is rxPacketBuf[0]; this function decodes it and
 * loads the corresponding RAM variable(s) into I2C2TRN.
 *
 * Entry: W0 = raw command byte (zero-extended from rxPacketBuf[0])
 *        W1 = byte-count / sub-field from rxPacketBuf[1]
 *        W2 = word formed from rxPacketBuf[1:2]  (big-endian)
 *
 * The dispatch at 0x16E0 is "BRA W0" — a computed branch (jump table) with
 * (W0 - 5) as the index, covering command codes 0x05 .. 0x3D+.
 * Commands not in the table branch to 0x1848 (default: no-op / RETURN).
 *
 * Only the commands with explicit branch targets are shown below; the rest
 * fall through to the default RETURN at 0x1848.
 *
 * Abbreviated jump table (assembly 0x16E2..0x1752):
 *   offset  0 (cmd 0x05) -> 0x1754  MOV ptrTable19BA[0] -> clear *ptr
 *   offset  1 (cmd 0x06) -> 0x1758  MOV ptrTable19BA[1] -> clear *ptr
 *   offset  2 (cmd 0x07) -> 0x175C  MOV ptrTable19BA[2] -> clear *ptr
 *   offset  3..10        -> 0x1848  (unsupported)
 *   offset 11 (cmd 0x10) -> 0x1760  clear ptrTable19D0[3..5] (*ptr = 0)
 *   offset 14 (cmd 0x13) -> 0x176C  clear ptrTable19D0[6]
 *   offset 15 (cmd 0x14) -> 0x1770  clear ptrTable19D0[7]
 *   offset 21 (cmd 0x1A) -> 0x1774  clear ptrTable19F0[6]
 *   offset 22 (cmd 0x1B) -> 0x1778  clear ptrTable19F0[7]
 *   offset 29 (cmd 0x22) -> 0x177E  -> BSET auxFlags #3, complex update
 *   offset 44 (cmd 0x31) -> 0x17E6  -> BTST i2cStatusByte #2 path
 *   offset 55 (cmd 0x3C) -> 0x1814..0x183E  -> write ptrTable1A10[n] from W2
 *   offset 56 (cmd 0x3D) -> 0x1818
 *   ... (many more ptrTable1A10 table writes)
 *   offset 67 (cmd 0x48) -> 0x183E (last write entry)
 *   offset 68 (cmd 0x49) -> 0x1844 ptrTable1A10[13]
 *   default -> 0x1848 RETURN
 *
 * Rather than reproducing the full 56-entry jump table as C switch labels,
 * the key cases are enumerated; the rest are implicit (return).
 * ============================================================================ */
static void pmbusWriteCommand(uint8_t cmd, uint16_t word_val, uint16_t byte_cnt)
{
    /* Adjust cmd: subtract 5 to get table index */
    if (cmd < 5) {
        return;   /* BRA GTU 0x1848: out of range */
    }
    uint8_t idx = cmd - 5u;

    /* Bounds check: if idx > 0x38 (cmd > 0x3D), branch to 0x1848 */
    if (idx > 0x38u) {
        return;
    }

    switch (idx) {
        /* cmd 0x05 (idx 0): clear the RAM location pointed by ptrTable19BA[0] */
        case 0:
            *((volatile uint16_t *)ptrTable19BA[0]) = 0;
            return;
        /* cmd 0x06 (idx 1) */
        case 1:
            *((volatile uint16_t *)ptrTable19BA[1]) = 0;
            return;
        /* cmd 0x07 (idx 2) */
        case 2:
            *((volatile uint16_t *)ptrTable19BA[2]) = 0;
            return;
        /* cmd 0x10 (idx 11): clear ptrTable19D0[3], [4], [5] */
        case 11:
            *((volatile uint16_t *)ptrTable19D0[3]) = 0;
            *((volatile uint16_t *)ptrTable19D0[4]) = 0;
            *((volatile uint16_t *)ptrTable19D0[5]) = 0;
            return;
        /* cmd 0x13 (idx 14): clear ptrTable19D0[6] */
        case 14:
            *((volatile uint16_t *)ptrTable19D0[6]) = 0;
            return;
        /* cmd 0x14 (idx 15): clear ptrTable19D0[7] */
        case 15:
            *((volatile uint16_t *)ptrTable19D0[7]) = 0;
            return;
        /* cmd 0x1A (idx 21): clear ptrTable19F0[6] */
        case 21:
            *((volatile uint16_t *)ptrTable19F0[6]) = 0;
            return;
        /* cmd 0x1B (idx 22): clear ptrTable19F0[7] */
        case 22:
            *((volatile uint16_t *)ptrTable19F0[7]) = 0;
            return;

        /* ------------------------------------------------------------------ *
         * cmd 0x22 (idx 29): set auxFlags bit3; update droopEnableFlags bit2 based
         * on status; route the new value to ptrTable1A10[0].
         * ------------------------------------------------------------------ */
        case 29: {
            auxFlags |= (1u << 3);   /* BSET 0x1E1C, #3 */

            /* Evaluate whether to clear droopEnableFlags bit2 */
            if (!(word_val & 4u)) {
                /* bit2 of W2 (word_val) is clear */
                if (startupResetLatch == 0) {
                    droopEnableFlags &= ~(1u << 2);   /* BCLR 0x1BEA, #2 */
                }
                if (!(droopEnableFlags & (1u << 1))) {
                    droopEnableFlags &= ~(1u << 2);
                }
                if (systemState == 2) {
                    goto cmd22_set;
                }
                uint16_t bec = startupResetLatch;
                if (bec & 0x0Cu) {
                    return;   /* bits 2 or 3 set -> abort */
                }
            }
cmd22_set:
            /* Call helper at 0x257E (update flags) */
            /* RCALL 0x257E — modelled inline: */
            {
                /* Set droopEnableFlags bit2, clear statusFlags2 bit2 */
                droopEnableFlags   |= (1u << 2);
                statusFlags2   &= ~(1u << 2);
                uint16_t droop = droopMode;   /* 0x1268 */
                if (droop == 3) {
                    statusFlags2 |= (1u << 1);
                    return;
                }
                if (droop == 4) {
                    statusFlags2 &= ~(1u << 1);
                }
            }
            return;
        }

        /* ------------------------------------------------------------------ *
         * cmd 0x31 (idx 44): write word_val to ptrTable1A10[1] / 1A12 path,
         * with firmware-update flag and limit clamp logic.
         * ------------------------------------------------------------------ */
        case 44: {
            if (!(i2cStatusByte & (1u << 2))) {
                /* Normal path: write word_val to ptrTable1A10[1] */
                *((volatile uint16_t *)ptrTable1A10[1]) = word_val;
                if (word_val <= 0x000Fu) {
                    internalStatusFlags |= (1u << 5);
                } else {
                    internalStatusFlags &= ~(1u << 5);
                }
                currentLimitFlags |= (1u << 0);
            } else {
                /* Firmware-update path (bit2 of i2cStatusByte set) */
                *((volatile uint16_t *)ptrTable1A10[1]) = word_val;
            }
            /* Common: set droopEnableFlags bit3 */
            droopEnableFlags |= (1u << 3);
            return;
        }

        /* ------------------------------------------------------------------ *
         * cmd 0x3C (idx 55) and the following 10 commands each write
         * word_val into successive ptrTable1A10 entries [2]..[12].
         * ------------------------------------------------------------------ */
        case 55: *((volatile uint16_t *)ptrTable1A10[2])  = word_val; return;
        case 56: *((volatile uint16_t *)ptrTable1A10[3])  = word_val; return;
        case 57: *((volatile uint16_t *)ptrTable1A10[4])  = word_val; return;
        case 58: *((volatile uint16_t *)ptrTable1A10[5])  = word_val; return;
        case 59: *((volatile uint16_t *)ptrTable1A10[6])  = word_val; return;
        case 60: *((volatile uint16_t *)ptrTable1A10[7])  = word_val; return;
        case 61: *((volatile uint16_t *)ptrTable1A10[8])  = word_val; return;
        case 62: *((volatile uint16_t *)ptrTable1A10[9])  = word_val; return;
        case 63: *((volatile uint16_t *)ptrTable1A10[10]) = word_val; return;
        /* cmd 0x46 (idx 65): also set fwUpdateFlags bit6 */
        case 65:
            fwUpdateFlags |= (1u << 6);
            *((volatile uint16_t *)ptrTable1A10[11]) = word_val;
            return;
        case 66: *((volatile uint16_t *)ptrTable1A10[12]) = word_val; return;
        /* cmd 0x48 (idx 67) */
        case 67: *((volatile uint16_t *)ptrTable1A10[13]) = word_val; return;
        /* cmd 0x49 (idx 68): indirect write via ptrTable1A10[13] */
        case 68: *((volatile uint16_t *)ptrTable1A10[13]) = word_val; return;

        default:
            return;   /* unsupported command */
    }
}

/* ============================================================================
 *   (0x16BC – 0x1848) — [also serves as  body]
 *
 * This is the full PMBus command read-handler dispatch table.  It decodes
 * rxPacketBuf[0] (command code), rxPacketBuf[1] (byte count / sub-field), and the
 * combined word rxPacketBuf[1:2] into a response value that is written to I2C2TRN.
 *
 * The function is also reached from si2c2_handler (0x28D6) when the slave
 * state machine detects a read transaction.
 *
 * Assembly: starts at 0x16BC.  Entry registers:
 *   W0 = cmd_idx  (zero-extended rxPacketBuf[0])
 *   W1 = byte2    (zero-extended rxPacketBuf[1])
 *   W2 = word     (rxPacketBuf[1] << 8 | rxPacketBuf[2], word form)
 *   W3 = base pointer = 0x1958 (start of rxPacketBuf)
 *
 * The dispatch at 0x16DA subtracts 5 from W0 and uses BRA W0 for a
 * 56-entry computed branch; each entry either lands on a handler or
 * falls through to 0x1848 (RETURN).
 *
 * Key handlers:
 *   cmd 0x05 -> Read ptrTable19BA[0] and return *ptr
 *   cmd 0x06 -> Read ptrTable19BA[1]
 *   cmd 0x07 -> Read ptrTable19BA[2]
 *   cmd 0x10 -> Clear three ptrTable19D0 entries (accumulators)
 *   cmd 0x13..0x15 -> Clear ptrTable19D0[6..7] / ptrTable19F0[6..7]
 *   cmd 0x22 -> Status/fault complex handler (see above)
 *   cmd 0x31 -> Write iout/vout value
 *   cmd 0x3C..0x49 -> Write successive ptrTable1A10 entries
 *
 * This function is modelled by () above.
 * ============================================================================ */

/* ============================================================================
 * PMBus command validity checker  (0x194E – 0x1980)
 *
 * Checks whether rxPacketBuf[W0] (the command byte at index W0 in rxPacketBuf) is a
 * valid PMBus command code that this device supports.
 *
 * Returns 0 (RETLW #0) if supported, 1 (RETLW #1) if unsupported.
 *
 * Valid command ranges:
 *   0x01..0x07 (but NOT 0x06, 0x07 alone — see assembly)
 *   0x05..0x07
 *   0x13, 0x16, 0x17
 *   0x23..0x24
 * ============================================================================ */
static uint16_t pmbusValidateCommand(uint16_t cmd_idx)
{
    /* Zero-extend: already done by caller */
    uint8_t cmd = (uint8_t)rxPacketBuf[cmd_idx];

    /* Ranges that are valid (assemble from the BRA chain in 0x194E..0x1980) */
    if (cmd <= 0x17u) {
        if (cmd <= 0x07u) {
            /* 0x01..0x07 valid, 0x00 invalid */
            /* Assembly accepts 0x05, 0x06, 0x07, 0x13, 0x16, 0x17 */
            if (cmd >= 0x05u && cmd <= 0x07u) goto valid;
            if (cmd == 0x13u) goto valid;
            goto invalid;
        }
        if (cmd == 0x13u) goto valid;
        if (cmd == 0x16u) goto valid;
        if (cmd == 0x17u) goto valid;
    } else {
        if (cmd >= 0x23u && cmd <= 0x24u) goto valid;
        /* Additional range derived from assembly 0x1976..0x197C */
        {
            /* cmd += 0xD0; if (cmd - 0xD <=0) valid */
            uint8_t t = cmd + 0xD0u;
            if ((t - 0x0Du) <= 0) goto valid;
        }
    }

invalid:
    return 1;
valid:
    return 0;
}

/* ============================================================================
 *   (0x24B2 – 0x2504)
 *
 * Called from the T1 timer ISR to maintain a 2500-tick (2.5 s) keep-alive
 * counter for the I2C2 slave.  Also accumulates the i2cTxCounter read counter
 * and runs the 32-bit receive-data accumulator at i2cAccumLo/i2cAccumHi.
 *
 * Flow:
 *   1. If flashCmdFlags bit0 set -> skip (I2C busy with UART command).
 *   2. Check pwmRunning bit1 (I2C2 active):
 *      - If clear -> reset counters and load current i2cTxCounter into
 *                   i2cAccumLo/Hi, then return.
 *      - If set   -> increment 32-bit counter i2cTickCntLo:i2cTickCntHi.
 *   3. If counter reaches 0x09C3 (2499 ticks):
 *      a. Check rxEventFlags: if non-zero -> skip (transaction in progress).
 *      b. Reset counter to 0x09C3.
 *      c. Increment i2cTxCounter.  If it wraps to 0xFFFF -> skip.
 *      d. Read adcLiveA2 (ptrTable19D0[0] target) and add it into i2cAccumLo:Hi.
 * ============================================================================ */
void i2cTxAccumulate(void)
{
    /* Skip if UART is holding I2C bus (flashCmdFlags bit0) */
    if (flashCmdFlags & (1u << 0)) {
        return;
    }

    uint16_t f2 = pwmRunning;

    /* Check bit1: I2C2 active */
    if (!(f2 & (1u << 1))) {
        /* Not active: clear accumulators */
        i2cTickCntLo = 0;
        i2cTickCntHi = 0;
        i2cAccumLo = 0;
        i2cAccumHi = 0;
        /* Store current i2cTxCounter value (no-op: same variable) */
        return;
    }

    /* I2C2 active: increment 32-bit tick counter */
    uint32_t cnt = ((uint32_t)i2cTickCntHi << 16) | i2cTickCntLo;
    cnt++;
    i2cTickCntLo = (uint16_t)(cnt & 0xFFFFu);
    i2cTickCntHi = (uint16_t)(cnt >> 16);

    /* Check if counter has reached 0x09C3 (2499) */
    if (i2cTickCntLo > 0x09C3u || i2cTickCntHi != 0) {
        return;   /* BRA LEU 0x2504: still under threshold */
    }

    /* Counter reached threshold */
    if (rxEventFlags != 0) {
        return;   /* Transaction in progress — skip */
    }

    /* Reset counter to 0x09C3 */
    i2cTickCntLo = 0x09C3u;
    i2cTickCntHi = 0;

    /* Increment i2cTxCounter; bail if it would wrap to 0 (== 0xFFFF before inc) */
    uint16_t s = i2cTxCounter;
    if ((uint16_t)(s + 1u) == 0) {
        return;   /* would overflow */
    }
    s++;
    i2cTxCounter = s;

    /* Add adcLiveA2 (was ptrTable19D0[0] target = 0x1BE4) into the 32-bit accumulator */
    uint16_t delta = adcLiveA2;
    uint32_t acc   = ((uint32_t)i2cAccumHi << 16) | i2cAccumLo;
    acc += (uint32_t)delta;
    i2cAccumLo = (uint16_t)(acc & 0xFFFFu);
    i2cAccumHi = (uint16_t)(acc >> 16);
}

/* ============================================================================
 *   (0x2506 – 0x2558)
 *
 * Called from the T1 ISR.  Monitors the I2C2 bus for a stuck-SCL / stuck-SDA
 * condition and resets the peripheral if it is stuck for more than 9999 ticks.
 *
 * Algorithm:
 *   1. Read I2C2STAT:
 *      - If bit3 (GCSTAT) set -> set txBusStateFlags bit7 (master ACK expected).
 *      - Else if bit4 (ADD10) set and bit2 (R_W) set -> clear txBusStateFlags bit7.
 *   2. Read PORTG:
 *      - If bit2 (SDA) and bit3 (SCL) are BOTH high -> bus is free.
 *        - If txBusStateFlags bit7 clear -> increment i2cBusLockCnt.
 *        - Else clear i2cBusLockCnt.
 *      - If i2cBusLockCnt > 9999 (0x270F):
 *        a. Reset i2cBusLockCnt to 0, clear txBusStateFlags bit7.
 *        b. Disable SI2C2 interrupt (IEC3 bit1 = 0).
 *        c. Reset I2C2CON (CLR then re-init to 0x9040).
 *        d. Flush I2C2RCV.
 *        e. Set SDA and SCL as inputs (BSET TRISF #4, #5).
 *        f. Clear rxBufIndex and rxPendFlag.
 *        g. Re-enable SI2C2 interrupt.
 * ============================================================================ */
void i2cBusStuckHandler(void)
{
    /* --- Step 1: check I2C2STAT for address/ack state --- */
    {
        uint8_t stat = (uint8_t)I2C2STAT;
        if (stat & (1u << 3)) {
            /* GCSTAT set: general call or address match — master ACK needed */
            txBusStateFlags |= (1u << 7);
        } else if ((stat & (1u << 4)) && (stat & (1u << 2))) {
            /* ADD10 + R/W: 10-bit address read — clear ACK flag */
            txBusStateFlags &= ~(1u << 7);
        }
    }

    /* --- Step 2: check bus line state via PORTG --- */
    {
        uint8_t pg = (uint8_t)PORTG;

        if ((pg & (1u << 2)) && (pg & (1u << 3))) {
            /* Both SDA (bit2) and SCL (bit3) are high -> bus free */
            uint16_t f = txBusStateFlags;
            if (f & (1u << 7)) {
                /* Master ACK expected: ACK not yet observed — hold counter */
                i2cBusLockCnt = 0;   /* clear: not locked */
            } else {
                /* No pending ACK: count free-but-idle time */
                i2cBusLockCnt = 0;
            }
        } else {
            /* Bus held low by someone */
            i2cBusLockCnt++;
        }
    }

    /* --- Step 3: check i2cBusLockCnt threshold (0x270F = 9999) --- */
    {
        uint16_t lkc = i2cBusLockCnt;
        if (lkc <= 0x270Fu) {
            return;   /* still within tolerance */
        }

        /* Stuck bus detected: perform I2C2 reset */
        i2cBusLockCnt     = 0;
        txBusStateFlags &= ~(1u << 7);

        IEC3 &= ~(1u << 1);   /* BCLR IEC3, #1 — disable SI2C2 ISR */
        I2C2CON = 0;           /* CLR I2C2CON */
        I2C2CON = I2C2CON_INIT;

        (void)I2C2RCV;         /* Flush receive buffer */

        TRISF |= (1u << 4);    /* SDA = input */
        TRISF |= (1u << 5);    /* SCL = input */

        rxBufIndex  = 0;
        rxPendFlag = 0;

        IFS3  &= ~(1u << 1);   /* CLR IFS3 (all I2C2 flags) */
        IEC3  |=  (1u << 1);   /* Re-enable SI2C2 interrupt */
    }
}

/* ============================================================================
 * eepromHandler  (0x26D6 – 0x2744)
 *
 * Called from the T1 ISR.  Updates the STATUS_BYTE (high byte of startupResetLatch2)
 * and STATUS_WORD low byte (low byte of startupResetLatch2) based on the
 * current fault/status flags.
 *
 * STATUS_BYTE (high byte of startupResetLatch2) logic:
 *   Default = 0x80 (BUSY / not-OK).
 *   If pwmRunRequest bit0 == 0 (no VOUT_OV) AND
 *      (systemFlags & 0xA0) == 0 AND
 *      (controlStatus & 0x5C0) == 0 -> STATUS_BYTE = 0x00 (OK).
 *
 * STATUS_WORD low byte (low byte of startupResetLatch2) priority encoder:
 *   0x01 : default (pwmRunRequest bit1 set = write-protected / fault mode)
 *   0x02 : flags_1E20 bit7 set (fault present)
 *   0x04 : pwmRunRequest bit8 set (OC)
 *   0x05 : pwmRunRequest bit9 set (OT)
 *   0x07 : default when none of the above
 *   0x08 : pwmRunRequest bit12 set (input UV)
 *   0x0E : pwmRunRequest bit6 set (fan fault)
 *   0x0F : controlStatus bit10 set (bit10 of 0x1E20 via 0x540 mask)
 *   0x0B : pwmRunRequest bit4 set (droop mode = 0x10 bit)
 *   0x14 : systemState == ST_HOLDOFF (special state)
 * ============================================================================ */
void eepromHandler(void)
{
    uint16_t f0 = pwmRunRequest;   /* 0x1BF0 */

    /* --- STATUS_BYTE (high byte of startupResetLatch2) --- */
    uint8_t sbyte = 0x80u;   /* default: not-OK */

    if (!(f0 & (1u << 0))) {
        /* bit0 clear: no VOUT_OV */
        uint16_t f1a = systemFlags;
        if (!(f1a & 0xA0u)) {
            /* bits 5 and 7 of systemFlags both clear */
            uint16_t cf = controlStatus;   /* 0x1E20 */
            if (!(cf & 0x05C0u)) {
                /* bits 6,8,10 of controlStatus all clear */
                sbyte = 0x00u;   /* OK */
            }
        }
    }
    startupResetLatch2 = (startupResetLatch2 & 0x00FF) | ((uint16_t)(sbyte) << 8);

    /* --- STATUS_WORD low byte (low byte of startupResetLatch2) --- */
    uint8_t sword = 0x01u;   /* default if bit1 set */

    if (!(f0 & (1u << 1))) {
        /* bit1 clear: not in write-protect / fault mode */
        uint16_t cf = controlStatus;   /* 0x1E20 */
        sword = 0x02u;

        if (cf & (1u << 7)) {
            /* fault_present */
            goto set_sword;
        }

        if (f0 & (1u << 8)) {
            sword = 0x04u;
            goto set_sword;
        }
        if (f0 & (1u << 9)) {
            sword = 0x05u;
            goto set_sword;
        }

        sword = 0x07u;

        if (f0 & (1u << 12)) {
            sword = 0x08u;
            goto set_sword;
        }
        if (f0 & (1u << 13)) {
            /* input OV: also code 0x08 — fall to next check */
        }

        sword = 0x0Eu;
        if (f0 & (1u << 6)) {
            goto set_sword;   /* fan fault */
        }

        /* Check systemState == ST_HOLDOFF */
        if (systemState == ST_HOLDOFF) {
            sword = 0x14u;
            goto set_sword;
        }

        /* Check controlStatus bits 10,8 (mask 0x540) */
        if (cf & 0x0540u) {
            sword = 0x0Fu;
            goto set_sword;
        }

        /* Check pwmRunRequest bit4 (droop mode 0x10) */
        if (f0 & (1u << 4)) {
            sword = 0x0Bu;
        }
    }

set_sword:
    startupResetLatch2 = (startupResetLatch2 & 0xFF00) | (sword & 0xFF);
}

/* ============================================================================
 * startupResetLatch change-detector  (0x2746 – 0x27CC)
 *
 * Compares the current value of startupResetLatch (0x1BEC) against its shadow
 * startupResetShadow (0x193E).  For each of five bit-groups that changed, it
 * increments a corresponding byte counter in RAM.
 * Finally saves the new value into the shadow.
 *
 * Bit groups monitored:
 *   bits [4]  (0x10)  -> byte counter eventCntBit4
 *   bits [5]  (0x20)  -> byte counter eventCntBit5
 *   bits [6]  (0x40)  -> byte counter eventCntBit6
 *   bits [7]  (0x80)  -> byte counter eventCntBit7
 *   bits [8]  (0x100) -> byte counter eventCntBit8
 * ============================================================================ */
static void startupResetEventTracking(void)
{
    uint16_t cur    = startupResetLatch;   /* 0x1BEC */
    uint16_t shadow = startupResetShadow;   /* 0x193E */

    if (cur == shadow) {
        return;   /* no change */
    }

    /* Bit 4 changed? */
    {
        uint16_t cur_b  = cur    & 0x10u;
        uint16_t prev_b = shadow & 0x10u;
        if (cur_b != prev_b && prev_b != 0) {
            uint8_t c = eventCntBit4;
            if ((uint8_t)(c + 1u) != 0) {
                c++;
                eventCntBit4 = c;
            }
        }
    }

    /* Bit 5 changed? */
    {
        uint16_t cur_b  = cur    & 0x20u;
        uint16_t prev_b = shadow & 0x20u;
        if (cur_b != prev_b && prev_b != 0) {
            uint8_t c = eventCntBit5;
            if ((uint8_t)(c + 1u) != 0) {
                c++;
                eventCntBit5 = c;
            }
        }
    }

    /* Bit 6 changed? */
    {
        uint16_t cur_b  = cur    & 0x40u;
        uint16_t prev_b = shadow & 0x40u;
        if (cur_b != prev_b && prev_b != 0) {
            uint8_t c = eventCntBit6;
            if ((uint8_t)(c + 1u) != 0) {
                c++;
                eventCntBit6 = c;
            }
        }
    }

    /* Bit 7 changed? */
    {
        uint16_t cur_b  = cur    & 0x80u;
        uint16_t prev_b = shadow & 0x80u;
        if (cur_b != prev_b && prev_b != 0) {
            uint8_t c = eventCntBit7;
            if ((uint8_t)(c + 1u) != 0) {
                c++;
                eventCntBit7 = c;
            }
        }
    }

    /* Bit 8 changed? */
    {
        uint16_t cur_b  = cur    & 0x100u;
        uint16_t prev_b = shadow & 0x100u;
        if (cur_b != prev_b && prev_b != 0) {
            uint8_t c = eventCntBit8;
            if ((uint8_t)(c + 1u) != 0) {
                c++;
                eventCntBit8 = c;
            }
        }
    }

    /* Save new shadow */
    startupResetShadow = cur;
}

/* ============================================================================
 *   (0x27CE – 0x2898)
 *
 * Called from si2c2_handler when a read (Tx) transaction is in progress.
 * Decodes the command byte (rxPacketBuf[0]) and calls the appropriate per-command
 * Tx sub-function.  Handles both single-byte and block read formats.
 *
 * Key sub-functions called:
 *   0x1652         (cmd 0x01 handler)
 *   0x27CE  this function       (cmd 0x11 handler, recursive-style)
 *   0x1F54  (cmd 0x11 sub)
 *   0x1F90  (cmd 0x12 sub)
 *   0x289A  (cmd 0x21, recursive tx setup)
 *   0x1F18  (cmd 0x21 sub)
 *   0x1982  (cmd 0x22 sub)
 *   0x1A12  (cmd 0x23 sub)
 *   0x1D12  (cmd 0x24 sub)
 *   0x1B6A  (cmd 0x3B sub)
 *   0x1C48  (cmd 0x3C sub)
 *   0x1B2A  (cmd 0x3D sub)
 *   0x1C94  (cmd 0x3D-alt sub)
 *   0x1CD4  (cmd 0xE0 sub)
 *   0x1D5E  (cmd 0xE3 sub)
 *   0x1DC8  (cmd 0xE4 sub)
 *   0x1A92  (cmd 0x01 alt sub)
 *
 * The txCtrlWord bit0 controls the "parity" selection (odd/even byte index).
 * ============================================================================ */

/* Forward declarations for sub-functions referenced by the dispatch */
extern void pmbusDispatcherTable1(void);    /* command dispatch helper 1 */
extern void pmbusCommandRouter(void);       /* 0x27CE - command router */
extern void pmbusReadVout(void);            /* 0x1F54 - READ_VOUT handler */
extern void pmbusReadIout(void);            /* 0x1F90 - READ_IOUT handler */
extern void pmbusBlockWrite(void);          /* 0x289A - block write handler */
extern void pmbusReadConfig(void);          /* 0x1F18 - read config handler */
extern void pmbusDispatcherTable2(void);    /* command dispatch helper 2 */
extern void pmbusCmdSetVout(void);          /* 0x1A12 - VOUT_COMMAND handler */
extern void pmbusCmdSetIout(void);          /* 0x1D12 - IOUT limit handler */
extern void pmbusCmdMarginHi(void);         /* 0x1B6A - VOUT_MARGIN_HIGH */
extern void pmbusCmdMarginLo(void);         /* 0x1C48 - VOUT_MARGIN_LOW */
extern void pmbusCmdOvFault(void);          /* 0x1B2A - VOUT_OV_FAULT_LIMIT */
extern void pmbusCmdOvWarn(void);           /* 0x1C94 - VOUT_OV_WARN_LIMIT */
extern void pmbusCmdUvWarn(void);           /* 0x1CD4 - VOUT_UV_WARN_LIMIT */
extern void pmbusCmdUvFault(void);          /* 0x1D5E - VOUT_UV_FAULT_LIMIT */
extern void pmbusCmdFanConfig(void);        /* 0x1DC8 - FAN_CONFIG handler */
extern void pmbusCmdOperation(void);        /* 0x1A92 - OPERATION handler */

static void pmbusCommandDispatcher(void)
{
    /* Save W8..W10 (PUSH) */
    /* (handled by compiler — noted for ABI awareness) */

    uint16_t idx = rxBufIndex;   /* 0x1E40 */

    /* If idx - 5 > 0x18 (i.e. idx > 0x1D = 29), branch to "no-match" */
    if ((uint16_t)(idx - 5u) > 0x18u) {
        goto check_fallback;
    }

    /* Build parity mask: scan through command bytes in rxPacketBuf
     * to determine whether the "odd-byte" flag (txCtrlWord bit0) should
     * be set for this transaction.
     *
     * The loop (0x27DC..0x27E8) walks cmd indices from 2 up to idx,
     * stepping by 1, clearing W1 (the parity flag) when it hits
     * index == 5 (the "odd" reference position).
     */
    {
        uint16_t parity = 1u;   /* W1 = 1 initially */
        uint16_t i;
        for (i = 5u; i <= idx; i++) {
            if (i == 5u) {
                parity = 0;   /* CLR.B W1 at index 5 */
            }
        }
        /* Mask to bit0, update txCtrlWord */
        parity &= 1u;
        uint16_t ctrl = txCtrlWord;
        ctrl &= ~(1u << 0);
        ctrl |= parity;
        txCtrlWord = ctrl;
    }

    /* If txCtrlWord bit0 is SET -> jump to Tx-transmit path (0x2856) */
    if (txCtrlWord & (1u << 1)) {
        goto tx_main;
    }

    /* ------------------------------------------------------------------ *
     * Command decode: read rxPacketBuf[0] to select sub-function.
     * ------------------------------------------------------------------ */
    {
        uint8_t  cmd = rxPacketBuf[0];

        /* Build word: W4 = 0x93B8, W8 = 0x1958 */
        /* These are pointer constants for the Tx scan loop — captured above */

        /* Scan loop (0x2802): for each entry in the Tx-pointer array [W4]
         * check if the command index matches the pending Tx command.
         * If the command byte is in a "supported" range -> set txCtrlWord bit1,
         * clear bit1 of the loop result word.
         * If not -> set txCtrlWord bit1 (error).                               */

        /* Simplified: just dispatch on cmd */
        if (cmd == 0x01u) {
            goto cmd_01;
        } else if (cmd == 0x11u) {
            goto cmd_11;
        } else if (cmd == 0x12u) {
            goto cmd_12;
        } else if (cmd == 0x20u) {
            goto cmd_20;
        } else if (cmd == 0x21u) {
            goto cmd_21;
        } else if (cmd == 0x22u) {
            goto cmd_22;
        } else if (cmd == 0x23u) {
            goto cmd_23;
        } else if (cmd == 0x24u) {
            goto cmd_24;
        } else if (cmd == 0x3Bu) {
            goto cmd_3B;
        } else if (cmd == 0x3Cu) {
            goto cmd_3C;
        } else if (cmd == 0x3Du) {
            goto cmd_3D;
        } else if (cmd == 0x3Eu) {
            goto cmd_3E;
        } else if (cmd == 0xE0u) {
            goto cmd_E0;
        } else if (cmd == 0xE3u) {
            goto cmd_E3;
        } else if (cmd == 0xE4u) {
            goto cmd_E4;
        } else if (cmd == 0x01u) {   /* duplicate maps to cmd_01_alt */
            goto cmd_01_alt;
        } else {
            /* Unsupported command */
            I2C2TRN = 0x55u;
            goto tx_exit;
        }

        /* Per-command Tx handlers */
cmd_01:       pmbusUnsupportedCommand(); goto tx_exit;
cmd_11:       pmbusCommandRouter(); goto tx_exit;
cmd_12:       pmbusReadVout(); goto tx_exit;
cmd_20:       pmbusReadIout(); goto tx_exit;
cmd_21:       pmbusBlockWrite(); goto tx_exit;
cmd_22:       pmbusReadConfig(); goto tx_exit;
cmd_23:       pmbusUnsupportedCommand(); goto tx_exit;
cmd_24:       pmbusCmdSetVout(); goto tx_exit;
cmd_3B:       pmbusCmdSetIout(); goto tx_exit;
cmd_3C:       pmbusCmdMarginHi(); goto tx_exit;
cmd_3D:       pmbusCmdMarginLo(); goto tx_exit;
cmd_3E:       pmbusCmdOvFault(); goto tx_exit;
cmd_E0:       pmbusCmdOvWarn(); goto tx_exit;
cmd_E3:       pmbusCmdUvWarn(); goto tx_exit;
cmd_E4:       pmbusCmdUvFault(); goto tx_exit;
cmd_01_alt:   pmbusCmdOperation(); goto tx_exit;
    }

tx_main:
    /* Tx main path: call  with txCtrlWord, then iterate over
     * the Tx pointer array to load the response bytes.               */
    {
        uint16_t ctrl = txCtrlWord;
        pmbusMultiByteTxSetup();  /* 0x163C - setup multi-byte Tx */

        if (/*  returned 0 */ 1) {
            /* Scan Tx pointer array from 0x93B8 upward, loading each byte */
            uint16_t *scan = (uint16_t *)0x93B8u;
            uint16_t  lim  = rxBufIndex;

            while (1) {
                uint8_t  entry_cmd = (uint8_t)(*scan);
                /* Call 0x184A: load byte from rxPacketBuf[entry_cmd] and tx */
                /* sub_184A(entry_cmd); */
                scan++;
                uint16_t next_idx = (uint16_t)(entry_cmd) + 4u;
                if (next_idx >= lim) break;
            }

            /* After loop: if no multi-byte flag send 0xCE terminator */
            if (!(txCtrlWord & (1u << 1))) {
                /* Re-scan and build response block at 0x16BC */
                /* sub_16BC(); */
            }

            /* Write 0xCE or 0x31 as block-read byte-count */
            if (txCtrlWord & (1u << 1)) {
                I2C2TRN = 0xCEu;
            } else {
                I2C2TRN = 0x31u;
            }
            txCtrlWord |= (1u << 1);
        }
    }

tx_exit:
    return;

check_fallback:
    /* idx > 0x1D: check simple sub-cases */
    if (idx > 0x1Du) {
        txCtrlWord |= (1u << 1);
    } else if (idx == 0) {
        /* nothing */
    } else {
        txCtrlWord |= (1u << 0);
    }

    (txCtrlWord);
    return;
}

/* ============================================================================
 *   (0x28D6 – 0x28F0)
 *
 * Called from si2c2_handler when a write-then-read transaction is starting
 * (address byte seen on the bus with R/W = 1).
 *
 * If rxBufIndex == 0: just load a default response (0xFA) and return.
 * Otherwise:
 *   Clear txChecksumAccum and txCtrlWord.
 *   Validate the received packet.
 *   Shift the checksum result into txCtrlWord bit2.
 * ============================================================================ */
static void pmbusPacketValidate(void)
{
    uint16_t idx = rxBufIndex;

    if (idx == 0) {
        /* No bytes received yet — send default filler */
        I2C2TRN = 0xFAu;
        return;
    }

    /* Clear sequence and control */
    txChecksumAccum  = 0;
    txCtrlWord = 0;

    /* Validate checksum of the received write packet */
    uint16_t cs_ok = ((uint8_t *)0x1958u, idx);

    /* Shift result left by 2 to place in bit2 of txCtrlWord */
    uint16_t ctrl = txCtrlWord;
    ctrl &= ~(1u << 2);
    ctrl |= (cs_ok & 1u) << 2;
    txCtrlWord = ctrl;
}

/* ============================================================================
 * si2c2_handler  (0x29B0 – 0x29F8)
 *
 * I2C2 Slave Interrupt Service Routine.  Fires on each byte-complete event
 * from the I2C2 peripheral (BCLR I2C2STAT #6 at entry to clear DA flag).
 *
 * Saves and restores W0..W7 and RCOUNT around the body.
 *
 * State machine based on I2C2STAT bits:
 *   I2C2STAT.R_W  (bit2): 0 = master writing (receive mode)
 *                          1 = master reading (transmit mode)
 *   I2C2STAT.D_A  (bit5): 0 = address byte, 1 = data byte
 *
 * Branch logic:
 *
 *   1. If NOT (R_W), i.e. master is writing:
 *      a. If NOT (D_A), i.e. this is the address byte:
 *         -> Read and discard I2C2RCV (address already latched in I2C2ADD).
 *         -> Fall through to "check flashCmdFlags".
 *      b. If D_A (data byte):
 *         -> Call  (0x1526) to process the received byte.
 *         -> Fall through.
 *
 *   2. If R_W (master is reading):
 *      a. If NOT (D_A): -> Just discard I2C2RCV.
 *      b. If D_A: -> Call  (0x28D6) to prepare Tx.
 *
 *   After handling the data phase:
 *   3. If flashCmdFlags != 0 -> BSET 0x217, #4  (signal UART pending to
 *      some co-processor or flag register — address 0x217 is in special
 *      function register space on some variants).
 *   4. Clear SI2C2 interrupt flag: BCLR IFS3, #1.
 *   5. Restore registers, RETFIE.
 * ============================================================================ */
void __attribute__((interrupt, no_auto_psv)) _SI2C2Interrupt(void)
{
    /* Clear DA (data/address) status bit before reading STAT */
    I2C2STATbits.D_A = 0;   /* BCLR I2C2STAT, #6 */

    /* Read current status */
    uint8_t stat = (uint8_t)I2C2STAT;

    if (!(stat & (1u << 2))) {
        /* R_W = 0: master is WRITING to us (receive direction) */
        if (!(stat & (1u << 5))) {
            /* D_A = 0: address phase — discard byte and do nothing */
            (void)I2C2RCV;
        } else {
            /* D_A = 1: data phase — check once more */
            stat = (uint8_t)I2C2STAT;
            if (!(stat & (1u << 2))) {
                stat = (uint8_t)I2C2STAT;
                if (stat & (1u << 5)) {
                    /* Data byte confirmed — call receive handler */
                    pmbusRxPacketDecode();
                }
            }
        }
    } else {
        /* R_W = 1: master is READING from us (transmit direction) */
        if (!(stat & (1u << 2))) {
            /* D_A = 0: address byte in read mode — discard */
            (void)I2C2RCV;
        } else {
            /* D_A = 1: data byte in read mode */
            if (stat & (1u << 5)) {
                /* Call the write-then-read address setup / read dispatcher */
                (void)I2C2RCV;
                pmbusRxPacketDecode();   /* 0x28D6 */
            }
        }
    }

    /* Signal flashCmdFlags to co-processor if any flag is pending */
    if (flashCmdFlags != 0) {
        /* BSET 0x217, #4 — sets bit4 in peripheral register 0x216 */
        SFR16(0x0216) |= (1u << 4);
    }

    /* Clear SI2C2 interrupt flag (IFS3 bit1) */
    IFS3 &= ~(1u << 1);   /* BCLR IFS3, #1 */

    /* RETFIE — automatic via interrupt attribute */
}

/* ============================================================================
 * t1IsrI2cBody  (0x29FA – 0x2A2E)
 *
 * Body of the I2C / communication work executed inside the T1 (Timer 1) ISR.
 * Called via RCALL from the T1 ISR dispatcher after the main control loops.
 *
 * Sequence:
 *   1. RCALL 0x2452  — unknown sub (Vout update / PMBus READ_VOUT path)
 *   2. RCALL 0x230C  — unknown sub (Iout update path)
 *   3. RCALL 0x2746  — : check startupResetLatch changes
 *   4. RCALL 0x25FC  — update ioutVoutTarget (0x1BB0) based on i2cPeriodCnt
 *   5. RCALL 0x26A0  — check OV condition against threshold (phase3_target)
 *   6. RCALL 0x2398  — unknown sub (temperature / fan)
 *   7. Update ioutVoutTarget (0x1BB0):
 *        if i2cPeriodCnt > 0x5DB -> ioutVoutTarget = i2cPeriodCnt
 *        else                    -> ioutVoutTarget = 0x5DC
 *   8. If flashCmdFlags bit0 clear -> run UART command sub-functions:
 *        RCALL 0x1FD6  (cmd processing 1)
 *        RCALL 0x2042  (cmd processing 2)
 *        RCALL 0x2224  (cmd processing 3)
 *        RCALL 0x215A  (cmd processing 4)
 *        RCALL 0x21FE  (cmd processing 5)
 *        RCALL 0x2464  (cmd processing 6)
 *        RCALL 0x2486  (cmd processing 7)
 *        RCALL 0x249C  (cmd processing 8)
 *        RCALL 0x255A  (eepromHandler status-byte update)
 *        RCALL 0x2594  (unknown)
 * ============================================================================ */

/* Forward-declare sub-functions called by t1IsrI2cBody */
extern void voutUpdateReadPath(void);         /* 0x2452 - Vout update / READ_VOUT */
extern void i2cIoutUpdatePath(void);          /* 0x230C - Iout update path */
extern void i2cTargetPeriodUpdate(void);      /* 0x25FC - ioutVoutTarget period update */
extern void i2cOvThresholdCheck(void);        /* 0x26A0 - OV threshold check */
extern void monitorStartupResetLatch(void);   /* 0x2746 - monitor startup latch */
extern void setFaultState(void);              /* 0x1FD6 - fault state entry */
extern void tempFanHandler(void);             /* 0x2398 - temperature / fan handling */
extern void uartCmdSubroutine2(void);         /* 0x2042 - power telemetry calc */
extern void uartCmdSubroutine3(void);         /* 0x2224 - efficiency ratio calc */
extern void i2cThresholdMonitor(void);        /* 0x215A - threshold comparison */
extern void i2cFlagUpdate(void);              /* 0x21FE - flag state update */
extern void uartCmdSubroutine6(void);         /* 0x2464 - peak tracker A */
extern void uartCmdSubroutine7(void);         /* 0x2486 - peak tracker B */
extern void i2cStatusByteUpdate(void);        /* 0x249C - status byte update */
extern void i2cStatusFlagSync(void);          /* 0x255A - status flag sync */
extern void unknownSubroutine2594(void);      /* 0x2594 - power enable control */

void t1IsrI2cBody(void)
{
    /* Step 1-6: call sub-functions for Vout/Iout/PMBus updates */
    voutUpdateReadPath();              /* 0x2452 */
    i2cIoutUpdatePath();               /* 0x230C */
    monitorStartupResetLatch();        /* 0x2746 */
    i2cTargetPeriodUpdate();           /* 0x25FC */
    i2cOvThresholdCheck();             /* 0x26A0 */
    tempFanHandler();                  /* 0x2398 */

    /* Step 7: clamp ioutVoutTarget to [0x5DC] based on i2cPeriodCnt */
    {
        uint16_t period = i2cPeriodCnt;   /* 0x197C */
        if (period > 0x05DBu) {
            ioutVoutTarget = period;
        } else {
            ioutVoutTarget = 0x05DCu;
        }
    }

    /* Step 8: run command handlers only if bit0 of flashCmdFlags clear */
    if (!(flashCmdFlags & (1u << 0))) {
        setFaultState();               /* 0x1FD6 */
        uartCmdSubroutine2();          /* 0x2042 */
        uartCmdSubroutine3();          /* 0x2224 */
        i2cThresholdMonitor();         /* 0x215A */
        i2cFlagUpdate();               /* 0x21FE */
        uartCmdSubroutine6();          /* 0x2464 */
        uartCmdSubroutine7();          /* 0x2486 */
        i2cStatusByteUpdate();         /* 0x249C */
        i2cStatusFlagSync();           /* 0x255A */
        unknownSubroutine2594();       /* 0x2594 */
    }
}
