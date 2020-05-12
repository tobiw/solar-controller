#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CUnit/Console.h"

#include <stdio.h>

#include "core/sensors.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void ntc_conversion_1(void) {
    const int adc = 785;
    float temp = sc_sensors_ntc_conversion(adc);
    printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 0);
}

void ntc_conversion_2(void) {
    const int adc = 67;
    float temp = sc_sensors_ntc_conversion(adc);
    printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 100);
}

void pt1000_conversion_1(void) {
    const int adc = 0;
    float temp = sc_sensors_pt1000_conversion(adc);
    printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 0);
}

void pt1000_conversion_2(void) {
    const int adc = 800;
    float temp = sc_sensors_pt1000_conversion(adc);
    printf("%u -> %f\n", adc, temp);
    CU_ASSERT_EQUAL((int)round(temp), 100);
}

int main (void)
{
   CU_pSuite pSuite = NULL;

   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = CU_add_suite("sensors", init_suite, clean_suite);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if ((NULL == CU_add_test(pSuite, "ntc_conversion_1", ntc_conversion_1)) ||
       (NULL == CU_add_test(pSuite, "ntc_conversion_2", ntc_conversion_2)) ||
       (NULL == CU_add_test(pSuite, "pt1000_conversion_1", pt1000_conversion_1)) ||
       (NULL == CU_add_test(pSuite, "pt1000_conversion_2", pt1000_conversion_2)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

/*
   // Run all tests using the automated interface
   CU_automated_run_tests();
   CU_list_tests_to_file();

   // Run all tests using the console interface
   CU_console_run_tests();
*/

   CU_cleanup_registry();
   return CU_get_error();
}
