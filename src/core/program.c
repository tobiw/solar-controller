#include "hw/io.h"

void sc_setup()
{
    hw_setup_leds();
}

void sc_main_loop()
{
    hw_blink_led(0);
    hw_mssleep(1000);
}
