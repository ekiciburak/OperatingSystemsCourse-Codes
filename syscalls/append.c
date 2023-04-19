#include "stdio.h"
#include "stdlib.h"     // malloc
#include "unistd.h"     // read, write, etc
#include "fcntl.h"      // open, close, link, unlink, etc
#include "sys/stat.h"   // mode_t, etc

#define size 1024

void concat(char *, char *, char *, int, int);
int length(char *);

int main(int argc, char const *argv[])
{
    int fd1, fd2;
    int i = 0;
    int l1, l2;

    mode_t mode = S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR;
    int flag1   = O_RDONLY;
    int flag2   = O_CREAT | O_WRONLY | O_TRUNC;
    
    char *p = malloc(size*sizeof(char));
    char *q = malloc(size*sizeof(char));

    printf("opening \"%s\"...\n", argv[1]);
    fd1 = open(argv[1], flag1, mode);
    printf("opening \"%s\"...\n", argv[2]);
    fd2 = open(argv[2], flag1, mode);

    while (read(fd1, p, 1) != 0)
    {
        p++;
        i++;
    }
    p = p - i;

    i = 0;
    while (read(fd2, q, 1) != 0)
    {
        q++;
        i++;
    }
    q = q - i;
    close(fd1);
    close(fd2);
    printf("closing \"%s\"...\n", argv[1]);
    printf("closing \"%s\"...\n", argv[2]);


    l1 = length(p);
    l2 = length(q);

    char *s = malloc((l1+l2+1)*sizeof(char));

    printf("re-opening \"%s\"...\n", argv[2]);
    fd2 = open(argv[2], flag2, mode);

    if(l2 == 0)
        write(fd2, p, (l1*sizeof(char)));
    else
    {
        concat(s, q, p, l2, l1);
        write(fd2, s, ((l1+l2+1)*sizeof(char)));
    }
    close(fd2);
    printf("closing \"%s\"...\n", argv[2]);
    free(p);
    free(q);
    free(s);

    return 0;
}

int length (char *p)
{
    int i = 0;
    for (i = 0; *(p+i) != '\0' ; i++);
    return i;
}
void concat (char *s, char *p, char *q, int l1, int l2)
{
    int j = 0;
    for (size_t i = 0; i < l1+l2+1; i++)
    {
        if(i < l1)
            *(s+i) = *(p+i);
        else if (i == l1)
            *(s+i) = '\n';
        else
        {
            *(s+i) = *(q+j);
            j++;
        }
    }
    
}
