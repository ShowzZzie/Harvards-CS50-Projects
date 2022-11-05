# TODO

from cs50 import get_int

height = 0

while height < 1 or height > 8:
    height = get_int("Height: ")

for i in range(height):
    for spaces in range(height - 1 - i):
        print(" ", end="")
    for hashes1 in range(i + 1):
        print("#", end="")

    print("  ", end="")
    
    for hashes2 in range(i + 1):
        print("#", end="")

    print("\n", end="")