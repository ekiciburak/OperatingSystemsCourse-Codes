#include "stdio.h"

int main(int argc, char const *argv[])
{
    char a = 'b';
    void *p;

    p = &a;

    printf("memory address of p = %p\n", &p);
    printf("memory address of a = %p\n", &a);

    printf("content of p = %p\n", p);
    printf("content of a = %c\n", a);
    printf("content of *p = %c\n", *((char *)p));

    void **q;
    q = &p;

    printf("memory address of q = %p\n", &q);
    printf("content of q = %p\n", q);
    printf("content of *q = %p\n", *((char **)q));
    printf("content of **q = %c\n", **((char **)q));

    printf("%ld\n", sizeof(char *));
    printf("%ld\n", sizeof(int *));

    return 0;
}
