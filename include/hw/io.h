#include <stdint.h>

void hw_setup_leds(uint8_t *pins, uint8_t len);
void hw_setup_inputs(uint8_t *pins, uint8_t len);
void hw_blink_led(uint8_t i);
int hw_get_adc_input(uint8_t i);
int hw_output_set(uint8_t i, uint8_t state);
void hw_mssleep(unsigned long d);
void hw_ussleep(unsigned long d);
void hw_exit();

void hw_uart_init(void);
void hw_uart_flush(void);
void hw_uart_putc(char c);
void hw_uart_puts(char *s);
char hw_uart_getc();

void hw_spi_init(uint8_t enable_interrupts);
void hw_spi_select(uint8_t select);
unsigned char hw_spi_xmit(unsigned char data);

void hw_i2c_init();
void hw_i2c_write(uint16_t addr, uint8_t data);
