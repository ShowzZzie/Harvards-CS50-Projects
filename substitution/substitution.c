#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // if user inputs more or less than 2 arguments in command line
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // checks whether the user's key is 26 characters long
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // for loop below checks if user has input numbers
    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key cannot contain numbers or special characters.\n");
            return 1;
        }
    }
    // check for duplicates
    for (int i = 0, len = strlen(argv[1]); i < len; i++)
    {
        for (int j = i + 1; j < len; j++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][j]))
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    string key = argv[1];
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    // for loop below converts plaintext into ciphertext
    for (int i = 0, len = strlen(plaintext); i < len; i++)
    {
        // if loop below converts all lowercase letters from plaintext
        if (islower(plaintext[i]))
        {
            int a = plaintext[i] - 'a';
            char x = key[a];
            // tolower() is mandatory in case the corresponding letter from key is uppercase
            // this allows us to maintain case of plaintext submitted by the user
            printf("%c", tolower(x));
        }

        else if (isupper(plaintext[i]))
        {
            int a = plaintext[i] - 'A';
            char x = key[a];
            printf("%c", toupper(x));
        }

        else
        {
            char x = plaintext[i];
            printf("%c", x);
        }
    }
    printf("\n");
    return 0;
}
