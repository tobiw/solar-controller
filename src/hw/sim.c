#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sim/sim.h"

extern sim_state_t g_sim_state;

void hw_setup_leds()
{
    memset(g_sim_state.leds, 0, sizeof(g_sim_state.leds));
}

void hw_blink_led(uint8_t i)
{
    printf("Toggling LED %u\n", i);
    g_sim_state.leds[i] ^= 1;
}

void hw_mssleep(unsigned long d)
{
    usleep(d * 1000);
}
