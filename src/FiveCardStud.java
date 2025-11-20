import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;

public class FiveCardStud {
    public enum Suit {Spades, Hearts, Clubs, Diamonds}

    public enum Rank {Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace}

    private static class CardComparator implements Comparator<Card> {

        @Override
        public int compare(Card c1, Card c2) {
            return c1.r.compareTo(c2.r);
        }
    }

    private static class Card {
        Suit s;
        Rank r;

        public Card (Rank r, Suit s) {
            this.s = s;
            this.r = r;
        }

        @Override
        public String toString() {
            return r + " of " + s;
        }
    }

    public static void main(String[] args) {
        LinkedList<Card> deck = new LinkedList<>();
        ArrayList<Card> dealer = new ArrayList<>();
        ArrayList<Card> player = new ArrayList<>();

        // populate the deck
        for (Suit suit : Suit.values())
            for (Rank rank : Rank.values())
                deck.add(new Card(rank, suit));

        // how many cards?
        System.out.println("There are " + deck.size() + " cards in the deck.");

        // displaying unshuffled deck.
        //showHand(deck);

        Collections.shuffle(deck);
        // displaying shuffled deck.
        //showHand("The deck", deck);

        deal(deck, dealer, player);

        CardComparator cc = new CardComparator();

        dealer.sort(cc);
        showHand("Dealer", dealer);
        player.sort(cc);
        showHand("Player", player);

        System.out.print("Dealer has ");
        handRanking(dealer);
        System.out.print("Player has ");
        handRanking(player);
    }

    private static void handRanking(ArrayList<Card> h) {

        if (isRoyalFlush(h) )
            System.out.println("Royal Flush");
        else if (isStraightFlush(h))
            System.out.println("Straight Flush");
        else if (is4OfAKind(h))
            System.out.println("4 of a kind");
        else if (isFullHouse(h))
            System.out.println("Full House");
        else if (isFlush(h))
            System.out.println("Flush");
        else if (isStraight(h))
            System.out.println("Straight");
        else if (is3OfAKind(h))
            System.out.println("3 of a kind");
        else if (isTwoPair(h))
            System.out.println("Two Pair");
        else if (isPair(h))
            System.out.println("Pair");
        else
            System.out.println(h.get(4) + " high");
    }

    private static void deal (LinkedList<Card> deck, ArrayList<Card> d, ArrayList<Card> p) {
        for ( int x = 0; x < 5; x++) {
            p.add(deck.pop());
            d.add(deck.pop());
        }
    }

    private static void showHand(String name, List<Card> hand) {
        System.out.println("========================================");
        System.out.println(name + " has:");
        for (Card c : hand)
            System.out.println(c);
    }

    private static boolean isFlush(ArrayList<Card> h) {
        Suit s = h.getFirst().s;

        for (int x = 1; x < 5; x++)
            if (h.get(x).s != s)
                return false;

        return true;
    }

    private static boolean isStraight(ArrayList<Card> h) {
        int r = h.getFirst().r.ordinal();

        for (int x = 1; x < 5; x++)
            if (h.get(x).r.ordinal() != r+x)
                return false;

        return true;
    }

    private static boolean isStraightFlush(ArrayList<Card> h) {
        return isFlush(h) && isStraight(h);
    }

    private static boolean isRoyalFlush(ArrayList<Card> h) {
        return isStraightFlush(h) && h.getFirst().r == Rank.Ten;
    }

    private static boolean is4OfAKind(ArrayList<Card> h) {
        Rank r1 = h.get(0).r;
        Rank r2 = h.get(1).r;
        Rank r3 = h.get(2).r;
        Rank r4 = h.get(3).r;
        Rank r5 = h.get(4).r;

        return (r1 == r2 && r2 == r3 && r3 == r4) || (r2 == r3 && r3 == r4 && r4 == r5);
    }

    private static boolean isFullHouse(ArrayList<Card> h) {
        Rank r1 = h.get(0).r;
        Rank r2 = h.get(1).r;
        Rank r3 = h.get(2).r;
        Rank r4 = h.get(3).r;
        Rank r5 = h.get(4).r;

        return ((r1 == r2) && (r3 == r4 && r4 == r5)) || ((r1 == r2 && r2 == r3) && (r4 == r5));
    }

    private static boolean is3OfAKind(ArrayList<Card> h) {
        Rank r1 = h.get(0).r;
        Rank r2 = h.get(1).r;
        Rank r3 = h.get(2).r;
        Rank r4 = h.get(3).r;
        Rank r5 = h.get(4).r;

        return (r1 == r2 && r2 == r3) || (r2 == r3 && r3 == r4) || (r3 == r4 && r4 == r5);
    }

    private static boolean isTwoPair(ArrayList<Card> h) {
        Rank r1 = h.get(0).r;
        Rank r2 = h.get(1).r;
        Rank r3 = h.get(2).r;
        Rank r4 = h.get(3).r;
        Rank r5 = h.get(4).r;

        return (r1 == r2) && ((r3 == r4) || (r4 == r5)) ||
                ((r2 == r3) && (r4 == r5));
    }

    private static boolean isPair(ArrayList<Card> h) {
        Rank r1 = h.get(0).r;
        Rank r2 = h.get(1).r;
        Rank r3 = h.get(2).r;
        Rank r4 = h.get(3).r;
        Rank r5 = h.get(4).r;

        return (r1 == r2) || (r2 == r3) || (r3 == r4) || (r4 == r5);
    }
}