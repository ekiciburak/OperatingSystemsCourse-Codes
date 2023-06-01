#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "semaphore.h"
#include "pthread.h"

struct process
{
    char *id;
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
    NODE *rear;
};

typedef struct queue QUEUE;

void initProcess(PROCESS *, char *, int);
void printProcess(PROCESS *);

void initNode(NODE *, PROCESS *);
void printNode(NODE *);

void initQueue(QUEUE *);
void enQueue(QUEUE *, NODE *);
NODE * deQueue(QUEUE *);
void printQueue(QUEUE *);

struct board
{
    NODE *cn;   // cn stands for the "current node"
    QUEUE *oq;  // oq abbreviated "ordinary queue"
    sem_t s1, s2, s3, s4, s5, sd;
    bool sched;
};

typedef struct board BOARD;

void initBoard(BOARD *, QUEUE *);
void process1(BOARD *);
void process2(BOARD *);
void process3(BOARD *);
void process4(BOARD *);
void process5(BOARD *);
void dispatcher(BOARD *);

int main(int argc, char const *argv[])
{
    PROCESS *p1 = malloc(sizeof(PROCESS));    
    PROCESS *p2 = malloc(sizeof(PROCESS));
    PROCESS *p3 = malloc(sizeof(PROCESS));    
    PROCESS *p4 = malloc(sizeof(PROCESS));
    PROCESS *p5 = malloc(sizeof(PROCESS));    

    initProcess(p1, "p1", 5);
    initProcess(p2, "p2", 4);
    initProcess(p3, "p3", 3);
    initProcess(p4, "p4", 2);
    initProcess(p5, "p5", 1);

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
    initQueue(q);

    enQueue(q, n1);
    enQueue(q, n2);
    enQueue(q, n3);
    enQueue(q, n5);
    enQueue(q, n4);
    printQueue(q);

    n6 = deQueue(q);
    //printNode(n6);

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

    printf("terminating...\n");

    return 0;
}

void initProcess(PROCESS *p, char *id, int burstTime)
{
    p->id = id;
    p->burstTime = burstTime;
}

void printProcess(PROCESS *p)
{
    printf("pid = %s   ", p->id);
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
    q->front = q->rear = NULL;
}

void enQueue(QUEUE *q, NODE *n)
{
    if(q->front == NULL && q->rear == NULL)
    {
        q->front = q->rear = n;
    }
    else 
    {
        q->rear->next = n;
        q->rear = n;
        q->rear->next = NULL;
    }
}

NODE * deQueue(QUEUE *q)
{
    if(q->front == NULL && q->rear == NULL)
        return NULL;
    else
    {
        NODE *n = malloc(sizeof(NODE));
        n = q->front;
        q->front = q->front->next;

        if(q->front == NULL)
            q->rear = NULL;
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
    b->oq = q;
    b->sched = true;
    sem_init(&b->s1, 1, 0);
    sem_init(&b->s2, 1, 0);
    sem_init(&b->s3, 1, 0);
    sem_init(&b->s4, 1, 0);
    sem_init(&b->s5, 1, 0);
    sem_init(&b->sd, 1, 1);
}

void dispatcher(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->sd);

        printf("\nthe queue:\n");
        printQueue(b->oq);
        sleep(3);
        printf("\n");

        b->cn = deQueue(b->oq);

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
        
        printf("***** thread 1 is running for %d seconds *****\n", b->cn->proc->burstTime);
        sleep(b->cn->proc->burstTime);
        sem_post(&b->sd);
    }
}

void process2(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s2);

        if(b->sched == false)
            pthread_exit(NULL);
        
        printf("***** thread 2 is running for %d seconds *****\n", b->cn->proc->burstTime);
        sleep(b->cn->proc->burstTime);
        sem_post(&b->sd);
    }
}

void process3(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s3);

        if(b->sched == false)
            pthread_exit(NULL);
        
        printf("***** thread 3 is running for %d seconds *****\n", b->cn->proc->burstTime);
        sleep(b->cn->proc->burstTime);
        sem_post(&b->sd);
    }
}

void process4(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s4);

        if(b->sched == false)
            pthread_exit(NULL);
        
        printf("***** thread 4 is running for %d seconds *****\n", b->cn->proc->burstTime);
        sleep(b->cn->proc->burstTime);
        sem_post(&b->sd);
    }
}

void process5(BOARD *b)
{
    while (b->sched)
    {
        sem_wait(&b->s5);

        if(b->sched == false)
            pthread_exit(NULL);
        
        printf("***** thread 5 is running for %d seconds *****\n", b->cn->proc->burstTime);
        sleep(b->cn->proc->burstTime);
        sem_post(&b->sd);
    }
}