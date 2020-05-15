#include <stdint.h>
#include <string.h>

uint8_t spi_buf_pos = 0;
extern uint8_t spi_send_buffer[8];
extern uint8_t spi_recv_buffer[8];
extern uint8_t spi_selected;

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
