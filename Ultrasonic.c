#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "digital.h"


#define ECHO_PIN     PB0  // Digital pin 8
#define TRIGGER_PIN  PD4  // Digital pin 4
#define LED_PIN      PD3  // Digital pin 3
#define LED2_PIN     PD5  // Digital pin 5

#define PULSE_TIMEOUT 60000  // Timeout in timer1 ticks (~30ms at 0.5us per tick)
#define SOUND_SPEED   343.0  // Speed of sound in m/s

volatile uint16_t pulse_start = 0;
volatile uint16_t pulse_time = 0;
volatile bool pulse_ready = false;
volatile uint8_t prev_pinb = 0;

void timer1_init() {
    TCCR1A = 0;
    TCCR1B = (1 << CS11);  // Prescaler = 8, 1 tick = 0.5us @16MHz
    TCNT1 = 0;
}

ISR(PCINT0_vect) {
    uint8_t current = PINB;

    if ((current ^ prev_pinb) & (1 << ECHO_PIN)) {
        if (pulse_ready) {
            prev_pinb = current;
            return;
        }

        if (current & (1 << ECHO_PIN)) {
            // Rising edge — echo started
            pulse_start = TCNT1;
        } else {
            // Falling edge — echo ended
            pulse_time = TCNT1 - pulse_start;
            pulse_ready = true;

            PORTD ^= (1 << LED2_PIN);  // Toggle debug LED
        }
    }

    prev_pinb = current;
}

void pin_setup() {
    // Trigger pin as output
    DDRD |= (1 << TRIGGER_PIN);
    // LED pins as output
    DDRD |= (1 << LED_PIN) | (1 << LED2_PIN);
    // Echo pin as input with pull-up
    DDRB &= ~(1 << ECHO_PIN);
    PORTB |= (1 << ECHO_PIN);

    // Initialize previous PINB state
    prev_pinb = PINB;

    // Enable PCINT0 on PB0 (digital pin 8)
    PCICR |= (1 << PCIE0);        // Enable pin change interrupt group 0
    PCMSK0 |= (1 << PCINT0);      // Enable pin change on PB0
}

void send_ultrasonic_pulse() {
    PORTD |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIGGER_PIN);
}

int main(void) {
    cli();              // Disable global interrupts during setup
    pin_setup();
    timer1_init();
    sei();              // Enable global interrupts

    while (1) {
        pulse_ready = false;
        send_ultrasonic_pulse();

        uint16_t start = TCNT1;
        while (!pulse_ready && ((uint16_t)(TCNT1 - start) < PULSE_TIMEOUT)) {
            // Wait with timeout (~30ms)
        }

        float distance_m = -1.0;

        if (pulse_ready) {
            // Convert ticks to time (0.5us per tick)
            float time_us = pulse_time * 0.5;
            distance_m = (time_us / 1000000.0) * (SOUND_SPEED / 2.0);
        }

        // Display logic
        if (distance_m > 0 && distance_m < 0.5) {
            PORTD |= (1 << LED_PIN);   // Turn on LED
        } else {
            PORTD &= ~(1 << LED_PIN);  // Turn off LED
        }

        _delay_ms(200);  // Wait before next measurement
    }
}
