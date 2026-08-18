#include <stdio.h>

int main()
{
    int altura = 5;

    for (int i = 1; i <= altura; i++)
    {
        for (int j = i; j < altura; j++)
        {
            printf(" ");
        }

        for (int j = 1; j <= (2 * i - 1); j++)
        {
            printf("*");
        }

        printf("\n");
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < altura - 1; j++)
        {
            printf(" ");
        }

        printf("|\n");
    }

    return 0;
}
