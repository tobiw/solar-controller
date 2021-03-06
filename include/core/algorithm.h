#include <stdint.h>

int8_t sc_is_temperature_above_pump_threshold(int16_t temp_collector, int16_t temp_tank);
int8_t sc_is_temperature_above_tank_alarm_threshold(int16_t temp_tank);
int8_t sc_is_temperature_above_tank_critical_threshold(int16_t temp_tank);
int8_t sc_is_temperature_above_collector_critical_threshold(int16_t temp_collector);
int8_t sc_is_temperature_below_collector_freezing_threshold(int16_t temp_collector);
int8_t sc_should_pump_turn_on(int16_t *temperatures, uint8_t len);
int8_t sc_should_hot_water_dump_valve_open(int16_t temp_tank);
