#ifndef INPUT
#define INPUT
#include <stdint.h>
/** \file input.h
 * \brief Describes the interface to the input library
 * 
 * This library describes the interface to the input library for
 * the power meter application. This library is responsible for validating
 * the input file. For valid input files, this library shall return a 
 * two-dimensional array of input bits, and set a field width parameter.
 * 
 * The caller is responsible for freeing said array.
 */

/** Get a two-dimensional input array from a valid file
 * 
 * @param path_to_file A path to the requested file
 * @param reason Indicates the reason for failure, or success
 * @param field_width Width of a line of input
 * @param count Number of entries
 * return A two-dimensional array containing the input bits
 */
uint8_t **get_input(const char *path_to_file, int *reason,
    size_t *field_width, size_t *count);

/** Enumerate reasons for failure
 *
 * OK
 * Invalid File
 * Forbidden
 * Not Found
 */
enum { OK = 200, NO_CONTENT = 204, FORBIDDEN = 403, NOT_FOUND = 404 };

#endif