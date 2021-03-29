#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

string get_key(int argc, string argv[]);
void is_key_valid(string key);
string set_key_to_upper(string key);
string get_text(void);
string make_chipher_text(string text, string key);
char substitute(int c, string key);
int find_char_index(char c);

string DICT = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, string argv[])
{
    // get key from args
    string key = get_key(argc, argv);

    // check if key is valid
    is_key_valid(key);
    
    // get plain text
    string plain_text = get_text();

    // chipher plain text
    string chipher_text = make_chipher_text(plain_text, key);
    printf("ciphertext: %s\n", plain_text);

}

// ***************************
// get key from CLI
// ***************************
string get_key(int argc, string argv[])
{
    
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        exit(1);
    }
    
    return argv[1];

}

// *****************************************************
// check length,letters in the key & not exixsts letters
// *****************************************************
void is_key_valid(string key)
{
    // check length
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        exit(1);
    }
    
    // check not letter present
    for (int i = 0; i <= strlen(key) - 1; i++)
    {
        if(key[i] < 65 || key[i] > 122)
        {
            printf("Key contains not letters! %c\n", key[i]);
            exit(1);
        }
    }

    // check if letter is out 
    string key_upper = set_key_to_upper(key);

    for (int i = 0; i <= strlen(key_upper) - 1; i++)
    {
        if (!strchr(key_upper, DICT[i]))
        {
            printf("Substitute letter '%c' not found!\n", DICT[i]);
            exit(1);
        }
    }
}

// *****************************************************
// set key to uppercase
// *****************************************************
string set_key_to_upper(string key)
{
    for (int i = 0; i <= strlen(key); i++)
    {
        if (key[i] >=97 && key[i] <= 122)
        {
            key[i] = toupper(key[i]);
        }
    }
    return key;
}

// *****************************************************
// get plain text from user input
// *****************************************************
string get_text(void)
{
    string text;
    do 
    {
        text = get_string("plaintext:  ");
    } 
    while(strlen(text) == 0);

    return text;
}

// *****************************************************
// chipher plain text
// *****************************************************
string make_chipher_text(string text, string key)
{
    int text_len = strlen(text);

    for (int i = 0; i <= text_len; i++)
    {
        text[i] = substitute(text[i], key);
    }

    return text;

}

// *****************************************************
// substitute char by key
// *****************************************************
char substitute(int c, string key)

{
    // nothing to be done if `c` is not letter
    if (c < 65 || c > 122)
    {
        return (char) c;
    }

    // find char `c` in the DICT
    if (islower(c))
    {
        char c_upper = toupper(c);
        int idx = find_char_index(c_upper);
        return tolower(key[idx]);   
    }
    else
    {
        int idx = find_char_index(c);
        return key[idx];   
    }
}

// *****************************************************
// find char in DICT and return index
// *****************************************************
int find_char_index(char c)
{
    for (int i = 0; i <= 26; i++)
    {
        if (c == DICT[i])
        {
            return i;
        }
    }
    return -1;
    
}
