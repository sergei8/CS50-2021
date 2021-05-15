// recover lost jpg files from `image`

#include <stdio.h>
#include <stdlib.h>
 
int FAT_BLOCK = 512;
int find_jpg(unsigned char data[FAT_BLOCK]);
int write_img(char* file_name, int blocks_counter, unsigned char** img);
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

    // read and process input image
    unsigned char buffer[FAT_BLOCK];
    int first_jpg = 1;
    int jpg_found = 0;
    int blocks_counter;
    int number_of_imgs = 0;
    char file_name[4];

    // allocate memory for 1-st block of jpg image
    unsigned char** img = malloc(FAT_BLOCK);

    while (fread(buffer, FAT_BLOCK, 1, input) == 1)
    {
        // find JPG in the buffer
        if (find_jpg(buffer) == 1)
        {
            // check if first image
            if (first_jpg == 1) first_jpg = 0;
            else
            {
                // count images
                number_of_imgs++;
                sprintf(file_name, "%03i", number_of_imgs);
                // TODO write `old img` to the file
                if (write_img(file_name, blocks_counter, img) != 0)
                {
                    printf("Error writing file %s \n", file_name);
                    return 3;
                }
                // printf("%s\n", file_name);
            } 
            
            // set controls
            jpg_found = 1;
            blocks_counter = 0;
            
            // decrease memory for new img to one block
            img = realloc(img, FAT_BLOCK);
            img[blocks_counter] = buffer;
        }
        else
        {
            // continue accumulate blocks of img
            if (jpg_found == 1)
            {
                blocks_counter++;
                // increase memory for additional blocks
                img = realloc(img, blocks_counter * FAT_BLOCK);
                // store part of image
                img[blocks_counter] = buffer;
            }
        }
    }

    free(img);
    printf("Number of images: %05i\n", number_of_imgs);
    return 0;

}

/**
 * find jpg header in the bytes sequence
 * return 1 - true/0 - false
 */
 int find_jpg(unsigned char data[FAT_BLOCK])
 {
    if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff && (data[3] & 0xf0) == 0xe0)
        return 1;
    else
        return 0;
 }

int write_img(char* file_name, int blocks_counter, unsigned char** img)
{
    char name[8];
    sprintf(name, "%s.jpg", file_name);
    printf("%s %i\n", name, blocks_counter);
    FILE *output = fopen(name, "w");
    for (int i = 0; i < blocks_counter; i++)
        fwrite(img[i], FAT_BLOCK, 1, output);
        exit(0);
    fclose(output);
    
    return 0;

}
