#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define size 52
#define len  13

struct card
{
    char *face;
    char *suit;  
};

typedef struct card Card;

void fillDeck(Card *, char **, char **);
void shuffleDeck(Card *);
void printDeck(Card *);

int main(int argc, char const *argv[])
{
    Card deck[size];

    char *faces[] = { "Ace", "Duece", "Three", "Four", "Five", "Six", "Seven",
                      "Eigth", "Nine", "Ten", "Jack", "Queen", "King" };

    char *suits[] = { "Hearts", "Clubs", "Diamonds", "Spades" };

    srand(time(NULL));

    fillDeck(deck, faces, suits);
    shuffleDeck(deck);
    printDeck(deck);

    return 0;
}

void fillDeck(Card *deck, char **faces, char **suits)
{
    for (size_t i = 0; i < size; i++)
    {
        // (*(deck+i)).face = *(faces+(i % len)); // the same with the one below
        (deck+i)->face   = *(faces+(i % len)); 
        (*(deck+i)).suit = *(suits+(i / len));
    } 
}

void shuffleDeck(Card *deck)
{
    int j;
    Card c;

    for (size_t i = 0; i < size; i++)
    {
        j = rand() % size;
        c = (*(deck+i));
        (*(deck+i)) = (*(deck+j));
        (*(deck+j)) = c;
    }
}

void printDeck(Card *deck)
{
    for (size_t i = 0; i < size; i++)
        printf("%s of %s\n", (*(deck+i)).face, (*(deck+i)).suit);
}
