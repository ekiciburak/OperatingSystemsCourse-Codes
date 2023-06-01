#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include "stdbool.h"
#include "semaphore.h"
#include "pthread.h"

struct board
{
    char *buffer[2];
    sem_t empty;
    sem_t full;
};

typedef struct board BRD;

void clearBoard(BRD *);
void updateBoard(BRD *, char *, int);
void player1(BRD *);
void player2(BRD *);
void printBoard(BRD *, int);
int  decideHandWinner(BRD *);

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    BRD *p = malloc(sizeof(BRD));
    int w = 0;
    int c1 = 0, c2 = 0;
    bool gameon = true;

    pthread_t p1, p2;

    int n = atoi(argv[1]);

    while(gameon)
    {        
        clearBoard(p);
        
        pthread_create(&p1, NULL, (void *) player1, p);
        pthread_create(&p2, NULL, (void *) player2, p);

        pthread_join(p1, NULL);
        pthread_join(p2, NULL);

        w = decideHandWinner(p);
        printBoard(p, w);
        if(w == 1)
            c1++;
        else if(w == 2)
            c2++;
        
        if((c1+c2 == n && c1 > c2) || c1 > n/2)
        {
            printf("\n\t\t*** player 1 achieves the best of %d with the score: (%d-%d); congrats player 1 ***\n\n", n, c1, c2);
            gameon = false;
        }
        else if((c1+c2 == n && c2 > c1) || c2 > n/2)
        {
            printf("\n\t\t*** player 2 achieves the best of %d with the score: (%d-%d); congrats player 2 ***\n\n", n, c1, c2);
            gameon = false;
        }
        else if((c1+c2 == n && c2 == c1))
        {
            printf("\n\t\t*** no winner in the best of %d with the score: (%d-%d) ***\n\n", n, c1, c2);
            gameon = false;
        }  
        else     
            sleep(1);
    }

    return 0;
}

int decideHandWinner(BRD *b)
{
    int v = 0;
    if((*(b)).buffer[0] == "p1: rock" && (*(b)).buffer[1] == "p2: paper")
        v = 2;
    else if((*(b)).buffer[0] == "p1: rock" && (*(b)).buffer[1] == "p2: scissors")
        v = 1;
    else if((*(b)).buffer[0] == "p1: paper" && (*(b)).buffer[1] == "p2: rock")
        v = 1;
    else if((*(b)).buffer[0] == "p1: paper" && (*(b)).buffer[1] == "p2: scissors")
        v = 2;
    else if((*(b)).buffer[0] == "p1: scissors" && (*(b)).buffer[1] == "p2: paper")
        v = 1;
    else if((*(b)).buffer[0] == "p1: scissors" && (*(b)).buffer[1] == "p2: rock")
        v = 2; 

    return v;   
}

void clearBoard(BRD *b)
{
    for (size_t i = 0; i < 2; i++)
        (*(b)).buffer[i] = NULL;
    sem_init(&(*(b)).empty, 1, 1);
    sem_init(&(*(b)).full, 1, 0);
}

void player1(BRD *b)
{
    char *s;
    int v = (rand() % 3) + 1;
    if(v == 1)
        s = "p1: rock";
    else if (v == 2)
        s = "p1: paper";
    else 
        s = "p1: scissors";

    sem_wait(&(*(b)).empty);
    updateBoard(b, s, 0);
    sem_post(&(*(b)).full);
}

void player2(BRD *b)
{
    char *s;
    int v = (rand() % 3) + 1;
    if(v == 1)
        s = "p2: rock";
    else if (v == 2)
        s = "p2: paper";
    else 
        s = "p2: scissors";

    sem_wait(&(*(b)).full);
    updateBoard(b, s, 1);
    sem_post(&(*(b)).empty);
}

void updateBoard(BRD *b, char *s, int i)
{
    (*(b)).buffer[i] = s;
}

void printBoard(BRD *b, int w)
{  
    char *s;
    if (w == 1)
        s = "player 1 wins the hand!";
    else if (w == 2)
        s = "player 2 wins the hand!";
    else
        s = "draw hand; no winner!";
    
    printf("[");
    for (size_t i = 0; i < 2; i++)
    {
        if(i < 1)
            printf("%-12s | ", (*(b)).buffer[i]);
        else
            printf("%-12s", (*(b)).buffer[i]);       
    }
    printf("]  ---  %s\n", s);
    
}
