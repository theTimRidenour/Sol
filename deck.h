#ifndef DECK_H
#define DECK_H

#include <string>

class Deck {
    struct card {
        int value = 0;      // value of card
        int suit;           // 0 = Clubs, 1 = Spades, 2 = Diamonds, 3 = Hearts
        char name[20]{};    // [value] OF [suit]
        bool faceUp = false;
        bool isRed = false;
        bool isBlack = false;
        int x = 0;
        int y = 0;
    };

    card cards[52];

    public:    

        Deck() {
            int cnt = 0;        // card position in deck
            int cardValue = 0;  // value of card number [1] = ACE ... [13] = KING
            int suitValue = 0;  // value of suit number
            // for each suit by name
            for (const char* s: {"CLUBS", "SPADES", "DIAMONDS", "HEARTS"}) {
                // for each value by name
                for (const char* v: {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"}) {
                    cardValue = cnt + 1;
                    suitValue = 0;
                    while (cardValue > 13) {
                        cardValue -= 13;    // [14] = 1, [27] = 1, [40] = 1
                        suitValue++;        // [0 - 13] = 0, [14 - 26] = 1, [27 - 39] = 2 [40 - 52] = 3
                    }
                    cards[cnt].value = cardValue;
                    cards[cnt].suit = suitValue;
                    std::sprintf(cards[cnt].name, "%s OF %s", v, s);    // [v] OF [s]
                    if (cnt < 26) cards[cnt].isBlack = true;
                    else cards[cnt].isRed = true;
                    cnt++;
                }
            }
        }

        char* getCardName(int i) { return cards[i].name; }  // returns name of card in "[value] OF [suit]" format eg. "ACE OF CLUBS"

        int getValue(int i) { return cards[i].value; }      // returns value of card in number form

        int getCardSuit(int i) { return cards[i].suit; }    // returns value of suit in number form

        int getX(int i) { return cards[i].x; }              // returns x position of card

        int getY(int i) { return cards[i].y; }              // returns y position of card

        bool isRed(int i) { return cards[i].isRed; }        // returns true if card is red

        bool isBlack(int i) { return cards[i].isBlack; }    // returns true if card is black

        bool isFaceUp(int i) { return cards[i].faceUp; }    // returns true if card is face up

        void setX(int index, int xValue) { cards[index].x = xValue; }                   // sets the x position of the card

        void setY(int index, int yValue) { cards[index].y = yValue; }                   // sets the y position of the card

        void setFaceUp(int index, bool isFaceUp) { cards[index].faceUp = isFaceUp; }    // sets the card as face up or face down

        // changes the order of the cards
        void shuffle() {
            card newOrder[52]; // temp deck to store new order
            int cnt = 0;       // position of card in old deck
            while (cnt < 52) {
                int random = rand()%51;  // picks a random number between 0 - 51
                bool insertedCard = false;
                while (!insertedCard) {  // loop until card is inserted into temp deck [newOrder]
                    if (newOrder[random].isBlack == true || newOrder[random].isRed == true) {  // if card is already at random positon
                        if (random < 51) random++;  // check next position
                        else random = 0;            // if at end of deck go back to first position [0]
                    } else {                            // if no card at new position
                        newOrder[random] = cards[cnt];  // copy card [cnt] to new position [random] in temp deck [newOrder]
                        cnt++;                          // move to next card in old deck
                        insertedCard = true;            // inform loop that the card has been inserted
                    }
                }
            }
            // move cards from temp deck [newOrder] to the main deck.
            cnt = 0;
            for (card c: newOrder) {
                cards[cnt].value = c.value;
                cards[cnt].suit = c.suit;
                std::sprintf(cards[cnt].name, "%s", c.name);
                cards[cnt].faceUp = false;
                cards[cnt].isRed = c.isRed;
                cards[cnt].isBlack = c.isBlack;
                cards[cnt].x = c.x;
                cards[cnt].y = c.y;
                cnt++;
            }
            return;
        }
};

#endif