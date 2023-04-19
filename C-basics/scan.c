#include <stdio.h> // printf, scanf

int main (void)
{
    int integer1;
    int integer2;

    printf("enter an integer: ");
    scanf("%d", &integer1);

    printf("enter another integer: ");
    scanf("%d", &integer2);

    int sum;
    sum = integer1 + integer2;

    printf("%d + %d = %d\n", integer2, integer1, sum);

    return 0;
}

