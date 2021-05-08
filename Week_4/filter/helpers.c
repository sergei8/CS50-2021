#include "helpers.h"
#include <stdio.h>
#include <math.h>

// prototypes
void top_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void top_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void bottom_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void bottom_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void left_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void right_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void top_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void bottom_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void middle(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
   for (int h = 0; h <= height - 1; h++)
       for (int w = 0; w <= width - 1; w++)
       {
           // calculate average of RGB rounded as float
           BYTE avg = (BYTE) round((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0);
          // change pixel to gray
           image[h][w].rgbtBlue  = avg;  
           image[h][w].rgbtGreen = avg;  
           image[h][w].rgbtRed   = avg;  
       }
   return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
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

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // allocate space for the blured image
    RGBTRIPLE blur_image[height][width];

    // make image corners
    top_left(height, width, image, blur_image);
    top_right(height, width, image, blur_image);
    bottom_left(height, width, image, blur_image);
    bottom_right(height, width, image, blur_image);

    // make image edges
    left_edge(height, width, image, blur_image);
    right_edge(height, width, image, blur_image);
    top_edge(height, width, image, blur_image);
    bottom_edge(height, width, image, blur_image);

    // make image middle pixels
    middle(height, width, image, blur_image);

    // fill `image` with `blur_image`
    for (int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            image[i][j] = blur_image[i][j];

    
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

// left edge
void left_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int i = 1; i < h - 1; i++)
    {
        blur_image[i][0].rgbtRed =  (image[i-1][0].rgbtRed +
                                    image[i-1][1].rgbtRed +  
                                    image[i][0].rgbtRed +
                                    image[i][1].rgbtRed +
                                    image[i+1][0].rgbtRed +
                                    image[i+1][1].rgbtRed) / 6;

        blur_image[i][0].rgbtBlue =  (image[i-1][0].rgbtBlue +
                                    image[i-1][1].rgbtBlue +  
                                    image[i][0].rgbtBlue +
                                    image[i][1].rgbtBlue +
                                    image[i+1][0].rgbtBlue +
                                    image[i+1][1].rgbtBlue) / 6;

        blur_image[i][0].rgbtGreen =  (image[i-1][0].rgbtGreen +
                                    image[i-1][1].rgbtGreen +  
                                    image[i][0].rgbtGreen +
                                    image[i][1].rgbtGreen +
                                    image[i+1][0].rgbtGreen +
                                    image[i+1][1].rgbtGreen) / 6;
    }
    
    return;
}

// right edge
void right_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int i = 1; i < h - 1; i++)
    {
        blur_image[i][w].rgbtRed =  (image[i-1][w-1].rgbtRed +
                                    image[i-1][w].rgbtRed +  
                                    image[i][w-1].rgbtRed +
                                    image[i][w].rgbtRed +
                                    image[i+1][w-1].rgbtRed +
                                    image[i+1][w].rgbtRed) / 6;

        blur_image[i][w].rgbtBlue =  (image[i-1][w-1].rgbtBlue +
                                    image[i-1][w].rgbtBlue +  
                                    image[i][w-1].rgbtBlue +
                                    image[i][w].rgbtBlue +
                                    image[i+1][w-1].rgbtBlue +
                                    image[i+1][w].rgbtBlue) / 6;

        blur_image[i][w].rgbtGreen =  (image[i-1][w-1].rgbtGreen +
                                    image[i-1][w].rgbtGreen +  
                                    image[i][w-1].rgbtGreen +
                                    image[i][w].rgbtGreen +
                                    image[i+1][w-1].rgbtGreen +
                                    image[i+1][w].rgbtGreen) / 6;
    }
    
    return;
}

// top edge
void top_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int j = 1; j < w - 1; j++)
    {
        blur_image[0][j].rgbtRed =  (image[0][j-1].rgbtRed +
                                    image[0][j].rgbtRed +  
                                    image[0][j+1].rgbtRed +
                                    image[1][j-1].rgbtRed +
                                    image[1][j].rgbtRed +
                                    image[1][j+1].rgbtRed) / 6;

        blur_image[0][j].rgbtBlue =  (image[0][j-1].rgbtBlue +
                                    image[0][j].rgbtBlue +  
                                    image[0][j+1].rgbtBlue +
                                    image[1][j-1].rgbtBlue +
                                    image[1][j].rgbtBlue +
                                    image[1][j+1].rgbtBlue) / 6;

        blur_image[0][j].rgbtGreen =  (image[0][j-1].rgbtGreen +
                                    image[0][j].rgbtGreen +  
                                    image[0][j+1].rgbtGreen +
                                    image[1][j-1].rgbtGreen +
                                    image[1][j].rgbtGreen +
                                    image[1][j+1].rgbtGreen) / 6;
    }
    
    return;
}

// bottom edge
void bottom_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    h--;
    for (int j = 1; j < w - 1; j++)
    {
        blur_image[h][j].rgbtRed =  (image[h-1][j-1].rgbtRed +
                                    image[h-1][j].rgbtRed +  
                                    image[h-1][j+1].rgbtRed +
                                    image[h][j-1].rgbtRed +
                                    image[h][j].rgbtRed +
                                    image[h][j+1].rgbtRed) / 6;

        blur_image[h][j].rgbtBlue =  (image[h-1][j-1].rgbtBlue +
                                    image[h-1][j].rgbtBlue +  
                                    image[h-1][j+1].rgbtBlue +
                                    image[h][j-1].rgbtBlue +
                                    image[h][j].rgbtBlue +
                                    image[h][j+1].rgbtBlue) / 6;

        blur_image[h][j].rgbtGreen =  (image[h-1][j-1].rgbtGreen +
                                    image[h-1][j].rgbtGreen +  
                                    image[h-1][j+1].rgbtGreen +
                                    image[h][j-1].rgbtGreen +
                                    image[h][j].rgbtGreen +
                                    image[h][j+1].rgbtGreen) / 6;

    }
    
    return;
}

//  middle pixels
void middle(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    printf("%i  %i\n", h, w);

    for (int i = 1; i < h; i++){
        for (int j = 1; j < w; j++)
        {
            blur_image[i][j].rgbtRed =  (image[i-1][j-1].rgbtRed +
                            image[i-1][j].rgbtRed +  
                            image[i-1][j+1].rgbtRed +
                            image[i][j-1].rgbtRed +
                            image[i][j].rgbtRed +
                            image[i][j+1].rgbtRed + 
                            image[i+1][j-1].rgbtRed + 
                            image[i+1][j].rgbtRed + 
                            image[i+1][j+1].rgbtRed) / 9; 

            blur_image[i][j].rgbtBlue =  (image[i-1][j-1].rgbtBlue +
                            image[i-1][j].rgbtBlue +  
                            image[i-1][j+1].rgbtBlue +
                            image[i][j-1].rgbtBlue +
                            image[i][j].rgbtBlue +
                            image[i][j+1].rgbtBlue + 
                            image[i+1][j-1].rgbtBlue + 
                            image[i+1][j].rgbtBlue + 
                            image[i+1][j+1].rgbtBlue) / 9; 

            blur_image[i][j].rgbtGreen =  (image[i-1][j-1].rgbtGreen +
                            image[i-1][j].rgbtGreen +  
                            image[i-1][j+1].rgbtGreen +
                            image[i][j-1].rgbtGreen +
                            image[i][j].rgbtGreen +
                            image[i][j+1].rgbtGreen + 
                            image[i+1][j-1].rgbtGreen + 
                            image[i+1][j].rgbtGreen + 
                            image[i+1][j+1].rgbtGreen) / 9; 
        }
    }
}
