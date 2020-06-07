#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t spi_buf_pos = 0;
extern uint8_t spi_send_buffer[8];
extern uint8_t spi_recv_buffer[8];
extern uint8_t spi_selected;
extern uint8_t output_pins[3];

int hw_output_set(uint8_t i, uint8_t state)
{
    output_pins[i] = state;
    return 0;
}

void hw_spi_init(uint8_t enable_interrupts)
{
    memset(spi_send_buffer, 0, 8);
    memset(spi_recv_buffer, 0, 8);
}

void hw_spi_select(uint8_t select)
{
    spi_selected = select;
}

unsigned char hw_spi_xmit(unsigned char data)
{
    spi_send_buffer[spi_buf_pos] = data;
    return spi_recv_buffer[spi_buf_pos++];
}

void hw_mssleep(unsigned long d)
{
    usleep(d * 1000);
}

void hw_ussleep(unsigned long d)
{
    usleep(d);
}

void hw_uart_puts(char *s)
{
    printf("%s", s);
}
