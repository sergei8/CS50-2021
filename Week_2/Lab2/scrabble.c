#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1,
                3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Prototypes
int compute_score(string word);
int get_index(int c, int char_code[]);
string word_to_lower(int len, string word);

int main(void)
{    
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("%s\n", "Player 1 wins!");
    }
    else if(score2 > score1)
    {
        printf("%s\n", "Player 2 wins!");
    }
    else 
    {
        printf("%s\n", "Tie!");
    }
}

// *****************************************
// Compute and return score for string
// *****************************************
int compute_score(string word)
{
    // get word length
    int word_length = strlen(word);

    // check if word exists
    if (word_length == 0)
    {
        return 0;
    }
    
    // make chars a-z array
    int char_code[26];
    for (int i = 0, l = 97; l <= 122; i++, l++)
    {
        char_code[i] = l;
    }

    // convert word to lowercase
    string word_lower = word_to_lower(word_length, word);

    // calculate sum of scores
    int sum_of_scores = 0;
    for (int i = 0; i <= word_length; i++)
    {
        if (word[i] >= 97 && word[i] <= 122)
        {
            int index = get_index(word[i], char_code);
            if (index != -1)
            {
                sum_of_scores += POINTS[index];
            }
        }
    }

    return sum_of_scores;
}

// *************************************
// return string converted to lower case
// *************************************
string word_to_lower(int len, string word)
{
    for (int i = 0; i <= len; i++)
    {
        if (word[i] >=65 && word[i] <= 90){
            word[i] = word[i] + 32;
        }
    }

    return word;
}

// *********************************************
// find and return index of `char` in `char_code`
// *********************************************
int get_index(int c, int char_code[])
{
    for (int i = 0; i <= 26; i++)
    {
        if (c == char_code[i])
        {
            return i;
        }
    }
    // if not found
    return -1;
}