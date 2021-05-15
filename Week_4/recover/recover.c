// recover lost jpg files from `image`

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
int FAT_BLOCK = 512;

int check_start_jpg(BYTE buffer[FAT_BLOCK]);
int ckeck_end_jpg(BYTE buffer[FAT_BLOCK]);

int main(int argc, char *argv[])
{
    // check params
    if (argc != 2)
    {
        printf ("Usage: ./recover image\n");
        return 1;
    }

    // open image file 
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf ("Could't open file `%s`!\n", argv[1]);
        return 2;
    }

    BYTE buffer[FAT_BLOCK]; 
    int is_empty_block = 1;  // empty block marker
    
    while (fread(buffer, FAT_BLOCK, 1, input) == 1)
    {

        // analyse blok type
        int is_start_jpg   =  check_start_jpg(buffer);
        int is_end_jpg     =  ckeck_end_jpg(buffer);
        // printf("%i ", is_end_jpg);

        if( is_start_jpg == 1)
        {
            printf("\nS");

            is_empty_block = 0;

            if (is_end_jpg > 0)
            {
                is_empty_block = 1;

                // TODO accumulate `img` with rest of block
                // TODO write output
                printf("\nE\n");
                continue;
            }
            else
            {
                // TODO accumulate `img` with whole block
                printf("#");
                continue;
            }
        }
        else
        {
            if (is_empty_block == 1)
            {
                printf(".");
                continue;
            }
            else
            {
                // TODO accumulate `img` with rest of block 
                // (start & end in the same block)
                printf("@");
                continue;
            }
        }
    }



    return 0;

}

/**
 * @brief check if current block contains jpg header
 * 
 * @param buffer - current block
 * @return int - 1 - yes; 0 - no
 */
int check_start_jpg(BYTE buffer[FAT_BLOCK])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        return 1;
    else
        return 0;
}

/**
 * @brief check if current block contains jpg trailer
 * 
 * @param buffer - current block
 * @return int 0 - not contain; >0 - bytes to end of jpg
 */
int ckeck_end_jpg(BYTE buffer[FAT_BLOCK])
{
    for (int i = 0; i < FAT_BLOCK - 1; i++)
    {
        if (buffer[i] == 0xFF && buffer[i+1] == 0xD9)
            {
                // printf("E\n");
                return i+1;
            }
    }
    return 0;
}