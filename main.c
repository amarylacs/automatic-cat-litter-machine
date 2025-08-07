#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "functions.h"
#include <util/delay.h>

int main() {

    TCCR0B |= (1 << CS01) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
    sei();

    while (1) {
        digital_write(9, true);
        PWM_init();
        uint32_t val;
        val = micros();
    }
}#include "digital.h"
#include <avr/interrupt.h>

volatile unsigned long timer0_millis = 0;

unsigned long micros() {
  unsigned long m;
  uint8_t oldSREG = SREG;
  cli(); 
  m = timer0_millis;
  if (TCNT0 != 0) {
    m = (m << 8) + TCNT0;
  }
  else if (TCNT1L != 0) {
    m = (m << 8) + TCNT1L;
  }
  SREG = oldSREG;
  return m * 4;
}
 
ISR(TIMER0_OVF_vect) {
  timer0_millis++;
}

int main() {
  TCCR0B |= (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
  sei();

    while (1) {
        uint32_t val;
        val = micros();
    }
    return 0;
}



//this is main.c
