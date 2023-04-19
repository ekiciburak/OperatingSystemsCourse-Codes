#include "stdio.h"
#include "stdlib.h"        // malloc
#include "time.h"
#include "wait.h"          // waitpid
#include "unistd.h"        // fork
#include "stdbool.h"       // true, false
#include "semaphore.h"     // sem_t
#include "pthread.h"

#define size 5
#define psize 4096

struct circular
{
    int in;
    int out;
    int buffer[size];
    int counter;
    sem_t empty;
    sem_t full;
};

typedef struct circular CA;

void initCA(CA *);
void addCA(CA *);
void dropCA(CA *);
void printCA(char, CA *);

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    CA *a = malloc(sizeof(CA));
    initCA(a);

    pthread_t prod, cons;

    pthread_create(&prod, NULL, (void *) addCA, a);
    pthread_create(&prod, NULL, (void *) dropCA, a);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

void initCA(CA *a)
{
    (*(a)).in = 0;
    (*(a)).out = 0;
    for (size_t i = 0; i < size; i++)
        (*(a)).buffer[i] = 0;
    (*(a)).counter = 0;
    sem_init(&(*(a)).empty, 1, 5);
    sem_init(&(*(a)).full, 1, 0);
}

void addCA(CA *a)
{
    int next;

    while (true)
    {
        sleep(5);
        //while(((*(a)).in + 1) % size == (*(a)).out); 
        //while((*(a)).counter == size);
        /*
            sem_wait(S)
            {
                while(S <= 0);
                S--;
            }
        */
        sem_wait(&(*(a)).empty);
        next = (rand() % 9) + 1;
        (*(a)).buffer[(*(a)).in] = next;
        (*(a)).in = ((*(a)).in + 1) % size;
        (*(a)).counter++;
        printCA('p', a);
        sem_post(&(*(a)).full);
        /*
            sem_post(S)
            {
                S++;
            }
        */
    }
    
}

void dropCA(CA *a)
{
    int prev;

    while (true)
    {
        sleep(1);
        //while((*(a)).in == (*(a)).out);
        //while((*(a)).counter == 0);
        sem_wait(&(*(a)).full);
        prev = (*(a)).buffer[(*(a)).out];
        (*(a)).buffer[(*(a)).out] = 0;
        (*(a)).out = ((*(a)).out + 1) % size;
        (*(a)).counter--;
        printCA('c', a);
        sem_post(&(*(a)).empty);
    }

}

void printCA(char c, CA *a)
{
    printf("%c -- {", c);
    for (size_t i = 0; i < size; i++)
    {
        if(i < size - 1)
            printf("%d,", (*(a)).buffer[i]);
        else
            printf("%d", (*(a)).buffer[i]);
    }
    printf("} -- (%d,%d) -- %d\n", (*(a)).in, (*(a)).out, (*(a)).counter);
}
