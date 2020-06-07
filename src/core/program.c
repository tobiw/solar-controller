#include "config.h"
#include "hw/io.h" // TODO: try to avoid direct hw calls on this level
#include "core/algorithm.h"
#include "core/sensors.h"
#include "core/relays.h"
#include "core/serial.h"
#include "core/config.h" // config structure for program runtime
#include "config.h" // hardcoded config defines

#include <stdio.h>

#define NAME "OpenSC"
#define VERSION "0.1"

void sc_setup()
{
    uint8_t leds[2] = {PIN_LED1, PIN_LED2};
    hw_setup_leds(leds, 2);

    uint8_t pins[4] = {PIN_SENSOR1, PIN_SENSOR2, PIN_SENSOR3, PIN_SENSOR4};
    hw_setup_inputs(pins, 4);

#ifdef ENABLE_SPI_LCD
    hw_spi_init(0);
#endif

#ifdef ENABLE_SERIAL
    hw_uart_init();
    sc_serial_printf(NAME " " VERSION "\n----------\n");
#endif

    // Set default configuration
    sc_config.pump_threshold = 100; // 10 degC
    sc_config.tank_alarm_threshold = 800; // 80 degC
    sc_config.tank_critical_threshold = 950; // 95 degC
    sc_config.collector_critical_threshold = 1200; // 120 degC
    sc_config.collector_freezing_threshold = 10; // 1 degC

    sc_config_print();
    if (sc_config_validate() == 0)
    {
        sc_serial_printf("Error: Invalid config\n");
        hw_exit();
    }
}

void sc_main_loop()
{
    int adc;

    // Heartbeat
    hw_blink_led(PIN_LED1);

    // Get analog inputs, convert to temperatures and store via API
    adc = hw_get_adc_input(0);
    int temp_pt1000 = adc; //(int)(sc_sensors_pt1000_conversion(adc) * 10);
    sc_collector_temperature_set(temp_pt1000);

    int temp_ntc10k[3] = {0};
    for (int i = 0; i < 3; i++)
    {
        adc = hw_get_adc_input(i+1);
        temp_ntc10k[i] = adc; //(int)(sc_sensors_ntc_conversion(adc) * 10);
        sc_tank_temperature_set(i, temp_ntc10k[i]);
    }

    // Decision making based on temperatures retrieved via API
    int16_t temps[4];
    temps[0] = sc_collector_temperature_get();
    for (int i = 0; i < 3; i++)
    {
        temps[i+1] = sc_tank_temperature_get(i);
    }
    const int8_t pump_on = sc_should_pump_turn_on(temps, 4);
    const int8_t hot_water_dump_open = sc_should_hot_water_dump_valve_open(temps[3]); // use hottest part of tank

    // Action
    sc_pump_relay_set(pump_on);
    sc_hot_water_dump_valve_relay_set(hot_water_dump_open);

#ifdef ENABLE_SERIAL
    sc_serial_printf("%d / %d / %d\n", temp_pt1000, temp_ntc10k[0], temp_ntc10k[1]);
#endif
    hw_mssleep(1000);
}
