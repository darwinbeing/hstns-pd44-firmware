#ifndef XC_H_UNIT_TEST_STUB
#define XC_H_UNIT_TEST_STUB

#include <stdint.h>

#define Nop() do { } while (0)

extern uint16_t ADCBUF0;
extern uint16_t ADCBUF1;
extern uint16_t ADCBUF2;
extern uint16_t ADCBUF3;
extern uint16_t ADCBUF4;
extern uint16_t ADCBUF5;
extern uint16_t ADCBUF10;
extern uint16_t ADCBUF11;
extern uint16_t ADCBUF12;
extern uint16_t ADCBUF13;
extern uint16_t ADCBUF14;
extern uint16_t ADCBUF15;

extern uint16_t PDC5;
extern uint16_t PORTD;

typedef struct {
    uint16_t RD0;
    uint16_t RD6;
    uint16_t RD11;
} portd_bits_t;

extern volatile portd_bits_t PORTDbits;

typedef struct {
    uint16_t LATG9;
} latg_bits_t;

extern volatile latg_bits_t LATGbits;

#endif /* XC_H_UNIT_TEST_STUB */
