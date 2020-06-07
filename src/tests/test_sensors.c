#include "CUnit/CUnit.h"
#include <stdio.h>
#include "core/sensors.h"

int sensors_init_suite(void) { return 0; }
int sensors_clean_suite(void) { return 0; }

extern int16_t temperature_sensor_db[4];

void ntc_conversion_1(void)
{
    const int adc = 785;
    float temp = sc_sensors_ntc_conversion(adc);
    //printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 0);
}

void ntc_conversion_2(void)
{
    const int adc = 67;
    float temp = sc_sensors_ntc_conversion(adc);
    //printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 100);
}

void pt1000_conversion_1(void)
{
    const int adc = 0;
    float temp = sc_sensors_pt1000_conversion(adc);
    //printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 0);
}

void pt1000_conversion_2(void)
{
    const int adc = 800;
    float temp = sc_sensors_pt1000_conversion(adc);
    //printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 100);
}

void test_temperature_set()
{
    sc_collector_temperature_set(1100);
    sc_tank_temperature_set(0, 777);
    sc_tank_temperature_set(1, 888);
    sc_tank_temperature_set(2, 999);
    sc_tank_temperature_set(3, 555);
    CU_ASSERT_EQUAL(temperature_sensor_db[0], 1100);
    CU_ASSERT_EQUAL(temperature_sensor_db[1], 777);
    CU_ASSERT_EQUAL(temperature_sensor_db[2], 888);
    CU_ASSERT_EQUAL(temperature_sensor_db[3], 999);
}

void test_temperature_get()
{
    temperature_sensor_db[0] = 1200;
    temperature_sensor_db[1] = 400;
    temperature_sensor_db[2] = 450;
    temperature_sensor_db[3] = 600;
    CU_ASSERT_EQUAL(sc_collector_temperature_get(), 1200);
    CU_ASSERT_EQUAL(sc_tank_temperature_get(0), 400);
    CU_ASSERT_EQUAL(sc_tank_temperature_get(1), 450);
    CU_ASSERT_EQUAL(sc_tank_temperature_get(2), 600);
}

void test_temperature_get_invalid()
{
    CU_ASSERT_EQUAL(sc_tank_temperature_get(3), (int16_t)((1<<16) - (1<<15))); // -32768 / 0x8000
}

CU_TestInfo tests_sensors[] = {
    {"ntc_conversion_1", ntc_conversion_1},
    {"ntc_conversion_2", ntc_conversion_2},
    {"pt1000_conversion_1", pt1000_conversion_1},
    {"pt1000_conversion_2", pt1000_conversion_2},
    {"test_temperature_set", test_temperature_set},
    {"test_temperature_get", test_temperature_get},
    {"test_temperature_get_invalid", test_temperature_get_invalid},
    CU_TEST_INFO_NULL,
};
