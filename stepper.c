#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>

#define F_CPU 16000000UL

#define STEPS_PER_REV 200;

void stepper_init() { //STEP to 3, DIR to 2, direction: true = clkwise
    DDRD |= (1 << PD2)|(1 << PD3);
}

void stepper_direction(bool direction) {
    if (direction == true)
        PORTD |= (1 << PD2);
    if (direction == false)
        PORTD &= ~(1 << PD2);
}

static uint16_t map(float angle) {
    float result = (angle / 360.0) * 200;
    return (uint16_t)result;    
}

void stepper_set(float angle) {
    uint16_t step = map(angle);

    for (uint8_t i = 0; i < step; i++) {
        PORTD |= (1 << PD3);
        _delay_us(1000);
        PORTD &= ~(1 << PD3);
        _delay_us(1000);
    }
}
