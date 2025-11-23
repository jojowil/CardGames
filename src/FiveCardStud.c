#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "git-banned.h"

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
const char* suits[] = {"Spades", "Hearts", "Clubs", "Diamonds"};

static const char *strRank(Rank r);
static const char *strSuit(Suit s);
static void populateDeck(Deck *d);
static void shuffle(void *array, size_t n, size_t size);
static void addCard(Deck *deck, Deck *hand);
static void deal (Deck *deck, Deck *d, Deck *p);
static int compareCards( const void *a, const void *b );
static void showHand(char* name, Deck hand);

static int isFlush(Deck h);
static int isStraight(Deck h);
static int isStraightFlush(Deck h);
static int isRoyalFlush(Deck h);
static int is4OfAKind(Deck h);
static int isFullHouse(Deck h);
static int is3OfAKind(Deck h);
static int isTwoPair(Deck h);
static int isPair(Deck h);

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

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

// Determine the hand's rank.
static void handRanking(Deck h) {
    if (isRoyalFlush(h) )
        printf("Royal Flush\n");
    else if (isStraightFlush(h))
        printf("Straight Flush\n");
    else if (is4OfAKind(h))
        printf("4 of a kind\n");
    else if (isFullHouse(h))
        printf("Full House\n");
    else if (isFlush(h))
        printf("Flush\n");
    else if (isStraight(h))
        printf("Straight\n");
    else if (is3OfAKind(h))
        printf("3 of a kind\n");
    else if (isTwoPair(h))
        printf("Two Pair\n");
    else if (isPair(h))
        printf("Pair\n");
    else
        printf("%s of %s high\n", strRank(h.cards[4].rank), strSuit(h.cards[4].suit));
}

// Add a card to hand, removing it from the deck.
static void addCard(Deck *deck, Deck *hand) {
    hand->cards[hand->numCards++] = deck->cards[deck->deckPos++];
    printf("Num cards: %d\n", hand->numCards);
    printf("deckPos: %d\n", deck->deckPos);
}

// Alternate dealing 5 cards between player and dealer.
static void deal (Deck *deck, Deck *d, Deck *p) {
    for (int x = 0; x < 5; x++) {
        addCard(deck, p);
        addCard(deck, d);
    }
}

// Function for qsort to compare two cards for sorting by rank.
static int compareCards( const void *a, const void *b ) {
    if ( ((Card*)a)->rank < ((Card*)b)->rank ) return -1;
    if ( ((Card*)a)->rank > ((Card*)b)->rank ) return 1;
    return 0;
}

// Print text that represents the hand.
static void showHand(char* name, Deck hand) {
    printf("========================================\n");
    printf("%s has:\n", name);
    for (int x = 0; x < hand.numCards; x++)
        printf("%s of %s\n", strRank(hand.cards[x].rank), strSuit(hand.cards[x].suit));
}

// Check if hand is a flush
static int isFlush(Deck h) {
    Suit s = h.cards[0].suit;

    for (int x = 1; x < 5; x++)
        if (h.cards[x].suit != s)
            return 0;

    return 1;
}

// Check if hand is a straight
static int isStraight(Deck h) {
    int r = h.cards[0].rank;

    for (int x = 1; x < 5; x++)
        if (h.cards[x].rank != r+x)
            return 0;

    return 1;
}

// Check if a hand is a straight flush
static int isStraightFlush(Deck h) {
    return isFlush(h) && isStraight(h);
}

// Check if a hand is a royal flush
static int isRoyalFlush(Deck h) {
    return isStraightFlush(h) && h.cards[0].rank == Ten;
}

// Check if a hand has 4 of a kind
static int is4OfAKind(Deck h) {
    Rank r1 = h.cards[0].rank;
    Rank r2 = h.cards[1].rank;
    Rank r3 = h.cards[2].rank;
    Rank r4 = h.cards[3].rank;
    Rank r5 = h.cards[4].rank;

    return (r1 == r2 && r2 == r3 && r3 == r4) || (r2 == r3 && r3 == r4 && r4 == r5);
}

// Check is a hand is a full house
static int isFullHouse(Deck h) {
    Rank r1 = h.cards[0].rank;
    Rank r2 = h.cards[1].rank;
    Rank r3 = h.cards[2].rank;
    Rank r4 = h.cards[3].rank;
    Rank r5 = h.cards[4].rank;

    return ((r1 == r2) && (r3 == r4 && r4 == r5)) || ((r1 == r2 && r2 == r3) && (r4 == r5));
}

// Check is a hand has 3 of a kind
static int is3OfAKind(Deck h) {
    Rank r1 = h.cards[0].rank;
    Rank r2 = h.cards[1].rank;
    Rank r3 = h.cards[2].rank;
    Rank r4 = h.cards[3].rank;
    Rank r5 = h.cards[4].rank;

    return (r1 == r2 && r2 == r3) || (r2 == r3 && r3 == r4) || (r3 == r4 && r4 == r5);
}

// Check is a hand has two pair
static int isTwoPair(Deck h) {
    Rank r1 = h.cards[0].rank;
    Rank r2 = h.cards[1].rank;
    Rank r3 = h.cards[2].rank;
    Rank r4 = h.cards[3].rank;
    Rank r5 = h.cards[4].rank;

    return (r1 == r2) && ((r3 == r4) || (r4 == r5)) ||
            ((r2 == r3) && (r4 == r5));
}

// Check if a hand has a pair
static int isPair(Deck h) {
    Rank r1 = h.cards[0].rank;
    Rank r2 = h.cards[1].rank;
    Rank r3 = h.cards[2].rank;
    Rank r4 = h.cards[3].rank;
    Rank r5 = h.cards[4].rank;

    return (r1 == r2) || (r2 == r3) || (r3 == r4) || (r4 == r5);
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

    deal(&deck, &dealer, &player);

    qsort(dealer.cards, dealer.numCards, sizeof(Card), compareCards);
    showHand("Dealer", dealer);
    qsort(player.cards, player.numCards, sizeof(Card), compareCards);
    showHand("Player", player);

    printf("Dealer has ");
    handRanking(dealer);
    printf("Player has ");
    handRanking(player);
}
