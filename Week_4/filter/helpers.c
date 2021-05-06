#include "helpers.h"
#include <stdio.h>

// lengths of the arrays of the next pixels
int CORNER_LEN = 4;
int EDGE_LEN   = 6;
int MIDDLE_LEN = 9;

// prototypes
void top_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void top_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void bottom_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void bottom_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
   for (int h = 0; h <= height - 1; h++)
   {
       for (int w = 0; w <= width - 1; w++)
       {
           // calculate average of RGB
           BYTE avg = (BYTE) ((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3);
          // change pixel to gray
           image[h][w].rgbtBlue  = avg;  
           image[h][w].rgbtGreen = avg;  
           image[h][w].rgbtRed   = avg;  
       }
   }
   return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    printf ("%i %i\n", height, width);
    int w, h, j;
    for (h = 0; h < height; h++)
    {
        for ( w = 0, j = width - 1; w < width / 2; w++, j--)
        {
            RGBTRIPLE tmp = image[h][w];
            image[h][w]   = image[h][j];
            image[h][j]   = tmp;
        }
    }
    
    printf ("%i %i %i\n", h, w, j);

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // allocate spce for blured image
    RGBTRIPLE blur_image[height][width];

    // make image corners
    top_left(height, width, image, blur_image);
    top_right(height, width, image, blur_image);
    bottom_left(height, width, image, blur_image);
    bottom_right(height, width, image, blur_image);

    // TODO make image edges

    // TODO make image middle pixels

    // TODO fill `image` with `blur_image`
    
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}


// top left corner
void top_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    blur_image[0][0].rgbtRed =  (image[0][0].rgbtRed + 
                                 image[0][1].rgbtRed +
                                 image[1][0].rgbtRed +
                                 image[1][1].rgbtRed) / 4;

    blur_image[0][0].rgbtBlue = (image[0][0].rgbtBlue + 
                                 image[0][1].rgbtBlue +
                                 image[1][0].rgbtBlue +
                                 image[1][1].rgbtBlue) / 4;

    blur_image[0][0].rgbtGreen = (image[0][0].rgbtGreen + 
                                 image[0][1].rgbtGreen +
                                 image[1][0].rgbtGreen +
                                 image[1][1].rgbtGreen) / 4;

    
    return;
}

// top right corner
void top_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    blur_image[0][w].rgbtRed =  (image[0][w].rgbtRed + 
                                 image[0][w-1].rgbtRed +
                                 image[1][w-1].rgbtRed +
                                 image[1][w].rgbtRed) / 4;

    blur_image[0][w].rgbtBlue =  (image[0][w].rgbtBlue + 
                                 image[0][w-1].rgbtBlue +
                                 image[1][w-1].rgbtBlue +
                                 image[1][w].rgbtBlue) / 4;

    blur_image[0][w].rgbtGreen =  (image[0][w].rgbtGreen + 
                                 image[0][w-1].rgbtGreen +
                                 image[1][w-1].rgbtGreen +
                                 image[1][w].rgbtGreen) / 4;
    
    return;
}

// bottom left corner
void bottom_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    blur_image[h][0].rgbtRed =  (image[h][0].rgbtRed + 
                                 image[h-1][0].rgbtRed +
                                 image[h-1][1].rgbtRed +
                                 image[h][1].rgbtRed) / 4;

    blur_image[h][0].rgbtBlue =  (image[h][0].rgbtBlue + 
                                 image[h-1][0].rgbtBlue +
                                 image[h-1][1].rgbtBlue +
                                 image[h][1].rgbtBlue) / 4;

    blur_image[h][0].rgbtGreen =  (image[h][0].rgbtGreen + 
                                 image[h-1][0].rgbtGreen +
                                 image[h-1][1].rgbtGreen +
                                 image[h][1].rgbtGreen) / 4;

    return;
}

// bottom right corner
void bottom_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    blur_image[h][w].rgbtRed =  (image[h][w].rgbtRed + 
                                 image[h-1][w-1].rgbtRed +
                                 image[h-1][w].rgbtRed +
                                 image[h][w-1].rgbtRed) / 4;

    blur_image[h][w].rgbtBlue =  (image[h][w].rgbtBlue + 
                                 image[h-1][w-1].rgbtBlue +
                                 image[h-1][w].rgbtBlue +
                                 image[h][w-1].rgbtBlue) / 4;

    blur_image[h][w].rgbtGreen =  (image[h][w].rgbtGreen + 
                                 image[h-1][w-1].rgbtGreen +
                                 image[h-1][w].rgbtGreen +
                                 image[h][w-1].rgbtGreen) / 4;

    return;
}
