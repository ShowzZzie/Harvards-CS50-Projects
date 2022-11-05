// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Self made global int size_counter
int size_counter = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 35000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_num = hash(word);
    node *checker = table[hash_num];

    while (checker != NULL)
    {
        if (strcasecmp(word, checker->word) == 0)
        {
            return true;
        }
        checker = checker->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int count = 0;
    int temp_word[strlen(word)];
    for (int i = 0; i < strlen(word); i++)
    {
        temp_word[i] = toupper(word[i]);
    }

    for (int i = 1; i < strlen(word); i++)
    {
        count += temp_word[i] * temp_word[i - 1];
    }

    int hash_num = count % N;

    return hash_num;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        return false;
    }

    char buffer[LENGTH + 1];

    while (fscanf(dict, "%s", buffer) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return 1;
        }

        strcpy(new_node->word, buffer);
        int hash_num = hash(buffer);

        if (table[hash_num] == NULL)
        {
            table[hash_num] = new_node;
            size_counter++;
        }
        else
        {
            new_node->next = table[hash_num];
            table[hash_num] = new_node;
            size_counter++;
        }
    }

    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return size_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        if (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
