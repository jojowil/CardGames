//
// Created by William Jojo on 11/20/25.
//

#ifndef CARDGAMES_FIVECARDSTUD_H
#define CARDGAMES_FIVECARDSTUD_H

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

#endif //CARDGAMES_FIVECARDSTUD_H