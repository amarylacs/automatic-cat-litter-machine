#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "digital.h"
#include <util/delay.h>

int main() {
    digital_pin_mode(9, OUTPUT);
    while (1) {
        digital_write(9, true);
    }
    return 0;
}

//this is main.c
