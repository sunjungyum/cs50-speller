// Sun-Jung Yum
// October 11 2018
// Problem Set 4

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 100000

// Keeps trak of the count of words being loaded from the dictionary
int count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word using an algorithm suggested by Thomas Ballatore, who got it from Alan Xie
// Originally found at https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
// Adapted from Neel Mehta's algorithm: http://stackoverflow.com/questions/2571683/djb2-hash-function
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocates memory for new node
        node *addition = malloc(sizeof(node));
        if (addition == NULL)
        {
            unload();
            return false;
        }
        strcpy(addition->word, word);
        addition->next = NULL;

        // If something is in the linked list already
        if (hashtable[hash(word)])
        {
            addition->next = hashtable[hash(word)];
        }
        hashtable[hash(word)] = addition;

        // Keeps track of size of the dictionary
        count++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // If nothing is in that linked list
    if (hashtable[hash(word)] == NULL)
    {
        return false;
    }

    // If something is in the linked list, must traverse through
    else
    {
        node *cursor = hashtable[hash(word)];
        while (cursor != NULL)
        {
            if (strcasecmp(cursor->word, word) == 0)
            {
                return true;
            }
            cursor = cursor->next;
        }
        return false;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Traverses through each linked list
    for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}