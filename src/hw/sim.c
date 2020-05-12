#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sim/sim.h"

extern sim_state_t g_sim_state;

void hw_setup_leds()
{
    memset(g_sim_state.leds, 0, sizeof(g_sim_state.leds));
}

void hw_setup_inputs()
{
    g_sim_state.temp_sensors[0] = 0;  // collector
    g_sim_state.temp_sensors[1] = 0;  // tank bottom
    g_sim_state.temp_sensors[2] = 0;  // tank middle
    g_sim_state.temp_sensors[3] = 0;  // tank top
}

void hw_blink_led(uint8_t i)
{
    if (i < 3)
    {
        printf("Toggling LED %u\n", i);
        g_sim_state.leds[i] ^= 1;
    }
}

int hw_get_adc_input(uint8_t i)
{
    if (i < 4)
        return g_sim_state.temp_sensors[i];
    else
        return -1;
}

void hw_mssleep(unsigned long d)
{
    usleep(d * 1000);
}
