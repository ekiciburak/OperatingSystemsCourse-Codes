#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "stdbool.h"

#define NUMOFP 5

struct board
{
    int lock;
    bool waiting[NUMOFP];
};

typedef struct board BOARD;
BOARD b;

void initBoard(void);
void processes(int *);
bool compare_and_swap(int *, int, int);

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
    b.lock = 0;
    for (size_t i = 0; i < NUMOFP; i++)
        b.waiting[i] = false;
    
}

bool compare_and_swap(int *value, int expected, int new_value)
{
    int temp = *value;
    if(*value == expected)
        *value = new_value;
    return temp;
}

void processes(int *identity)
{   
    int id = *identity;
    int next;
    int key;

    while(true)
    {
        printf("process %d is waiting -- lock value = %d\n", id, b.lock);
        b.waiting[id] = true;
        key = 1;

        while (b.waiting[id] && key == 1)
            key = compare_and_swap(&b.lock, 0, 1);
        b.waiting[id] = false;

        printf("process %d is in CS\n", id);
        sleep(1);

        printf("process %d is in RS\n", id);
        sleep(1);
        
        next = (id + 1) % NUMOFP;
        while ((next != id) && !b.waiting[next])
            next = (next + 1) % NUMOFP;
        if(next == id)
            b.lock = 0;
        else
            b.waiting[next] = false;
    }    
}