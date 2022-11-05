# TODO

from cs50 import get_string

text = get_string("Text: ")

letter_counter = 0
word_counter = 1
sentence_counter = 0

for i in text:
    if i.isalpha():
        letter_counter += 1
    elif i == " ":
        word_counter += 1
    elif i == "!" or i == "?" or i == ".":
        sentence_counter += 1

L = letter_counter / word_counter * 100
S = sentence_counter / word_counter * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")