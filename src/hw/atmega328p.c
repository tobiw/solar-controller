#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void hw_setup_leds(uint8_t *pins, uint8_t len)
{
}

void hw_setup_inputs(uint8_t *pins, uint8_t len)
{
    // DDRx ...
    // PULLUP?
    for (int i = 0; i < len; i++)
    {
        pins[i] = pins[i];
    }
}

void hw_blink_led(uint8_t i)
{
    PORTC ^= 1;
}

uint16_t hw_get_adc_input(uint8_t i)
{
    ADMUX = (1<<REFS1)|(1<<REFS0); // 1.1V internal reference
    ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // high resolution
    ADCSRA |= (1<<ADEN); // enable ADC
    ADMUX &= 0xF0; // set previous MUX channels to 0
    ADMUX |= i; // set ADCi as active MUX channel
    ADCSRB = 0; // free-run mode
    ADCSRA |= (1<<ADSC); // start conversion
    while (ADCSRA & (1<<ADSC)); // wait for conversion
    uint8_t low = ADCL; // read low value first
    uint8_t hi = ADCH;
    return ((hi << 8) | low) & 0x3ff; // mask 10 bit value
}

void hw_mssleep(unsigned long d)
{
    for (unsigned long i = 0; i < d; i++)
        _delay_ms(1);
}

void hw_ussleep(unsigned long d)
{
    for (unsigned long i = 0; i < d; i++)
        _delay_us(1);
}

void hw_exit()
{
    while(1) {}
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
    if (c == '\n') // insert \r for correct tty behaviour
    {
        hw_uart_putc('\r');
    }
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
    DDRB = (1<<DDB5) | (1<<DDB3) | (1<<DDB2); // MOSI, SCK and SS as output
    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (enable_interrupts<<SPIE); // enable SPI master
}

void hw_spi_select(uint8_t select)
{
    if (select)
        PORTB &= ~(1<<PORTB2); // active low
    else
        PORTB |= (1<<PORTB2);
}

unsigned char hw_spi_xmit(unsigned char data)
{
    SPDR = data; // transmit by putting data into buffer register
    while (!(SPSR & (1<<SPIF))); // wait for transmission complete
    return SPDR; // return received data from buffer register
}

void hw_i2c_init()
{
}

void hw_i2c_write(uint16_t addr, uint8_t data)
{
}
