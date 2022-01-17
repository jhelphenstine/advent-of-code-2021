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

int mask_match(uint8_t *entry, uint8_t *mask, size_t position)
{
    for (size_t i = 0; i < position; i++) {
        if (entry[i] != mask[i]) {
            return 0;
        }
    }
    return 1;
}

// Determine the mask bit
uint8_t get_mask_bit(uint8_t **readings, size_t *entries,
    int request, size_t position, uint8_t *mask)
{
    ssize_t tally = 0;
    
    int one_bit_seen = 0;
    int zero_bit_seen = 0;
    int tie = 0;
    for (size_t e  = 0; e < *entries; e++) {
        // IF this isn't the first run AND we don't match the mask - skip
        if ((position > 0) && !(mask_match(readings[e], mask, position))) {
            continue;
        } 
        if (readings[e][position] == 1) {
            one_bit_seen = 1;
            tally++;
        } else {
            zero_bit_seen = 1;
            tally--;
        }

    }

    // Determine the tally
    // positive: 1's dominate
    // tie: need to break the tie
    // negative: 0's dominate
    if (tally > 0) {
        tally = 1;
    } else if (0 == tally) {
        tie = 1;
        if (OXYGEN == request) {
            if (one_bit_seen) {
                tally = 1;
            }
        }
    } else {
        tally = 0;
    }

    if (OXYGEN == request) {
        return tally;
    }

    // Carbon dioxide has a few different rules
    if ((1 == tally) && !zero_bit_seen) {
        // Flip it, accounting for our final flip
        tally ^= 1;
    }
    if ((0 == tally) && !one_bit_seen) {
        // Flip it, accounting for our final flip
        tally ^= 1;
    }
    if ((0 == tally) && tie) {
        // Flip it, accounting for our final flip
        tally ^= 1;
    }
    return tally ^ 1;
}

// Determine the requested rating
size_t get_rating(uint8_t **readings, size_t *entries,
    size_t *field_width, int request)
{
    size_t rating = 0;
    uint8_t *mask =  calloc(sizeof (uint8_t *), *field_width);
    size_t position = 0;

    // Build out the mask, one position at a time
    for (size_t i = 0; i < *field_width; i++) {
        // Iterate through each column
        rating += get_mask_bit(readings, entries, request, position, mask);
        mask[i] = rating & 1;
        rating <<= 1;
        position++;
    }

    // We overshifted
    rating >>= 1;

    return rating;
}

size_t get_power_consumption(uint8_t **readings, size_t *entries,
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
    
    return gamma * epsilon;
}

size_t get_life_support(uint8_t **readings, size_t *entries,
    size_t *field_width)
{
    // Get the oxygen generator rate
    size_t oxygen = get_rating(readings, entries, field_width, OXYGEN);

    size_t carbon_dioxide = get_rating(readings, entries, field_width,
                                       CARBON_DIOXIDE);

    return oxygen * carbon_dioxide;
}
