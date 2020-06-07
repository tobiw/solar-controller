#include "core/relays.h"
#include "hw/io.h"
#include "config.h"

int8_t sc_pump_relay_set(uint8_t enable)
{
    hw_output_set(PIN_RELAY1, enable);
    return 0;
}

int8_t sc_hot_water_dump_valve_relay_set(uint8_t open)
{
    hw_output_set(PIN_RELAY2, open);
    return 0;
}

int8_t sc_tank_heater_relay_set(uint8_t enable)
{
    hw_output_set(PIN_RELAY3, enable);
    return 0;
}
