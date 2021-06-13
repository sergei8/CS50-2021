// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

int words_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char *line_buf = NULL;
    ssize_t line_size;
    size_t line_buf_size = LENGTH;

    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL) return false;

    // read dictionary line by line
    line_size = getline(&line_buf, &line_buf_size, fp);
    while (line_size >= 0)
    {
        // printf("size = %zi, %s", line_size, line_buf);
        words_count++;
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    printf("words_count = %i\n", words_count);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}
