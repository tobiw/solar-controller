#include <stdio.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

int main_sensors();
int main_wiznet();
int main_algorithm();

int main(void)
{
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   main_sensors();
   main_wiznet();
   main_algorithm();

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
