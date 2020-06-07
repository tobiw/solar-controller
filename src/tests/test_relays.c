#include "CUnit/CUnit.h"
#include "core/relays.h"

uint8_t output_pins[3];

static int init_suite(void)
{
    memset(output_pins, 0, sizeof(output_pins));
    return 0;
}
static int clean_suite(void) { return 0; }

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

int main_relays (void)
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("relays", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

#define ADD_TEST(t) \
    if (NULL == CU_add_test(pSuite, #t, t)) \
    {                                       \
        CU_cleanup_registry();              \
        return CU_get_error();              \
    }

    ADD_TEST(test_pump_relay)
    ADD_TEST(test_hot_water_dump_relay)
    ADD_TEST(test_tank_heater_relay)

    return CU_get_error();
}
