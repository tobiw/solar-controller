#include "CUnit/CUnit.h"
#include <stdio.h>
#include "core/sensors.h"

int sensors_init_suite(void) { return 0; }
int sensors_clean_suite(void) { return 0; }

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

int main_sensors (void)
{
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("sensors", sensors_init_suite, sensors_clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

   if ((NULL == CU_add_test(pSuite, "ntc_conversion_1", ntc_conversion_1)) ||
       (NULL == CU_add_test(pSuite, "ntc_conversion_2", ntc_conversion_2)) ||
       (NULL == CU_add_test(pSuite, "pt1000_conversion_1", pt1000_conversion_1)) ||
       (NULL == CU_add_test(pSuite, "pt1000_conversion_2", pt1000_conversion_2)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CU_get_error();
}
