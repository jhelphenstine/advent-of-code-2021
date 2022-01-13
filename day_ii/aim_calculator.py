#!/usr/bin/python3

def compute_travel(directives: list):
    # Calculate our depth change
    total_depth_change = 0
    total_horizontal_change = 0
    aim = 0

    for entry in directives:
        task = entry.split()[0]
        value = int(entry.split()[1])

        if task == "up":
            aim -= value
        if task == "down":
            aim += value
        if task == "forward":
            total_horizontal_change += value
            total_depth_change += (value * aim)

    return total_horizontal_change * total_depth_change

def main():
    # Get our sample set
    with open("d2_input") as test_input:
        directives = test_input.readlines()

    print(f"Answer: {compute_travel(directives)}")
    

if __name__ == "__main__":
    main()
