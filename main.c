#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "Ultrasonic.h"
#include <util/delay.h>
#include "stepper.h"
#include "servo.h"
#include "digital.h"

enum {
  IDLE,
  CAT,
  REWARD,
  TURN
};

int main(){
  stepper_init();
  uint8_t current_state = IDLE;
  uint8_t new_state;

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  while(1){
    switch(current_state) {
      case IDLE: {
        new_state = STATE_IDLE();
        break;
      }
      case CAT: {
        new_state = STATE_CAT();
        _delay_ms(1000);

        break;
      }
      case REWARD: {
        new_state = STATE_REWARD();
        _delay_ms(1000);

        break;
      }
      case TURN: { 
        new_state = STATE_TURN();
        _delay_ms(1000);

        break;
      }
    }
    current_state = new_state;
  }
}

int STATE_IDLE(void) {
  float distance = ultrasonic_distance();
  digitalWrite(12,1);
  digitalWrite(13,0);
  digitalWrite(10,0);
  digitalWrite(11,0);
  if (distance > 0 && distance < 0.25) {
    return CAT;
  }
  return IDLE;
}

int STATE_CAT(void) {
  float distance = ultrasonic_distance();
  digitalWrite(12,0);
  digitalWrite(13,1);
  digitalWrite(10,0);
  digitalWrite(11,0);

  if (distance > 0 && distance < 0.25) {
    return CAT;
  }
  else {
    return REWARD;
  }
}

int STATE_REWARD(void) {
  digitalWrite(12,0);
  digitalWrite(13,0);
  digitalWrite(10,1);
  digitalWrite(11,0);

  servo_init();
  servo_set(4000);
  _delay_ms(1000);
  servo_set(2000);
  _delay_ms(1000);
  return TURN;
}

int STATE_TURN(void) {

  digitalWrite(12,0);
  digitalWrite(13,0);
  digitalWrite(10,0);
  digitalWrite(11,1);

  stepper_direction(false);
  stepper_set(90);
  _delay_ms(1500);
  stepper_set(110);
  _delay_ms(1500);

  stepper_direction(true);
  stepper_set(200);

  return IDLE;
}
