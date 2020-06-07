#include <stdint.h>

int16_t sc_collector_temperature_get();
void sc_collector_temperature_set(int16_t t);
int16_t sc_tank_temperature_get(uint8_t i);
void sc_tank_temperature_set(uint8_t i, int16_t t);
float sc_sensors_ntc_conversion(int raw);
float sc_sensors_pt1000_conversion(int raw);
