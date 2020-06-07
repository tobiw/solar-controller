#include "CUnit/CUnit.h"
#include <stdio.h>
#include "core/algorithm.h"
#include "core/config.h"

int algorithm_init_suite(void)
{
    sc_config.pump_threshold = 100; // 10 degC
    sc_config.tank_alarm_threshold = 800; // 80 degC
    sc_config.tank_critical_threshold = 950; // 95 degC
    sc_config.collector_critical_threshold = 1200; // 120 degC
    sc_config.collector_freezing_threshold = 10; // 1 degC
    return 0;
}

int algorithm_clean_suite(void) { return 0; }

void test_validate_config()
{
    CU_ASSERT_EQUAL(sc_config_validate(), 1);
}

void test_temperature_below_pump_threshold()
{

    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(-100, 300), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(0, 300), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(400, 400), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(490, 400), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(499, 400), 0);
}

void test_temperature_above_pump_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(500, 400), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(1000, 400), 1);
}

void test_temperature_equal_pump_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_above_pump_threshold(500, 400), 1);
}

void test_temperature_tank_alarm_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_alarm_threshold(790), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_alarm_threshold(800), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_alarm_threshold(900), 1);
}

void test_temperature_tank_critical_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_critical_threshold(900), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_critical_threshold(950), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_above_tank_critical_threshold(1000), 1);
}

void test_temperature_collector_critical_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_above_collector_critical_threshold(1100), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_above_collector_critical_threshold(1200), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_above_collector_critical_threshold(2000), 1);
}

void test_temperature_collector_freezing_threshold()
{
    CU_ASSERT_EQUAL(sc_is_temperature_below_collector_freezing_threshold(15), 0);
    CU_ASSERT_EQUAL(sc_is_temperature_below_collector_freezing_threshold(10), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_below_collector_freezing_threshold(0), 1);
    CU_ASSERT_EQUAL(sc_is_temperature_below_collector_freezing_threshold(-200), 1);
}

void test_pump_off()
{
    // collector, tank bottom, tank middle, tank top
    int16_t temps1[4] = { 400, 400, 500, 600 };
    CU_ASSERT_EQUAL(sc_should_pump_turn_on(temps1, 4), 0);
}

void test_pump_on()
{
    // collector, tank bottom, tank middle, tank top
    int16_t temps1[4] = { 520, 400, 500, 600 };
    CU_ASSERT_EQUAL(sc_should_pump_turn_on(temps1, 4), 1);
}

void test_pump_force_off()
{
    // collector, tank bottom, tank middle, tank top
    int16_t temps1[4] = { 1100, 500, 600, 800 };
    CU_ASSERT_EQUAL(sc_should_pump_turn_on(temps1, 4), 0);
}

void test_pump_force_on()
{
    // collector, tank bottom, tank middle, tank top
    int16_t temps1[4] = { 10, 200, 300, 300 };
    CU_ASSERT_EQUAL(sc_should_pump_turn_on(temps1, 4), 1);
}

void test_dump_water()
{
    CU_ASSERT_EQUAL(sc_should_hot_water_dump_valve_open(0), 0);
    CU_ASSERT_EQUAL(sc_should_hot_water_dump_valve_open(900), 0);
    CU_ASSERT_EQUAL(sc_should_hot_water_dump_valve_open(950), 1);
    CU_ASSERT_EQUAL(sc_should_hot_water_dump_valve_open(1000), 1);
}

CU_TestInfo tests_algorithm[] = {
    {"test_validate_config", test_validate_config},
    {"test_temperature_below_pump_threshold", test_temperature_below_pump_threshold},
    {"test_temperature_above_pump_threshold", test_temperature_above_pump_threshold},
    {"test_temperature_equal_pump_threshold", test_temperature_equal_pump_threshold},
    {"test_temperature_tank_alarm_threshold", test_temperature_tank_alarm_threshold},
    {"test_temperature_tank_critical_threshold", test_temperature_tank_critical_threshold},
    {"test_temperature_collector_critical_threshold", test_temperature_collector_critical_threshold},
    {"test_temperature_collector_freezing_threshold", test_temperature_collector_freezing_threshold},
    {"test_pump_off", test_pump_off},
    {"test_pump_on", test_pump_on},
    {"test_pump_force_off", test_pump_force_off},
    {"test_pump_force_on", test_pump_force_on},
    {"test_dump_water", test_dump_water},
    CU_TEST_INFO_NULL,
};
