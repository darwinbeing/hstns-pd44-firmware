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
 * i2cTickCnt (0x1936:0x1938) – 32-bit tick counter for slave handler
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
static uint16_t pmbusBlockCommandInvalid(uint16_t cmd_idx);
static void pmbusFlashReadWindow(void);
static void pmbusFlashWriteWindow(void);
static void pmbusOperationUnlock(void);
static void pmbusProgramHeader(void);
static void pmbusProgramBlock(void);
static void pmbusReadbackBlock(void);
static void pmbusStartFlashWrite(void);
static void pmbusReadDeviceInfo(void);
static void pmbusEnterProgramming(void);
static void pmbusStoreProgrammingData(void);
static void pmbusReadFlashSummaryBlock(void);
static void pmbusReadFlashDataBlock(void);
static void pmbusReadFlashStatusBlock(void);
static void pmbusReadVoutCommand(void);
static void pmbusReadIoutCommand(void);
static void pmbusSetOperationMode(void);
static void pmbusBlockWriteModeSelect(void);
static void pmbusReadConfigCommand(void);
static void pmbusWriteCommand(uint8_t cmd, uint16_t word_val, uint16_t byte_cnt);
static uint16_t pmbusValidateCommand(uint16_t cmd_idx);
void i2cTxAccumulate(void);
void i2cBusStuckHandler(void);
void eepromHandler(void);
void eepromCfgUpdate(uint8_t mode, uint8_t status, uint8_t enable);
uint16_t eepromCfgRead(void);
uint8_t flash_lookup_15D0(uint16_t idx);
uint8_t flash_lookup_15B0(uint16_t idx);
uint8_t flash_lookup_1598(uint16_t idx);
/* si2c2_handler is the ISR — declared below */

/*
 * 0x93B8: ROM byte table used by the block dispatcher at 0x27CE.
 * Each entry points at the command byte of a 3-byte tuple in rxPacketBuf:
 *   [command, low byte, high byte] at offsets 1, 4, 7, ...
 */
static const uint8_t pmbusBlockOffsets[] = {  /* 0x93B8 */
    0x01u, 0x04u, 0x07u, 0x0Au, 0x0Du, 0x10u, 0x13u, 0x16u, 0x19u
};


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
    ptrTable19D0[4] = (uint16_t)((uint16_t*)&i2cAccum + 1); /* hi half, was 0x194C */
    ptrTable19D0[5] = (uint16_t)&i2cAccum;                 /* lo half, was 0x194A */
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
 *   store I2C2RCV -> rxPacketBuf[rxBufIndex] (RAM 0x1958 + index),
 *   then increment rxBufIndex.
 *
 * After each byte the function checks rxPacketBuf[0] (command code) against
 * known commands and sets bits in rxEventFlags.
 *
 * When rxBufIndex reaches 4 (full 3-data-byte packet):
 *   Command 0x3B (PAGE command): reconstruct 16-bit value from rxPacketBuf[2:3],
 *     clamp/wrap to [0x0800 .. 0xF7FE], store in i2cRxData (0x1922),
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
        /* Normal data byte: store into rxPacketBuf[idx] (base address 0x1958) */
        uint8_t b = (uint8_t)I2C2RCV;
        rxPacketBuf[idx] = b;   /* [0x1958 + rxBufIndex] */
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

            /* Store in i2cRxData (0x1922) */
            i2cRxData = val;

            /* Clamp: if val > 0xF7FE -> val = 0xF7FF (saturate high) */
            if (val > 0xF7FEu) {
                val = 0xF7FF;
            } else {
                /* Wrap-around add: val += 0x0800 */
                val += 0x0800u;
            }
            i2cRxData = val;

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
 * Computes the PMBus PEC-style additive checksum used by this firmware and
 * tests the final byte against the two's-complement accumulator.
 *
 * Entry: W0 = buffer pointer (0x1958),
 *        W1 = buffer length, including the checksum byte,
 *        W5 = pointer to start of buffer (set by caller at 0x15EB).
 * Exit:  W0 = 1 if the checksum is non-zero (bad), 0 if it matches.
 *
 * Algorithm:
 *   sum = 2 * I2C2ADD   (device address counted twice)
 *   for i in 0 .. len-2:
 *       sum += buf[i]
 *   return (uint8_t)(buf[len-1] + sum) != 0
 * ============================================================================ */
static uint16_t pmbusChecksumVerify(uint8_t *buf, uint16_t len)
{
    uint16_t addr = (uint16_t)I2C2ADD;
    uint16_t sum  = addr + addr;   /* 2 * slave address */
    uint16_t i;

    if (len == 0) {
        return 0;
    }

    for (i = 0; i < (uint16_t)(len - 1u); i++) {
        sum += (uint16_t)(uint8_t)buf[i];
    }

    return ((uint8_t)((uint8_t)buf[len - 1u] + (uint8_t)sum) != 0u) ? 1u : 0u;
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
 *   other        -> return 1 without touching I2C2TRN
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
            return 1;
        }
    } else if (code == 1) {
        tx_byte = 0xFE;
    } else if (code == 2) {
        tx_byte = 0xCE;
    } else {
        /* code == 0 or unknown */
        return 1;
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
 *   5. Calls pmbusEncodeResponse(txCtrlWord).
 *   6. If no header byte was loaded (non-zero return):
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
        uint16_t result = pmbusEncodeResponse(ctrl);

        if (result == 0) {
            /* Header byte was loaded in TRN — done */
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

static void pmbusSendDataByte(uint8_t value)
{
    I2C2TRN = (uint16_t)value;
    txChecksumAccum = (uint16_t)(txChecksumAccum + (uint16_t)I2C2TRN);
}

static void pmbusSendEndChecksum(void)
{
    txCtrlWord |= (1u << 1);
    I2C2TRN = (uint8_t)(-(int8_t)(uint8_t)txChecksumAccum);
}

static uint16_t pmbusCommandCodeSupported(uint8_t cmd)
{
    if (cmd < 0x18u) {
        if (cmd > 0x15u) {
            return 1;
        }
        if (cmd > 0x04u) {
            if (cmd < 0x08u || cmd == 0x13u) {
                return 1;
            }
        }
        return 0;
    }

    if (cmd < 0x23u) {
        return 0;
    }
    if (cmd <= 0x24u) {
        return 1;
    }
    if ((uint8_t)(cmd - 0x30u) <= 0x0Cu) {
        return 1;
    }
    return (cmd == 0x3Du) ? 1u : 0u;
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
 * (W0 - 5) as the index, covering command codes 0x05 .. 0x3D.
 * Commands not in the table branch to 0x1848 (default: no-op / RETURN).
 *
 * Jump table entries with explicit branch targets (assembly 0x16E2..0x1752):
 *   offset  0 (cmd 0x05) -> 0x1754  MOV ptrTable19BA[0] -> clear *ptr
 *   offset  1 (cmd 0x06) -> 0x1758  MOV ptrTable19BA[1] -> clear *ptr
 *   offset  2 (cmd 0x07) -> 0x175C  MOV ptrTable19BA[2] -> clear *ptr
 *   offset 14 (cmd 0x13) -> 0x1760  clear ptrTable19D0[3..5]
 *   offset 17 (cmd 0x16) -> 0x176C  clear ptrTable19D0[6]
 *   offset 18 (cmd 0x17) -> 0x1770  clear ptrTable19D0[7]
 *   offset 30 (cmd 0x23) -> 0x1774  clear ptrTable19F0[6]
 *   offset 31 (cmd 0x24) -> 0x1778  clear ptrTable19F0[7]
 *   offset 43 (cmd 0x30) -> 0x177E  status/droop word update
 *   offset 44 (cmd 0x31) -> 0x17E6  -> BTST droopEnableFlags bit10 (0x1BEB bit2) path
 *   offset 45..56        -> 0x1814..0x1844 write ptrTable1A10[2..13]
 *   default -> 0x1848 RETURN
 * ============================================================================ */
static void pmbusWriteCommand(uint8_t cmd, uint16_t word_val, uint16_t byte_cnt)
{
    (void)byte_cnt;

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
        /* cmd 0x13 (idx 14): clear ptrTable19D0[3], [4], [5] */
        case 14:
            *((volatile uint16_t *)ptrTable19D0[3]) = 0;
            *((volatile uint16_t *)ptrTable19D0[4]) = 0;
            *((volatile uint16_t *)ptrTable19D0[5]) = 0;
            return;
        /* cmd 0x16 (idx 17): clear ptrTable19D0[6] */
        case 17:
            *((volatile uint16_t *)ptrTable19D0[6]) = 0;
            return;
        /* cmd 0x17 (idx 18): clear ptrTable19D0[7] */
        case 18:
            *((volatile uint16_t *)ptrTable19D0[7]) = 0;
            return;
        /* cmd 0x23 (idx 30): clear ptrTable19F0[6] */
        case 30:
            *((volatile uint16_t *)ptrTable19F0[6]) = 0;
            return;
        /* cmd 0x24 (idx 31): clear ptrTable19F0[7] */
        case 31:
            *((volatile uint16_t *)ptrTable19F0[7]) = 0;
            return;

        /* ------------------------------------------------------------------ *
         * cmd 0x30 (idx 43): set auxFlags bit3; update droopEnableFlags bit2 based
         * on status; route the new value to ptrTable1A10[0].
         * ------------------------------------------------------------------ */
        case 43: {
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
                    goto cmd30_set;
                }
                uint16_t bec = startupResetLatch;
                if (bec & 0x0Cu) {
                    return;   /* bits 2 or 3 set -> abort */
                }
            }
cmd30_set:
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
         * 0x17E6..0x1810: bit0 is set only on the droopEnableFlags bit10 path.
         * ------------------------------------------------------------------ */
        case 44: {
            if (droopEnableFlags & (1u << 10)) {
                /* Firmware-update path (droopEnableFlags bit10 / 0x1BEB bit2 set) */
                *((volatile uint16_t *)ptrTable1A10[1]) = word_val;
                if (word_val == 0x8000u) {
                    flash_read_buf_15E6[0x10] = 1;
                    flashCmdFlags |= (1u << 6);
                } else {
                    if (word_val <= 0x000Fu) {
                        currentLimitFlags |= (1u << 13);
                    } else {
                        currentLimitFlags &= ~(1u << 13);
                    }
                }
                currentLimitFlags |= (1u << 0);
            } else {
                /* Normal path: write word_val to ptrTable1A10[1] and skip flags */
                *((volatile uint16_t *)ptrTable1A10[1]) = word_val;
            }
            /* Common: set droopEnableFlags bit3 */
            droopEnableFlags |= (1u << 3);
            return;
        }

        /* ------------------------------------------------------------------ *
         * cmd 0x32..0x3D (idx 45..56) write successive ptrTable1A10 entries.
         * ------------------------------------------------------------------ */
        case 45: *((volatile uint16_t *)ptrTable1A10[2])  = word_val; return;
        case 46: *((volatile uint16_t *)ptrTable1A10[3])  = word_val; return;
        case 47: *((volatile uint16_t *)ptrTable1A10[4])  = word_val; return;
        case 48: *((volatile uint16_t *)ptrTable1A10[5])  = word_val; return;
        case 49: *((volatile uint16_t *)ptrTable1A10[6])  = word_val; return;
        case 50: *((volatile uint16_t *)ptrTable1A10[7])  = word_val; return;
        case 51: *((volatile uint16_t *)ptrTable1A10[8])  = word_val; return;
        case 52: *((volatile uint16_t *)ptrTable1A10[9])  = word_val; return;
        case 53: *((volatile uint16_t *)ptrTable1A10[10]) = word_val; return;
        /* cmd 0x3B (idx 54): also set systemFlags bit14 (0x1E1B bit6) */
        case 54:
            systemFlags |= (1u << 14);
            *((volatile uint16_t *)ptrTable1A10[11]) = word_val;
            return;
        case 55: *((volatile uint16_t *)ptrTable1A10[12]) = word_val; return;
        case 56: *((volatile uint16_t *)ptrTable1A10[13]) = word_val; return;

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
 *   cmd 0x13 -> Clear three ptrTable19D0 entries (accumulators)
 *   cmd 0x16..0x17 -> Clear ptrTable19D0[6..7]
 *   cmd 0x23..0x24 -> Clear ptrTable19F0[6..7]
 *   cmd 0x30 -> Status/fault complex handler (see above)
 *   cmd 0x31 -> Write iout/vout value
 *   cmd 0x32..0x3D -> Write successive ptrTable1A10 entries
 *
 * This function is modelled by pmbusWriteCommand() above.
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
 *   0x05..0x07
 *   0x13, 0x16, 0x17
 *   0x23..0x24
 *   0x30..0x3D
 * ============================================================================ */
static uint16_t pmbusValidateCommand(uint16_t cmd_idx)
{
    return pmbusCommandCodeSupported((uint8_t)rxPacketBuf[cmd_idx]) ? 0u : 1u;
}

/*
 * 0x184A: second-stage validator used by the block-write dispatcher.
 * pmbusValidateCommand() only checks the command code; this also checks that
 * the associated 16-bit value is in the range accepted by the original image.
 * Return value follows the assembly convention: 0 = valid, 1 = reject.
 */
static uint16_t pmbusBlockCommandInvalid(uint16_t cmd_idx)
{
    uint8_t cmd = (uint8_t)rxPacketBuf[cmd_idx];
    uint16_t word_val = (uint16_t)rxPacketBuf[cmd_idx + 1u]
                      | ((uint16_t)rxPacketBuf[cmd_idx + 2u] << 8);

    switch (cmd) {
        case 0x05u:
        case 0x06u:
        case 0x07u:
        case 0x13u:
        case 0x16u:
        case 0x17u:
        case 0x23u:
        case 0x24u:
            return (word_val != 0u) ? 1u : 0u;

        case 0x30u:
            return ((word_val & 0xF010u) != 0u) ? 1u : 0u;

        case 0x31u:
            if (flash_read_buf_15E6[0x10] == 1) {
                return 1;
            }
            if ((droopEnableFlags & 0x0400u) == 0) {
                return (word_val == 0u) ? 1u : 0u;
            }
            if (word_val < 0x0100u || word_val == 0x8000u) {
                return 0;
            }
            return 1;

        case 0x32u:
            return (word_val <= 0x2FFFu || word_val > 0x3133u) ? 1u : 0u;

        case 0x33u:
            return (word_val <= 0x003Fu || word_val > 0x0640u) ? 1u : 0u;

        case 0x34u:
            return (word_val <= 0x003Fu || word_val > 0x1180u) ? 1u : 0u;

        case 0x35u:
            return (word_val > (uint16_t)(pmbusReadPtr0 - 0x0100u)) ? 1u : 0u;

        case 0x36u:
            return (word_val < (uint16_t)(pmbusReadPtr1 + 0x0100u)) ? 1u : 0u;

        case 0x37u:
            return (word_val > (uint16_t)(voutCalD - 0x0100u)) ? 1u : 0u;

        case 0x38u:
            return (word_val < (uint16_t)(voutCalE + 0x0100u)) ? 1u : 0u;

        case 0x3Du: {
            uint8_t low = (uint8_t)word_val;
            if (low == 0u) {
                return 1;
            }
            if (low <= 0xEFu) {
                return 0;
            }
            return (low == 0xFAu) ? 0u : 1u;
        }

        default:
            return 0;
    }
}

/*
 * 0x1982: stream a window out of flash_read_buf_160E.
 * rxBufIndex selects one of two firmware formats:
 *   len 3: rxPacketBuf[1] is a 16-byte page index (page << 4).
 *   len 4: rxPacketBuf[1] is a start byte, rxPacketBuf[2] is a byte count.
 */
static void pmbusFlashReadWindow(void)
{
    if (rxBufIndex == 3u) {
        if (rxPacketBuf[1] < 0x10u) {
            txByteCntPreset = 0;
        } else {
            txCtrlWord |= (1u << 1);
        }
    } else if (rxBufIndex == 4u) {
        uint16_t start = (uint16_t)rxPacketBuf[1];
        uint16_t count = (uint16_t)rxPacketBuf[2];
        if (count != 0u && (uint16_t)(start + count) < 0x0101u) {
            txByteCntPreset = 1;
        } else {
            txCtrlWord |= (1u << 1);
        }
    } else if (rxBufIndex < 5u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
    } else {
        txCtrlWord |= (1u << 1);
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    if (txByteCntPreset == 0u) {
        if (rxPendFlag > 0x000Fu) {
            pmbusSendEndChecksum();
            return;
        }
        pmbusSendDataByte(flash_read_buf_160E[((uint16_t)rxPacketBuf[1] << 4) + rxPendFlag]);
    } else {
        if ((uint16_t)rxPacketBuf[2] <= rxPendFlag) {
            pmbusSendEndChecksum();
            return;
        }
        pmbusSendDataByte(flash_read_buf_160E[(uint16_t)rxPacketBuf[1] + rxPendFlag]);
    }
}

/*
 * 0x1A12: write a received byte window into flash_read_buf_160E.
 * The first 200 bytes are protected unless LLC programming mode is active
 * (llcStatus bit8); this mirrors the original dst > 0x00C7 test.
 */
static void pmbusFlashWriteWindow(void)
{
    uint16_t changed;

    if (rxBufIndex < 4u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
    } else if ((uint16_t)((uint16_t)rxPacketBuf[1] + rxBufIndex - 4u) > 0x00FFu) {
        txCtrlWord |= (1u << 1);
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    changed = (flashCmdFlags >> 4) & 1u;
    for (uint16_t i = 0; i < (uint16_t)(rxBufIndex - 3u); i++) {
        uint16_t dst = (uint16_t)rxPacketBuf[1] + i;
        if ((llcStatus & 0x0100u) != 0u || dst > 0x00C7u) {
            flash_read_buf_160E[dst] = rxPacketBuf[i + 2u];
            changed = 1;
        } else {
            break;
        }
    }

    flashCmdFlags = (flashCmdFlags & (uint16_t)~0x0010u) | (uint16_t)(changed << 4);
    txCtrlWord |= (1u << 1);
    I2C2TRN = (changed != 0u) ? 0x31u : 0xAAu;
}

/*
 * 0x1A92: programming unlock handshake.
 * The packet includes a fixed magic prefix plus the EEPROM CRC shadow, then
 * toggles systemFlags bit9 and persists the enable state through eepromCfgUpdate().
 */
static void pmbusOperationUnlock(void)
{
    if (rxBufIndex != 8u) {
        if (rxBufIndex < 9u) {
            if (rxBufIndex != 0u) {
                txCtrlWord |= (1u << 0);
            }
        } else {
            txCtrlWord |= (1u << 1);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    txCtrlWord |= (1u << 1);
    if (rxPacketBuf[1] == 0x05u && rxPacketBuf[2] == 0xA6u && rxPacketBuf[4] == 0x01u &&
        rxPacketBuf[3] == 0x02u &&
        rxPacketBuf[5] == (uint8_t)(eepromCrcShadow >> 8) &&
        rxPacketBuf[6] == (uint8_t)eepromCrcShadow) {
        systemFlags |= 0x0200u;
        eepromCfgUpdate(0xFFu, 0xFFu, 1u);
        flashUpdateResult = eepromCfgRead();
        I2C2TRN = 0x31u;
        return;
    }

    systemFlags &= (uint16_t)~0x0200u;
    eepromCfgUpdate(0xFFu, 0xFFu, 0u);
    flashUpdateResult = eepromCfgRead();
    I2C2TRN = 0xCEu;
}

/* 0x1B2A: accept the "PH" programming header only after the unlock bit is set. */
static void pmbusProgramHeader(void)
{
    if (rxBufIndex == 4u) {
        if ((systemFlags & 0x0200u) == 0u) {
            txCtrlWord |= (1u << 4);
        }
    } else if (rxBufIndex < 5u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
    } else {
        txCtrlWord |= (1u << 1);
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    txCtrlWord |= (1u << 1);
    if (rxPacketBuf[1] == 'P' && rxPacketBuf[2] == 'H') {
        systemFlags |= 0x0001u;
        I2C2TRN = 0x31u;
    } else {
        I2C2TRN = 0xCEu;
    }
}

/*
 * 0x1B6A: receive a programming data block.
 * The block has an internal 8-bit additive checksum at byte_count + 5, separate
 * from the outer PMBus checksum.  The page word is then translated into the
 * AT45DB write window by adding 0x0800, saturating at 0xF7FF.
 */
static void pmbusProgramBlock(void)
{
    uint8_t byte_count = rxPacketBuf[1];
    uint16_t end = (uint16_t)byte_count + 4u;
    uint16_t sum = 0;

    if ((rxBufIndex >= 7u && rxBufIndex <= 0x17u) || rxBufIndex == 0x17u) {
        if (byte_count < 0x11u) {
            if ((systemFlags & 0x0200u) == 0u) {
                txCtrlWord |= (1u << 4);
            }
        } else {
            txCtrlWord |= (1u << 1);
        }
    } else {
        if (rxBufIndex > 0x17u) {
            txCtrlWord |= (1u << 1);
        } else if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    txCtrlWord |= (1u << 1);
    /* Original code verifies the block-local checksum before copying bytes. */
    for (uint16_t i = 1u; i <= end; i++) {
        sum += rxPacketBuf[i];
    }

    txChecksumAccum = (uint8_t)(-(int8_t)(uint8_t)sum);
    if ((uint8_t)txChecksumAccum != rxPacketBuf[(uint16_t)byte_count + 5u]) {
        I2C2TRN = 0xCEu;
        return;
    }

    if (rxPacketBuf[4] == 0u) {
        if (rxBufIndex < 8u || byte_count == 0u) {
            I2C2TRN = 0xCEu;
            return;
        }

        /*
         * 0x1BD6..0x1BE4 copies rxPacketBuf[5..byte_count+4] to scratch RAM
         * at 0x170E, then 0x1C0E..0x1C20 copies scratch[0..byte_count-1] to
         * the 0x171E programming page buffer.  The 0x181E readback buffer is
         * not updated by this command.
         */
        for (uint16_t i = 0; i < byte_count; i++) {
            flash_program_scratch[i] = rxPacketBuf[5u + i];
        }

        flashBlockPageWord = ((uint16_t)rxPacketBuf[2] << 8) | rxPacketBuf[3];
        /* Host page word is offset into the flash write address window. */
        if (flashBlockPageWord < 0xF7FFu) {
            flashBlockPageWord = (uint16_t)(flashBlockPageWord + 0x0800u);
        } else {
            flashBlockPageWord = 0xF7FFu;
        }

        flashBlockByteCount = byte_count;
        uint16_t dst = flashBlockPageWord & 0x00FFu;
        for (uint16_t i = 0; i < byte_count; i++) {
            flashBlockLastOffset = (uint16_t)(dst + i);
            flash_buf_171E[flashBlockLastOffset] = flash_program_scratch[i];
        }
        if (flashBlockLastOffset == 0x00FFu) {
            flashCmdFlags |= (1u << 0);
        }
    }

    eepromCfgUpdate(0xFFu, 0u, 0xFFu);
    I2C2TRN = 0x31u;
}

/* 0x1C48: read back bytes from the assembled programming page buffer. */
static void pmbusReadbackBlock(void)
{
    if (rxBufIndex == 5u) {
        if (rxPacketBuf[1] <= 0x10u) {
            goto ok;
        }
        txCtrlWord |= (1u << 1);
    } else if (rxBufIndex < 6u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
        goto ok;
    } else {
        txCtrlWord |= (1u << 1);
    }

ok:
    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }
    if (rxPendFlag == rxPacketBuf[1]) {
        pmbusSendEndChecksum();
        return;
    }
    pmbusSendDataByte(flash_buf_181E[rxPendFlag]);
}

/* 0x1C94: start the deferred flash write sequence at flash address 0x0800. */
static void pmbusStartFlashWrite(void)
{
    if (rxBufIndex != 2u) {
        if (rxBufIndex < 3u) {
            if (rxBufIndex != 0u) {
                txCtrlWord |= (1u << 3);
            }
        } else {
            txCtrlWord |= (1u << 1);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    txCtrlWord |= (1u << 1);
    I2C2TRN = 0x31u;
    if ((systemFlags & 0x0100u) == 0u) {
        systemFlags |= 0x0100u;
        flash_page_addr = 0;
        flash_write_offset = 0x0800u;
        eepromCfgUpdate(0xFFu, 1u, 0xFFu);
    }
}

/* 0x1CD4: 10-byte device information response from the table at 0x1D03. */
static void pmbusReadDeviceInfo(void)
{
    if (rxBufIndex == 3u) {
        if (rxPacketBuf[1] == 0xE0u) {
            goto ok;
        }
    } else if (rxBufIndex == 0u) {
        goto ok;
    }
    txCtrlWord |= (1u << 3);

ok:
    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }
    if (rxPendFlag == 10u) {
        pmbusSendEndChecksum();
        return;
    }
    pmbusSendDataByte(pmbusInfoBytes[rxPendFlag]);
}

static uint8_t pmbusProgrammingStringByte(uint16_t idx)
{
    if (idx < sizeof(pmbusStringBuf)) {
        return pmbusStringBuf[idx];
    }

    idx = (uint16_t)(idx - sizeof(pmbusStringBuf));
    if (idx < sizeof(pmbusInfoBytes)) {
        return pmbusInfoBytes[idx];
    }

    return 0;
}

/* 0x1DC8 read path: streams the 20-byte programming metadata window. */
static void pmbusReadProgrammingString(void)
{
    if ((llcStatus & 0x0100u) == 0u) {
        I2C2TRN = 0x55u;
        return;
    }

    if (rxPendFlag == 0x14u) {
        txCtrlWord |= (1u << 2);
        I2C2TRN = (uint8_t)(-(int8_t)(uint8_t)txChecksumAccum);
        return;
    }

    uint8_t b;
    if (rxPendFlag == 0u) {
        b = (uint8_t)flash_read_buf_15E6[0x11];
    } else if (rxPendFlag == 1u) {
        b = (uint8_t)flash_read_buf_15E6[0x12];
    } else if (rxPendFlag == 2u) {
        b = (uint8_t)flash_read_buf_15E6[0x13];
    } else if (rxPendFlag == 3u) {
        b = (uint8_t)flash_read_buf_15E6[0x14];
    } else {
        b = (uint8_t)flash_read_buf_15E6[(uint16_t)rxPendFlag - 4u];
    }

    pmbusSendDataByte(b);
}

/* 0x1D5E: stores or streams the 9-byte programming string buffer at 0x1CFA. */
static void pmbusEnterProgramming(void)
{
    if (rxBufIndex < 2u && rxBufIndex != 0u) {
        txCtrlWord |= (1u << 3);
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    if ((llcStatus & 0x0100u) != 0u) {
        if (rxBufIndex == 0x0Bu) {
            for (uint16_t i = 0; i < 9u; i++) {
                pmbusStringBuf[i] = rxPacketBuf[i + 1u];
            }
            currentLimitFlags |= (1u << 1);
            txCtrlWord |= (1u << 2);
            I2C2TRN = 0x31u;
            return;
        }

        if (rxPendFlag >= 2u &&
            pmbusProgrammingStringByte((uint16_t)(rxPendFlag - 2u)) == '\n' &&
            pmbusProgrammingStringByte((uint16_t)(rxPendFlag - 1u)) == '\r') {
            txCtrlWord |= (1u << 2);
            I2C2TRN = (uint8_t)(-(int8_t)(uint8_t)txChecksumAccum);
            return;
        }

        pmbusSendDataByte(pmbusProgrammingStringByte(rxPendFlag));
        return;
    }

    I2C2TRN = 0x55u;
}

/* 0x1DC8: store programming metadata into flash_read_buf_15E6. */
static void pmbusStoreProgrammingData(void)
{
    if (rxBufIndex < 2u && rxBufIndex != 0u) {
        txCtrlWord |= (1u << 3);
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    if ((llcStatus & 0x0100u) != 0u) {
        if (rxBufIndex == 0x16u) {
            flash_read_buf_15E6[0x11] = (int8_t)rxPacketBuf[1];
            flash_read_buf_15E6[0x12] = (int8_t)rxPacketBuf[2];
            flash_read_buf_15E6[0x13] = (int8_t)rxPacketBuf[3];
            flash_read_buf_15E6[0x14] = (int8_t)rxPacketBuf[4];
            for (uint16_t i = 0; i < 16u; i++) {
                flash_read_buf_15E6[i] = (int8_t)rxPacketBuf[i + 5u];
            }
            flashCmdFlags |= (1u << 6);
            statusFlags |= (1u << 6);
            txCtrlWord |= (1u << 2);
            I2C2TRN = 0x31u;
            return;
        }

        pmbusReadProgrammingString();
        return;
    }

    I2C2TRN = 0x55u;
}

/* 0x1E8A path: status-summary block sourced from flash_read_buf_15D0. */
static void pmbusReadFlashSummaryBlock(void)
{
    if (rxPendFlag < 0x16u) {
        if (rxPacketBuf[2] == 0u) {
            pmbusSendDataByte(flash_lookup_15D0(rxPendFlag));
        } else {
            pmbusSendDataByte(0);
        }
        return;
    }

    pmbusSendEndChecksum();
}

/* 0x1EB0 path: calibration/data block sourced from flash_read_buf_15B0. */
static void pmbusReadFlashDataBlock(void)
{
    if (rxPacketBuf[1] == 2u) {
        if (rxPendFlag < 0x1Cu) {
            goto send_byte;
        }
    } else if (rxPacketBuf[1] == 3u && rxPendFlag < 0x1Du) {
        goto send_byte;
    }

    pmbusSendEndChecksum();
    return;

send_byte:
    if (rxPacketBuf[2] < 0x0Bu) {
        pmbusSendDataByte(flash_lookup_15B0(rxPendFlag));
    } else {
        pmbusSendDataByte(0);
    }
}

/* 0x1EEE path: flash status block sourced from flash_sector_buf_1598. */
static void pmbusReadFlashStatusBlock(void)
{
    if (rxPendFlag < 0x18u) {
        if (rxPacketBuf[2] == 0u) {
            pmbusSendDataByte(flash_lookup_1598(rxPendFlag));
        } else {
            pmbusSendDataByte(0);
        }
        return;
    }

    pmbusSendEndChecksum();
}

/* 0x289A: select one of the flash readback block formats. */
static void pmbusBlockWriteModeSelect(void)
{
    if (rxBufIndex != 4u) {
        if (rxBufIndex < 5u) {
            if (rxBufIndex != 0u) {
                txCtrlWord |= (1u << 0);
            }
        } else {
            txCtrlWord |= (1u << 1);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    if (rxPacketBuf[1] == 1u) {
        pmbusReadFlashStatusBlock();
    } else if (rxPacketBuf[1] == 0u) {
        pmbusReadFlashSummaryBlock();
    } else if (rxPacketBuf[1] == 2u || rxPacketBuf[1] == 3u) {
        pmbusReadFlashDataBlock();
    } else {
        I2C2TRN = 0;
    }
}

/* 0x1F18: enter config/update mode only when LLC programming mode is active. */
static void pmbusReadConfigCommand(void)
{
    if (rxBufIndex == 3u) {
        if (rxPacketBuf[1] == 1u) {
            goto ok;
        }
    } else if (rxBufIndex < 4u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
        goto ok;
    }
    txCtrlWord |= (1u << 1);

ok:
    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }
    txCtrlWord |= (1u << 1);
    if ((llcStatus & 0x0100u) != 0u) {
        systemFlags |= 0x0048u;
        I2C2TRN = 0x31u;
    } else {
        I2C2TRN = 0xAAu;
    }
}

/* 0x1F54: arm the VOUT read/update path and shadow the current calibration. */
static void pmbusReadVoutCommand(void)
{
    if (rxBufIndex == 3u) {
        if ((currentLimitFlags & 0x0100u) == 0u && systemState != ST_HOLDOFF &&
            rxPacketBuf[1] == 0xAAu) {
            goto ok;
        }
    } else if (rxBufIndex < 4u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
        goto ok;
    }
    txCtrlWord |= (1u << 1);

ok:
    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }
    txCtrlWord |= (1u << 1);
    currentLimitFlags |= 0x0100u;
    ioutCalFactorShadow = ioutCalFactor;
    I2C2TRN = 0x31u;
}

/* 0x1F90: clear IOUT/VOUT programming state while in LLC programming mode. */
static void pmbusReadIoutCommand(void)
{
    if (rxBufIndex == 3u) {
        if (rxPacketBuf[1] == 1u) {
            goto ok;
        }
    } else if (rxBufIndex < 4u) {
        if (rxBufIndex != 0u) {
            txCtrlWord |= (1u << 0);
        }
        goto ok;
    }
    txCtrlWord |= (1u << 1);

ok:
    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }
    txCtrlWord |= (1u << 1);
    if ((llcStatus & 0x0100u) != 0u) {
        *((volatile uint16_t *)ptrTable1A10[0]) = 0;
        *((volatile uint16_t *)ptrTable1A10[1]) = 0;
        flash_read_buf_15E6[0x10] = 0;
        droopEnableFlags &= (uint16_t)~0x0008u;
        flashCmdFlags |= (1u << 6);
        I2C2TRN = 0x31u;
    } else {
        I2C2TRN = 0xAAu;
    }
}

/* 0x1D12: set or clear LLC programming mode with the "PH" command prefix. */
static void pmbusSetOperationMode(void)
{
    if (rxBufIndex != 5u) {
        if (rxBufIndex < 6u) {
            if (rxBufIndex != 0u) {
                txCtrlWord |= (1u << 0);
            }
        } else {
            txCtrlWord |= (1u << 1);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) == 0u) {
        return;
    }

    txCtrlWord |= (1u << 1);
    if (rxPacketBuf[1] == 'P' && rxPacketBuf[2] == 'H') {
        if (rxPacketBuf[3] == 1u) {
            llcStatus |= 0x0100u;
            I2C2TRN = 0x31u;
            return;
        }
        if (rxPacketBuf[3] == 0u) {
            llcStatus &= (uint16_t)~0x0100u;
            I2C2TRN = 0x31u;
            return;
        }
    }

    llcStatus &= (uint16_t)~0x0100u;
    I2C2TRN = 0xCEu;
}

/* ============================================================================
 *   (0x24B2 – 0x2504)
 *
 * Called from the T1 timer ISR to maintain a 2500-tick (2.5 s) keep-alive
 * counter for the I2C2 slave.  Also accumulates the i2cTxCounter read counter
 * and runs the 32-bit receive-data accumulator at i2cAccum.
 *
 * Flow:
 *   1. If flashCmdFlags bit0 set -> skip (I2C busy with UART command).
 *   2. Check pwmRunning bit1 (I2C2 active):
 *      - If clear -> reset counters and load current i2cTxCounter into
 *                   i2cAccum, then return.
 *      - If set   -> increment 32-bit counter i2cTickCnt.
 *   3. If counter reaches 0x09C3 (2499 ticks):
 *      a. Check rxEventFlags: if non-zero -> skip (transaction in progress).
 *      b. Reset counter to 0x09C3.
 *      c. Increment i2cTxCounter.  If it wraps to 0xFFFF -> skip.
 *      d. Read adcLiveA2 (ptrTable19D0[0] target) and add it into i2cAccum.
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
        i2cTickCnt = 0;
        i2cAccum = 0;
        /* Store current i2cTxCounter value (no-op: same variable) */
        return;
    }

    /* I2C2 active: increment 32-bit tick counter */
    i2cTickCnt++;

    /* Check if counter has reached 0x09C3 (2499) */
    if (i2cTickCnt > 0x09C3u) {
        return;   /* BRA LEU 0x2504: still under threshold */
    }

    /* Counter reached threshold */
    if (rxEventFlags != 0) {
        return;   /* Transaction in progress — skip */
    }

    /* Reset counter to 0x09C3 */
    i2cTickCnt = 0x09C3u;

    /* Increment i2cTxCounter; bail if it would wrap to 0 (== 0xFFFF before inc) */
    uint16_t s = i2cTxCounter;
    if ((uint16_t)(s + 1u) == 0) {
        return;   /* would overflow */
    }
    s++;
    i2cTxCounter = s;

    /* Add adcLiveA2 (was ptrTable19D0[0] target = 0x1BE4) into the 32-bit accumulator */
    i2cAccum += (uint32_t)adcLiveA2;
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
 *   0x1652  pmbusMultiByteTxSetup       (cmd 0x00)
 *   0x27CE  pmbusCommandDispatcher      (cmd 0x01)
 *   0x1F54  pmbusReadVoutCommand        (cmd 0x11)
 *   0x1F90  pmbusReadIoutCommand        (cmd 0x12)
 *   0x289A  pmbusBlockWriteModeSelect   (cmd 0x20)
 *   0x1F18  pmbusReadConfigCommand      (cmd 0x21)
 *   0x1982  pmbusFlashReadWindow        (cmd 0x22)
 *   0x1A12  pmbusFlashWriteWindow       (cmd 0x23)
 *   0x1D12  pmbusSetOperationMode       (cmd 0x24)
 *   0x1B6A  pmbusProgramBlock           (cmd 0x3A)
 *   0x1C48  pmbusReadbackBlock          (cmd 0x3B)
 *   0x1B2A  pmbusProgramHeader          (cmd 0x3C)
 *   0x1C94  pmbusStartFlashWrite        (cmd 0x3D)
 *   0x1CD4  pmbusReadDeviceInfo         (cmd 0xE0)
 *   0x1D5E  pmbusEnterProgramming       (cmd 0xE2)
 *   0x1DC8  pmbusStoreProgrammingData   (cmd 0xE3)
 *   0x1A92  pmbusOperationUnlock        (cmd 0xFF)
 *
 * The txCtrlWord bit0 controls the "parity" selection (odd/even byte index).
 * ============================================================================ */

static void pmbusCommandDispatcher(void)
{
    uint16_t idx = rxBufIndex;   /* 0x1E40 */

    if (idx >= 5u && idx <= 0x1Du) {
        uint16_t parity = 1u;
        uint16_t probe = 2u;

        /*
         * 0x27DA..0x27EA: valid block lengths are 5, 8, 11, ... 29.
         * txCtrlWord bit0 records whether the packet ended on a valid tuple
         * boundary before the command table is scanned.
         */
        do {
            probe = (uint16_t)(probe + 3u);
            if (idx == probe) {
                parity = 0;
            }
        } while (probe != 0x1Du);

        txCtrlWord = (txCtrlWord & (uint16_t)~0x0001u) | (parity & 1u);

        if (parity == 0u) {
            for (uint16_t i = 0; i < sizeof(pmbusBlockOffsets); i++) {
                uint8_t entry = pmbusBlockOffsets[i];
                uint8_t cmd = rxPacketBuf[entry];
                uint16_t invalid = pmbusCommandCodeSupported(cmd) ? 0u : 1u;

                txCtrlWord = (txCtrlWord & (uint16_t)~0x0002u) |
                             (uint16_t)(invalid << 1);
                if ((uint16_t)(entry + 4u) >= idx || invalid != 0u) {
                    break;
                }
            }
        }
    } else {
        if (idx > 0x1Du) {
            txCtrlWord |= (1u << 1);
        } else if (idx != 0u) {
            txCtrlWord |= (1u << 0);
        }
    }

    if (pmbusEncodeResponse(txCtrlWord) != 0u) {
        for (uint16_t i = 0; i < sizeof(pmbusBlockOffsets); i++) {
            uint8_t entry = pmbusBlockOffsets[i];
            if (pmbusBlockCommandInvalid(entry) != 0u) {
                txCtrlWord |= (1u << 1);
            }
            if ((uint16_t)(entry + 4u) >= idx) {
                break;
            }
        }

        if ((txCtrlWord & (1u << 1)) == 0u) {
            for (uint16_t i = 0; i < sizeof(pmbusBlockOffsets); i += 2u) {
                uint8_t entry = pmbusBlockOffsets[i];
                uint16_t word_val = (uint16_t)rxPacketBuf[entry + 1u]
                                  | ((uint16_t)rxPacketBuf[entry + 2u] << 8);
                /*
                 * 0x2880 uses ZE [W9++], which consumes a word from the ROM
                 * table, so this pass applies every other byte entry.
                 */
                pmbusWriteCommand(rxPacketBuf[entry], word_val, rxPacketBuf[entry + 1u]);
                if ((uint16_t)(entry + 4u) >= rxBufIndex) {
                    break;
                }
            }
            I2C2TRN = 0x31u;
        } else {
            I2C2TRN = 0xCEu;
        }

        txCtrlWord |= (1u << 1);
    }
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

    if (idx != 0u) {
        txChecksumAccum = 0;
        txCtrlWord = 0;

        uint16_t cs_bad = pmbusChecksumVerify((uint8_t *)rxPacketBuf, idx);
        txCtrlWord = (txCtrlWord & (uint16_t)~0x0004u) |
                     (uint16_t)((cs_bad & 1u) << 2);
    }

    if ((txCtrlWord & (1u << 2)) == 0u) {
        switch (rxPacketBuf[0]) {
            case 0x00u: pmbusMultiByteTxSetup(); goto done;
            case 0x01u: pmbusCommandDispatcher(); goto done;
            case 0x11u: pmbusReadVoutCommand(); goto done;
            case 0x12u: pmbusReadIoutCommand(); goto done;
            case 0x20u: pmbusBlockWriteModeSelect(); goto done;
            case 0x21u: pmbusReadConfigCommand(); goto done;
            case 0x22u: pmbusFlashReadWindow(); goto done;
            case 0x23u: pmbusFlashWriteWindow(); goto done;
            case 0x24u: pmbusSetOperationMode(); goto done;
            case 0x3Au: pmbusProgramBlock(); goto done;
            case 0x3Bu: pmbusReadbackBlock(); goto done;
            case 0x3Cu: pmbusProgramHeader(); goto done;
            case 0x3Du: pmbusStartFlashWrite(); goto done;
            case 0xE0u: pmbusReadDeviceInfo(); goto done;
            case 0xE2u: pmbusEnterProgramming(); goto done;
            case 0xE3u: pmbusStoreProgrammingData(); goto done;
            case 0xFFu: pmbusOperationUnlock(); goto done;
            default:
                I2C2TRN = 0x55u;
                break;
        }
    } else {
        I2C2TRN = 0xFAu;
    }

done:
    rxBufIndex = 0;
    rxPendFlag = (uint16_t)(rxPendFlag + 1u);
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
        (void)I2C2RCV;
        pmbusPacketValidate();   /* 0x28D6 */
    }

    /* Release SCL when no flash/I2C command flag is pending */
    if (flashCmdFlags == 0) {
        I2C2CON |= (1u << 12);
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
extern void statusByteSync249c(void);         /* 0x249C - status byte sync */
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
        statusByteSync249c();          /* 0x249C */
        i2cStatusFlagSync();           /* 0x255A */
        unknownSubroutine2594();       /* 0x2594 */
    }
}
