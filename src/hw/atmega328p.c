#include <avr/io.h>
#include <util/delay.h>

void hw_setup_leds()
{
    DDRC = DDC0;
}

void hw_blink_led(uint8_t i)
{
    PORTC ^= 1;
}

void hw_mssleep(unsigned long d)
{
    for (unsigned long i = 0; i < d; i++)
        _delay_ms(1);
}
