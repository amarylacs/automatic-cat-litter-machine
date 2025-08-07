#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>

void servo_init();
void servo_set(uint16_t value);
//value must be 2000-5000 (0 degree to 180 degree). I can't map this.

#endif
