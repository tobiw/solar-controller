#include <avr/io.h>
#include <util/delay.h>

void setup_leds()
{
    DDRC = DDC0;
}

void blink_led(uint8_t i)
{
    PORTC ^= 1;
}

void mssleep(unsigned long d)
{
    for (unsigned long i = 0; i < d; i++)
        _delay_ms(1);
}
