#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "stdbool.h"

#define NUMOFP 2

struct board
{
    int turn;
    bool flag[NUMOFP];
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
    for (size_t i = 0; i < NUMOFP; i++)
        b.flag[i] = false;
}

void processes(int *identity)
{
    int id = *identity;
    int next;
    while (true)
    {
        printf("process %d is waiting\n", id);
        b.flag[id] = true;
        next = (id+1) % NUMOFP;
        b.turn = next;
        while (b.flag[next] && b.turn == next);

        /* critical section */
        printf("process %d is in CS\n", id);
        sleep(1);

        /* reminder section */
        printf("process %d is in RS\n", id);
        b.flag[id] = false;
        sleep(1);
    }
    
}