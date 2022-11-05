#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // define height as int in the beginning so that the do while loop works as intended
    int height;
    // do while - asks for input untill it gets a demanded integer defined in while(); || in C functions as [or] operator
    do
    {
        // no need to put [ int height = get_int ] as the data type is defined earlier, above
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 1; i <= height; i++)
    {
        for (int j = height - 1; j >= i; j--)
        {
            printf(" ");
        }
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }

        // Putting this outside of the loop is crucial, so that the space isn't put everytime after #
        printf("  ");

        for (int m = 1; m <= i; m++)
        {
            printf("#");
        }
        printf("\n");
    }
}