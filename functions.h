#include "functions.c"

volatile uint8_t *digital_pin_to_port(uint8_t pin);
int digital_port_offset(volatile uint8_t * const port, digital_register_type_t type);
int digital_pin_to_bit(uint8_t pin);
bool digital_pin_mode(uint8_t pin, digital_mode_t mode);
bool digital_write(uint8_t pin, bool high);
bool digital_read(uint8_t pin);
bool analog_write(uint8_t pin, int val);
bool analog_read(uint8_t pin);