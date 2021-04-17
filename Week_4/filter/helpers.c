#include "helpers.h"
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
   printf("GRAYSCALE !!!\n");

   for (int h = 0; h <= height - 1; h++)
   {
       for (int w = 0; w <= width - 1; w++)
       {
           // calculate average of RGB
           BYTE avg = (BYTE) ((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3);
          // change to gray
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
 return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
