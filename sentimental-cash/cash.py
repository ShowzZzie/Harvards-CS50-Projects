# TODO
import math
from cs50 import get_float

change = 0.00

while change <= 0.00:
    change = get_float("Change owed: ")

change = int(change * 100)

quarters = int(change / 25)
change = change - quarters * 25

dimes = int(change / 10)
change = change - dimes * 10

nickels = int(change / 5)
change = change - nickels * 5

pennies = int(change / 1)
change = change - pennies * 1

coins = quarters + dimes + nickels + pennies

print(coins)