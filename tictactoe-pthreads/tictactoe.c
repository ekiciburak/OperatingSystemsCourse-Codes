#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "semaphore.h"
#include "pthread.h"

#define size 3

struct board
{
    char buffer[size][size];
    bool gameon;
    sem_t empty;
    sem_t full;
};

typedef struct board BRD;

void initBoard(BRD *);
void printBoard(char, BRD *, int, int);
bool isFull(BRD *);
bool isWinner(char, BRD *);
void player1(BRD *);
void player2(BRD *);

int main(int argc, char const *argv[])
{
    BRD *b = malloc(sizeof(BRD));
    initBoard(b);
    printBoard(' ', b, size, size);

    pthread_t p1, p2;

    pthread_create(&p1, NULL, (void *) player1, b);
    pthread_create(&p2, NULL, (void *) player2, b);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    
    //printf("I am here\n");

    return 0;
}

void initBoard(BRD *b)
{
    (*(b)).gameon = true;
    sem_init(&(*(b)).empty, 1, 1);
    sem_init(&(*(b)).full, 1, 0);
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            (*(b)).buffer[i][j] = '-';
    }
}

void printBoard(char c, BRD *b, int row, int col)
{
    printf("\t\t");
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
            printf("%c ", (*(b)).buffer[i][j]);
        printf("\n\t\t");
    }
    printf("----------- %c --- (%d,%d)\n", c, row, col);
}

bool isFull(BRD *b)
{
    bool v = true;
    for (size_t i = 0; i < size && v; i++)
    {
        for (size_t j = 0; j < size && v; j++)
            v = ((*(b)).buffer[i][j] != '-') && v;
    }
    return v;
}

bool isWinner(char c, BRD *b)
{
    bool v1 = true;
    bool v2 = true;
    bool v3 = true;
    bool v4 = true;
    bool v5 = true;

    for (size_t i = 0; i < size && v3; i++)
    {
        v1 = true;
        v2 = true;
        for (size_t j = 0; j < size && (v1 || v2); j++)
        {
            v1 = ((*(b)).buffer[i][j] == c) && v1;
            v2 = ((*(b)).buffer[j][i] == c) && v2;
        }
        if(v1 || v2)
            v3 = false;
    }

    for (size_t i = 0; i < size; i++)
    {
        v4 = ((*(b)).buffer[i][i] == c) && v4;
        v5 = ((*(b)).buffer[i][size-i-1] == c) && v5;
    }

    return(v1 || v2 || v4 || v5);
}

void player1(BRD *b)
{
    int row, col;

    while ((*(b)).gameon)
    {
        sem_wait(&(*(b)).empty);
        if(!(*(b)).gameon)
            pthread_exit(NULL);

        printf("player 1 row: ");
        scanf("%d", &row);
        printf("player 1 col: ");
        scanf("%d", &col);

        while (((*(b)).buffer[row][col] != '-' && !isFull(b)) || col < 0 || row < 0 || col >= size || row >= size)
        {
            printf("invalid move by player 1; please re-enter\n");
            printf("player 1 row: ");
            scanf("%d", &row);
            printf("player 1 col: ");
            scanf("%d", &col);
        }

        (*(b)).buffer[row][col] = 'X';

        (*(b)).gameon = !isWinner('X', b);
        if(!(*(b)).gameon)
        {
            printBoard('1', b, row, col);
            printf("\n\t ***** player 1 wins! congrats! ***** \n");
            sem_post(&(*(b)).full);
            pthread_exit(NULL);
        }

        (*(b)).gameon = !isFull(b);
        if(!(*(b)).gameon)
        {
            printBoard('1', b, row, col);
            printf("\n\t no winner; draw game!\n");
            sem_post(&(*(b)).full);
            pthread_exit(NULL);
        }
        
        printBoard('1', b, row, col);
        sem_post(&(*(b)).full);
    }
}

void player2(BRD *b)
{
    int row, col;

    while ((*(b)).gameon)
    {
        sem_wait(&(*(b)).full);
        if(!(*(b)).gameon)
            pthread_exit(NULL);

        printf("player 2 row: ");
        scanf("%d", &row);
        printf("player 2 col: ");
        scanf("%d", &col);

        while (((*(b)).buffer[row][col] != '-' && !isFull(b)) || col < 0 || row < 0 || col >= size || row >= size)
        {
            printf("invalid move by player 2; please re-enter\n");
            printf("player 2 row: ");
            scanf("%d", &row);
            printf("player 2 col: ");
            scanf("%d", &col);
        }

        (*(b)).buffer[row][col] = 'O';

        (*(b)).gameon = !isWinner('O', b);
        if(!(*(b)).gameon)
        {
            printBoard('2', b, row, col);
            printf("\n\t ***** player 2 wins! congrats! ***** \n");
            sem_post(&(*(b)).empty);
            pthread_exit(NULL);
        }

        (*(b)).gameon = !isFull(b);
        if(!(*(b)).gameon)
        {
            printBoard('2', b, row, col);
            printf("\n\t no winner; draw game!\n");
            sem_post(&(*(b)).empty);
            pthread_exit(NULL);
        }
        
        printBoard('2', b, row, col);
        sem_post(&(*(b)).empty);
    }
}