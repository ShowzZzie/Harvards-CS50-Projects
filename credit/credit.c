#include <cs50.h>
#include <stdio.h>
#include <math.h>

// turns out, the thing I struggled with the most was not making "secondary" variables like nm_sum, m_sum, issue_number
// and hence Luhn always returned all 0's for me

int main(void)
{
    long number;

    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    long temp_number = number;

    int length = 0;
    while (temp_number > 0)
    {
        temp_number = temp_number / 10;
        length++;
    }

    if (length <= 12 || length == 14 || length >= 17)
    {
        printf("INVALID\n");
        return 0;
    }
    //nm - notmultiplying | m - multiplying
    int nm_sum = 0;
    int m_sum = 0;
    long nm_number = number;
    long m_number = number;

    // counting from last digit - DO NOT MULTIPLY
    while (nm_number > 0)
    {
        int nm_digit = nm_number % 10;
        nm_number = nm_number / 100;
        nm_sum = nm_sum + nm_digit;
    }

    //counting from second-to-last digit - MULTIPLY *2
    m_number = m_number / 10;

    while (m_number > 0)
    {
        int m_digit = (m_number % 10) * 2;
        m_number = m_number / 100;
        int m_digit1 = m_digit % 10;
        int m_digit2 = m_digit / 10;
        m_sum = m_sum + m_digit1 + m_digit2;
    }

    // add and check wether the sums fit with Luhn's
    if ((nm_sum + m_sum) % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // time to test card issuers

    long issue_number = number;
    int issue_length = length;

    // ftd = fist_two_digits
    int ftd = issue_number / pow(10, (issue_length - 2));
    // fd = first_digit
    int fd = issue_number / pow(10, (issue_length - 1));

    if (issue_length == 15 && (ftd == 34 || ftd == 37))
    {
        printf("AMEX\n");
        return 0;
    }

    if (fd == 4 && (issue_length == 13 || issue_length == 16))
    {
        printf("VISA\n");
        return 0;
    }

    if (issue_length == 16 && (ftd == 51 || ftd == 52 || ftd == 53 || ftd == 54 || ftd == 55))
    {
        printf("MASTERCARD\n");
        return 0;
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }

}