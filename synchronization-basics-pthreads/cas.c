#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "stdbool.h"
#include "stdatomic.h"

struct board
{
    int lock;
};

typedef struct board BOARD;

void initBoard(BOARD *);
int compare_and_swap(int *, int, int);
void thread1(BOARD *);
void thread2(BOARD *);

int main(int argc, char const *argv[])
{
    BOARD *b = malloc(sizeof(BOARD));
    initBoard(b);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, (void *) thread1, b);
    pthread_create(&t2, NULL, (void *) thread2, b);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

void initBoard(BOARD *b)
{
    b->lock = 0;
}

int compare_and_swap(int *value, int expected, int new_value)
{
    int temp = *value;

    if(*value == expected)
        *value = new_value;
    
    return temp;
}

void thread1(BOARD *b)
{
    while (true)
    {
        printf("t1 is waiting -- (%d)\n", b->lock);
        while (compare_and_swap(&b->lock,0,1) != 0);
        printf("t1 is in CS -- (%d)\n", b->lock);
        sleep(1);
        printf("t1 is in RS -- (%d)\n", b->lock);        
        b->lock = 0;
        sleep(1);
    }
     
}

void thread2(BOARD *b)
{
    while (true)
    {
        printf("t2 is waiting -- (%d)\n", b->lock);
        while (compare_and_swap(&b->lock,0,1) != 0);
        printf("t2 is in CS -- (%d)\n", b->lock);
        sleep(1);
        printf("t2 is in RS -- (%d)\n", b->lock);        
        b->lock = 0;
        sleep(1);       
    }
}