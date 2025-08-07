#include "digital.h"
#include <avr/interrupt.h>

volatile unsigned timer0_millis = 0;

unsigned micros() {
  unsigned m;
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
