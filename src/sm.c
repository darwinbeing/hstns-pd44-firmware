#include <xc.h>

/* ============================================================================
 * System state machine states
 * ============================================================================ */
#define ST_IDLE      0
#define ST_STARTUP   1
#define ST_ACTIVE    2
#define ST_FAULT     3
#define ST_HOLDOFF   5
#define ST_RELAY     6

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

void state0Idle(void) {}
void state1Startup(void) {}
void state2Active(void) {}
void state3Fault(void) {}
void state5Handler(void) {}
void state6Handler(void) {}

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

    ClrWdt();
    IFS1bits.T4IF = 0;     /* clear T4 trigger flag */
}