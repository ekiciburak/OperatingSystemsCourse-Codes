#include "stdio.h"
#include "stdlib.h"       //malloc
#include "unistd.h"
#include "time.h"         //time
#include "stdbool.h"      //true
#include "semaphore.h"    //sem_t, sem_init, etc
#include "pthread.h"      //threads

#define size 10
#define psize 4096
#define ptnum 5
#define ctnum 7

struct circular
{
    int in;
    int out;
    int buffer[size];
    int counter;
    sem_t empty;
    sem_t full;
    pthread_mutex_t mut;
};

typedef struct circular CA;

void initCA(void);
void producers(int *);
void consumers(int *);
void printCA(char, int, int);
CA a;

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    initCA();

    pthread_t prod[ptnum], cons[ctnum];
    int idp[ptnum], idc[ctnum];

    for (size_t i = 0; i < ptnum; i++)
    {
        idp[i] = i;
        pthread_create(&prod[i], NULL, (void *) producers, &idp[i]);
    }
    
    for (size_t i = 0; i < ctnum; i++)
    {
        idc[i] = i;
        pthread_create(&cons[i], NULL, (void *) consumers, &idc[i]);
    }  

    for (size_t i = 0; i < ptnum; i++)
        pthread_join(prod[i], NULL);

    for (size_t i = 0; i < ctnum; i++)
        pthread_join(cons[i], NULL);

    return 0;
}

void initCA(void)
{
    a.in = 0;
    a.out = 0;
    a.counter = 0;
    for (size_t i = 0; i < size; i++)
        a.buffer[i] = 0;
    sem_init(&a.empty, 1, 5);
    sem_init(&a.full, 1, 0);
    pthread_mutex_init(&a.mut, NULL);
}

void producers(int *identity)
{
    int id = *identity;
    int next;
    while (true)
    {
        sem_wait(&a.empty);
        //five threads can enter here at a time
        pthread_mutex_lock(&a.mut);
        //only one thread can be here at a time
        next = (rand() % 8) + 1;
        a.buffer[a.in] = next;
        a.in = (a.in + 1) % size;    
        a.counter++;
        printCA('p', id, next);
        pthread_mutex_unlock(&a.mut);
        sem_post(&a.full);
    }
}

void consumers(int *identity)
{
    int id = *identity;
    int prev;

    while (true)
    {
        sem_wait(&a.full);
        //five threads can enter here at a time
        pthread_mutex_lock(&a.mut);
        //only one thread can be here at a time
        prev = a.buffer[a.out];
        a.buffer[a.out] = 0;
        a.out = (a.out + 1) % size;
        a.counter--;
        printCA('c', id, prev);
        pthread_mutex_unlock(&a.mut);
        sem_post(&a.empty);
    }
}

void printCA(char c, int id, int v)
{
    printf("%c%d -- {", c, id);
    for (size_t i = 0; i < size; i++)
    {
        if(i < size - 1)
            printf("%d,", a.buffer[i]);
        else
            printf("%d", a.buffer[i]);
    }
    printf("} -- (%d, %d) -- %d -- value = %d\n", a.in, a.out, a.counter, v);
    
}