#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover filename");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open the file.");
        return 1;
    }

    BYTE buffer[512];
    int counter = 0;
    char filename[8];
    FILE *recovered = NULL;

    while (fread(&buffer, 512, 1, input) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (!(counter == 0))
            {
                fclose(recovered);
            }
            sprintf(filename, "%03i.jpg", counter);
            recovered = fopen(filename, "w");
            counter++;
        }
        // If JPEG has been found, write to file
        if (!(counter == 0))
        {
            fwrite(&buffer, 512, 1, recovered);
        }
    }
    fclose(input);
    fclose(recovered);
    return 0;
}