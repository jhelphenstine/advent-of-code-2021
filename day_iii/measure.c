/** \file measure.c
 * \brief Implements the interface described in measure.h
 * 
 * This file implements the measure library, as described in the
 * measure.h header file.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "measure.h"

// Determine which measurement we are after
enum { OXYGEN = 1, CARBON_DIOXIDE = 2 };

// Determine gamma consumption rate, the most common bit per column
size_t get_gamma_rate(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    if (0 == *field_width) {
        return 0;
    }

    int *tally = calloc(*field_width, sizeof(int));

    for (size_t e  = 0; e < *entries; e++) {
        for (size_t i = 0; i < *field_width; i++) {
            if (readings[e][i] == 1) {
                tally[i]++;
            } else {
                tally[i]--;
            }
        }
    }
    // Calculate the gamma rate
    size_t gamma = 0;

    for (size_t idx = 0; idx < *field_width; idx++) {
        if (tally[idx] > 0) {
            gamma += 1;
        }
        gamma <<= 1;
    }

    // We overshifted
    gamma >>= 1;

    // Cleanup
    free(tally);

    // We have our gamma rate
    return gamma;
}

// Determine the mask bit
uint8_t get_mask_bit(uint8_t **readings, size_t *entries,
    size_t *field_width, int request, int position)
{
    ssize_t tally = 0;

    for (size_t e  = 0; e < *entries; e++) {
        if (readings[e][position] == 1) {
            tally++;
        } else {
            tally--;
        }
    }

    // Determine the tally
    if (tally > 0) {
        tally = 1;
    } else {
        tally = 0;
    }

    // Do we want oxygen or carbon dioxide?
    if (OXYGEN == request) {
        return tally;
    } else {
        return tally ^ 1;
    }
}

// Determine the requested rating
size_t get_rating(uint8_t **readings, size_t *entries,
    size_t *field_width, int request)
{
    // Which is faster: building "field_width #" of linked lists,
    // or pruning from one?
    return 0;
}

int get_power_consumption(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    // Get the gamma rate
    size_t gamma = get_gamma_rate(readings, entries, field_width);

    // Epsilon is the inverse
    size_t epsilon = 0;
    // Bit we want to check
    size_t pos = *field_width;

    for (size_t idx = 0; idx < *field_width; idx++) {
        // Shift bits into epsilon such that it ends up as
        // the logical NOT of gamma
        if (!(gamma & (1 << (pos - 1)))) {
            epsilon += 1;
        }
        pos--;
        epsilon <<= 1;
    }

    // We overshifted
    epsilon >>= 1;
    
    return (int)gamma * (int)epsilon;
}

int get_life_support(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    // Get the oxygen generator rate
    size_t oxygen = get_rating(readings, entries, field_width, OXYGEN);

    size_t carbon_dioxide = get_rating(readings, entries, field_width,
                                       CARBON_DIOXIDE);

    return (int)oxygen * (int)carbon_dioxide;
}
