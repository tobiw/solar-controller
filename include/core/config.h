#include <stdint.h>

struct sc_config_t {
    int16_t pump_threshold;
    int16_t tank_alarm_threshold;
    int16_t tank_critical_threshold;
    int16_t collector_critical_threshold;
    int16_t collector_freezing_threshold;
};

extern struct sc_config_t sc_config;

int8_t sc_validate_config();
