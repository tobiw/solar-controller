#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void hw_setup_leds()
{
    DDRC = DDC0;
}

void hw_setup_inputs()
{
    // DDRx ...
    // PULLUP?
    // ADC?
}

void hw_blink_led(uint8_t i)
{
    PORTC ^= 1;
}

int hw_get_adc_input(uint8_t i)
{
    return ADC;
}

void hw_mssleep(unsigned long d)
{
    for (unsigned long i = 0; i < d; i++)
        _delay_ms(1);
}

void hw_spi_init(uint8_t enable_interrupts)
{
    DDRB = (1<<DDB5) | (1<<DDB3); // MOSI and SCK as output
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (enable_interrupts<<SPIE); // enable SPI master
}

unsigned char hw_spi_xmit(unsigned char data)
{
    SPDR = data; // transmit by putting data into buffer register
    while (!(SPSR & (1<<SPIF))); // wait for transmission complete
    return SPDR; // return received data from buffer register
}

int hw_spi_xmit_buf(unsigned char *data, int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
    {
        char c = data[i];
        data[i] = hw_spi_xmit(c);
    }
    return i;
}
