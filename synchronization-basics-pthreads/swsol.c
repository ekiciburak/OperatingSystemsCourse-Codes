#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "stdbool.h"

#define NUMOFP 5

struct board
{
    int turn;
};

typedef struct board BOARD;

BOARD b;

void initBoard(void);
void processes(int *);

int main(int argc, char const *argv[])
{
    initBoard();
    pthread_t t[NUMOFP];

    int id[NUMOFP];

    for (size_t i = 0; i < NUMOFP; i++)
    {
        id[i] = i;
        pthread_create(&t[i], NULL, (void *) processes, &id[i]);
    }

    for (size_t i = 0; i < NUMOFP; i++)
        pthread_join(t[i], NULL);

    return 0;
}

void initBoard(void)
{
    b.turn = 0;
}

void processes(int *identity)
{
    int id = *identity;

    while (true)
    {
        printf("process %d is waiting\n", id);
        while (b.turn != id);

        /* critical section */
        printf("process %d is in CS\n", id);
        sleep(1);

        /* reminder section */
        printf("process %d is in RS\n", id);
        b.turn = (b.turn + 1) % NUMOFP;
        sleep(1);
    }
    
}