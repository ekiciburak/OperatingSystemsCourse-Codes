#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "semaphore.h"
#include "pthread.h"
#include "unistd.h"
#include "stdbool.h"
#include "procqueue.h"

struct board
{
    NODE *cp;
    QUEUE *pq;
    sem_t s1, s2, s3, s4, s5, sd;
    bool  sched;
    int   qtime;
};

typedef struct board BOARD;

void initBoard(BOARD *, QUEUE *);
void proc1(BOARD *);
void proc2(BOARD *);
void proc3(BOARD *);
void proc4(BOARD *);
void proc5(BOARD *);
void dispatcher(BOARD *);

int main(int argc, char const *argv[])
{
    PROCESS *p1 = malloc(sizeof(PROCESS));
    PROCESS *p2 = malloc(sizeof(PROCESS));
    PROCESS *p3 = malloc(sizeof(PROCESS));
    PROCESS *p4 = malloc(sizeof(PROCESS));
    PROCESS *p5 = malloc(sizeof(PROCESS));
    NODE *n1    = malloc(sizeof(NODE));
    NODE *n2    = malloc(sizeof(NODE));
    NODE *n3    = malloc(sizeof(NODE));
    NODE *n4    = malloc(sizeof(NODE));
    NODE *n5    = malloc(sizeof(NODE));
    QUEUE *q    = malloc(size*size*sizeof(NODE));
    BOARD *b    = malloc(sizeof(BOARD));

    createProcess(p1, "p1", 3, 4);
    createProcess(p2, "p2", 2, 5);
    createProcess(p3, "p3", 2, 8);
    createProcess(p4, "p4", 1, 7);
    createProcess(p5, "p5", 3, 3);

    createNode(n1, p1);
    createNode(n2, p2);
    createNode(n3, p3);
    createNode(n4, p4);
    createNode(n5, p5);

    createQueue(q);
    enQueue(n1, q);
    enQueue(n2, q);
    enQueue(n3, q);
    enQueue(n4, q);
    enQueue(n5, q);
    
    initBoard(b, q);
    /*
    printf("the initial priority queue:\n");
    printQueue((*(b)).pq);
    sleep(3);
    */
    pthread_t t1, t2, t3, t4, t5, d;

    pthread_create(&d,  NULL, (void *) dispatcher,  b);
    pthread_create(&t1, NULL, (void *) proc1, b);
    pthread_create(&t2, NULL, (void *) proc2, b);
    pthread_create(&t3, NULL, (void *) proc3, b);
    pthread_create(&t4, NULL, (void *) proc4, b);
    pthread_create(&t5, NULL, (void *) proc5, b);

    pthread_join(d,  NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    printf("Terminating...\n");

    return 0;
}


void initBoard(BOARD *b, QUEUE *q)
{
    (*(b)).pq = q;
    sem_init(&(*(b)).sd, 1, 1);
    sem_init(&(*(b)).s1, 1, 0);
    sem_init(&(*(b)).s2, 1, 0);
    sem_init(&(*(b)).s3, 1, 0);
    sem_init(&(*(b)).s4, 1, 0);
    sem_init(&(*(b)).s5, 1, 0);
    ((*b)).sched = true;
    (*(b)).qtime = qt;
    (*(b)).cp = NULL;
}

void proc1 (BOARD *b)
{
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).s1);
        
        if((*b).sched == false) 
            pthread_exit(NULL);
        if((*(b)).cp->proc->burstTime > (*(b)).qtime)
        {
            printf(" ***** process 1 is running for %d seconds *****\n", (*(b)).qtime);
            updateBurstTime((*(b)).cp->proc, (*(b)).qtime);
            printQueue((*(b)).pq);
            enQueue((*(b)).cp, (*(b)).pq);
            sleep((*(b)).qtime);
            sem_post(&(*(b)).sd);
        }
        else
        {
            printf(" ***** process 1 is running for %d seconds *****\n", (*(b)).cp->proc->burstTime);
            printQueue((*(b)).pq);
            sleep((*(b)).cp->proc->burstTime);
            updateBurstTime((*(b)).cp->proc, (*(b)).cp->proc->burstTime);
            sem_post(&(*(b)).sd);
        }
    }
}

void proc2 (BOARD *b)
{
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).s2);
        
        if((*b).sched == false) 
            pthread_exit(NULL);
        if((*(b)).cp->proc->burstTime > (*(b)).qtime)
        {
            printf(" ***** process 2 is running for %d seconds *****\n", (*(b)).qtime);
            updateBurstTime((*(b)).cp->proc, (*(b)).qtime);
            printQueue((*(b)).pq);
            enQueue((*(b)).cp, (*(b)).pq);
            sleep((*(b)).qtime);
            sem_post(&(*(b)).sd);
        }
        else
        {
            printf(" ***** process 2 is running for %d seconds *****\n", (*(b)).cp->proc->burstTime);
            printQueue((*(b)).pq);
            sleep((*(b)).cp->proc->burstTime);
            updateBurstTime((*(b)).cp->proc, (*(b)).cp->proc->burstTime);
            sem_post(&(*(b)).sd);
        }
    }
}

void proc3 (BOARD *b)
{
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).s3);
        
        if((*b).sched == false) 
            pthread_exit(NULL);
        if((*(b)).cp->proc->burstTime > (*(b)).qtime)
        {
            printf(" ***** process 3 is running for %d seconds *****\n", (*(b)).qtime);
            updateBurstTime((*(b)).cp->proc, (*(b)).qtime);
            printQueue((*(b)).pq);
            enQueue((*(b)).cp, (*(b)).pq);
            sleep((*(b)).qtime);
            sem_post(&(*(b)).sd);
        }
        else
        {
            printf(" ***** process 3 is running for %d seconds *****\n", (*(b)).cp->proc->burstTime);
            printQueue((*(b)).pq);
            sleep((*(b)).cp->proc->burstTime);
            updateBurstTime((*(b)).cp->proc, (*(b)).cp->proc->burstTime);
            sem_post(&(*(b)).sd);
        }
    }
}

void proc4 (BOARD *b)
{
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).s4);
        
        if((*b).sched == false) 
            pthread_exit(NULL);
        if((*(b)).cp->proc->burstTime > (*(b)).qtime)
        {
            printf(" ***** process 4 is running for %d seconds *****\n", (*(b)).qtime);
            updateBurstTime((*(b)).cp->proc, (*(b)).qtime);
            printQueue((*(b)).pq);
            enQueue((*(b)).cp, (*(b)).pq);
            sleep((*(b)).qtime);
            sem_post(&(*(b)).sd);
        }
        else
        {
            printf(" ***** process 4 is running for %d seconds *****\n", (*(b)).cp->proc->burstTime);
            printQueue((*(b)).pq);
            sleep((*(b)).cp->proc->burstTime);
            updateBurstTime((*(b)).cp->proc, (*(b)).cp->proc->burstTime);
            sem_post(&(*(b)).sd);
        }
    }
}

void proc5 (BOARD *b)
{
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).s5);
        
        if((*b).sched == false) 
            pthread_exit(NULL);
        if((*(b)).cp->proc->burstTime > (*(b)).qtime)
        {
            printf(" ***** process 5 is running for %d seconds *****\n", (*(b)).qtime);
            updateBurstTime((*(b)).cp->proc, (*(b)).qtime);
            printQueue((*(b)).pq);
            enQueue((*(b)).cp, (*(b)).pq);
            sleep((*(b)).qtime);
            sem_post(&(*(b)).sd);
        }
        else
        {
            printf(" ***** process 5 is running for %d seconds *****\n", (*(b)).cp->proc->burstTime);
            printQueue((*(b)).pq);
            sleep((*(b)).cp->proc->burstTime);
            updateBurstTime((*(b)).cp->proc, (*(b)).cp->proc->burstTime);
            sem_post(&(*(b)).sd);
        }
    }
}

void dispatcher (BOARD *b)
{
    NODE *n;
    while ((*(b)).sched)
    {
        sem_wait(&(*(b)).sd);
        printf("\ndispatcher schedules processes in the priority queue below:\n");
        printQueue((*(b)).pq);
        sleep(3);
        (*(b)).cp = deQueue((*(b)).pq);        

        if((*(b)).cp == NULL) 
        {
            (*(b)).sched = false;
            sem_post(&(*(b)).s1);
            sem_post(&(*(b)).s2);
            sem_post(&(*(b)).s3);
            sem_post(&(*(b)).s4);
            sem_post(&(*(b)).s5);
            pthread_exit(NULL);
        }
        else if((*(b)).cp->proc->id == "p1")
            sem_post(&(*(b)).s1);
        else if((*(b)).cp->proc->id == "p2")
            sem_post(&(*(b)).s2);
        else if((*(b)).cp->proc->id == "p3")
            sem_post(&(*(b)).s3);
        else if((*(b)).cp->proc->id == "p4")
            sem_post(&(*(b)).s4);
        else if((*(b)).cp->proc->id == "p5")
            sem_post(&(*(b)).s5);
    }
}