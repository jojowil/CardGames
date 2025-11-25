#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "git-banned.h"

// Todo: Hide dealer hole card.

// Suits enumeration.
typedef enum {Spades, Hearts, Clubs, Diamonds, NumSuit} Suit;

// Ranks enumeration.
typedef enum {
    Two, Three, Four, Five, Six, Seven, Eight,
    Nine, Ten, Jack, Queen, King, Ace, NumRank
} Rank;

// Card definition.
typedef struct {
    Rank rank;
    Suit suit;
} Card;

// A deck of cards.
typedef struct {
    Card cards[52];
    int numCards;
    int deckPos;
} Deck;

// A player's hand. (UNUSED)
typedef struct {
    Card cards[10];
    int numCards;
} Hand;

const char* ranks[] = {
    "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
    "Nine", "Ten", "Jack", "Queen", "King", "Ace"
};
int rankVal[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
const char* suits[] = {"Spades", "Hearts", "Clubs", "Diamonds"};

static int getValue(Card c);
static const char *strRank(Rank r);
static const char *strSuit(Suit s);
static void populateDeck(Deck *d);
static void shuffle(void *array, size_t n, size_t size);
static void addCard(Deck *deck, Deck *hand);
static void deal (Deck *deck, Deck *d, Deck *p);
static int compareInts( const void *a, const void *b );
static int handValue(Deck hand);
static void showHand(char* name, Deck hand);

// Get the value of a given rank (not ordinal!)
static int getValue(Card c) {return rankVal[c.rank];}

// Convert rank to string.
static const char *strRank(Rank r) {return ranks[r];}

// Convert suit to string.
static const char *strSuit(Suit s) {return suits[s];}

// initialize the deck with card ranks and suits.
static void populateDeck(Deck *d) {
    int card = 0;
    for (Suit s = 0; s < NumSuit; s++) {
        for (Rank r = 0; r < NumRank; r++) {
            d->cards[card].rank = r;
            d->cards[card++].suit = s;
        }
    }
    d->numCards = card;
    d->deckPos = 0;
}

/* arrange the N elements of ARRAY in random order.
 * Only effective if N is much smaller than RAND_MAX;
 * if this may not be the case, use a better random
 * number generator.
 *
 * This was on StackOverflow as a generic version of
 * Ben Pfaff's solution.
 * http://benpfaff.org/writings/clc/shuffle.html
 */
static void shuffle(void *array, size_t n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);
    srand(time(NULL));
    if (n > 1) {
        for (size_t i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            if ( i != j) { // avoid overlap! reported by valgrind
                memcpy(tmp, arr + j * stride, size);
                memcpy(arr + j * stride, arr + i * stride, size);
                memcpy(arr + i * stride, tmp, size);
            }
        }
    }
}

// Add a card to hand, removing it from the deck.
static void addCard(Deck *deck, Deck *hand) {
    hand->cards[hand->numCards++] = deck->cards[deck->deckPos++];
    printf("Num cards: %d\n", hand->numCards);
    printf("deckPos: %d\n", deck->deckPos);
}

// Alternate dealing 2 cards between player and dealer.
static void deal (Deck *deck, Deck *d, Deck *p) {
    for (int x = 0; x < 2; x++) {
        addCard(deck, p);
        addCard(deck, d);
    }
}

// Function for qsort to compare two int for sorting by card value.
static int compareInts( const void *a, const void *b ) {
    if ( *(int*)a < *(int*)b ) return -1;
    if ( *(int*)a > *(int*)b ) return 1;
    return 0;
}

// Determine the value of a hand based on subtotal and Aces.
static int handValue(Deck hand) {
    int total = 0;
    // No hand should have more than 5 cards...
    int ia[10] = {0};

    for (int x=0; x < hand.numCards; x++)
        ia[x] = getValue(hand.cards[x]);

    qsort(ia, hand.numCards, sizeof(int), compareInts);

    for (int x = 0; x < hand.numCards; x++) {
        int i = ia[x];
        //printf("Card value %d\n", i);
        if ( i == 11 && (total + 11 > 21) )
            i = 1;

        total = total + i;
    }

    return total;
}

// Print text that represents the hand.
static void showHand(char* name, Deck hand) {
    printf("========================================\n");
    printf("%s has:\n", name);
    for (int x = 0; x < hand.numCards; x++)
        printf("%s of %s\n", strRank(hand.cards[x].rank), strSuit(hand.cards[x].suit));
}

int main(void) {
    Deck deck, dealer, player;

    dealer.deckPos = 0; dealer.numCards = 0;
    player.deckPos = 0; player.numCards = 0;

    // populate the deck
    populateDeck(&deck);
    showHand("The Deck", deck);
    shuffle(&deck.cards, 52, sizeof(Card));
    showHand("The Deck", deck);

    // how many cards?
    printf("There are %d cards in the deck.\n", deck.numCards);

    deal(&deck, &dealer, &player);

    showHand("Dealer", dealer);
    printf("%d\n", handValue(dealer));
    showHand("Player", player);
    printf("%d\n", handValue(player));
    int dhv = handValue(dealer);
    int phv = handValue(player);
    if ( dhv != 21 ) {
        char choice;
        do {
            printf("(H)it or (S)tay? ");
            choice = (char)toupper(getchar());
            if ( choice == 'H' ) {
                addCard(&deck, &player);
                showHand("Player", player);
                phv = handValue(player);
                printf("%d\n", phv);
            }
        } while (choice != 'S' && phv < 22 && player.numCards < 5);

        //dhv = handValue(dealer);
        phv = handValue(player);

        if ( player.numCards < 5 )
            while (dhv < phv && (phv <= 21)) {
                addCard(&deck, &dealer);
                dhv = handValue(dealer);
            }
    }

    if ( dhv == 21 && dealer.numCards == 2)
        printf("Dealer has BlackJack!\n");
    else if ( player.numCards == 5 && phv < 22 )
        printf("You won with a 5-card Charlie! Woohoo!\n");
    else if (dhv > 21)
        printf("Dealer has bust! You Win!\n");
    else if (phv > 21)
        printf("Player has bust! Dealer Wins!\n");
    else if ( dhv >= phv )
        printf("Dealer wins!\n");
    else
        printf("You win!\n");
    showHand("Dealer", dealer);
    showHand("Player", player);
    printf("Dealer has %d. Player has %d.\n", dhv, phv);
}
