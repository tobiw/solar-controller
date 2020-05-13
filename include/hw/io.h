#include <stdint.h>

void hw_setup_leds();
void hw_setup_inputs();
void hw_blink_led(uint8_t i);
int hw_get_adc_input(uint8_t i);
void hw_mssleep(unsigned long d);
void hw_spi_init(uint8_t enable_interrupts);
unsigned char hw_spi_xmit(unsigned char data);
int hw_spi_xmit_buf(unsigned char *data, int len);
