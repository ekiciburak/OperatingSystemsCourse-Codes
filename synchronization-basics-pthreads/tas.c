#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "stdbool.h"
#include "stdatomic.h"

struct board
{
   bool lock;
};

typedef struct board BOARD;

void initBoard(BOARD *);
bool test_and_set(bool *);
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
    b->lock = false;
}

bool test_and_set(bool *target)
{
    bool rv = *target;
    *target = true;
    return rv;
}

void thread1(BOARD *b)
{
    do
    {
        printf("t1 is waiting -- (%d)\n", b->lock);
        while (test_and_set(&b->lock));
        printf("t1 is in CS -- (%d)\n", b->lock);
        sleep(1);
        printf("t1 is in RS -- (%d)\n", b->lock);
        b->lock = false;
        sleep(1);
    } while (true); 
}

void thread2(BOARD *b)
{
    do
    {
        printf("t2 is waiting -- (%d)\n", b->lock);
        while (test_and_set(&b->lock));
        printf("t2 is in CS -- (%d)\n", b->lock);
        sleep(1);
        printf("t2 is in RS -- (%d)\n", b->lock);
        b->lock = false;
        sleep(1);  
    } while (true); 
}