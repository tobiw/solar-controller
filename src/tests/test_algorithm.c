#include "CUnit/CUnit.h"
#include <stdio.h>
#include "core/algorithm.h"
#include "core/config.h"

static int algorithm_init_suite(void)
{
    sc_config.pump_threshold = 100; // 10 degC
    sc_config.tank_alarm_threshold = 800; // 80 degC
    sc_config.tank_critical_threshold = 950; // 95 degC
    sc_config.collector_critical_threshold = 1200; // 120 degC
    sc_config.collector_freezing_threshold = 10; // 1 degC
    return 0;
}

static int algorithm_clean_suite(void) { return 0; }

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

int main_algorithm (void)
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("algorithm", algorithm_init_suite, algorithm_clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

#define ADD_TEST(t) \
    if (NULL == CU_add_test(pSuite, #t, t)) \
    {                                       \
        CU_cleanup_registry();              \
        return CU_get_error();              \
    }

    ADD_TEST(test_temperature_below_pump_threshold)
    ADD_TEST(test_temperature_above_pump_threshold)
    ADD_TEST(test_temperature_equal_pump_threshold)
    ADD_TEST(test_temperature_tank_alarm_threshold)
    ADD_TEST(test_temperature_tank_critical_threshold)
    ADD_TEST(test_temperature_collector_critical_threshold)
    ADD_TEST(test_temperature_collector_freezing_threshold)

    return CU_get_error();
}
