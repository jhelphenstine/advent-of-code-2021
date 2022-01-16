#include <CUnit/CUnit.h>

// Test against invalid file
void test_valid_file(void)
{
    ;
}

void test_invalid_file(void)
{
    ;
}

void test_get_field_width(void)
{
    ;
}

void test_get_input_array(void)
{
    ;
}

static CU_TestInfo file_tests[] = {
    {"Input file [valid]", test_valid_file},
    {"Input file [invalid]", test_invalid_file},
    {"Determine field width", test_get_field_width},
    {"Return input array", test_get_input_array},
    CU_TEST_INFO_NULL
};

CU_SuiteInfo file_suite = {
    "File tests",
    NULL,
    NULL,
    NULL,
    NULL,
    file_tests
};
