#include <math.h>
#include <core/sensors.h>

// hw_set_adc_ref(DEFAULT); // followed by fake read
// hw_get_analog_averaged(PIN_NTC_x);

// hw_set_adc_ref(INTERNAL); // followed by fake read
// hw_get_analog_averaged(PIN_PT1000);

float sc_sensors_ntc_conversion(int raw)
{
    const unsigned int THERMISTORNOMINAL = 10000;
    const unsigned int TEMPERATURENOMINAL = 25;
    const unsigned int BCOEFFICIENT = 3950;
    const unsigned int PULLRESISTOR = 10000;

    float v = raw;

    // convert to resistance
    v = 1023 / v - 1;
    v = PULLRESISTOR / v;

    v = v / THERMISTORNOMINAL;                  // (R/Ro)
    v = log(v);                                 // ln(R/Ro)
    v /= BCOEFFICIENT;                          // 1/B * ln(R/Ro)
    v += 1.0 / (TEMPERATURENOMINAL + 273.15);   // + (1/To)
    v = 1.0 / v;                                // Invert

    return v - 273.15; // return in degrees C
}

float sc_sensors_pt1000_conversion(int raw)
{
    float v = (float)raw / 1023.0 * 1.1; // voltage at analog input

    // TODO: refine
    const float c = 1000;
    const float slope = 449.24;

    // from PT1000 datasheet/standard
    const float alpha = 0.00385;

    v = (v * slope) + c; // resistance
    return ((v / 1000) - 1.0) / alpha;
}
