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
 */

/** Get a two-dimensional input array from a valid file
 * 
 * @param path_to_file A path to the requested file
 * @param reason Indicates the reason for failure, or success
 * @param field_width Width of a line of input
 * return A two-dimensional array containing the input bits
 */
uint8_t **get_input(char *path_to_file, int *reason, int *field_width);

#endif