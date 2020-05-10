#include "hw/io.h"

void setup()
{
    setup_leds();
}

void main_loop()
{
    blink_led(0);
    mssleep(1000);
}
