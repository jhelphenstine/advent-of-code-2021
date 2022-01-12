#!/usr/bin/python

"""
    Time for a function! This function returns the sum of
    the measurements in the sliding window.
"""
def get_window(lines: list, seed: int):
    # Can I make a window?
    if len(lines) < (seed + 3):
        return 0 # "Decrease" as failure condition

    # Calculate our window sum
    return int(lines[seed]) + int(lines[seed + 1]) + int(lines[seed + 2])


def main():
    with open("Day1-Input") as f:
        lines = f.readlines()

    # Get the first value
    prev_depth = get_window(lines, 0)
    extension = 0

    # Count depth increases
    for x in range(0, len(lines)):
        # Get sum of next depth window
        new_depth = get_window(lines, x)
        if new_depth > prev_depth:
            # Farther sweep
            extension += 1

        # Update our previous depth
        prev_depth = new_depth

    # Print the number of depth increases
    print(f"Depth increased {extension} times")


if __name__== "__main__":
    main()
