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

#define UART_BAUDRATE 9600

void hw_uart_init(void)
{
    UBRR0H = (((F_CPU/UART_BAUDRATE)/16)-1)>>8; // set baud rate
    UBRR0L = (((F_CPU/UART_BAUDRATE)/16)-1);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);             // enable Rx & Tx
    UCSR0C=  (1<<UCSZ01)|(1<<UCSZ00);           // config USART; 8N1
}

void hw_uart_flush(void)
{
    unsigned char x;
    while (UCSR0A & (1<<RXC0)) x = UDR0;
    x = x;
}

void hw_uart_putc(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void hw_uart_puts(char *s)
{
    while (*s != 0) hw_uart_putc(*(s++));
}

char hw_uart_getc()
{
    while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
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
