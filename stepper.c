#include "stepper.h"
#include "digital.h"
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
    float result = (angle / 360.0) * STEPS_PER_REV;
    return (uint16_t)result;    
}

static void stepper_move_one_step(void) {
    PORTD |= (1 << PD3);
    _delay_us(2000);
    PORTD &= ~(1 << PD3);
    _delay_us(2000);
}

void stepper_set(float angle) {
    uint16_t target_steps = map(angle);
    float distance = ultrasonic_distance();
    
    for (uint16_t i = 0; i < target_steps; i++) {
        while(distance > 0 && distance < 0.3) {
            distance = ultrasonic_distance();
            digitalWrite(13, 1);
            _delay_ms(10);
        }
        stepper_move_one_step();
        digitalWrite(13,0);
        distance = ultrasonic_distance();
    }
}
