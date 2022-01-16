#ifndef MEASURE
#define MEASURE
/** \file measure.h
 *\brief Describes the interface to the measure library for the power meter
 *
 * This library determines the power consumption of our submarine, as a product
 * of the gamma and epsilon rates.
 */

/** Get the power consumption of the submarine
 *
 * @param readings A two dimensional array of bits
 * @param entries The number of entries
 * @param field_width The width of an entry
 * return Power consumption of the submarine as an integer
 */
int get_power_consumption(uint8_t **readings, size_t *entries,
    size_t *field_width);

/** Enum for failure
 *
 * 
 * Invalid File
 * Forbidden
 * Not Found
 */
enum { FAIL = -1 };

#endif
