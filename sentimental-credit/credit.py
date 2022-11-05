# TODO

from cs50 import get_int
from cs50 import get_string
import sys
from math import floor

number = 0

while number == 0:
    number = get_int("Number: ")

int_number = number
str_number = str(number)

length = len(str_number)

m_digit = 0
nm_digit = 0
m_sum = 0
nm_sum = 0

temp_card = int_number // 10
temp_card2 = int_number

while temp_card > 0:
    m_digit = (temp_card % 10) * 2
    temp_card = temp_card // 100
    if m_digit >= 10:
        m_sum += m_digit % 10
        m_sum += m_digit // 10
    else:
        m_sum += m_digit

while temp_card2 > 0:
    nm_digit = temp_card2 % 10
    temp_card2 = temp_card2 // 100
    nm_sum += nm_digit

checksum = (m_sum + nm_sum) % 10

# first two digits
ftd = int(str_number[:2])

# first digit
fd = int(str_number[:1])

if checksum != 0:
    print("INVALID")
    sys.exit(0)
else:
    if length == 15 and ftd == 34 or ftd == 37:
        print("AMEX")
    elif length == 13 and fd == 4:
        print("VISA")
    elif length == 16:
        if ftd >= 51 and ftd <= 55:
            print("MASTERCARD")
        elif fd == 4:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")