#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "semaphore.h"
#include "stdbool.h"
#include "wait.h"

#define NUMOP 5
#define TLIMIT 5
#define MAXWT 1000000
#define LEAVET 20000000000

enum state
{
    THINKING, EATING, HUNGRY
};

typedef enum state STATE;

struct board
{
    STATE state[NUMOP];
    sem_t cs[NUMOP];
    long WT[NUMOP];
    long TWT[NUMOP];
};

typedef struct board BOARD;
BOARD b;

long getSystemTime(void);
void initBoard(void);
void philosophers(int *);
bool canRun(int);
void updateOthersWT(int, long);

int main(int argc, char const *argv[])
{
    pthread_t t[NUMOP];
    int id[NUMOP];

    initBoard();

    for (size_t i = 0; i < NUMOP; i++)
    {
        id[i] = i;
        pthread_create(&t[i], NULL, (void *) philosophers, &id[i]);
    }

    for (size_t i = 0; i < NUMOP; i++)
        pthread_join(t[i], NULL);

    printf("all philosophers left...\n");
    
    return 0;
}

long getSystemTime(void)
{
    struct timespec ts;

    if(clock_gettime(1, &ts) == 0)
        return (long) ts.tv_sec * 1000000000 + ts.tv_nsec;
    
    return 0;
}

void initBoard(void)
{
    for (size_t i = 0; i < NUMOP; i++)
    {
        b.state[i] = THINKING;
        sem_init(&b.cs[i], 1, 1);
        b.WT[i] = 0;
        b.TWT[i] = 0;
    }
}

void philosophers(int *identity)
{
    int id = *identity;

    while (true)
    {
        srand(time(NULL) * id);
        long start = getSystemTime();
        
        int thinkTime = (rand() % TLIMIT) + 1;
        printf("philosopher %d is thinking for %d seconds\n", id, thinkTime);
        b.state[id] = THINKING;
        sleep(thinkTime);

        long now = getSystemTime();
        printf("philosopher %d is hungry\n", id);
        b.state[id] = HUNGRY;
        b.WT[id] = b.WT[id] + (now - start);
        b.TWT[id] = b.TWT[id] + b.WT[id];

        long check = getSystemTime();

        if(canRun(id))
        {
            sem_wait(&b.cs[id]);
            sem_wait(&b.cs[(id+1)%NUMOP]);
            int eatTime = (rand() % TLIMIT) + 1;
            printf("philosopher %d is eating for %d seconds\n", id, eatTime);
            b.state[id] = EATING;
            sleep(eatTime);
            printf("philosopher %d finished eating\n", id);
            b.WT[id] = 0;
            updateOthersWT(id, check);

            if(b.TWT[id] > LEAVET)
            {
                printf("philosopher %d is leaving the table\n", id);
                sem_post(&b.cs[id]);
                sem_post(&b.cs[(id+1)%NUMOP]);
                pthread_exit(NULL);
            }
            else
            {
                sem_post(&b.cs[id]);
                sem_post(&b.cs[(id+1)%NUMOP]);
            }
        }
    }
}

void updateOthersWT(int id, long check)
{
    long now = getSystemTime();
    for (size_t i = 0; i < NUMOP && i != id; i++)
    {
        b.WT[i] = b.WT[i] + (now - check);
        b.TWT[i] = b.TWT[i] + b.WT[i];
    }
}

bool canRun(int id)
{
    long current = b.WT[id];
    long left = b.WT[(id+NUMOP-1)%NUMOP];
    long right = b.WT[(id+1)%NUMOP];

    if(b.state[(id+NUMOP-1)%NUMOP] == HUNGRY && left > current && left > MAXWT)
    {
        printf("philosopher %d is giving way to phliosopher %d to avoid starvation\n", id, (id+NUMOP-1)%NUMOP);
        return false;
    }
    if(b.state[(id+1)%NUMOP] == HUNGRY && right > current && right > MAXWT)
    {
        printf("philosopher %d is giving way to phliosopher %d to avoid starvation\n", id, (id+1)%NUMOP);
        return false;
    }
    return true;
}
