#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "digital.h"
#include <util/delay.h>

int main() {
    digital_pin_mode(9, OUTPUT);
    while (1) {
        if (digital_read(4) == true) {
            digital_write(3, true);
        }
        else {
            digital_write(3, false);
        }
    }
    return 0;
}
