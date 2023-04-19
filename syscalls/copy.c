#include "stdio.h"
#include "stdlib.h"     // malloc
#include "unistd.h"     // read, write, etc
#include "fcntl.h"      // open, close, link, unlink, etc
#include "sys/stat.h"   // mode_t, etc

#define size 1024

int main(int argc, char const *argv[])
{
    int fd1, fd2;
    int i = 1;

    mode_t mode = S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR;
    int flag1   = O_RDONLY;
    int flag2   = O_CREAT | O_WRONLY | O_APPEND;

    printf("%d\n", flag1);
    printf("%d\n", flag2);

    char *p = malloc(size*sizeof(char));

    fd1 = open(argv[1], flag1, mode);
    fd2 = open(argv[2], flag2, mode);

    while (read(fd1, p, 1) != 0)
        write(fd2, p, 1);

    write(fd2, "\n", 1);
    close(fd1);
    close(fd2);
    free(p);
    return 0;
}
