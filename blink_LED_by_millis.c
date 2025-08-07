#include <avr/io.h>
#include <avr/interrupt.h>


void timer0_init() {

TCCR0A |= (0b10 << WGM00);
TCCR0B |= (0 << WGM02) | (0b011 << CS00); 

OCR0A = 249;

TIMSK0 = 1 << OCIE0A;
TCNT0 = 0;

} 

volatile millis_counter = 0;

ISR(TIMER0_COMPA_vect) {
    millis_counter++;
}

uint32_t millis(void) {
    uint32_t ms;
    cli();
    ms = millis_counter;
    sei();
    return ms;
}

void main(void) {

    timer0_init();
    DDRD |= (1 << 4);

    sei();

    uint32_t first_time = 0;
    uint32_t interval = 1000; 

    while(1) {
        uint32_t second_time = millis();

        if ((second_time - first_time) >= interval) {
            first_time = second_time;
            PORTD ^= (1 << 4);
        }

    }
}
