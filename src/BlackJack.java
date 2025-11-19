import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Arrays;
import java.util.Scanner;

public class BlackJack {
    static Scanner kb = new Scanner(System.in);

    // Todo: Hide dealer hole card.
    public enum Suit {Spades, Hearts, Clubs, Diamonds}

    public enum Rank {
        Two(2), Three(3), Four(4), Five(5), Six(6), Seven(7), Eight(8),
        Nine(9), Ten(10), Jack(10), Queen(10), King(10), Ace(11);

        private final int value;

        Rank (int v) {
            value = v;
        }

        public int getValue() {
            return value;
        }
    }

    private record Card (Rank r, Suit s) {
        @Override
        public String toString() {
            return r + " of " + s;
        }

        public int getValue() {
            return r.getValue();
        }
    }

    public static void main(String[] args) {
        char choice;
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

        showHand("Dealer", dealer);
        System.out.println(handValue(dealer));
        showHand("Player", player);
        System.out.println(handValue(player));
        int dhv = handValue(dealer);
        int phv = handValue(player);
        if ( dhv != 21 ) {
            do {
                System.out.print("(H)it or (S)tay? ");
                choice = kb.next().toUpperCase().charAt(0);
                if ( choice == 'H' ) {
                    player.add(deck.pop());
                    showHand("Player", player);
                    phv = handValue(player);
                    System.out.println(handValue(player));
                }
            } while (choice != 'S' && phv < 22 && player.size() < 5);

            //dhv = handValue(dealer);
            phv = handValue(player);

            if ( player.size() < 5 )
                while (dhv < phv && (phv <= 21)) {
                    dealer.add(deck.pop());
                    dhv = handValue(dealer);
                }
        }

        if ( dhv == 21 && dealer.size() == 2)
            System.out.println("Dealer has BlackJack!");
        else if ( player.size() == 5 && phv < 22 )
            System.out.println("You won with a 5-card Charlie! Woohoo!");
        else if (dhv > 21)
            System.out.println("Dealer has bust! You Win!");
        else if (phv > 21)
            System.out.println("Player has bust! Dealer Wins!");
        else if ( dhv >= phv )
            System.out.println("Dealer wins!");
        else
            System.out.println("You win!");
        showHand("Dealer", dealer);
        showHand("Player", player);
        System.out.println("Dealer has " + dhv + ". Player has " + phv);
    }

    private static void deal (LinkedList<Card> deck, ArrayList<Card> d, ArrayList<Card> p) {
        for ( int x = 0; x < 2; x++) {
            p.add(deck.pop());
            d.add(deck.pop());
        }
    }

    private static int handValue(ArrayList<Card> hand) {
        int total = 0;
        //Object[] ca = hand.toArray();
        int[] ia = new int[hand.size()];

        int x = 0;
        for (Card c : hand) {
            int cv = c.getValue();
            ia[x] = cv;
            x++;
        }

        Arrays.sort(ia);
        for (int i : ia) {
            //  System.out.println(i);

            if ( i == 11 && (total + 11 > 21) )
                i = 1;

            total = total + i;
        }

        return total;
    }

    private static void showHand(String name, List<Card> hand) {
        System.out.println("========================================");
        System.out.println(name + " has:");
        for (Card c : hand)
            System.out.println(c);
    }

}