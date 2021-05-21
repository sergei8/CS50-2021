// recover lost jpg files from `image`

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;
int FAT_BLOCK = 512;
int img_len, img_len_old;

int check_start_jpg(BYTE buffer[FAT_BLOCK]);
int ckeck_end_jpg(BYTE buffer[FAT_BLOCK]);
int write_img(char* file_name, int len, BYTE* img);

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
    BYTE* img;
    int is_empty_block;  // empty block marker
    int blocks_counter;
    int img_counter = 0;
    char file_name[4];
 
    // allocate memory for 1-st block of jpg image

    while (!feof(input)) 
    {
        // clear buffer and read new block
        memset (buffer, 0, FAT_BLOCK);
        fread(buffer, FAT_BLOCK, 1, input);

        // analyse blok type
        int is_start_jpg   =  check_start_jpg(buffer);
        int is_end_jpg     =  ckeck_end_jpg(buffer);


        if( is_start_jpg == 1)
        {
            printf("start = %i; end = %i\n", is_start_jpg, is_end_jpg);
            is_empty_block = 0;
            blocks_counter = 1;

            img = malloc(FAT_BLOCK * sizeof(BYTE));
            img_len = FAT_BLOCK * sizeof(BYTE);
            // printf ("len = %i\n", img_len);

            // accumulate `img` with whole block
            for(int i = 0; i < img_len; i++) img[i] = buffer[i];

           continue;
        }
        else
        {
            if (is_end_jpg > 0)
            {
                is_empty_block = 1;
                // TODO delete `block_counters`
                blocks_counter++;
                img_len_old = img_len;

                // increase memory for additional blocks
                img_len += is_end_jpg * sizeof(BYTE);
                printf ("len = %i\n", img_len);

                img = realloc(img, img_len);
                
                // store part of image
                for(int i = img_len_old, j = 0; i < img_len; i++, j++) img[i] = buffer[j];

                // set output file name
                sprintf(file_name, "%03i", img_counter);

                // write output
                if (write_img(file_name, img_len, img) != 0)
                {
                    printf("\nError writing file %s \n", file_name);
                    return 3;
                }
                printf("End ");
                printf("*** %i %i\n", blocks_counter, img_counter);
                img_counter++;
                free(img);
                continue;
            }

            if (is_empty_block == 0)
            {
                // accumulate `img` with rest of block 
                blocks_counter++;
                img_len_old = img_len;
                img_len += FAT_BLOCK * sizeof(BYTE);
               
                // increase memory for additional blocks
                img = realloc(img, img_len);
                for(int i = img_len_old, j = 0; i < img_len; i++, j++) img[i] = buffer[j];

                continue;
            }
            // else continue;
        }
    }
    printf("\nEND OF APP\n");
    fclose(input);
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
                printf("i = %i \n", i+2);
                return i+2;
            }
    }
    return 0;
}

/**
 * @brief write `img` array into file with unique name
 * 
 * @param file_name 
 * @param len 
 * @param img 
 * @return int =  
 */
int write_img(char* file_name, int len, BYTE* img)
{
    
    char name[8];
    sprintf(name, "%s.jpg", file_name);
    FILE *output = fopen(name, "w");
    
    if (fwrite(img, len, 1, output) != 1) return 1;
    
    fclose(output);
    return 0;
    
}
