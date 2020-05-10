#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

void setup_leds()
{
    printf("Setting up LEDs\n");
}

void blink_led(uint8_t i)
{
    printf("Toggling LED %u\n", i);
}

void mssleep(unsigned long d)
{
    usleep(d * 1000);
}
