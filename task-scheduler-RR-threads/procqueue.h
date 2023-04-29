
#define size 5
#define qt 2

struct process
{
    char *id;
    int  priority;
    int  burstTime;
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


void createProcess(PROCESS *, char *, int, int);
void updateBurstTime(PROCESS *, int);
void printProcess(PROCESS *);

void createNode(NODE *, PROCESS *);
void printNode(NODE *);

void createQueue(QUEUE *);
void enQueue(NODE *, QUEUE *);
NODE * deQueue(QUEUE *);
void printQueue(QUEUE *);


void createProcess(PROCESS *p, char *s, int pri, int b)
{
   (*(p)).id = s;
   (*(p)).priority = pri;
   (*(p)).burstTime = b; 
}

void updateBurstTime(PROCESS *p, int t)
{
    (*(p)).burstTime = (*(p)).burstTime - t;
}

void printProcess(PROCESS *p)
{
    printf("pname: %s  ", (*(p)).id);
    printf("prior: %d  ", (*(p)).priority);
    printf("btime: %d  \n", (*(p)).burstTime);
}

void createNode(NODE *n, PROCESS *p)
{
    (*(n)).proc = p;
    (*(n)).next = NULL;
}

void printNode(NODE *n)
{
    printProcess((*(n)).proc);
}

void createQueue(QUEUE *q)
{
    (*(q)).front = NULL;
}

void enQueue(NODE *n, QUEUE *q)
{
    if((*(q)).front == NULL)
    {
        (*(q)).front = n;
    }
    else if((*(n)).proc->priority < (*(q)).front->proc->priority)
    {
        (*(n)).next = (*(q)).front;
        (*(q)).front = n;
    }
    else
    {
        NODE *t = (*(q)).front;
        while (t->next != NULL && (*(n)).proc->priority >= t->next->proc->priority)
        {
            t = t->next;
        }
        (*(n)).next = t->next;
        t->next = n;
    }
}

NODE * deQueue(QUEUE *q)
{
    NODE *n = malloc(sizeof(NODE));

    if((*(q)).front == NULL)
        return NULL;
    else
    {
        n            = (*(q)).front;
        (*(q)).front = (*((*(q)).front)).next;
        return n;
        free(n);
    }

}

void printQueue(QUEUE *q)
{
    NODE *n = malloc(sizeof(NODE));

    n = (*(q)).front;
    printf("--------QUEUE---------\n");
    while (n != NULL)
    {
        printf("pname: %s  ", n->proc->id);
        printf("prior: %d  ", n->proc->priority);
        printf("btime: %d  ", n->proc->burstTime);
        printf("\n");
        n = n->next;
    }
    printf("----------------------\n");
}