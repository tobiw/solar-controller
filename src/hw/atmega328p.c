#include <avr/io.h>

void setup_leds()
{
    DDRC = DDC0;
}

void blink_led(uint8_t i)
{
    PORTC ^= 1;
}
