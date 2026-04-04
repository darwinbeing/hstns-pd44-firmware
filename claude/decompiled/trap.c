#include <xc.h>
#include <stdint.h>

/*
 * Trap diagnostics for simulator/bring-up.
 * These symbols are intentionally global so they can be watched in MPLAB.
 */
volatile uint16_t trapLastCause = 0;
volatile uint16_t trapLastIntcon1 = 0;
volatile uint16_t trapHitCount = 0;
volatile uint16_t trapPcHigh = 0;
volatile uint16_t trapPcLow = 0;

enum {
    TRAP_CAUSE_OSCFAIL = 1,
    TRAP_CAUSE_ADDRERR = 2,
    TRAP_CAUSE_STKERR = 3,
    TRAP_CAUSE_MATHERR = 4,
    TRAP_CAUSE_DEFAULT = 0xFFFF
};

static void trapRecordAndHalt(uint16_t cause)
{
    trapLastCause = cause;
    trapLastIntcon1 = INTCON1;
    trapHitCount++;
    trapPcHigh = TBLPAG;
    trapPcLow = 0;

    /* Clear trap latches (INTCON1<3:0>) to avoid immediate retrigger storm. */
    INTCON1 &= (uint16_t)~0x000Fu;

    /* Stop PWM timebase if enabled. */
    PTCONbits.PTEN = 0;

    for (;;) {
        Nop();
    }
}

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail(void)
{
    trapRecordAndHalt(TRAP_CAUSE_OSCFAIL);
}

void __attribute__((interrupt, no_auto_psv)) _AddressError(void)
{
    trapRecordAndHalt(TRAP_CAUSE_ADDRERR);
}

void __attribute__((interrupt, no_auto_psv)) _StackError(void)
{
    trapRecordAndHalt(TRAP_CAUSE_STKERR);
}

void __attribute__((interrupt, no_auto_psv)) _MathError(void)
{
    trapRecordAndHalt(TRAP_CAUSE_MATHERR);
}

void __attribute__((interrupt, no_auto_psv)) _DefaultInterrupt(void)
{
    trapRecordAndHalt(TRAP_CAUSE_DEFAULT);
}
