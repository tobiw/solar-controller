#include "core/algorithm.h"
#include "core/config.h"

/*
 * Calculate the difference between collector and tank in order to activate the pump.
 * Only if the collector is sufficiently above the tank temperature does it make sense
 * to pump hot water.
 */
int8_t sc_is_temperature_above_pump_threshold(int16_t temp_collector, int16_t temp_tank)
{
    if ((temp_collector - temp_tank) >= sc_config.pump_threshold)
    {
        return 1;
    }
    return 0;
}

/*
 * Check if tank alarm threshold (too hot but not critical) has been reached.
 * This will usually stop the pump in order to protect plumbing and the tank.
 */
int8_t sc_is_temperature_above_tank_alarm_threshold(int16_t temp_tank)
{
    if (temp_tank >= sc_config.tank_alarm_threshold)
    {
        return 1;
    }
    return 0;
}

/*
 * Check if tank critical temperature threshold has been reached.
 * This will usually open the hot water dump valve in order to get rid of the hot water.
 */
int8_t sc_is_temperature_above_tank_critical_threshold(int16_t temp_tank)
{
    if (temp_tank >= sc_config.tank_critical_threshold)
    {
        return 1;
    }
    return 0;
}

/*
 * Check if collector critical temperature threshold has been reached.
 * This will usually force the pump on in order to prevent the collector from overheating.
 */
int8_t sc_is_temperature_above_collector_critical_threshold(int16_t temp_collector)
{
    if (temp_collector >= sc_config.collector_critical_threshold)
    {
        return 1;
    }
    return 0;
}

/*
 * Check if collector freezing protection threshold has been reached.
 * This will usually force the pump on in order to prevent the water in the collector from freezing.
 */
int8_t sc_is_temperature_below_collector_freezing_threshold(int16_t temp_collector)
{
    if (temp_collector <= sc_config.collector_freezing_threshold)
    {
        return 1;
    }
    return 0;
}

/*
 * Check if pump should turn on. This depends on difference between tank and collector temperatures
 * as well as edge cases (collector critically hot or too cold; tank too hot?).
 */
int8_t sc_should_pump_turn_on(int16_t *temperatures, uint8_t len)
{
    if (len != 4) return -1;

    const int16_t t_collector = temperatures[0];
    const int16_t t_tank1 = temperatures[1];
    const int16_t t_tank3 = temperatures[3];
    int8_t pump_on = 0;

    if (sc_is_temperature_above_pump_threshold(t_collector, t_tank1) == 1)
    {
        pump_on = 1;
    }
    else if (sc_is_temperature_below_collector_freezing_threshold(t_collector) == 1)
    {
        // force pump on immediately
        return 1;
    }

    if (sc_is_temperature_above_tank_alarm_threshold(t_tank3) == 1) // TODO: max of all tank temperatures?
    {
        pump_on = 0;
    }

    // A critically hot collector overrides a hot tank because the hot tank water can be dumped
    if (sc_is_temperature_above_collector_critical_threshold(t_collector) == 1)
    {
        pump_on = 1;
    }

    return pump_on;
}
