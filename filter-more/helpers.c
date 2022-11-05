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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float gx_red = 0;
            float gy_red = 0;
            float gx_green = 0;
            float gy_green = 0;
            float gx_blue = 0;
            float gy_blue = 0;

            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    if (i + a < 0 || i + a >= height)
                    {
                        continue;
                    }
                    else if (j + b < 0 || j + b >= width)
                    {
                        continue;
                    }
                    else
                    {
                        gx_red += copy[i + a][j + b].rgbtRed * gx[a + 1][b + 1];
                        gy_red += copy[i + a][j + b].rgbtRed * gy[a + 1][b + 1];
                        gx_green += copy[i + a][j + b].rgbtGreen * gx[a + 1][b + 1];
                        gy_green += copy[i + a][j + b].rgbtGreen * gy[a + 1][b + 1];
                        gx_blue += copy[i + a][j + b].rgbtBlue * gx[a + 1][b + 1];
                        gy_blue += copy[i + a][j + b].rgbtBlue * gy[a + 1][b + 1];
                    }
                }
            }
            int SobelRed = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            int SobelGreen = round(sqrt(gx_green * gx_green + gy_green * gy_green));
            int SobelBlue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));

            if (SobelRed > 255)
            {
                SobelRed = 255;
            }
            if (SobelGreen > 255)
            {
                SobelGreen = 255;
            }
            if (SobelBlue > 255)
            {
                SobelBlue = 255;
            }

            image[i][j].rgbtRed = SobelRed;
            image[i][j].rgbtGreen = SobelGreen;
            image[i][j].rgbtBlue = SobelBlue;
        }
    }

    return;
}
