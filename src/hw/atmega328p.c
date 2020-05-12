#include <avr/io.h>
#include <util/delay.h>

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
