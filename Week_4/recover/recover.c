// recover lost jpg files from `image`

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
int FAT_BLOCK = 512;

int check_start_jpg(BYTE buffer[FAT_BLOCK]);
int ckeck_end_jpg(BYTE buffer[FAT_BLOCK]);
int write_img(char* file_name, int blocks_counter, unsigned char** img);
// void print_img(BYTE** img, int n);
void print_buffer(BYTE buffer[FAT_BLOCK], int n);

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
    int blocks_counter;
    int img_counter = 0;
    char file_name[4];

    // allocate memory for 1-st block of jpg image
    BYTE** img = malloc(FAT_BLOCK);

    while (fread(buffer, FAT_BLOCK, 1, input) == 1)
    {
        print_buffer(buffer, FAT_BLOCK);

        // analyse blok type
        int is_start_jpg   =  check_start_jpg(buffer);
        int is_end_jpg     =  ckeck_end_jpg(buffer);
        printf("start = %i; end = %i\n", is_start_jpg, is_end_jpg);
        if( is_start_jpg == 1)
        {

            is_empty_block = 0;
            blocks_counter = 0;

            // accumulate `img` with whole block
            // img = realloc(img, FAT_BLOCK);
            img[blocks_counter] = buffer;
            print_buffer(buffer, 10);   
            printf("#\n");
           continue;
        }
        else
        {
            if (is_end_jpg > 0)
            {
                is_empty_block = 1;

                // accumulate `img` with rest of block
                blocks_counter++;
                // increase memory for additional blocks
                img = realloc(img, blocks_counter * FAT_BLOCK);
                // store part of image
                img[blocks_counter] = buffer;

                // set output file name
                sprintf(file_name, "%03i", img_counter);

                // write output
                if (write_img(file_name, blocks_counter, img) != 0)
                {
                    printf("\nError writing file %s \n", file_name);
                    return 3;
                }
                printf("\n%i ", blocks_counter);
                printf("E");
                img_counter++;
                continue;
            }

            if (is_empty_block == 0)
            {
                // accumulate `img` with rest of block 
                blocks_counter++;
                // increase memory for additional blocks
                img = realloc(img, blocks_counter * FAT_BLOCK);
                // store part of image
                img[blocks_counter] = buffer;
                printf(".");
                continue;
            }
        }
    }
    
    free(img);
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
                // printf("%x %x ", buffer[i], buffer[i+1]);
                // printf("ret=%i; BLOCK=%i\n ", i+1, FAT_BLOCK);
                return i+1;
            }
    }
    return 0;
}

/**
 * @brief write `img` array into file with unique name
 * 
 * @param file_name 
 * @param blocks_counter 
 * @param img 
 * @return int =  
 */
int write_img(char* file_name, int blocks_counter, BYTE** img)
{
    
    char name[8];
    sprintf(name, "%s.jpg", file_name);
    FILE *output = fopen(name, "w");
    for (int i = 0; i < blocks_counter; i++)
    {
        // fwrite(img[i], FAT_BLOCK, 1, output);
        if (fwrite(img[i], FAT_BLOCK, 1, output) != FAT_BLOCK)
        {
            fclose(output);
            return 0;
        }
    }
    
    fclose(output);

    return 0;
    
}

// void print_img(BYTE** img, int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//         printf("%i ", img[i]);
//     }
// }
void print_buffer(BYTE buffer[FAT_BLOCK], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%x ", buffer[i]);
    }
    printf("\n");
}
