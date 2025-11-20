#ifndef CARDGAMES_BLACKJACK_H
#define CARDGAMES_BLACKJACK_H

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

#endif //CARDGAMES_BLACKJACK_H