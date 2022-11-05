#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // made a mistake as I followed index maths too blindly - earlier I defined L and S used in formula and got incorrect results
    // after putting letters and sentences as single variables in the formula below the outputs are correct
    float f_index = 0.0588 * letters / words * 100 - 0.296 * sentences / words * 100 - 15.8;
    int i_index = round(f_index);

    // check for length and print adequate message
    if (i_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (i_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", i_index);
    }

}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        else
        {
            letters += 0;
        }
    }
    return letters;
}


int count_words(string text)
{
    int words = 1;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isblank(text[i]))
        {
            words++;
        }
        else
        {
            words += 0;
        }
    }
    return words;
}


int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
        else
        {
            sentences += 0;
        }
    }
    return sentences;
}