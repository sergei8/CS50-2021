#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>

int get_letters_number(string text, int text_len);
int get_words_number(string text, int text_len);
int get_sent_number(string text, int text_len);

int main(void)
{
    // get text for analise and calc length
    string text =  get_string("Text: ");
    int text_len = strlen(text);
    if (text_len == 0)
    {
        printf ("No text!\n");
        return 1;
    }

    // get number of the letters
    int letters_number = get_letters_number(text, text_len);
    // printf("Letters: %i\n", letters_number);

    // get number of words
    int words_number = get_words_number(text, text_len);
    // printf("Words: %i\n", words_number);

    // get number of sentences
    int sent_number = get_sent_number(text, text_len);
    // printf("Sentences: %i\n", sent_number);

    // calculate avarages 
    float avg_letters = letters_number * 100 / (float)words_number;
    float avg_sent = sent_number * 100 / (float)words_number;
    // printf("%f  %f\n", avg_letters, avg_sent);

    // calculate  Coleman-Liau index
    int coleman_index = (int) round(0.0588 * avg_letters - 0.296 * avg_sent - 15.8);
    // printf("index= %i\n", coleman_index);

    // output grade
    if (coleman_index >= 16)
    {
        printf("%s\n","Grade 16+");
    }
    else if (coleman_index < 1)
    {
        printf("%s\n", "Before Grade 1");
    }
    else
    {
        printf("Grade %i\n", coleman_index);
    }


}
// ***************************************
// count numbers of letters in the text
// ***************************************
int get_letters_number(string text, int text_len)
{
    int letters_count = 0;
    for (int i = 0; i <= text_len; i++)
    {
        if (text[i] >=65 && text[i] <= 122)
        {
            letters_count ++;
        }
    }

    return letters_count;
}

// ***************************************
// count numbers of words in the text
// ***************************************
int get_words_number(string text, int text_len)
{
    int words_count = 0;
    for (int i = 0; i <= text_len; i++)
    {
        if (text[i] == 32)
        {
            words_count++;
        }
    }

    return words_count + 1;
}

// ***************************************
// count numbers of sentences in the text
// ***************************************
int get_sent_number(string text, int text_len)
{
    int sent_count = 0;
    for (int i = 0; i <= text_len; i++)
    {
        if (text[i] == 46 || text[i] == 33 || text[i] == 63)
        {
            sent_count++;
        }
    }

    return sent_count;
}