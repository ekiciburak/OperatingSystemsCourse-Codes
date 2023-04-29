#include "stdio.h"
#include "stdlib.h"       // malloc
#include "stdbool.h"      // true, false
#include "semaphore.h"    // sem_t
#include "pthread.h"      // pthread_t
#include "unistd.h"       // sleep

#define size 3
#define winner 15

struct board
{
    int buffer[size][size];
    bool gameon;
    sem_t empty;
    sem_t full;
    sem_t ref;
};

typedef struct board BRD;

void initBoard(BRD *);
void printBoard(int, BRD *, int, int);
bool isFull(BRD *);
bool isWinner(BRD *);
void player1(BRD *);
void player2(BRD *);
void referee(BRD *);
bool isValid(int, BRD *);


int main(int argc, char const *argv[])
{
    BRD *p = malloc(sizeof(BRD));
    initBoard(p);
    printBoard(15, p, size, size);

    pthread_t p1, p2, r;

    pthread_create(&r,  NULL, (void *) referee, p);
    pthread_create(&p1, NULL, (void *) player1, p);
    pthread_create(&p2, NULL, (void *) player2, p);

    pthread_join(r, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("we are all back and joint with the main thread, now terminating...\n");

    return 0;
}

void initBoard(BRD *b)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            (*(b)).buffer[i][j] = 0;
    }
    sem_init(&(*(b)).ref, 1, 1);
    sem_init(&(*(b)).empty, 1, 0);
    sem_init(&(*(b)).full, 1, 0);
    (*(b)).gameon = true;
}

void printBoard(int c, BRD *b, int row, int col)
{
    printf("\t\t");
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {   
            printf("%d ", (*(b)).buffer[i][j]);
        }
        printf("\n\t\t");
    }
    printf("---------------%d -- (%d, %d)\n", c, row, col);
}

bool isValid(int val, BRD *b)
{
    bool v = true;
    for (size_t i = 0; (i < size) && v; i++)
    {
        for (size_t j = 0; (j < size) && v; j++)
        {
            v = ((*(b)).buffer[i][j] != val) && v;
        }
    }
    return v;
}

bool isFull(BRD *b)
{
    bool v = true;
    for (size_t i = 0; (i < size) && v; i++)
    {
        for (size_t j = 0; (j < size) && v; j++)
        {
            v = ((*(b)).buffer[i][j] != 0) && v;
        }
    }
    return v;
}

bool isWinner(BRD *p)
{
    bool v1 = true;
    bool v2 = true;
    bool v3 = true;
    bool v4 = true;
    bool v5 = true;
    int i1 = 0;
    int i3 = 0;
    int i4 = 0;
    int i5 = 0;

    for (size_t i = 0; i < size && v1; i++)
    {
        i1 = 0;
        i3 = 0;
        v2 = true;
        v3 = true;
        for (size_t j = 0; j < size && (i1 != winner || i3 != winner); j++)
        {
            i1 = (*(p)).buffer[i][j] + i1;
            i3 = (*(p)).buffer[j][i] + i3;
            v2 = ((*(p)).buffer[i][j] != 0) && v2;
            v3 = ((*(p)).buffer[j][i] != 0) && v3;
        }
        if((i1 == winner && v2) || (i3 == winner && v3))
            v1 = false;
    }

    for (size_t i = 0; i < size; i++)
    {
        i4 = (*(p)).buffer[i][i] + i4;
        i5 = (*(p)).buffer[i][(size-i-1)] + i5;
        v4 = ((*(p)).buffer[i][i] != 0) && v4;
        v5 = ((*(p)).buffer[i][size-i-1] != 0) && v5;
    }

    return ((i1 == winner && v2) || (i3 == winner && v3) || (i4 == winner && v4) || (i5 == winner && v5));
}

void player1(BRD *p)
{
    int row, col, val;
    while ((*(p)).gameon)
    {
        sem_wait(&(*(p)).empty);
        if(!(*(p)).gameon)
        { 
            pthread_exit(NULL);
        }

        printf("player 1 row: ");
        scanf("%d", &row);
        printf("player 1 col: ");
        scanf("%d", &col);
        printf("player 1 val: ");
        scanf("%d", &val); 
        while (((*(p)).buffer[row][col] != 0 && !isFull(p)) || !isValid(val, p) || col < 0 || row < 0 || col >= size || row >= size || val >= 10 || val <= 0)
        {
            printf("invalid choice by player 1, please reenter\n");
            printf("player 1 row: ");
            scanf("%d", &row);
            printf("player 1 col: ");
            scanf("%d", &col);
            printf("player 1 val: ");
            scanf("%d", &val);       
        }

        (*(p)).buffer[row][col] = val;

        (*(p)).gameon = !isWinner(p);
        if(!(*(p)).gameon)
        {   
            printBoard(1, p, row, col);       
            printf("***** player 1 wins! congrats *****\n");
            sem_post(&(*(p)).full); 
            sem_post(&(*(p)).ref); 
            pthread_exit(NULL);
        }
        
        (*(p)).gameon = !isFull(p);
        if(!(*(p)).gameon)
        {   
            printBoard(1, p, row, col);       
            printf("***** draw game! *****\n");
            sem_post(&(*(p)).full); 
            sem_post(&(*(p)).ref); 
            pthread_exit(NULL);
        }
        
        printBoard(1, p, row, col);       
        sem_post(&(*(p)).ref);
    }

}

void player2(BRD *p)
{
    int row, col, val;
    while ((*(p)).gameon)
    {
        sem_wait(&(*(p)).full);
        if(!(*(p)).gameon)
        { 
            pthread_exit(NULL);
        }

        printf("player 2 row: ");
        scanf("%d", &row);
        printf("player 2 col: ");
        scanf("%d", &col);
        printf("player 2 val: ");
        scanf("%d", &val);  
        while (((*(p)).buffer[row][col] != 0 && !isFull(p)) || !isValid(val, p) || col < 0 || row < 0 || col >= size || row >= size || val >= 10 || val <= 0)
        {
            printf("invalid choice by player 2, please reenter\n");
            printf("player 2 row: ");
            scanf("%d", &row);
            printf("player 2 col: ");
            scanf("%d", &col);     
            printf("player 2 val: ");
            scanf("%d", &val);       
        }

        (*(p)).buffer[row][col] = val;

        (*(p)).gameon = !isWinner(p);
        if(!(*(p)).gameon)
        {   
            printBoard(2, p, row, col);       
            printf("***** player 2 wins! congrats *****\n");
            sem_post(&(*(p)).empty); 
            sem_post(&(*(p)).ref); 
            pthread_exit(NULL);
        }
        
        (*(p)).gameon = !isFull(p);
        if(!(*(p)).gameon)
        {   
            printBoard(2, p, row, col);       
            printf("***** draw game! *****\n");
            sem_post(&(*(p)).empty); 
            sem_post(&(*(p)).ref); 
            pthread_exit(NULL);
        }        

        printBoard(2, p, row, col);
        sem_post(&(*(p)).ref); 
    } 
}

void referee(BRD *p)
{
    int token;
    while ((*(p)).gameon)
    {
        sem_wait(&(*(p)).ref);
        if(!(*(p)).gameon)
        { 
            pthread_exit(NULL);
        }

        srand(time(NULL));
        token = (rand() % 99) + 1;

        if (token % 2 == 0)
        {   
            printf("ref is talking: token is %d therefore player %d makes the next move\n", token, (token % 2 + 2));
            sleep(1);
            sem_post(&(*(p)).full);
        }
        else
        {
            printf("ref is talking: token is %d therefore player %d makes the next move\n", token, (token % 2));
            sleep(1);
            sem_post(&(*p).empty);
        }
    }
}