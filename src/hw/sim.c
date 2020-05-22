#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sim/sim.h"
#include "config.h"

extern sim_state_t g_sim_state;

void hw_setup_leds(uint8_t *pins, uint8_t len)
{
    memset(g_sim_state.leds, 0, sizeof(g_sim_state.leds));
}

void hw_setup_inputs(uint8_t *pins, uint8_t len)
{
    g_sim_state.temp_sensors[0] = 0;  // collector
    g_sim_state.temp_sensors[1] = 0;  // tank bottom
    g_sim_state.temp_sensors[2] = 0;  // tank middle
    g_sim_state.temp_sensors[3] = 0;  // tank top
}

void hw_blink_led(uint8_t i)
{
    if (i == PIN_LED1 || i == PIN_LED2)
    {
        g_sim_state.leds[i - PIN_LED1] ^= 1;
    }
}

int hw_get_adc_input(uint8_t i)
{
    if ((i - PIN_SENSOR1) < 4)
        return g_sim_state.temp_sensors[i - PIN_SENSOR1];
    else
        return -1;
}

void hw_mssleep(unsigned long d)
{
    usleep(d * 1000);
}

void hw_ussleep(unsigned long d)
{
    usleep(d);
}

void hw_uart_init(void) {}
void hw_uart_flush(void) {}
char hw_uart_getc() { return 0; }
void hw_uart_putc(char c) {}
void hw_uart_puts(char *s)
{
    printf("%s", s);
}

void hw_spi_init(uint8_t enable_interrupts) {}
void hw_spi_select(uint8_t select) {}
unsigned char hw_spi_xmit(unsigned char data) { return 0; }

void hw_i2c_init()
{
}

void hw_i2c_write(uint16_t addr, uint8_t data)
{
}
