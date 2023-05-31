#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "pthread.h"
#include "semaphore.h"

#define NUMOP 5
#define NUMOR 3

struct board
{
    sem_t th[NUMOP], bk[NUMOP];
    pthread_mutex_t m;
    int available[NUMOR];
    int max[NUMOP][NUMOR];
    int allocation[NUMOP][NUMOR];
    int need[NUMOP][NUMOR];
    int total[NUMOR];
    bool finish[NUMOP];
    int proc[NUMOR];
};

typedef struct board BOARD;
BOARD b;

void initBoard(void);
void threads(int *);
void banker(void *);
void printState(void);
bool isSafe(void);
bool needLtAvailable(int id);
void printArray(void);

int main(int argc, char const *argv[])
{
    pthread_t t[NUMOP], bt;
    int id[NUMOP];
    
    initBoard();

    for (size_t i = 0; i < NUMOP; i++)
    {
        id[i] = i;
        pthread_create(&t[i], NULL, (void *) threads, &id[i]);
    }

    pthread_create(&bt, NULL, (void *) banker, NULL);  

    for (size_t i = 0; i < NUMOP; i++)
        pthread_join(t[i], NULL);

    pthread_join(bt, NULL);

    return 0;
}

void initBoard(void)
{
    for (size_t i = 0; i < NUMOP; i++)
    {
        sem_init(&b.th[i],1,1);
        sem_init(&b.bk[i],1,0);
        b.total[i] = 0;
        b.finish[i] = false;
    }
    pthread_mutex_init(&b.m, NULL);

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

    for (size_t i = 0; i < NUMOP; i++)
    {
        b.total[i] = 0;
    }
    pthread_mutex_init(&b.m, NULL);

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


void threads(int *identity)
{
    int id = *identity;

    while (true)
    {
        sem_wait(&b.th[id]);
        srand(time(NULL) * id);
        printf("thread %d is requesting...\n", id);
        for (size_t i = 0; i < NUMOR; i++)
        {
            if(b.max[id][i] == 0)
                b.allocation[id][i] = 0;
            else
                b.allocation[id][i] = rand() % (b.max[id][i]);
        }
        sleep(1);
        sem_post(&b.bk[id]);
    }
}

void banker(void *unused)
{
    while (true)
    {
        for (size_t i = 0; i < NUMOP; i++)
            sem_wait(&b.bk[i]);
        printf("banker is checking...\n");

        printf(" Allocation       Max          Needed        Available\n");
        for (size_t i = 0; i < NUMOR; i++)
        {
            for (size_t j = 0; j < NUMOP; j++)
                b.total[i] = b.total[i] + b.allocation[j][i];
        }        

        for (size_t i = 0; i < NUMOR; i++)
        {
            b.available[i] = b.available[i] - b.total[i];
        }

        for (size_t i = 0; i < NUMOP; i++)
        {
            for (size_t j = 0; j < NUMOR; j++)
                b.need[i][j] = b.max[i][j] - b.allocation[i][j];            
        }

        printState();
        bool v = isSafe();
        if(v)
        {
            printf("the state is safe with the pid order: ");
            printArray();
        }
        else
            printf("the state is unsafe\n");

        refreshBoard();

        sleep(1);
        for (size_t i = 0; i < NUMOP; i++)
            sem_post(&b.th[i]);     
    }    
}

void printState(void)
{
    bool v = true;
    for (size_t i = 0; i < NUMOP; i++)
    {
        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%d   ", b.allocation[i][j]);
            else
                printf("%d   |  ", b.allocation[i][j]);
        }
        
        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%d   ", b.max[i][j]);
            else
                printf("%d   |  ", b.max[i][j]);
        }

        for (size_t j = 0; j < NUMOR; j++)
        {
            if(j < NUMOR - 1)
                printf("%-2d  ", b.need[i][j]);
            else
                printf("%-2d  |  ", b.need[i][j]);
        }
        
        for (size_t k = 0; k < NUMOR && v; k++)
            printf("%d   ", b.available[k]);
    
        v = false;
        printf("\n");        
    }
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

bool isSafe(void)
{
    bool v1 = true;
    int m = 0;
    for (size_t j = 0; j < NUMOP; j++)   
    {
        v1 = true;

        for (size_t i = 0; i < NUMOP; i++)
        {           
            if(b.finish[i] == false && needLtAvailable(i))
            {
                for (size_t k = 0; k < NUMOR; k++)
                {
                    b.available[k] = b.available[k] + b.allocation[i][k];
                    b.finish[i] = true;
                }
                b.proc[m] = i;
                m++;
            }
        }

        for (size_t i = 0; i < NUMOP; i++)
        {
            if(b.finish[i] == true)
                v1 = v1 && true;
            else
                v1 = false;
        }
    }   
    return v1;
}

void printArray(void)
{
    printf("<");
    for (size_t i = 0; i < NUMOP; i++)
    {
        if(i < NUMOP-1)
        {
            printf("t%d,", b.proc[i]);
        }
        else
            printf("t%d", b.proc[i]);
    }
    printf(">\n");
    
}