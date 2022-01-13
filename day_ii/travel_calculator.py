#!/usr/bin/python3

# Thank you Aaron for this little snippet
from typing import List

def test():
    # Get our sample set
    with open("d2_input") as test_input:
        directives = test_input.readlines()

    aheads = [int(x.split()[1]) for x in directives if x.split()[0] == "forward"]
    rises = [int(x.split()[1]) for x in directives if x.split()[0] == "up"]
    dives = [int(x.split()[1]) for x in directives if x.split()[0] == "down"]

    print(f"Answer: {sum(aheads) * (sum(dives) - sum(rises))}")
    

if __name__ == "__main__":
    test()
