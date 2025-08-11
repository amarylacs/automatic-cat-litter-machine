#ifndef STEPPER_H
#define STEPPER_H

#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>

#define F_CPU 16000000UL

#define STEPS_PER_REV = 200;

void stepper_init();
void stepper_set(float angle);
void stepper_direction(bool direction);

#endif
