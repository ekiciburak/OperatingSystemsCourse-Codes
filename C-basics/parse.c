#include "stdio.h"  //printf, fgets, etc.
#include "string.h" //strtok_r 

#define size 5
#define max  512

int parse (char *, char *[size][size]);
void printMatrix (int, char *[size][size]);

int main(int argc, char const *argv[])
{
    char buf[max];
    char *line;
    char *a[size][size] = {NULL};
    int row;

    printf("$ ");
    line = fgets(buf, sizeof(buf), stdin);

    row = parse(line, a);
    printMatrix(row-1, a);

    return 0;
}

int parse (char *line, char *a[size][size])
{
    char *a1, *a2, *rest1, *rest2;
    int i = 0, n = 0;

    a1 = strtok_r(line, ";", &rest1);
    while (a1 != NULL)
    {
        a2 = strtok_r (a1, ",", &rest2);
        while (a2 != NULL)
        {
            a[i][n] = a2;
            a2 = strtok_r (rest2, ",", &rest2);
            n++;
        }        
        a1 = strtok_r(rest1, ";", &rest1);
        i++;
        n = 0;
    }
    return i;
}

/*
line = "irem,kizilirmak;msku,ceng;2023;"
a1 = "irem,kizilirmak"
    a2 = "irem"
    rest2 = "kizilirmak"
rest1 = "msku,ceng;2023;"
a1 = "msku,ceng"
    a2 = "msku"
    rest2 = "ceng"
rest1 = "2023;"
a1 = "2023;"
    a2 = "2023"
    rest2 = ""

[
    "irem"   "kizilirmak"
    "msku"   "ceng"
    "2023"   
]
*/

void printMatrix(int n, char *a[size][size])
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; a[i][j] != NULL; j++)
        {
            printf("%s ", a[i][j]);
        }
        printf("\n");
    }
    
}