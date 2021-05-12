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

RGBTRIPLE compute_Gxy(RGBTRIPLE region[3][3]);
void middle_edges(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w]);
void top_left_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void top_right_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void bottom_left_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void bottom_right_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void edge_top_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void edge_left_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void edge_right_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);
void edge_bottom_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w]);

// `virtual` (unborded) pixel color
RGBTRIPLE unborded_pixel;

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
    // allocate space for the edged image
    RGBTRIPLE edge_image[height][width];

    // make middle edges
    // middle_edges(height, width, image, edge_image);

    // make edged borders
    // edge_top_border(height, width, image, edge_image);
    edge_left_border(height, width, image, edge_image);
    // edge_bottom_border(height, width, image, edge_image);
    // edge_right_border(height, width, image, edge_image);

    // // make corbers edges
    // top_left_edged_corner(height, width, image, edge_image);
    // top_right_edged_corner(height, width, image, edge_image);
    // bottom_left_edged_corner(height, width, image, edge_image);
    // bottom_right_edged_corner(height, width, image, edge_image);


    // fill `image` with `edge_image`
    for (int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            image[i][j] = edge_image[i][j];

    return;
}

// top left corner
void top_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    blur_image[0][0].rgbtRed =  (BYTE) round((image[0][0].rgbtRed +
                                 image[0][1].rgbtRed +
                                 image[1][0].rgbtRed +
                                 image[1][1].rgbtRed) / 4.0);

    blur_image[0][0].rgbtBlue = (BYTE) round((image[0][0].rgbtBlue +
                                 image[0][1].rgbtBlue +
                                 image[1][0].rgbtBlue +
                                 image[1][1].rgbtBlue) / 4.0);

    blur_image[0][0].rgbtGreen = (BYTE) round((image[0][0].rgbtGreen +
                                 image[0][1].rgbtGreen +
                                 image[1][0].rgbtGreen +
                                 image[1][1].rgbtGreen) / 4.0);

    return;
}

// top right corner
void top_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    w--;
    blur_image[0][w].rgbtRed =  (BYTE) round((image[0][w].rgbtRed +
                                 image[0][w-1].rgbtRed +
                                 image[1][w-1].rgbtRed +
                                 image[1][w].rgbtRed) / 4.0);

    blur_image[0][w].rgbtBlue =  (BYTE) round((image[0][w].rgbtBlue +
                                 image[0][w-1].rgbtBlue +
                                 image[1][w-1].rgbtBlue +
                                 image[1][w].rgbtBlue) / 4.0);

    blur_image[0][w].rgbtGreen =  (BYTE) round((image[0][w].rgbtGreen +
                                 image[0][w-1].rgbtGreen +
                                 image[1][w-1].rgbtGreen +
                                 image[1][w].rgbtGreen) / 4.0);

    return;
}

// bottom left corner
void bottom_left(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    h--;
    blur_image[h][0].rgbtRed =  (BYTE) round((image[h][0].rgbtRed +
                                 image[h-1][0].rgbtRed +
                                 image[h-1][1].rgbtRed +
                                 image[h][1].rgbtRed) / 4.0);

    blur_image[h][0].rgbtBlue =  (BYTE) round((image[h][0].rgbtBlue +
                                 image[h-1][0].rgbtBlue +
                                 image[h-1][1].rgbtBlue +
                                 image[h][1].rgbtBlue) / 4.0);

    blur_image[h][0].rgbtGreen =  (BYTE) round((image[h][0].rgbtGreen +
                                 image[h-1][0].rgbtGreen +
                                 image[h-1][1].rgbtGreen +
                                 image[h][1].rgbtGreen) / 4.0);

    return;
}

// bottom right corner
void bottom_right(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    w--; h--;
    blur_image[h][w].rgbtRed =  (BYTE) round((image[h][w].rgbtRed +
                                 image[h-1][w-1].rgbtRed +
                                 image[h-1][w].rgbtRed +
                                 image[h][w-1].rgbtRed) / 4.0);

    blur_image[h][w].rgbtBlue =  (BYTE) round((image[h][w].rgbtBlue +
                                 image[h-1][w-1].rgbtBlue +
                                 image[h-1][w].rgbtBlue +
                                 image[h][w-1].rgbtBlue) / 4.0);

    blur_image[h][w].rgbtGreen =  (BYTE) round((image[h][w].rgbtGreen +
                                 image[h-1][w-1].rgbtGreen +
                                 image[h-1][w].rgbtGreen +
                                 image[h][w-1].rgbtGreen) / 4.0);

    return;
}

// left edge
void left_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int i = 1; i < h - 1; i++)
    {
        blur_image[i][0].rgbtRed =  (BYTE) round((image[i-1][0].rgbtRed +
                                    image[i-1][1].rgbtRed +
                                    image[i][0].rgbtRed +
                                    image[i][1].rgbtRed +
                                    image[i+1][0].rgbtRed +
                                    image[i+1][1].rgbtRed) / 6.0);

        blur_image[i][0].rgbtBlue =  (BYTE) round((image[i-1][0].rgbtBlue +
                                    image[i-1][1].rgbtBlue +
                                    image[i][0].rgbtBlue +
                                    image[i][1].rgbtBlue +
                                    image[i+1][0].rgbtBlue +
                                    image[i+1][1].rgbtBlue) / 6.0);

        blur_image[i][0].rgbtGreen =  (BYTE) round((image[i-1][0].rgbtGreen +
                                    image[i-1][1].rgbtGreen +
                                    image[i][0].rgbtGreen +
                                    image[i][1].rgbtGreen +
                                    image[i+1][0].rgbtGreen +
                                    image[i+1][1].rgbtGreen) / 6.0);
    }

    return;
}

// right edge
void right_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    w--;
    for (int i = 1; i < h - 1; i++)
    {
        blur_image[i][w].rgbtRed =  (BYTE) round((image[i-1][w-1].rgbtRed +
                                    image[i-1][w].rgbtRed +
                                    image[i][w-1].rgbtRed +
                                    image[i][w].rgbtRed +
                                    image[i+1][w-1].rgbtRed +
                                    image[i+1][w].rgbtRed) / 6.0);

        blur_image[i][w].rgbtBlue =  (BYTE) round((image[i-1][w-1].rgbtBlue +
                                    image[i-1][w].rgbtBlue +
                                    image[i][w-1].rgbtBlue +
                                    image[i][w].rgbtBlue +
                                    image[i+1][w-1].rgbtBlue +
                                    image[i+1][w].rgbtBlue) / 6.0);

        blur_image[i][w].rgbtGreen =  (BYTE) round((image[i-1][w-1].rgbtGreen +
                                    image[i-1][w].rgbtGreen +
                                    image[i][w-1].rgbtGreen +
                                    image[i][w].rgbtGreen +
                                    image[i+1][w-1].rgbtGreen +
                                    image[i+1][w].rgbtGreen) / 6.0);
    }

    return;
}

// top edge
void top_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int j = 1; j < w - 1; j++)
    {
        blur_image[0][j].rgbtRed =  (BYTE) round((image[0][j-1].rgbtRed +
                                    image[0][j].rgbtRed +
                                    image[0][j+1].rgbtRed +
                                    image[1][j-1].rgbtRed +
                                    image[1][j].rgbtRed +
                                    image[1][j+1].rgbtRed) / 6.0);

        blur_image[0][j].rgbtBlue =  (BYTE) round((image[0][j-1].rgbtBlue +
                                    image[0][j].rgbtBlue +
                                    image[0][j+1].rgbtBlue +
                                    image[1][j-1].rgbtBlue +
                                    image[1][j].rgbtBlue +
                                    image[1][j+1].rgbtBlue) / 6.0);

        blur_image[0][j].rgbtGreen =  (BYTE) round((image[0][j-1].rgbtGreen +
                                    image[0][j].rgbtGreen +
                                    image[0][j+1].rgbtGreen +
                                    image[1][j-1].rgbtGreen +
                                    image[1][j].rgbtGreen +
                                    image[1][j+1].rgbtGreen) / 6.0);
    }

    return;
}

// bottom edge
void bottom_edge(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    h--;
    for (int j = 1; j < w - 1; j++)
    {
        blur_image[h][j].rgbtRed =  (BYTE) round((image[h-1][j-1].rgbtRed +
                                    image[h-1][j].rgbtRed +
                                    image[h-1][j+1].rgbtRed +
                                    image[h][j-1].rgbtRed +
                                    image[h][j].rgbtRed +
                                    image[h][j+1].rgbtRed) / 6.0);

        blur_image[h][j].rgbtBlue =  (BYTE) round((image[h-1][j-1].rgbtBlue +
                                    image[h-1][j].rgbtBlue +
                                    image[h-1][j+1].rgbtBlue +
                                    image[h][j-1].rgbtBlue +
                                    image[h][j].rgbtBlue +
                                    image[h][j+1].rgbtBlue) / 6.0);

        blur_image[h][j].rgbtGreen =  (BYTE) round((image[h-1][j-1].rgbtGreen +
                                    image[h-1][j].rgbtGreen +
                                    image[h-1][j+1].rgbtGreen +
                                    image[h][j-1].rgbtGreen +
                                    image[h][j].rgbtGreen +
                                    image[h][j+1].rgbtGreen) / 6.0);

    }

    return;
}

//  middle pixels
void middle(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE blur_image[h][w])
{
    for (int i = 1; i < h - 1; i++){
        for (int j = 1; j < w -1; j++)
        {
            blur_image[i][j].rgbtRed =  (BYTE) round((image[i-1][j-1].rgbtRed +
                            image[i-1][j].rgbtRed +
                            image[i-1][j+1].rgbtRed +
                            image[i][j-1].rgbtRed +
                            image[i][j].rgbtRed +
                            image[i][j+1].rgbtRed +
                            image[i+1][j-1].rgbtRed +
                            image[i+1][j].rgbtRed +
                            image[i+1][j+1].rgbtRed) / 9.0);

            blur_image[i][j].rgbtBlue =  (BYTE) round((image[i-1][j-1].rgbtBlue +
                            image[i-1][j].rgbtBlue +
                            image[i-1][j+1].rgbtBlue +
                            image[i][j-1].rgbtBlue +
                            image[i][j].rgbtBlue +
                            image[i][j+1].rgbtBlue +
                            image[i+1][j-1].rgbtBlue +
                            image[i+1][j].rgbtBlue +
                            image[i+1][j+1].rgbtBlue) / 9.0);

            blur_image[i][j].rgbtGreen =  (BYTE) round((image[i-1][j-1].rgbtGreen +
                            image[i-1][j].rgbtGreen +
                            image[i-1][j+1].rgbtGreen +
                            image[i][j-1].rgbtGreen +
                            image[i][j].rgbtGreen +
                            image[i][j+1].rgbtGreen +
                            image[i+1][j-1].rgbtGreen +
                            image[i+1][j].rgbtGreen +
                            image[i+1][j+1].rgbtGreen) / 9.0);
        }
    }
}

//  make middle edges
void middle_edges(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    for (int i = 1; i < h - 1; i++){
        for (int j = 1; j < w -1; j++)
        {
                image[i-1][j-1] =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 25};
                image[i-1][j]   =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 30};
                image[i-1][j+1] =   (RGBTRIPLE) {.rgbtRed = 40, .rgbtGreen = 60, .rgbtBlue  = 80};
                
                image[i][j-1]   =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 30, .rgbtBlue  = 90};
                image[i][j]     =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 40, .rgbtBlue  = 100};
                image[i][j+1]   =   (RGBTRIPLE) {.rgbtRed = 80, .rgbtGreen = 70, .rgbtBlue  = 90};

                image[i+1][j-1] =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 20, .rgbtBlue  = 40};
                image[i+1][j]   =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 10, .rgbtBlue  = 30};
                image[i+1][j+1] =   (RGBTRIPLE) {.rgbtRed = 50, .rgbtGreen = 40, .rgbtBlue  = 10}; 
            
            RGBTRIPLE region[3][3] = 
            {
                {image[i-1][j-1], image[i-1][j], image[i-1][j+1]},
                {image[i][j-1],   image[i][j],   image[i][j+1]},
                {image[i+1][j-1], image[i+1][j], image[i+1][j+1]}
            };
            
            printf("(%i %i %i)\n", image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
            edge_image[i][j] = compute_Gxy(region);
            printf("(%i %i %i)\n", edge_image[i][j].rgbtRed, edge_image[i][j].rgbtGreen, edge_image[i][j].rgbtBlue);
            return;
        }
    }
}

// edge top left corner
void top_left_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    RGBTRIPLE region[3][3] = 
    {
        {unborded_pixel, unborded_pixel, unborded_pixel},
        {unborded_pixel, image[0][0],   image[0][1]},
        {unborded_pixel, image[1][0], image[1][1]}
    };
            
    edge_image[0][0] = compute_Gxy(region);
}

// edge top right corner
void top_right_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    RGBTRIPLE region[3][3] = 
    {
        {unborded_pixel, unborded_pixel, unborded_pixel},
        {image[0][w-1], image[0][w], unborded_pixel},
        {image[1][w-1], image[1][w], unborded_pixel}
    };
            
    edge_image[0][w] = compute_Gxy(region);
}

// edge bottom left corner
void bottom_left_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    RGBTRIPLE region[3][3] = 
    {
        {unborded_pixel, image[h-1][0], image[h-1][1]},
        {unborded_pixel, image[h][0], image[h][1]},
        {unborded_pixel, unborded_pixel, unborded_pixel}
    };
            
    edge_image[0][w] = compute_Gxy(region);
}

// edge bottom right corner
void bottom_right_edged_corner(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    RGBTRIPLE region[3][3] = 
    {
        {image[h-1][w-1], image[h-1][w], unborded_pixel},
        {image[h][w-1], image[h][w], unborded_pixel},
        {unborded_pixel, unborded_pixel, unborded_pixel}
    };
            
    edge_image[0][w] = compute_Gxy(region);
}

// edge top border
void edge_top_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;
    
    printf("top border");
    for (int i = 1; i < h - 1; i++){
        for (int j = 1; j < w -1; j++)
        {
            image[i-1][j-1] =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 25};
            image[i-1][j]   =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 30};
            image[i-1][j+1] =   (RGBTRIPLE) {.rgbtRed = 40, .rgbtGreen = 60, .rgbtBlue  = 80};
            
            image[i][j-1]   =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 30, .rgbtBlue  = 90};
            image[i][j]     =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 40, .rgbtBlue  = 100};
            image[i][j+1]   =   (RGBTRIPLE) {.rgbtRed = 80, .rgbtGreen = 70, .rgbtBlue  = 90};

            image[i+1][j-1] =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 20, .rgbtBlue  = 40};
            image[i+1][j]   =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 10, .rgbtBlue  = 30};
            image[i+1][j+1] =   (RGBTRIPLE) {.rgbtRed = 50, .rgbtGreen = 40, .rgbtBlue  = 10}; 

        }
    }

    for (int j = 1; j < w - 1; j++)
    {    
        RGBTRIPLE region[3][3] = 
        {
            {unborded_pixel, unborded_pixel, unborded_pixel},
            {image[0][j-1], image[0][j], image[0][j+1]},
            {image[1][j-1], image[1][j], image[1][j+1]}
        };
        
        printf("(%i %i %i)\n", image[0][j].rgbtRed, image[0][j].rgbtGreen, image[0][j].rgbtBlue);
        edge_image[0][j] = compute_Gxy(region);
        printf("(%i %i %i)\n", edge_image[0][j].rgbtRed, edge_image[0][j].rgbtGreen, edge_image[0][j].rgbtBlue);
        return;
    }
}

// edge left border
void edge_left_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    for (int i = 1; i < h - 1; i++){
        for (int j = 1; j < w -1; j++)
        {
            image[i-1][j-1] =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 25};
            image[i-1][j]   =   (RGBTRIPLE) {.rgbtRed = 0, .rgbtGreen = 10, .rgbtBlue  = 30};
            image[i-1][j+1] =   (RGBTRIPLE) {.rgbtRed = 40, .rgbtGreen = 60, .rgbtBlue  = 80};
            
            image[i][j-1]   =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 30, .rgbtBlue  = 90};
            image[i][j]     =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 40, .rgbtBlue  = 100};
            image[i][j+1]   =   (RGBTRIPLE) {.rgbtRed = 80, .rgbtGreen = 70, .rgbtBlue  = 90};

            image[i+1][j-1] =   (RGBTRIPLE) {.rgbtRed = 20, .rgbtGreen = 20, .rgbtBlue  = 40};
            image[i+1][j]   =   (RGBTRIPLE) {.rgbtRed = 30, .rgbtGreen = 10, .rgbtBlue  = 30};
            image[i+1][j+1] =   (RGBTRIPLE) {.rgbtRed = 50, .rgbtGreen = 40, .rgbtBlue  = 10}; 

        }
    }

    for (int i = 1; i < h - 1; i++)
    {    
        RGBTRIPLE region[3][3] = 
        {
            {unborded_pixel, image[i-1][0], image[i-1][1]},
            {unborded_pixel, image[i][0], image[i][1]},
            {unborded_pixel, image[i+1][0], image[i+1][1]}
        };
        
        printf("(%i %i %i)\n", image[i][0].rgbtRed, image[i][0].rgbtGreen, image[i][0].rgbtBlue);
        edge_image[i][0] = compute_Gxy(region);
        printf("(%i %i %i)\n", edge_image[i][0].rgbtRed, edge_image[i][0].rgbtGreen, edge_image[i][0].rgbtBlue);
        return;
    }
}

// edge right border
void edge_right_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    w--;
    for (int i = 1; i < h - 1; i++)
    {    
        RGBTRIPLE region[3][3] = 
        {
            {image[i-1][w-1], image[i-1][w], unborded_pixel},
            {image[i][w-1],   image[i][w],   unborded_pixel},
            {image[i+1][w-1], image[i+1][w], unborded_pixel}
        };
        
        edge_image[i][0] = compute_Gxy(region);
    }
}

// edge bottom border
void edge_bottom_border(int h, int w, RGBTRIPLE image[h][w], RGBTRIPLE edge_image[h][w])
{
    unborded_pixel.rgbtRed = unborded_pixel.rgbtBlue = unborded_pixel.rgbtGreen = 0;

    h--;
    for (int j = 1; j < w - 1; j++)
    {    
        RGBTRIPLE region[3][3] = 
        {
            {image[h-1][j-1], image[h-1][j], image[h-1][j+1]},
            {image[h][j-1],   image[h][j],   image[h][j+1]},
            {unborded_pixel, unborded_pixel, unborded_pixel}
        };
        
        edge_image[h][j] = compute_Gxy(region);
    }
}

// compute edge value
RGBTRIPLE compute_Gxy(RGBTRIPLE region[3][3])
{
    // convolution matrix (kernels)    
    
    // printf("calculate\n");

    RGBTRIPLE edge_pixel;

    int Gx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = 
    {
        {-1, -2, -1},
        {0,   0,  0},
        {1,   2,  1}
    };

    int sum_Gx_red = 0;     int sum_Gy_red = 0;
    int sum_Gx_blue = 0;    int sum_Gy_blue = 0;
    int sum_Gx_green = 0;   int sum_Gy_green = 0;
    
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            sum_Gx_red      += region[i][j].rgbtRed * Gx[i][j];
            sum_Gy_red      += region[i][j].rgbtRed * Gy[i][j];
            sum_Gx_blue     += region[i][j].rgbtBlue * Gx[i][j];
            sum_Gy_blue     += region[i][j].rgbtBlue * Gy[i][j];
            sum_Gx_green    += region[i][j].rgbtGreen * Gx[i][j];
            sum_Gy_green    += region[i][j].rgbtGreen * Gy[i][j];
        }

    edge_pixel.rgbtRed      = round(sqrt(pow(sum_Gx_red, 2) + pow(sum_Gy_red, 2)));
    edge_pixel.rgbtBlue     = round(sqrt(pow(sum_Gx_blue, 2) + pow(sum_Gy_blue, 2)));
    edge_pixel.rgbtGreen    = round(sqrt(pow(sum_Gx_green, 2) + pow(sum_Gy_green, 2)));
    
    
    // printf("--- %i, %i \n", sum_Gx_green, sum_Gx_green);
    printf("*** (%i %i %i)\n", edge_pixel.rgbtRed, edge_pixel.rgbtGreen, edge_pixel.rgbtBlue);

    if (edge_pixel.rgbtRed   > 255) edge_pixel.rgbtRed = 255;
    if (edge_pixel.rgbtBlue  > 255) edge_pixel.rgbtBlue = 255;
    if (edge_pixel.rgbtGreen > 255) edge_pixel.rgbtGreen = 255;
    
    return edge_pixel;
}
