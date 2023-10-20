#include "stdio.h"
#include "stdlib.h"       //malloc
#include "unistd.h"
#include "time.h"         //time
#include "stdbool.h"      //true
#include "semaphore.h"    //sem_t, sem_init, etc
#include "pthread.h"      //threads

#define size 10
#define psize 4096

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

void initCA(CA *);
void addCA(CA *);
void dropCA(CA *);
void printCA(CA *, char);

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    CA *a = malloc(sizeof(CA));
    initCA(a);

    pthread_t prod, cons;

    pthread_create(&prod, NULL, (void *) addCA , a);
    pthread_create(&cons, NULL, (void *) dropCA, a);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

void initCA(CA *a)
{
    (*(a)).in  = 0;
    (*(a)).out = 0;
    (*(a)).counter = 0;
    for (size_t i = 0; i < size; i++)
        (*(a)).buffer[i] = 0;
    sem_init(&(*(a)).empty, 1, 5);
    sem_init(&(*(a)).full, 1, 0);
    pthread_mutex_init(&a->mut, NULL);
}

void addCA(CA *a)
{
    int next;
    while (true)
    {
        sem_wait(&(*(a)).empty);
        pthread_mutex_lock(&a->mut);
        next = (rand() % 8) + 1;
        (*(a)).buffer[(*(a)).in] = next;
        (*(a)).in = ((*(a)).in + 1) % size;    
        (*(a)).counter++;
        printCA(a, 'p');
        pthread_mutex_unlock(&a->mut);
        sem_post(&(*(a)).full);
    }
    pthread_exit(0);

}

void dropCA(CA *a)
{
    int prev;
    while (true)
    {
        sem_wait(&(*(a)).full);
        pthread_mutex_lock(&a->mut);
        prev = (*(a)).buffer[(*(a)).out];
        (*(a)).buffer[(*(a)).out] = 0;
        (*(a)).out = ((*(a)).out + 1) % size;
        (*(a)).counter--;
        printCA(a, 'c');
        pthread_mutex_unlock(&a->mut);
        sem_post(&(*(a)).empty);
    }
    pthread_exit(0);
}

void printCA(CA *a, char c)
{
    printf("%c  -- {", c);
    for (size_t i = 0; i < size; i++)
    {
        if(i < size - 1)
            printf("%d,", (*(a)).buffer[i]);
        else
            printf("%d", (*(a)).buffer[i]);
    }
    printf("} -- (%d, %d) -- %d\n", (*(a)).in, (*(a)).out, (*(a)).counter);
    
}