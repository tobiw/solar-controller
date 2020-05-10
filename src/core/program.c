#include <util/delay.h>

#include "hw/io.h"

void setup()
{
    setup_leds();
}

void main_loop()
{
    blink_led(0);
    _delay_ms(1000);
}
