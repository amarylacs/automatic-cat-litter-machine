#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "functions.h"
#include <util/delay.h>

int main() {
    digital_pin_mode(9, OUTPUT);
    while (1) {
        digital_write(9, true);
        PWM_init();
    }
    return 0;
}

//this is main.c
