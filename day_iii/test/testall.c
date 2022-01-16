#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>

int main(void)
{
    // Pull in suites from other test files
    extern CU_SuiteInfo file_suite;
    extern CU_SuiteInfo measurement_suite;

    // Create a container for our suites
    CU_SuiteInfo suites[] = {
        file_suite,
        measurement_suite,
        CU_SUITE_INFO_NULL
    };

    // Initialize CUnit test registry
    CU_ErrorCode e = CU_initialize_registry();
	if (e == CUE_NOMEMORY) {
		fprintf(stderr, "[!] Cannot test -- cannot create CUnit db\n");
	}
	//Register our suites with the CUNit database
	CU_register_suites(suites);

	// Run our tests
	// Can only have one of these uncommented;
	// it's tester's choice
	//CU_automated_run_tests();
	CU_basic_run_tests();

	//Clean up the test registry
	CU_cleanup_registry();
}
