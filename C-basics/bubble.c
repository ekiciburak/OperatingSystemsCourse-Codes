#include "stdio.h"

#define size 10

void bSort (int []);
void printArray (int []);


int main (void)
{
    int a[size] = {4,8,6,-5,9,-6,1,3,7,10};

    bSort(a);
    printArray(a);

    return 0;
}

void bSort(int b[size])
{
    int temp; 

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size-i-1; j++)
        {
            if(b[j] > b[j+1])
            {
                temp   = b[j];
                b[j]   = b[j+1];
                b[j+1] = temp;
            }
        }
    }
}

// [1,2,3]

void printArray(int a[size])
{
    printf("[");
    for (size_t i = 0; i < size; i++)
    {
        if (i < size-1)
            printf("%d,", a[i]);
        else
            printf("%d", a[i]);
    }
    printf("]\n");
}