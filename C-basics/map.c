#include "stdio.h"

#define size 5

int incr (int);
int * map (int (*f) (int), int *);
void printArray(int *);

int mult (int, int);
int fold(int (*f) (int, int), int, int *);


int main(int argc, char const *argv[])
{
    int a[size] = { 10, 5, 3, 8, 6 };
    int *p;

    p = map(incr, a);
    printArray(p);
    int v;
    v = fold(mult, 1, p);
    printf("%d\n", v);

    return 0;
}

int incr(int i)
{
    return(i+1);
}

int mult(int i, int j)
{
    return(i*j);
}

int fold(int (*f) (int, int), int d, int *p)
{
    int result = d;
    for (size_t i = 0; i < size; i++)
    {
        result = f (*(p+i), result);
    }
    return result;
}

int * map(int(*f)(int), int *p)
{
    for (size_t i = 0; i < size; i++)
    {
        *(p+i) = f(*(p+i));
    }
    return p;
}

void printArray(int *p)
{
    printf("{");
    for (size_t i = 0; i < size; i++)
    {
        if(i < size - 1)
            printf("%d,", *(p+i));
        else
            printf("%d", *(p+i));
    }
    printf("}\n");
}