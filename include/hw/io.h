#include <stdint.h>

void hw_setup_leds();
void hw_setup_inputs();
void hw_blink_led(uint8_t i);
int hw_get_adc_input(uint8_t i);
void hw_mssleep(unsigned long d);
