/** \file measure.c
 * \brief Implements the interface described in measure.h
 * 
 * This file implements the measure library, as described in the
 * measure.h header file.
 */
#include <stdio.h>
#include <stdint.h>
#include "measure.h"

// Determine gamma consumption rate, the most common bit per column
uint8_t get_gamma_rate(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    return 0;
}

int get_power_consumption(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    return FAIL;
}