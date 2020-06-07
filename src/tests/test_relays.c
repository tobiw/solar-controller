#include "CUnit/CUnit.h"
#include "core/relays.h"

uint8_t output_pins[3];

int relays_init_suite(void)
{
    memset(output_pins, 0, sizeof(output_pins));
    return 0;
}

int relays_clean_suite(void) { return 0; }

void test_pump_relay()
{
    CU_ASSERT_EQUAL(sc_pump_relay_set(1), 0);
    CU_ASSERT_EQUAL(output_pins[0], 1);
    CU_ASSERT_EQUAL(sc_pump_relay_set(0), 0);
    CU_ASSERT_EQUAL(output_pins[0], 0);
}

void test_hot_water_dump_relay()
{
    CU_ASSERT_EQUAL(sc_hot_water_dump_valve_relay_set(1), 0);
    CU_ASSERT_EQUAL(output_pins[1], 1);
    CU_ASSERT_EQUAL(sc_hot_water_dump_valve_relay_set(0), 0);
    CU_ASSERT_EQUAL(output_pins[1], 0);
}

void test_tank_heater_relay()
{
    CU_ASSERT_EQUAL(sc_tank_heater_relay_set(1), 0);
    CU_ASSERT_EQUAL(output_pins[2], 1);
    CU_ASSERT_EQUAL(sc_tank_heater_relay_set(0), 0);
    CU_ASSERT_EQUAL(output_pins[2], 0);
}

CU_TestInfo tests_relays[] = {
    {"test_pump_relay", test_pump_relay},
    {"test_hot_water_dump_relay", test_hot_water_dump_relay},
    {"test_tank_heater_relay", test_tank_heater_relay},
    CU_TEST_INFO_NULL,
};
