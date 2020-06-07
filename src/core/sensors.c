#include <math.h>
#include <core/sensors.h>

int16_t temperature_sensor_db[4];

int16_t sc_collector_temperature_get()
{
    return temperature_sensor_db[0];
}

void sc_collector_temperature_set(int16_t t)
{
    temperature_sensor_db[0] = t;
}

int16_t sc_tank_temperature_get(uint8_t i)
{
    if (i > 2)
    {
        return 0x8000; // smallest possible int16_t value
    }
    return temperature_sensor_db[i+1];
}

void sc_tank_temperature_set(uint8_t i, int16_t t)
{
    if (i <= 2)
    {
        temperature_sensor_db[i+1] = t;
    }
}

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
