#include "stdio.h"  //printf, scanf, etc
#include "stdlib.h" //malloc

void concat (char **, char **, char **, int, int);
int length(char **);
void printArray (char **);

int main(int argc, char const *argv[])
{
    char *s1[] = { "msku", "2023", NULL };
    char *s2[] = { "operating", "systems", "spring", "term", NULL };

    int l1 = length(s1);
    int l2 = length(s2);

    char **s = malloc((l1+l2+1) * sizeof(char *));

    concat(s, s1, s2, l1, l2);
    printArray(s);
    free(s);

    return 0;
}

void concat(char **s, char **s1, char **s2, int l1, int l2)
{
    int j = 0;

    for (size_t i = 0; i < l1+l2; i++)
    {
        if(i < l1)
            *(s+i) = *(s1+i);
        else
        {
            *(s+i) = *(s2+j);
            j++;   
        }
    }
}

int length(char **s)
{
    int i;
    for (i = 0; *(s+i) != NULL; i++);
    return i;    
}

void printArray(char **s)
{
    int l = length(s);
    printf("{");
    for (size_t i = 0; i < l; i++)
    {
        if(i < l - 1)
            printf("%s,", *(s+i));
        else
            printf("%s", *(s+i));
    }
    printf("}\n");
}