#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sysexits.h>

#include "input.h"
#include "measure.h"

int main(int argc, char **argv)
{
    // Proper number of args
    if (argc != 2) {
        fprintf(stderr, "Usage: ./power_meter <input_file>\n");
        exit(EX_USAGE);
    }

    // Try to get the input array
    ssize_t field_width = 0;
    int reason = 0;
    get_input(argv[1], &reason, &field_width);

    // Try to get the power consumption

    // Report the power consumption

    exit(EX_OK);
}
