#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LENGTH 45

unsigned long hash(unsigned char *str);

char *word;
int words_count = 0, col_count = 0; 
unsigned long buf_size = 100;
int table_size;
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node* table;

int main(void)
{
    FILE *fp = fopen("dictionaries/large", "r");
    if (fp == NULL) 
    {
        printf("error open dict");
        exit(1);
    }
    // count the length of the file
    while (!feof(fp)){
        getline(&word, &buf_size, fp);
        words_count++;
    }
    // set pointer to start of the file
    rewind(fp);

    // alloc memory for hash table
    table_size = words_count - 1;
    table = (node*)malloc(table_size * sizeof(node));
    if (table == NULL)
    {
        printf("error alloc %i ", table_size);
        exit(2);
    }
    // fill hash table items with `empty` values
    for (int i = 0; i < table_size; i++)
    {
        strcpy(table[i].word, "\0"); 
        table[i].next = NULL;
    }

    // cycle by all words, calculate hash and caunt collisions
    int line_size = getline(&word, &buf_size, fp);
    while(line_size >= 0)
    {
        
        // pass words > LENGTH
        if (strlen(word) >= LENGTH)
        {
            line_size = getline(&word, &buf_size, fp);
            continue;
        }
        
        // make hash from the `word`    
        unsigned long hash_str = hash((unsigned char*) word);

        // normalize hash according to table_size
        unsigned long n_hash = hash_str % table_size;

        // check for collision by checking first byte of `word` for "\0"
        if (table[n_hash].word[0] == '\0')
        {
            // no collision - add word to the node
            strcpy(table[n_hash].word, word);
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
                // insert node to first position
                p->next = table[n_hash].next;
                table[n_hash].next = p;
            }
        }

        line_size = getline(&word, &buf_size, fp);
    }

    free(table);

    printf("\ntable_size = %i\ncol_count = %i\n", table_size, col_count);

}

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

