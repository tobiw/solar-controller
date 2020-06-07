#include "core/config.h"
#include "core/serial.h"

#ifdef UNIT_TEST
#include <stdio.h>
#endif

struct sc_config_t sc_config;

int8_t sc_config_validate()
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

void sc_config_print()
{
    sc_serial_printf("Pump Threshold (delta degC): %u\n", sc_config.pump_threshold);
    sc_serial_printf("Tank Alarm Threshold (degC): %u\n", sc_config.tank_alarm_threshold);
    sc_serial_printf("Tank Critical Threshold (degC): %u\n", sc_config.tank_critical_threshold);
    sc_serial_printf("Collector Critical Threshold (degC): %u\n", sc_config.collector_critical_threshold);
    sc_serial_printf("Collector Freezing Threshold (degC): %u\n", sc_config.collector_freezing_threshold);
}
