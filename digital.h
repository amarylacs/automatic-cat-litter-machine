#ifndef DIGITAL_H
#define DIGITAL_H

#include "digital.c"
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <stddef.h>


// //typedef enum {OUTPUT,INPUT,INPUT_PULLUP} digitalMode_t;

// typedef enum {
//     PORTx = 0,   
//     DDRx = -1,
//     PINx = -2
// } digitalRegisterType_t;

volatile uint8_t *digitalPinToPort(uint8_t pin);
volatile uint8_t *digitalPortOffset(volatile uint8_t *const port, digitalRegisterType_t type);
int digitalPinToBitMask (uint8_t pin);

bool pinMode(uint8_t pin, digitalMode_t mode);
bool digitalWrite(uint8_t pin, bool high);
bool digitalRead(uint8_t pin);

#endif
