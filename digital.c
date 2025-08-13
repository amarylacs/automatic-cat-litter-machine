#include "digital.h"
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <stddef.h>

volatile uint8_t *digitalPinToPort(uint8_t pin) {
    if (pin >= 0 && pin <= 7) {
        return &PORTD;
    }
    if (pin >= 8 && pin <= 13) {
        return &PORTB;
    }
    if (pin >= 14 && pin <= 19) {
        return &PORTC;
    }
    return NULL;
}
typedef enum {
    PORTx = 0,
    DDRx = -1,
    PINx = -2
} digitalRegisterType_t;

volatile uint8_t *digitalPortOffset(volatile uint8_t *const port, digitalRegisterType_t type) {
    return (port + type);
}

int digitalPinToBitMask (uint8_t pin) {
    if (pin >= 0 && pin <= 7) {
        return (1 << pin);
    }
    if (pin >= 8 && pin <= 13) {
        return (1 << (pin - 8));
    }
    if (pin >= 14 && pin <= 19) {
        return (1 << (pin - 14));
    }
    else {
        return -1;
    }
}

typedef enum {OUTPUT, INPUT, INPUT_PULLUP} digitalMode_t;

bool pinMode(uint8_t pin, digitalMode_t mode) {
    //DDRx = DDRx|(1 << x)
    volatile uint8_t *reg_ddr, *reg_port;
    int8_t bitMask;
    
    reg_ddr = digitalPortOffset(digitalPinToPort(pin), DDRx);
    reg_port = digitalPortOffset(digitalPinToPort(pin), PORTx);
    bitMask = digitalPinToBitMask(pin);
    
    if (mode == INPUT) {
        *reg_ddr &= ~bitMask;
        return true;
    }
    else if (mode == OUTPUT) {
        *reg_ddr |= bitMask;
        return true;
    }
    else if (mode == INPUT_PULLUP) {
        *reg_ddr &= ~bitMask;
        *reg_port |= bitMask;
        return true;
    }
    return false;
}
bool digitalWrite(uint8_t pin, bool high) {
//PORTx
    volatile uint8_t *reg;
    int8_t bitMask;
    reg = digitalPortOffset(digitalPinToPort(pin), PORTx);
    bitMask = digitalPinToBitMask(pin);

    if (high) {
        *reg |= bitMask;
        return true;
    }
    else {
        *reg &= ~bitMask;
        return true;
    }
    return false;
}

bool digitalRead(uint8_t pin) {
    volatile uint8_t *reg;
    int8_t bitMask;
    reg = digitalPortOffset(digitalPinToPort(pin), PINx);
    bitMask = digitalPinToBitMask(pin);
    
    return (*reg & bitMask) ? 1 : 0;
}
