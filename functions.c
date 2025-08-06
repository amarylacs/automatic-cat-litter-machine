#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
typedef enum {
    PORT = 0,
    DDR = -1,
    PIN = -2,
} digital_register_type_t;

typedef enum {
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
} digital_mode_t;

volatile uint8_t *digital_pin_to_port(uint8_t pin) {
    if ((pin <= 7) && (pin >= 1)) {
        return &PORTD;
    }
    if ((pin >= 8) && (pin <= 13)) {
        return &PORTC;
    }
    if ((pin >= 14) && (pin <= 18)) {
        return &PORTB;
    }
    return NULL; 
}


int digital_port_offset(volatile uint8_t * const port, digital_register_type_t type) {
    return (port + type);
}

int digital_pin_to_bit(uint8_t pin) {
    if ((pin <= 7) && (pin >= 1)) {
        return (1<<(pin));
    }
    if ((pin >= 8) && (pin <= 13)) {
        return (1<<(pin-8));
    }
    if ((pin >= 14) && (pin <= 18)) {
        return (1<<(pin-14));
    }
    return -1; 
}

bool digital_pin_mode(uint8_t pin, digital_mode_t mode) {
    volatile uint8_t *port;
    uint8_t bit = digital_pin_to_bit(pin); //bit, such as 0000 0001
    port = digital_port_offset(digital_pin_to_port(pin), DDR); //& of PORTC, B, D, etc. + offset (DDRB)
    if (mode == INPUT) {
        *port &= ~(bit);
        return true;
    }
    if (mode == OUTPUT) {
        *port |= bit;
        return true;
    }
    if (mode == INPUT_PULLUP) {
        *port &= ~(bit);
        return true;
    }
    return false;
}

bool digital_write(uint8_t pin, bool high) {
    volatile uint8_t *port;
    uint8_t bit = digital_pin_to_bit(pin);
    port = digital_port_offset(digital_pin_to_port(pin), PORT);
    if (high) {
        *port |= bit;
        return true;
    }
    else {
        *port = ~(bit);
        return false;
    }
    return false;
    
}

bool digital_read(uint8_t pin) {
    volatile uint8_t *port;
    uint8_t bit = digital_pin_to_bit(pin);
    port = digital_port_offset(digital_pin_to_port(pin), PIN);
    uint8_t reading = *port&(bit) ? 1 : 0;
    if (reading == 1) {
        return true;
    }
    else {
        return false;
    }
}

void PWM_init() {
    unsigned char duty;
    TCCR0 = (1<<WGM00) | (1<<WGM10) | (1<<COM01) | (1<<CS00);
    DDRB |= (1<<PB3);
    while (1) {
        for(duty = 0; duty<255; duty++) {
            OCR0 = duty;
            _delay_ms(8);
        }
        for(duty= 255; duty > 1; duty--) {
            OCR0 = duty;
            _delay_ms(8);
        }
    }
}

bool analog_write(uint8_t pin, int val) {
    volatile uint8_t *port;
    uint8_t bit = digital_pin_to_bit(pin);
    port = digital_port_offset(digital_pin_to_port(pin), PORT);
    
    uint8_t dutycycle = val/255;

    
}