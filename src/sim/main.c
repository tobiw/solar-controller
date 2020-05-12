#include "core/program.h"
#include "sim/sim.h"
#include <stdio.h>

sim_state_t g_sim_state;

static void sim_update_inputs(sim_state_t *s)
{
    s->temp_sensors[0] = 400;
    s->temp_sensors[1] = 400;
    s->temp_sensors[2] = 400;
    s->temp_sensors[3] = 400;
}

static void sim_update_ui(sim_state_t *s)
{
    printf("SIM STATE: LED[%u,%u,%u] RLY[%u,%u,%u] TMP[%u,%u,%u,%u]\n",
           s->leds[0], s->leds[1], s->leds[2],
           s->relays[0], s->relays[1], s->relays[2],
           s->temp_sensors[0], s->temp_sensors[1], s->temp_sensors[2], s->temp_sensors[3]);
}

static void sim_main_loop()
{
    // Update simulated inputs (temperatures)
    sim_update_inputs(&g_sim_state);

    // Update simulator UI
    sim_update_ui(&g_sim_state);

    // Run SC core
    sc_main_loop();
}

int main()
{
    sc_setup();

    while (1)
    {
        sim_main_loop();
    }
}

