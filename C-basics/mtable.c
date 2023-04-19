#include "stdio.h"

int main (void)
{
    int length;
    length  = 10;
    int max = 10;

    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j <= max; j++)
        {
            if (i == 10)
                printf("%d x %d = %d\t", j, i, i*j);
            else
                printf("%d x %d  = %d\t", j, i, i*j);
        }
        printf("\n");
    }
    



    return 0;
}