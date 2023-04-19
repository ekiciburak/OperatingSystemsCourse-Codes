#include "stdio.h"

int fibonacci(int);

int main (void)
{
    int num;
    int result;

    printf("enter a non-negative integer: ");
    scanf("%d", &num);

    result = fibonacci(num);
    if(result == -1)
        printf("negative integer input\n");
    else
        printf("fib(%d) = %d\n", num, result);
}

int fibonacci (int n)
{
    int result;
    if (n < 0)
        result = -1;
    else if (n == 0)
        result = 0;
    else if (n == 1)
        result = 1;
    else
        result = fibonacci(n-1) + fibonacci(n-2);
}


