#include "config.h"
#include "hw/io.h" // TODO: try to avoid direct hw calls on this level
#include "core/sensors.h"
#include "core/serial.h"
#include "config.h"

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
}

void sc_main_loop()
{
    int adc;

    hw_blink_led(PIN_LED1);

    adc = hw_get_adc_input(0);
    int temp_pt1000 = adc; //(int)(sc_sensors_pt1000_conversion(adc) * 10);

    int temp_ntc10k[3] = {0};
    for (int i = 1; i < 2; i++)
    {
        adc = hw_get_adc_input(i);
        temp_ntc10k[i-1] = adc; //(int)(sc_sensors_ntc_conversion(adc) * 10);
    }

    sc_serial_printf("%d / %d / %d\n", temp_pt1000, temp_ntc10k[0], temp_ntc10k[1]);
    hw_mssleep(1000);
}
