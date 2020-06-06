#include "core/config.h"

#ifdef UNIT_TEST
#include <stdio.h>
#endif

struct sc_config_t sc_config;

int8_t sc_validate_config()
{
#ifdef UNIT_TEST
#define VALIDATE_CONFIG(e) if (!(e)) { printf("%s\n", #e); return 0; }
#else
#define VALIDATE_CONFIG(e) if (!(e)) { return 0; }
#endif

    VALIDATE_CONFIG(sc_config.pump_threshold > 0 && sc_config.pump_threshold < 1000)
    VALIDATE_CONFIG(sc_config.tank_alarm_threshold > 0 && sc_config.tank_alarm_threshold < 1000)
    VALIDATE_CONFIG(sc_config.tank_critical_threshold > 0 && sc_config.tank_critical_threshold < 1000)
    VALIDATE_CONFIG(sc_config.tank_alarm_threshold < sc_config.tank_critical_threshold)
    VALIDATE_CONFIG(sc_config.collector_critical_threshold > 0 && sc_config.collector_critical_threshold < 2000)
    VALIDATE_CONFIG(sc_config.tank_critical_threshold < sc_config.collector_critical_threshold)
    VALIDATE_CONFIG(sc_config.collector_freezing_threshold >= 0 && sc_config.collector_freezing_threshold < 100)
    return 1;
}
