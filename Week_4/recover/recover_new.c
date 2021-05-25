// recover lost jpg files from `image`

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char BYTE;
int FAT_BLOCK = 512;
int img_len, img_len_old;

int check_start_jpg(BYTE buffer[FAT_BLOCK]);
int end_jpg(BYTE buffer[FAT_BLOCK]);
void write_img(BYTE *buffer, FILE *output, char* file_name);

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
    int img_counter = 0;
    char file_name[8];
 
    while (fread(&buffer, FAT_BLOCK, 1, input) == 1) 
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // set output file name
            sprintf(file_name, "%03i.jpg", img_counter++);
            
            FILE *output = fopen(file_name, "a");
            if (output == NULL)  
            {    
                printf ("Error open output file %s ", file_name);
                return 3;
            }
            
            while (fread(&buffer, FAT_BLOCK, 1, input) == 1) 
            {
                if (end_jpg(buffer) == 0)
                {
                    write_img(buffer, output, file_name);
                    continue;
                }
                else
                {
                    write_img(buffer, output, file_name);
                    fclose(output);
                    break;
                }
            }
        }
    }
    // printf("\nEND OF APP\n");
    return 0;
}


/**
 * @brief check if current block contains jpg trailer
 * 
 * @param buffer - current block
 * @return int 0 - not contain; >0 - bytes to end of jpg
 */
int end_jpg(BYTE buffer[FAT_BLOCK])
{
    for (int i = 0; i < FAT_BLOCK - 1; i++)
        if (buffer[i] == 0xFF && buffer[i+1] == 0xD9) return 1;
    
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
void write_img(BYTE *buffer, FILE *output, char* file_name)
{    
    if (fwrite(buffer, FAT_BLOCK, 1, output) != 1) 
    { 
        printf ("Error write output file %s ", file_name);
        fclose(output);
        exit (1);
    }
}
