#include "stdio.h"

int main (void)
{
    char c;
    int i;
    double d;
    float f;

    printf("%ld\n", sizeof(c));
    printf("%ld\n", sizeof(i));
    printf("%ld\n", sizeof(d));
    printf("%ld\n", sizeof(f));

    return 0;
}