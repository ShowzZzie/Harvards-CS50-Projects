#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

char rotate(char c, int n);

int main(int argc, string argv[])
{
    string plaintext;

    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    int key = atoi(argv[1]);

    if (key >= 1)
    {
        plaintext = get_string("Plaintext: ");
    }

    printf("Ciphertext: ");

    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        // here we use the rotate function defined below which will help us encipher user's plaintext
        // worth noting - we don't print a string user has given
        // instead we are breaking down the string into array of characters and printing one character after another
        // rotate() only checks for uppercase and lowercase letters which means it will leave punctuation and spaces as they were
        // when user submitted his plaintext
        char rotated = rotate(plaintext[i], key);
        printf("%c", rotated);
    }
    printf("\n");
    return 0;
}

char rotate(char c, int n)
{
    int ciphered;
    int ciphered2;

    if (isupper(c))
    {
        ciphered = c + n;

        if (ciphered > 'Z')
        {
            ciphered2 = ((c - 65 + n) % 26 + 65);
            return ciphered2;
        }
        else
        {
            return ciphered;
        }
    }
    else if (islower(c))
    {
        ciphered = c + n;

        if (ciphered > 'z')
        {
            ciphered2 = ((c - 97 + n) % 26 + 97);
            return ciphered2;
        }
        else
        {
            return ciphered;
        }
    }
    else
    {
        return c;
    }
}