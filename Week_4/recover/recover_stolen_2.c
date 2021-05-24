#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Create data type byte
typedef uint8_t BYTE;

// Define block-sze
#define BLOCK 512

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover images\n");
        return 1;
    }

    // Remember filename
    char *data = argv[1];

    // Open file
    FILE *file = fopen(data, "r");
    if (!file)
    {
        printf("Could not open.\n");
        return 1;
    }

    // Create buffer array
    BYTE buffer[BLOCK];

    // String used to store filename "###.jpg\0" (8 bytes needed)
    char filename[8];

    // File pointer to write to
    FILE *img;

    // Use a counter to number the image files
    int counter = 0;

    // Read one 512B block at a time
    while (fread(buffer, BLOCK, 1, file) == 1)
    {
        // Find any JPEG header matches
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter == 0) // From the start of the card (no img files written yet)
            {
                // Print a formatted filename out to use
                sprintf(filename, "%03i.jpg", counter);

                // Open the created image file to write into
                img = fopen(filename, "w");

                // Write from the buffer into the image file
                fwrite(buffer, BLOCK, 1, img);

                counter++;
            }
            else if (counter > 0) // Discovering a new image while writing a current one
            {
                // Close current image file
                fclose(img);

                // Print a formatted filename out to use
                sprintf(filename, "%03i.jpg", counter);

                // Open the created image file to write into
                img = fopen(filename, "w");

                // Write from the buffer into the image file
                fwrite(buffer, BLOCK, 1, img);

                counter++;
            }
        }
        else if (counter > 0) // Continue writing img file after one is discovered
        {
            fwrite(buffer, BLOCK, 1, img);
        }
    }

    // Close card file
    fclose(file);
    // Close img file
    fclose(img);

    // Success
    return 0;
}