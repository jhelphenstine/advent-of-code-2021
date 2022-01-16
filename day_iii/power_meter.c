#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sysexits.h>

#include "input.h"
#include "measure.h"

// I need to have a way of cleaning up my input bits. The driver owns this.
void free_readings(uint8_t **readings, size_t *entries)
{
    for (size_t e = 0; e < *entries; e++) {
        free(readings[e]);
    }
    free(readings);
}

// This is for debugging
void print_inputs(uint8_t **readings, size_t *entries, size_t *field_width)
{
    for (size_t e = 0; e < *entries; e++) {
        fprintf(stderr, "Line %lu is: ", e + 1);
        for (size_t i = 0; i < *field_width; i++)
        {
            fprintf(stderr, "%u", readings[e][i]);
        }
        fprintf(stderr, "\n");
    }
}

int main(int argc, char **argv)
{
    // Proper number of args
    if (argc != 2) {
        fprintf(stderr, "Usage: ./power_meter <input_file>\n");
        exit(EX_USAGE);
    }

    // Try to get the input array
    size_t field_width = 0;
    int reason = 0;
    size_t count = 0;
    uint8_t **readings = get_input(argv[1], &reason, &field_width, &count);

    // Try to get the power consumption
    int power = get_power_consumption(readings, &count, &field_width);

    if (FAIL == power) {
        fprintf(stderr, "[ERROR] Failed to determine power consumption\n");
    } else {
        // Report the power consumption
        fprintf(stdout, "[*] Power consumption: %d\n", power);
    }

    // Cleanup
    free_readings(readings, &count);
    exit(EX_OK);
}
