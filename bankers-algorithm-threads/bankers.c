#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "semaphore.h"
#include "pthread.h"
#include "stdbool.h"

#define NUMOT 5
#define NUMOR 3

struct board
{
    sem_t tsem[NUMOT];
    sem_t bsem[NUMOT];
    int available[NUMOR];
    int max[NUMOT][NUMOR];
    int allocation[NUMOT][NUMOR];
    int need[NUMOT][NUMOR];
    int total[NUMOR];
    bool finish[NUMOT];
    int order[NUMOT];
};

typedef struct board BOARD;
BOARD b;

void initBoard(void);
void refreshBoard(void);
void threadsProc(int *);
void bankerProc(void *);
void printState(void);
bool isSafe(void);
bool needLtAvailable(int);
void printArray(void);

int main(int argc, char const *argv[])
{
    pthread_t threads[NUMOT];
    pthread_t banker;

    int id[NUMOT];

    initBoard();

    for (size_t i = 0; i < NUMOT; i++)
    {
        id[i] = i;
        pthread_create(&threads[i], NULL, (void *) threadsProc, &id[i]);
    }

    pthread_create(&banker, NULL, (void *) bankerProc, NULL);

    for (size_t i = 0; i < NUMOT; i++)
        pthread_join(threads[i], NULL);

    pthread_join(banker, NULL);   

    return 0;
}

void initBoard(void)
{
    for (size_t i = 0; i < NUMOT; i++)
    {
        sem_init(&b.tsem[i],1,1);
        sem_init(&b.bsem[i],0,0);
        b.total[i] = 0;
        b.finish[i] = false;
    }
    
    b.available[0] = 10;
    b.available[1] = 5;
    b.available[2] = 7;

    b.max[0][0] = 7;
    b.max[0][1] = 5;
    b.max[0][2] = 3;

    b.max[1][0] = 3;
    b.max[1][1] = 2;
    b.max[1][2] = 2;

    b.max[2][0] = 9;
    b.max[2][1] = 0;
    b.max[2][2] = 2;

    b.max[3][0] = 2;
    b.max[3][1] = 2;
    b.max[3][2] = 2;

    b.max[4][0] = 4;
    b.max[4][1] = 3;
    b.max[4][2] = 3;
}

void refreshBoard(void)
{
    for (size_t i = 0; i < NUMOT; i++)
    {
        b.total[i] = 0;
        b.finish[i] = false;
    }
    
    b.available[0] = 10;
    b.available[1] = 5;
    b.available[2] = 7;

    b.max[0][0] = 7;
    b.max[0][1] = 5;
    b.max[0][2] = 3;

    b.max[1][0] = 3;
    b.max[1][1] = 2;
    b.max[1][2] = 2;

    b.max[2][0] = 9;
    b.max[2][1] = 0;
    b.max[2][2] = 2;

    b.max[3][0] = 2;
    b.max[3][1] = 2;
    b.max[3][2] = 2;

    b.max[4][0] = 4;
    b.max[4][1] = 3;
    b.max[4][2] = 3;
}

void threadsProc(int *identity)
{
    int id = *identity;

    while (true)
    {
        sem_wait(&b.tsem[id]);
        srand(time(NULL) * id);
        printf("thread %d is requesting...\n", id);
        
        for (size_t i = 0; i < NUMOT; i++)
        {
            if(b.max[id][i] == 0)
                b.allocation[id][i] = 0;
            else
                b.allocation[id][i] = rand() % (b.max[id][i]);
        }
        
        sleep(1);
        sem_post(&b.bsem[id]);
    }
}

void bankerProc(void * unused)
{
    while (true)
    {
        for (size_t i = 0; i < NUMOT; i++)
            sem_wait(&b.bsem[i]);
        
        printf("banker is checking...\n");

        for (size_t i = 0; i < NUMOR; i++)
        {
            for (size_t j = 0; j < NUMOT; j++)
                b.total[i] = b.total[i] + b.allocation[j][i];            
        }
        
        for (size_t i = 0; i < NUMOR; i++)
            b.available[i] = b.available[i] - b.total[i];
        
        for (size_t i = 0; i < NUMOT; i++)
        {
            for (size_t j = 0; j < NUMOR; j++)
                b.need[i][j] = b.max[i][j] - b.allocation[i][j];
        }
        
        printState();
        bool v = isSafe();
        if(v)
        {
            printf("the state is safe with tid order: ");
            printArray();
        }
        else
            printf("the state is unsafe\n");

        refreshBoard();
        sleep(1);

        for (size_t i = 0; i < NUMOT; i++)
            sem_post(&b.tsem[i]);
    }
    
}

void printState(void)
{
    bool v = true;
    printf(" Allocation        Max          Needed        Available\n");

    for (size_t i = 0; i < NUMOT; i++)
    {
        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%-2d   ", b.allocation[i][j]);
            else
                printf("%-2d|  ", b.allocation[i][j]);
        }

        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%-2d   ", b.max[i][j]);
            else
                printf("%-2d|  ", b.max[i][j]);
        }

        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%-2d   ", b.need[i][j]);
            else
                printf("%-2d|  ", b.need[i][j]);
        }

        for (size_t k = 0; k < NUMOR && v; k++)
            printf("%-2d   ", b.available[k]);

        v = false;
        printf("\n");
    }
}

bool isSafe(void)
{
    bool v = true;
    int m = 0;

    for (size_t j = 0; j < NUMOT; j++)
    {
        v = true;
        for (size_t i = 0; i < NUMOT; i++)
        {
            if(b.finish[i] == false && needLtAvailable(i))
            {
                for (size_t k = 0; k < NUMOR; k++)
                {
                    b.available[k] = b.available[k] + b.allocation[i][k];
                    b.finish[i] = true;
                }
                b.order[m] = i;
                m++;
            }
        }

        for (size_t i = 0; i < NUMOT; i++)
        {
            if(b.finish[i] == true)
                v = v && true;
            else
                v = false;
        }
    }
    return v;
}

void printArray(void)
{
    printf("<");
    for (size_t i = 0; i < NUMOT; i++)
    {
        if(i < NUMOT - 1)
            printf("t%d,", b.order[i]);
        else
            printf("t%d", b.order[i]);
    }
    printf(">\n");
    
}

bool needLtAvailable(int id)
{
    bool v = true;
    for (size_t i = 0; i < NUMOR && v; i++)
    {
        if(b.need[id][i] <= b.available[i])
            v = v && true;
        else
            v = false;
    }
    return v;
}

