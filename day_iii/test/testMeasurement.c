#include <CUnit/CUnit.h>

// Test mask building
void test_build_mask(void)
{
    ;
}

// Test get gamma bits
void test_get_gamma_bits(void)
{
    ;
}

// Test getting gamma bits with too short of a mask
void test_short_mask_gamma(void)
{
    ;
}

// Test get epsilon bits
void test_get_epsilon_bits(void)
{
    ;
}

// Test getting epsilon bits with too short of a mask
void test_short_mask_epsilon(void)
{
    ;
}

// Test get power consumption
void test_get_power_consumption(void)
{
    ;
}

static CU_TestInfo measurement_tests[] = {
    {"Get bit mask", test_build_mask},
    {"Measure gamma bits", test_get_gamma_bits},
    {"Measure gamma bits, short mask", test_short_mask_gamma},
    {"Measure epsilon bits", test_get_epsilon_bits},
    {"Measure epsilon bits, short mask", test_short_mask_epsilon},
    {"Measure power consumption", test_get_power_consumption},
    CU_TEST_INFO_NULL
};

CU_SuiteInfo measurement_suite = {
    "Measurement tests",
    NULL,
    NULL,
    NULL,
    NULL,
    measurement_tests
};
