#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "Ultrasonic.c"

float ultrasonic_distance(void);

#endif
