// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <cs50.h>
#include <stdbool.h>
#include <stdint.h>

void copy_header(FILE* input, FILE* output);
void update_volume(FILE* input, FILE* output, float factor);

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    copy_header(input, output);

    // Read samples from input file and write updated data to output file
    update_volume(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

// Copy header from input file to output file
void copy_header(FILE* input, FILE* output)
{
    for (int i = 0; i <= HEADER_SIZE; i++)
    {
        char c = fgetc(input);
        fputc(c, output);
    }
}

void update_volume(FILE* input, FILE* output, float factor)
{
    int16_t *samp = malloc(2);
    do 
    {
        fread(samp, sizeof(int16_t), 1, input);
        int16_t s = (int16_t) ((float)samp * factor); 
        fwrite(samp, sizeof(int16_t), 1, output);
    } while(!feof(input));
}