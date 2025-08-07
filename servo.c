#include <avr/io.h>


void servo_init() {
//OC1A - PB1: set pin 9 to output
    DDRB |= 1 << 1;

    TCCR1A |= (0b10 << COM1A0)| (0b10 << WGM10);
    TCCR1B |= (0b11 << WGM12) | (0b010 << CS10);

    ICR1 = 40000;
}

void servo_set(uint16_t value) {
    OCR1A = value;
}
