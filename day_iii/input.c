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

bool is_valid_file(const char *file, ssize_t *field_width)
{
    // Open the file
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return false;
    }


    // Check to see if the file contains invalid characters
    char c;
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
            }
        }
    }

    // Reset fp
    rewind(fp);

    // Get field width
    char *line = NULL;
    size_t n = 0;
    *field_width = getline(&line, &n, fp);
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
        current_length = getline(&line, &n, fp);
        if ((current_length != *field_width) && !feof(fp)) {
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }
        // End of file is an edge case we need to check for
        // I don't like this though; I bet Aaron will point out how I could've
        // avoided duplicate work
        if ((current_length == *field_width) && feof(fp)) {
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }
        if (feof(fp) &&(current_length != (*field_width - 1))) {
            fclose(fp);
            if (line) {
                free(line);
            }
            return false;
        }
    } while (!feof(fp));
    // Cleanup
    if (line)
    {
        free(line);
    }

    // Valid contents
    fclose(fp);
    return true;
}

uint8_t **get_input(const char *path_to_file, int *reason, 
    ssize_t *field_width)
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

    // At this point we have a file, it is not a directory, and we can read it
    // Does it contain anything other than [ 0 | 1 | \n | EOF ]?
    if (!is_valid_file(path_to_file, field_width)) {
        *reason = NO_CONTENT;
        return NULL;
    }
    return NULL;
}
