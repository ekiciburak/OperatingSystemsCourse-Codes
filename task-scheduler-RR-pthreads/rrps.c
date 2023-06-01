#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "semaphore.h"
#include "pthread.h"
#include "stdbool.h"

#define quantum 2

struct process
{
   char *id;
   int priority;
   int burstTime;
};

typedef struct process PROCESS;

struct node
{
    PROCESS *proc;
    struct node *next;
};

typedef struct node NODE;

struct queue
{
    NODE *front;
};

typedef struct queue QUEUE;

void initProcess(PROCESS *, char *, int, int);
void updateBurstTime(PROCESS *, int);
void printProcess(PROCESS *);

void initNode(NODE *, PROCESS *);
void printNode(NODE *);

void initQueue(QUEUE *);
void enQueue(QUEUE *, NODE *);
NODE * deQueue(QUEUE *);
void printQueue(QUEUE *);

struct board
{
    NODE *cn;
    QUEUE *pq;
    sem_t s1, s2, s3, s4, s5, sd;
    bool sched;
    int qtime;
    int interval;
};

typedef struct board BOARD;

void initBoard(BOARD *, QUEUE *);
void dispatcher(BOARD *);
void process1(BOARD *);
void process2(BOARD *);
void process3(BOARD *);
void process4(BOARD *);
void process5(BOARD *);


int main(int argc, char const *argv[])
{
    PROCESS *p1 = malloc(sizeof(PROCESS));
    PROCESS *p2 = malloc(sizeof(PROCESS));
    PROCESS *p3 = malloc(sizeof(PROCESS));
    PROCESS *p4 = malloc(sizeof(PROCESS));
    PROCESS *p5 = malloc(sizeof(PROCESS));

    initProcess(p1, "p1", 3, 4);
    initProcess(p2, "p2", 2, 5);
    initProcess(p3, "p3", 2, 8);
    initProcess(p4, "p4", 1, 7);
    initProcess(p5, "p5", 3, 3);

    NODE *n1 = malloc(sizeof(NODE));
    NODE *n2 = malloc(sizeof(NODE));
    NODE *n3 = malloc(sizeof(NODE));
    NODE *n4 = malloc(sizeof(NODE));
    NODE *n5 = malloc(sizeof(NODE));
    NODE *n6 = malloc(sizeof(NODE));

    initNode(n1, p1);
    initNode(n2, p2);
    initNode(n3, p3);
    initNode(n4, p4);
    initNode(n5, p5);

    QUEUE *q = malloc(sizeof(QUEUE));

    enQueue(q, n1);
    enQueue(q, n2);
    enQueue(q, n3);
    enQueue(q, n4);
    enQueue(q, n5);

    printQueue(q);

    n6 = deQueue(q);
    printNode(n6);
    printQueue(q);

    enQueue(q, n6);
    printQueue(q);

    BOARD *b = malloc(sizeof(BOARD));
    initBoard(b, q);

    pthread_t t1, t2, t3, t4, t5, td;

    pthread_create(&t1, NULL, (void *) process1, b);
    pthread_create(&t2, NULL, (void *) process2, b);
    pthread_create(&t3, NULL, (void *) process3, b);
    pthread_create(&t4, NULL, (void *) process4, b);
    pthread_create(&t5, NULL, (void *) process5, b);
    pthread_create(&td, NULL, (void *) dispatcher, b);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(td, NULL);

    printf("terminating... \n");

    return 0;
}

void initProcess(PROCESS *p, char *id, int priority, int burstTime)
{
    p->id = id;
    p->priority = priority;
    p->burstTime = burstTime;
}

void updateBurstTime(PROCESS *p, int t)
{
    p->burstTime = p->burstTime - t;
}

void printProcess(PROCESS *p)
{
    printf("pid = %s  ", p->id);
    printf("priority = %d  ", p->priority);
    printf("burst time = %d\n", p->burstTime);
}

void initNode(NODE *n, PROCESS *p)
{
    n->proc = p;
    n->next = NULL;
}

void printNode(NODE *n)
{
    printProcess(n->proc);
}

void initQueue(QUEUE *q)
{
    q->front = NULL;
}

void enQueue(QUEUE *q, NODE *n)
{
    if(q->front == NULL)
        q->front = n;
    else if(q->front->proc->priority > n->proc->priority)
    {
        n->next = q->front;
        q->front = n;
    }
    else
    {
        NODE *t = malloc(sizeof(NODE));
        t = q->front;

        while ((t->next != NULL) && (t->next->proc->priority <= n->proc->priority))
            t = t->next;

        n->next = t->next;
        t->next = n;
    }
}

NODE * deQueue(QUEUE *q)
{
    if(q->front == NULL)
        return NULL;
    else
    {
        NODE *n = malloc(sizeof(NODE));
        n = q->front;
        q->front = q->front->next;
        return n;
    }
}

void printQueue(QUEUE *q)
{
    NODE *n = malloc(sizeof(NODE));
    n = q->front;
    printf("----- QUEUE -----\n");
    while (n != NULL)
    {
        printProcess(n->proc);
        n = n->next;
    }
    
    printf("-----------------\n");
}

void initBoard(BOARD *b, QUEUE *q)
{
    b->cn = NULL;
    b->pq = q;
    sem_init(&b->s1, 1, 0);
    sem_init(&b->s2, 1, 0);
    sem_init(&b->s3, 1, 0);
    sem_init(&b->s4, 1, 0);
    sem_init(&b->s5, 1, 0);
    sem_init(&b->sd, 1, 1);
    b->sched = true;
    b->qtime = quantum;
    b->interval = 0;
}

void dispatcher(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->sd);

        printf("\nthe priority queue:\n");
        printQueue(b->pq);
        sleep(3);
        printf("\n");

        b->cn = deQueue(b->pq);

        if(b->cn == NULL)
        {
            b->sched = false;
            sem_post(&b->s1);
            sem_post(&b->s2);
            sem_post(&b->s3);
            sem_post(&b->s4);
            sem_post(&b->s5);
            pthread_exit(NULL);
        }
        else if(b->cn->proc->id == "p1")
            sem_post(&b->s1);
        else if(b->cn->proc->id == "p2")
            sem_post(&b->s2);
        else if(b->cn->proc->id == "p3")
            sem_post(&b->s3);
        else if(b->cn->proc->id == "p4")
            sem_post(&b->s4);
        else if(b->cn->proc->id == "p5")
            sem_post(&b->s5);
    }
}

void process1(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s1);

        if(b->sched == false)
            pthread_exit(NULL);
        
        if((b->pq->front ==NULL) ||
           (b->cn->proc->priority < b->pq->front->proc->priority) ||
           (b->cn->proc->burstTime <= b->qtime)
          )
        {
            printf("***** process 1 is running for %d seconds [%d, %d] *****\n", b->cn->proc->burstTime, b->interval, b->interval+b->cn->proc->burstTime);
            printQueue(b->pq);
            sleep(b->cn->proc->burstTime);
            b->interval = b->interval+b->cn->proc->burstTime;
            updateBurstTime(b->cn->proc, b->cn->proc->burstTime);
            sem_post(&b->sd);
        }
        else
        {
            printf("***** process 1 is running for %d seconds [%d, %d] *****\n", b->qtime, b->interval, b->interval+b->qtime);
            printQueue(b->pq);
            sleep(b->qtime);
            b->interval = b->interval+b->qtime;
            updateBurstTime(b->cn->proc, b->qtime);
            enQueue(b->pq, b->cn);
            sem_post(&b->sd);
        }

    }
}

void process2(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s2);

        if(b->sched == false)
            pthread_exit(NULL);
        
        if((b->pq->front ==NULL) ||
           (b->cn->proc->priority < b->pq->front->proc->priority) ||
           (b->cn->proc->burstTime <= b->qtime)
          )
        {
            printf("***** process 2 is running for %d seconds [%d, %d] *****\n", b->cn->proc->burstTime, b->interval, b->interval+b->cn->proc->burstTime);
            printQueue(b->pq);
            sleep(b->cn->proc->burstTime);
            b->interval = b->interval+b->cn->proc->burstTime;
            updateBurstTime(b->cn->proc, b->cn->proc->burstTime);
            sem_post(&b->sd);
        }
        else
        {
            printf("***** process 2 is running for %d seconds [%d, %d] *****\n", b->qtime, b->interval, b->interval+b->qtime);
            printQueue(b->pq);
            sleep(b->qtime);
            b->interval = b->interval+b->qtime;
            updateBurstTime(b->cn->proc, b->qtime);
            enQueue(b->pq, b->cn);
            sem_post(&b->sd);
        }

    }
}

void process3(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s3);

        if(b->sched == false)
            pthread_exit(NULL);
        
        if((b->pq->front ==NULL) ||
           (b->cn->proc->priority < b->pq->front->proc->priority) ||
           (b->cn->proc->burstTime <= b->qtime)
          )
        {
            printf("***** process 3 is running for %d seconds [%d, %d] *****\n", b->cn->proc->burstTime, b->interval, b->interval+b->cn->proc->burstTime);
            printQueue(b->pq);
            sleep(b->cn->proc->burstTime);
            b->interval = b->interval+b->cn->proc->burstTime;
            updateBurstTime(b->cn->proc, b->cn->proc->burstTime);
            sem_post(&b->sd);
        }
        else
        {
            printf("***** process 3 is running for %d seconds [%d, %d] *****\n", b->qtime, b->interval, b->interval+b->qtime);
            printQueue(b->pq);
            sleep(b->qtime);
            b->interval = b->interval+b->qtime;
            updateBurstTime(b->cn->proc, b->qtime);
            enQueue(b->pq, b->cn);
            sem_post(&b->sd);
        }

    }
}

void process4(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s4);

        if(b->sched == false)
            pthread_exit(NULL);
        
        if((b->pq->front ==NULL) ||
           (b->cn->proc->priority < b->pq->front->proc->priority) ||
           (b->cn->proc->burstTime <= b->qtime)
          )
        {
            printf("***** process 4 is running for %d seconds [%d, %d] *****\n", b->cn->proc->burstTime, b->interval, b->interval+b->cn->proc->burstTime);
            printQueue(b->pq);
            sleep(b->cn->proc->burstTime);
            b->interval = b->interval+b->cn->proc->burstTime;
            updateBurstTime(b->cn->proc, b->cn->proc->burstTime);
            sem_post(&b->sd);
        }
        else
        {
            printf("***** process 4 is running for %d seconds [%d, %d] *****\n", b->qtime, b->interval, b->interval+b->qtime);
            printQueue(b->pq);
            sleep(b->qtime);
            b->interval = b->interval+b->qtime;
            updateBurstTime(b->cn->proc, b->qtime);
            enQueue(b->pq, b->cn);
            sem_post(&b->sd);
        }

    }
}

void process5(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s5);

        if(b->sched == false)
            pthread_exit(NULL);
        
        if((b->pq->front ==NULL) ||
           (b->cn->proc->priority < b->pq->front->proc->priority) ||
           (b->cn->proc->burstTime <= b->qtime)
          )
        {
            printf("***** process 5 is running for %d seconds [%d, %d] *****\n", b->cn->proc->burstTime, b->interval, b->interval+b->cn->proc->burstTime);
            printQueue(b->pq);
            sleep(b->cn->proc->burstTime);
            b->interval = b->interval+b->cn->proc->burstTime;
            updateBurstTime(b->cn->proc, b->cn->proc->burstTime);
            sem_post(&b->sd);
        }
        else
        {
            printf("***** process 5 is running for %d seconds [%d, %d] *****\n", b->qtime, b->interval, b->interval+b->qtime);
            printQueue(b->pq);
            sleep(b->qtime);
            b->interval = b->interval+b->qtime;
            updateBurstTime(b->cn->proc, b->qtime);
            enQueue(b->pq, b->cn);
            sem_post(&b->sd);
        }

    }
}