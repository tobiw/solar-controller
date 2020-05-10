#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void hw_setup_leds()
{
    printf("Setting up LEDs\n");
}

void hw_blink_led(uint8_t i)
{
    printf("Toggling LED %u\n", i);
}

void hw_mssleep(unsigned long d)
{
    usleep(d * 1000);
}
