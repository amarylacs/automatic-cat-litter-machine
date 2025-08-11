#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stddef.h>
#include "digital.h"
#include "millis.h"
#include <util/delay.h>
 
static const uint8_t ECHO_PIN = 8;
static const uint8_t TRIGGER_PIN = 4;
static const float STP_SOUND_METERS_PER_US = 343e-6f;
static const uint8_t PULSE_LENGTH = 10;
static const uint8_t DELAY_MS = 10;
static const uint8_t LED_PIN = 3;
static const uint8_t LED2_PIN = 5;
 
static volatile bool pulse_ready;
static volatile uint16_t pulse_time;
 
volatile unsigned long timer0_millis = 0;
 
ISR(TIMER0_OVF_vect) {
  timer0_millis++;
}

ISR(PCINT0_vect) { //triggered when PCMSK0 |= 1; or when pin 9 is set to high (?)
  static uint16_t pulse_start;

  if (pulse_ready == true)
      return;
  if (digital_read(ECHO_PIN) == true) {
      pulse_start = millis();

      return;
  }
  const uint16_t pulse_end = millis();

  pulse_time = pulse_end - pulse_start;
  pulse_ready = true;
 
}

ISR(PCINT1_vect) {
  PORTB ^= (1<<PB1);
}
 
 
int main() {
  digital_pin_mode(ECHO_PIN, INPUT);
  digital_pin_mode(TRIGGER_PIN, OUTPUT);
  digital_pin_mode(LED_PIN, OUTPUT);
  digital_pin_mode(LED2_PIN, OUTPUT);

  digital_write(LED_PIN, false);
  digital_write(TRIGGER_PIN, false);

  timer0_init();

  PCMSK0 |= (1<<PCINT0);
  PCICR |= (1<<PCIE0);
  PCMSK0 |= (1<<PCINT1);
  PCICR |= (1<<PCIE1);

  sei();


  while(1) {
    pulse_ready = false;
    digital_write(TRIGGER_PIN, true);
    _delay_ms(PULSE_LENGTH);
    digital_write(TRIGGER_PIN, false);

    while (!pulse_ready)
      continue;

    const float distance_m = STP_SOUND_METERS_PER_US * (pulse_time / 2);

    _delay_ms(100);

    if (distance_m < 0.5) {
      digital_write(LED_PIN, true);
      _delay_ms(1000);

    }
    else {
      digital_write(LED_PIN, false);
      _delay_ms(1000);
    }


  }
}