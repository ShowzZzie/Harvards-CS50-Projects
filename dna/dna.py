import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py FILENAME")

    # first Command-Line argument = csv file
    # second command-line argument = txt file

    # TODO: Read database file into a variable
    database = sys.argv[1]
    list_db = []
    with open(database, "r") as databasefile:
        reader = csv.DictReader(databasefile)
        for row in reader:
            list_db.append(row)

    # TODO: Read DNA sequence file into a variable
    text = sys.argv[2]
    with open(text, "r") as textfile:
        dna = textfile.read()

    # TODO: Find longest match of each STR in DNA sequence
    STRs = list(list_db[0].keys())[1:]

    results = {}
    for i in STRs:
        results[i] = longest_match(dna, i)

    # TODO: Check database for matching profiles
    for person in list_db:
        counter = 0
        for i in STRs:
            if int(person[i]) == results[i]:
                counter += 1

            if counter == len(STRs):
                print(person["name"])
                return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
