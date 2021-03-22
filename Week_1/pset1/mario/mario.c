#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    // accept height number with check
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    // set initial width
    int width = 1;

    // control height
    for (int h = height; h > 0; h--)
    {

        // print left spaces
        for(int s = 0; s < height - width; s++)
        {
            printf(" ");
        }

       //  print bricks
       for (int w = 1; w <= width; w++)
       {
            printf("#");
       }

       // goto next level
       printf("\n");
       width++;

    }
}