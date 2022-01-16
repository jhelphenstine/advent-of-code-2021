/** \file input.c
 * \brief Implements the interface described in input.h
 * 
 * This file implements the input library, as described in the
 * input.h header file.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "input.h"

// Simple check: is this a directory?
bool is_dir(const char *path)
{
	struct stat st = { 0 };
	stat(path, &st);
	if ((st.st_mode & S_IFMT) == __S_IFDIR) {
		return true;
	}
	return false;
}

// Simple check: is this file empty?
bool is_empty(const char *path)
{
	struct stat st = { 0 };
	stat(path, &st);
	if (st.st_size == 0) {
		return true;
	}
	return false;
}

// Validate that this is a file we have permission to read
static void check_file(const char *path_to_file, int *reason)
{
	// Assume exists until proven otherwise
	*reason = OK;
	if (access(path_to_file, F_OK) != -1) {
        if (access(path_to_file, R_OK) == -1) {
				*reason = FORBIDDEN;
        }
	} else {
		*reason = NOT_FOUND;
	}
}

// Is this a valid data input file?
bool is_valid_file(const char *file, size_t *field_width, size_t *count)
{
    // Open the file
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return false;
    }


    // Check to see if the file contains invalid characters
    char c;
    bool has_zero = false;
    bool has_one = false;
    while ((c = (char)fgetc(fp)) != -1)
    {
        
        if (strncmp(&c,"0", 1)) {
            if (strncmp(&c, "1", 1)) {
                if (strncmp(&c, "\n", 1)) {
                    // Could be created on Windows...
                    if (strncmp(&c, "\r", 1)) {
                        fclose(fp);
                        return false;
                    }
                }
            } else if (!has_one) {
                has_one = true;
            }
        } else if (!has_zero) {
            has_zero = true;
        }
    }

    // Was there any valid input?
    if (!has_one && !has_zero) {
        // No input in here!
        fclose(fp);
        return false;
    }

    // Reset fp
    rewind(fp);

    // Get line width
    char *line = NULL;
    size_t n = 0;
    ssize_t line_width = getline(&line, &n, fp);
    // Free our line
    if (line)
    {
        free(line);
    }
    
    // Reset fp
    rewind(fp);

    // Ensure each line honors this field width
    ssize_t current_length = 0;
    do {
        //fprintf(stderr, "[DEBUG] Fetching current length\n");
        line = NULL;
        n = 0;
        current_length = getline(&line, &n, fp);
        if ((current_length != line_width) && !feof(fp)) {
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }
        // End of file is an edge case we need to check for
        // I don't like this though; I bet Aaron will point out how I could've
        // avoided duplicate work
        if ((current_length == line_width) && feof(fp)) {
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }
        if (feof(fp) &&(current_length != (line_width - 1))) {
            // Edge case for end of file: line width includes the newline
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }

        // Increment the count of our entries
        (*count)++;

        // Free our line
        if (line)
        {
            free(line);
        }
    } while (!feof(fp));

    // Valid contents; field width is one less than line width
    *field_width = line_width - 1;
    fclose(fp);
    return true;
}

// Build out a multidimensional array with our input bits
uint8_t **get_input_bits(const char *path_to_file, size_t *field_width,
    size_t *count)
{
    // Populate a two-dimensional array of bits from our input file
    if (!path_to_file) {
        // This shouldn't be something we need to check, given the
        // guarding we do in is_valid_file, de-mo....
        return NULL;
    }

    // Open the file
    FILE *fp = fopen(path_to_file, "r");
    if (!fp) {
        return false;
    }

    // Build our array
    uint8_t **readings = calloc(sizeof (uint8_t *), *count);
    if (!readings) {
        // Failed to malloc
        fclose(fp);
        return false;
    }

    for (size_t i = 0; i < *count; i++) {
        readings[i] = calloc(sizeof (uint8_t *), *field_width);
        if (!readings[i]) {
            // Failed to malloc
            free(readings);
            // I am being lazy here - there could be alloc'd rows to delete...
            fclose(fp);
            return false;
        }
    }

    // Temporary variables for our looping
    char *line = NULL;
    size_t n = 0;
    
    ssize_t len = 0;
    
    // Loop over the input, mapping to our array
    for (size_t row = 0; row < *count; row++) {
        
        len = getline(&line, &n, fp);

        if (!len) {
            // TN1! Someone may have tampered between our fclose & fopen
            fclose(fp);
            return NULL;
        }

        // We only worry about the 1's; 0's are default thanks to calloc
        // Newlines aren't a problem
        for (size_t index = 0; index < (*field_width); index++) {
            if (0 == memcmp(&line[index], "1", 1)) {
                // Set the corresponding bit
                readings[row][index] = 1;
            }
        }
        free(line);
        n = 0;
    }

    // Cleanup
    fclose(fp);

    // Return our multidimensional array of bits
    return readings;
}

uint8_t **get_input(const char *path_to_file, int *reason, 
    size_t *field_width, size_t *count)
{
    // Step 0: Check that we were given something to work with
    if (!path_to_file) {
        *reason = NOT_FOUND;
        return NULL;
    }

    // Step 1: Check to see if this is a file we can read
    check_file(path_to_file, reason);
    if (*reason != OK) {
        return NULL;
    }

    // Step 2: Make sure this isn't a directory
    if (is_dir(path_to_file)) {
        *reason = NO_CONTENT;
        return NULL;
    }

    // Step 3: Validate the input file's contents
    // We use this to determine the number of records, too
    if (!is_valid_file(path_to_file, field_width, count)) {
        *reason = NO_CONTENT;
        return NULL;
    }

    // Step 4: Get our input bits
    return get_input_bits(path_to_file, field_width, count);
}
