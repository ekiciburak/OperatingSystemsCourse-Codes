#include "stdio.h"



int main (void)
{
    int num;
    int result = 1;

    printf("enter a non-negative integer: ");
    scanf("%d", &num);

    if(num < 0)
        printf("error: negative input\n");
    else
    {
        for (int i = 2; i <= num; i++)
        {
            result = result * i;
        }
        printf("!%d = %d\n", num, result);
    }

    return 0;
}