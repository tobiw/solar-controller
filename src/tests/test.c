#include <stdlib.h>
#include <stdio.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

extern CU_TestInfo tests_sensors[];
extern CU_TestInfo tests_relays[];
extern CU_TestInfo tests_wiznet[];
extern CU_TestInfo tests_algorithm[];

int sensors_init_suite();
int sensors_clean_suite();
int relays_init_suite();
int relays_clean_suite();
int wiznet_init_suite();
int wiznet_clean_suite();
int algorithm_init_suite();
int algorithm_clean_suite();

static CU_SuiteInfo suites[] = {
    {
        .pName = "Sensors",
        .pInitFunc = sensors_init_suite,
        .pCleanupFunc = sensors_clean_suite,
        .pTests = tests_sensors
    },
    {
        .pName = "Relays",
        .pInitFunc = relays_init_suite,
        .pCleanupFunc = relays_clean_suite,
        .pTests = tests_relays
    },
    {
        .pName = "Wiznet",
        .pInitFunc = wiznet_init_suite,
        .pCleanupFunc = wiznet_clean_suite,
        .pTests = tests_wiznet
    },
    {
        .pName = "Algorithm",
        .pInitFunc = algorithm_init_suite,
        .pCleanupFunc = algorithm_clean_suite,
        .pTests = tests_algorithm
    },
};

int main ()
{
    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry ())
        return;
    if (NULL == CU_get_registry()) return 1;
    if (CU_is_test_running()) return 1;

    /* Add tests */
    CU_SuiteInfo *suite = &suites[0];
    while (suite && suite->pName)
    {
        /* Default to running all tests of a suite */
        CU_pSuite pSuite = CU_add_suite(suite->pName, suite->pInitFunc, suite->pCleanupFunc);
        if (pSuite == NULL)
        {
            fprintf(stderr, "suite registration failed - %s\n", CU_get_error_msg ());
            return 1;
        }
        CU_TestInfo *test = &suite->pTests[0];
        while (test && test->pName)
        {
            if (CU_add_test(pSuite, test->pName, test->pTestFunc) == NULL)
            {
                fprintf(stderr, "test registration failed - %s\n", CU_get_error_msg ());
                return 1;
            }
            test++;
        }
        suite++;
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_set_error_action(CUEA_IGNORE);
    CU_basic_run_tests();

    const int number_of_failures = CU_get_number_of_failures();

    CU_cleanup_registry();
    return number_of_failures;
}
