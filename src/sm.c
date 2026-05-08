#include <xc.h>
#include "define.h"

/* ============================================================================
 * System state machine states
 * ============================================================================ */
#define ST_IDLE      0
#define ST_STARTUP   1
#define ST_ACTIVE    2
#define ST_FAULT     3
#define ST_HOLDOFF   5
#define ST_RELAY     6

extern s16 pdc1;
extern s16 pdc2;
extern s16 pdc3;
extern void pwm_force_off(void);

/* State machine & control flags */
volatile uint16_t systemState;
volatile uint16_t statusFlags;
volatile uint16_t protectionStatus;
volatile uint16_t runtimeFlags;
volatile uint16_t thermalFlags;
volatile uint16_t droopMode;
volatile uint16_t systemFlags;
volatile uint16_t auxFlags;
volatile uint16_t currentLimitFlags;
volatile uint16_t controlStatus;

volatile uint16_t pwmRunRequest;       /* DAT_ram_1bf0 */
volatile uint16_t pwmRunning;          /* DAT_ram_1bf2 */
volatile uint16_t pmbusAlertFlags;     /* DAT_ram_192a */
volatile uint16_t faultResetTimer;     /* DAT_ram_1e0e */
volatile uint16_t fanI2cAddr;          /* DAT_ram_1e08 */

/* Shared shutdown tail from faultHandler 0x3386 and OVP 0x444C. */
void llcFaultShutdown(uint16_t control_flags)
{
    controlStatus |= control_flags;
    pwmRunRequest |= (1u << 0);
    pwmRunning &= ~(1u << 0);
    pmbusAlertFlags |= (1u << 0);

    pdc1 = 0;
    pdc2 = 0;
    pdc3 = 0;
    PDC1 = 0;
    PDC2 = 0;
    PDC3 = 0;

    pwm_force_off();
    systemState = ST_FAULT;
}

void state0Idle(void)
{
    pwm_force_off();
    pwmRunning &= ~(1u << 0);
}

void state1Startup(void)
{
    statusFlags |= STATUS_FLAG_STARTUP;
    systemState = ST_ACTIVE;
}

void state2Active(void)
{
    if (controlStatus & 0x0001u)
        llcFaultShutdown(0);
}

void state3Fault(void)
{
    pwm_force_off();
    pwmRunning &= ~(1u << 0);
}

void state5Handler(void)
{
    state3Fault();
}

void state6Handler(void)
{
    state3Fault();
}

void checkFanControl(void)
{
    uint16_t addr_bits;

    /* 0x4FBC: update fan controller I2C address from hardware straps. */
    addr_bits = fanI2cAddr;
    addr_bits &= ~(1u << 1);
    addr_bits |= ((uint16_t)PORTCbits.RC14 << 1);
    addr_bits &= ~(1u << 2);
    addr_bits |= ((uint16_t)PORTCbits.RC13 << 2);
    addr_bits &= ~(1u << 3);
    addr_bits |= ((uint16_t)PORTDbits.RD10 << 3);
    fanI2cAddr = addr_bits;
    I2C2ADD = ((addr_bits & 0x000Eu) | 0x00B0u) >> 1;
}

void mainStateDispatch(void)
{
    /* Check T4 timer trigger flag (0x87 bit3 -> IFS1 bit11). */
    if(!IFS1bits.T4IF)
        return;
    
    /* ---- State dispatch ---- */
    switch (systemState) {
        case ST_IDLE:    state0Idle();      break;  /* 0x4C04 */
        case ST_STARTUP: state1Startup();   break;  /* 0x4C72 */
        case ST_ACTIVE:  state2Active();    break;  /* 0x518C */
        case ST_FAULT:   state3Fault();     break;  /* 0x4CA6 */
        case ST_HOLDOFF: state5Handler();   break;  /* 0x4D3A */
        case ST_RELAY:   state6Handler();   break;  /* 0x4D86 */
        default: break;
    }

    checkFanControl();        /* 0x4FBC */
    ClrWdt();
    IFS1bits.T4IF = 0;     /* clear T4 trigger flag */
}