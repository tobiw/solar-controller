#include "hw/io.h"
#include "core/sensors.h"
#include <stdio.h>

void sc_setup()
{
    hw_setup_leds();
    hw_setup_inputs();
}

void sc_main_loop()
{
    int adc;

    hw_blink_led(0);

    adc = hw_get_adc_input(0);
    float temp_pt1000 = sc_sensors_pt1000_conversion(adc);

    float temp_ntc10k[3];
    for (int i = 1; i < 4; i++)
    {
        adc = hw_get_adc_input(i);
        temp_ntc10k[i-1] = sc_sensors_ntc_conversion(adc);
    }

    printf("Tcollector = %f\n", temp_pt1000);
    printf("Ttank = %f, %f, %f\n", temp_ntc10k[0], temp_ntc10k[1], temp_ntc10k[2]);

    hw_mssleep(1000);
}
