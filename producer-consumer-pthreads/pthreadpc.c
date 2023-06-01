#include "stdio.h"
#include "stdlib.h"        // malloc
#include "time.h"
#include "wait.h"          // waitpid
#include "unistd.h"        // fork
#include "stdbool.h"       // true, false
#include "semaphore.h"     // sem_t
#include "pthread.h"

#define size 15
#define NUMOFPROD 5
#define NUMOFCONS 5

struct circular
{
    int in;
    int out;
    int buffer[size];
    int counter;
    sem_t empty;
    sem_t full;
    sem_t lock;
};

typedef struct circular CA;

CA a;

void initCA(void);
void addCA(int *);
void dropCA(int *);
void printCA(char, int);

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    pthread_t prod[NUMOFPROD], cons[NUMOFCONS];
    int idp[NUMOFPROD];
    int idc[NUMOFCONS];

    initCA();

    for (size_t i = 0; i < NUMOFPROD; i++)
    {
        idp[i] = i;
        pthread_create(&prod[i], NULL, (void *) addCA, &idp[i]);
    }

    for (size_t i = 0; i < NUMOFCONS; i++)
    {
        idc[i] = i;
        pthread_create(&cons[i], NULL, (void *) dropCA, &idc[i]);
    }


    for (size_t i = 0; i < NUMOFPROD; i++)
        pthread_join(prod[i], NULL);
    
    for (size_t i = 0; i < NUMOFCONS; i++)
        pthread_join(cons[i], NULL);


    return 0;
}

void initCA()
{
    a.in = 0;
    a.out = 0;
    for (size_t i = 0; i < size; i++)
        a.buffer[i] = 0;
    a.counter = 0;
    sem_init(&a.empty, 1, size);
    sem_init(&a.full, 1, 0);
    sem_init(&a.lock, 1, 1);
}

void addCA(int *identity)
{
    int id = *identity;
    int next;

    while (true)
    {
        sleep(1);
        sem_wait(&a.empty);
        sem_wait(&a.lock);
        next = (rand() % 9) + 1;
        a.buffer[a.in] = next;
        a.in = (a.in + 1) % size;
        a.counter++;
        printCA('p', id);
        sem_post(&a.lock);
        sem_post(&a.full);
    }
    
}

void dropCA(int *identity)
{
    int id = *identity;
    int prev;

    while (true)
    {
        sleep(10);
        sem_wait(&a.full);
        sem_wait(&a.lock);
        prev = a.buffer[a.out];
        a.buffer[a.out] = 0;
        a.out = (a.out + 1) % size;
        a.counter--;
        printCA('c', id);
        sem_post(&a.lock);
        sem_post(&a.empty);
    }

}

void printCA(char c, int k)
{
    printf("%c%d -- {", c, k);
    for (size_t i = 0; i < size; i++)
    {
        if(i < size - 1)
            printf("%d,", a.buffer[i]);
        else
            printf("%d", a.buffer[i]);
    }
    printf("} -- (%d,%d) -- %d\n", a.in, a.out, a.counter);
}
