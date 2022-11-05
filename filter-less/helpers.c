#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            if (sepiaRed >= 255)
            {
                sepiaRed = 255;
            }
            int sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            if (sepiaGreen >= 255)
            {
                sepiaGreen = 255;
            }
            int sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            if (sepiaBlue >= 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int tempR = image[i][j].rgbtRed;
            int tempG = image[i][j].rgbtGreen;
            int tempB = image[i][j].rgbtBlue;
            // [width - j - 1] because if image is 4px wide then computer will store said pixels in location 0, 1, 2, 3
            // if i'd want to move to the last pixel i have to use width - 1
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtRed = tempR;
            image[i][width - j - 1].rgbtGreen = tempG;
            image[i][width - j - 1].rgbtBlue = tempB;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_source[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_source[i][j].rgbtRed = image[i][j].rgbtRed;
            copy_source[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy_source[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // top left corner
            if (i == 0 && j == 0)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i + 1][j].rgbtRed + copy_source[i + 1][j + 1].rgbtRed + copy_source[i][j + 1].rgbtRed) / 4.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i + 1][j].rgbtGreen + copy_source[i + 1][j + 1].rgbtGreen + copy_source[i][j + 1].rgbtGreen) / 4.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i + 1][j].rgbtBlue + copy_source[i + 1][j + 1].rgbtBlue + copy_source[i][j + 1].rgbtBlue) / 4.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // bottom left corner
            else if (i == height - 1 && j == 0)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i - 1][j].rgbtRed + copy_source[i - 1][j + 1].rgbtRed + copy_source[i][j + 1].rgbtRed) / 4.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i - 1][j].rgbtGreen + copy_source[i - 1][j + 1].rgbtGreen + copy_source[i][j + 1].rgbtGreen) / 4.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i - 1][j].rgbtBlue + copy_source[i - 1][j + 1].rgbtBlue + copy_source[i][j + 1].rgbtBlue) / 4.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // top right corner
            else if (i == 0 && j == width - 1)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i + 1][j - 1].rgbtRed + copy_source[i + 1][j].rgbtRed) / 4.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i + 1][j - 1].rgbtGreen + copy_source[i + 1][j].rgbtGreen) / 4.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i + 1][j - 1].rgbtBlue + copy_source[i + 1][j].rgbtBlue) / 4.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i - 1][j - 1].rgbtRed + copy_source[i - 1][j].rgbtRed) / 4.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i - 1][j - 1].rgbtGreen + copy_source[i - 1][j].rgbtGreen) / 4.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i - 1][j - 1].rgbtBlue + copy_source[i - 1][j].rgbtBlue) / 4.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // left edge
            else if (j == 0)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i][j + 1].rgbtRed + copy_source[i - 1][j].rgbtRed + copy_source[i - 1][j + 1].rgbtRed + copy_source[i + 1][j].rgbtRed + copy_source[i + 1][j + 1].rgbtRed) / 6.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i][j + 1].rgbtGreen + copy_source[i - 1][j].rgbtGreen + copy_source[i - 1][j + 1].rgbtGreen + copy_source[i + 1][j].rgbtGreen + copy_source[i + 1][j + 1].rgbtGreen) / 6.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i][j + 1].rgbtBlue + copy_source[i - 1][j].rgbtBlue + copy_source[i - 1][j + 1].rgbtBlue + copy_source[i + 1][j].rgbtBlue + copy_source[i + 1][j + 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // right edge
            else if (j == width - 1)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i - 1][j].rgbtRed + copy_source[i - 1][j - 1].rgbtRed + copy_source[i + 1][j].rgbtRed + copy_source[i + 1][j - 1].rgbtRed) / 6.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i - 1][j].rgbtGreen + copy_source[i - 1][j - 1].rgbtGreen + copy_source[i + 1][j].rgbtGreen + copy_source[i + 1][j - 1].rgbtGreen) / 6.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i - 1][j].rgbtBlue + copy_source[i - 1][j - 1].rgbtBlue + copy_source[i + 1][j].rgbtBlue + copy_source[i + 1][j - 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // top edge
            else if (i == 0)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i + 1][j].rgbtRed + copy_source[i][j + 1].rgbtRed + copy_source[i + 1][j + 1].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i + 1][j - 1].rgbtRed) / 6.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i + 1][j].rgbtGreen + copy_source[i][j + 1].rgbtGreen + copy_source[i + 1][j + 1].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i + 1][j - 1].rgbtGreen) / 6.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i + 1][j].rgbtBlue + copy_source[i][j + 1].rgbtBlue + copy_source[i + 1][j + 1].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i + 1][j - 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            // bottom edge
            else if (i == height - 1)
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i - 1][j].rgbtRed + copy_source[i][j + 1].rgbtRed + copy_source[i - 1][j + 1].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i - 1][j - 1].rgbtRed) / 6.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i - 1][j].rgbtGreen + copy_source[i][j + 1].rgbtGreen + copy_source[i - 1][j + 1].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i - 1][j - 1].rgbtGreen) / 6.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i - 1][j].rgbtBlue + copy_source[i][j + 1].rgbtBlue + copy_source[i - 1][j + 1].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i - 1][j - 1].rgbtBlue) / 6.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
            else
            {
                int avgRed = round((copy_source[i][j].rgbtRed + copy_source[i][j - 1].rgbtRed + copy_source[i][j + 1].rgbtRed + copy_source[i - 1][j].rgbtRed + copy_source[i - 1][j + 1].rgbtRed + copy_source[i - 1][j - 1].rgbtRed + copy_source[i + 1][j].rgbtRed + copy_source[i + 1][j + 1].rgbtRed + copy_source[i + 1][j - 1].rgbtRed) / 9.0);
                int avgGreen = round((copy_source[i][j].rgbtGreen + copy_source[i][j - 1].rgbtGreen + copy_source[i][j + 1].rgbtGreen + copy_source[i - 1][j].rgbtGreen + copy_source[i - 1][j + 1].rgbtGreen + copy_source[i - 1][j - 1].rgbtGreen + copy_source[i + 1][j].rgbtGreen + copy_source[i + 1][j + 1].rgbtGreen + copy_source[i + 1][j - 1].rgbtGreen) / 9.0);
                int avgBlue = round((copy_source[i][j].rgbtBlue + copy_source[i][j - 1].rgbtBlue + copy_source[i][j + 1].rgbtBlue + copy_source[i - 1][j].rgbtBlue + copy_source[i - 1][j + 1].rgbtBlue + copy_source[i - 1][j - 1].rgbtBlue + copy_source[i + 1][j].rgbtBlue + copy_source[i + 1][j + 1].rgbtBlue + copy_source[i + 1][j - 1].rgbtBlue) / 9.0);
                image[i][j].rgbtRed = avgRed;
                image[i][j].rgbtGreen = avgGreen;
                image[i][j].rgbtBlue = avgBlue;
            }
        }
    }
    return;
}
