// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
// node *table[N];
node *table;

int words_count = 0;
int table_size;
int col_count = 0; 
unsigned long buf_size = 100;
char* current_dict;
char *line_buf, *cur_word;
ssize_t line_size;
size_t line_buf_size = LENGTH;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // make normalized hash of the `word`
    unsigned int n_hash = hash(word);

    // check if `word` in the backet
    if (strcmp(word, table[n_hash].word) == 0)
    {
        return true;
    }

    // empty backet - `word` is not in the dictionary?
    if (table[n_hash].word[0] == '\0')
    {
        return false;
    }

    // if `NOT` - scan chain and look for `word`
    node *p = table[n_hash].next;
    while(p->next != NULL)
    {
        if (strcmp(p->word, word))
        {
            return true;
        }
        else
        {
            p = p->next;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    // normalize hash according to table_size
    unsigned int n_hash = hash % table_size;

    return n_hash;
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL) return false;

    // read dictionary line by line
    line_size = getline(&line_buf, &line_buf_size, fp);
    while (line_size >= 0)
    {
        words_count++;
        line_size = getline(&line_buf, &line_buf_size, fp);
    }
    // printf("words_count = %i\n", words_count);
    
    fclose(fp);

    // alloc memory for hash table
    table_size = words_count - 1;
    table = (node*)malloc(table_size * sizeof(node));
    if (table == NULL)
    {
        printf("error alloc %i ", table_size);
        return false;
    }

    for (int i = 0; i < table_size; i++)
    {
        strcpy(table[i].word, "\0"); 
        table[i].next = NULL;
    }

    // open dictionary again
    fp = fopen(current_dict, "r");
    if (fp == NULL) return false;

    // cycle by all words, calculate hash and resolve collisions
    line_size = getline(&cur_word, &buf_size, fp);
    while(line_size >= 0)
    {
        // do not add words with big length
        if (strlen(cur_word) >= LENGTH)
        {
            line_size = getline(&cur_word, &buf_size, fp);
            continue;
        }

        // make normalized hash from the `word`    
        unsigned int n_hash = hash(cur_word);

        // check for collision by checking first byte of `word` for "\0"
        if (table[n_hash].word[0] == '\0')
        {
            // no collision - add word to the node
            strcpy(table[n_hash].word, cur_word);
        }
        else
        {
            // collision detected - create chain node
            col_count++;
            node* p = malloc(sizeof(node));
            
            // check if backet has chain of nodes
            if (table[n_hash].next == NULL)
            {
                // no chain - add first node 
                table[n_hash].next = p;
            }
            else
            {
                // insert node to first position and reorganize links
                p->next = table[n_hash].next;
                table[n_hash].next = p;
            }
        }

        line_size = getline(&cur_word, &buf_size, fp);
    }

    fclose(fp);

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
    free(table);

    return true;
}
