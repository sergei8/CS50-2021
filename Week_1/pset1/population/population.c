#include <stdio.h>
#include <cs50.h>

int main(void)
{

    signed int start_size, end_size;

    // Prompt for start size
    do
    {

        start_size = get_int("Start size: ");
    }
    while (start_size < 9);

    // Prompt for end size
    do
    {
        end_size = get_int("End size: ");
    }
    while (end_size < start_size);

    // initialize years
    unsigned int years = 0;

    // Calculate number of years until we reach threshold
    while (start_size < end_size)
    {
        // modify start populatin
        start_size += start_size / 3 - start_size / 4;

        // increment year
        years++;

    };

    // Print number of years
    printf("Years: %i\n", years);

}
