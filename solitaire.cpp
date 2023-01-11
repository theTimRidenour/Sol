#include "raylib.h"
#include <iostream>
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

class Node {
    public:
        int cardIndex;
        int currentStack;
        int previousStack;
        bool prevFaceUp;

        Node();

        Node(int cardIndex, int currentStack, int previousStack, bool prevFaceUp) {
            this->cardIndex = cardIndex;
            this->currentStack = currentStack;
            this->previousStack = previousStack;
            this->prevFaceUp = prevFaceUp;
            this->next = NULL;
        }
    Node* next;
};

void push(struct Node** headRef, int cardIndex, int currentStack, int previousStack, bool prevFaceUp) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->cardIndex = cardIndex;
    newNode->currentStack = currentStack;
    newNode->previousStack = previousStack;
    newNode->prevFaceUp = prevFaceUp;
    newNode->next = ( *headRef );
    ( *headRef ) = newNode;
};

void pop(struct Node** headRef) {
    ( *headRef ) = ( *headRef )->next;
};

struct cardGraphics {
    cardGraphics(): cardFaces(), cardBacks(), customCardFace(false) { }
    Texture2D cardFaces;
    Texture2D cardBacks;
    bool customCardFace = false;
};

void drawCard(Deck deck, int index, float cardWidth, float cardHeight, Color cardBack, Color cardFront, bool usingGraphics = false, cardGraphics cg = cardGraphics()) {
    Rectangle rec {(float)deck.getX(index), (float)deck.getY(index), cardWidth, cardHeight};
    if (!deck.isFaceUp(index)) {
        DrawRectangleRounded(rec, 0.1, 5, cardBack);
    } else {
        if (cg.customCardFace) {
            // Add code for custom card face.
        } else {
            DrawRectangleRounded(rec, 0.1, 5, cardFront);
        }
        if (!usingGraphics) {
            Color color;
            if (deck.isRed(index)) color = RED;
            else color = BLACK;
            DrawText(deck.getCardName(index), rec.x + 5, rec.y + 5, 7, color);
        } else {
            float x = (deck.getValue(index)-1) * cg.cardFaces.width/13;

            float y = deck.getCardSuit(index);
            if (cg.customCardFace) y *= cg.cardFaces.height/5;
            else y *= cg.cardFaces.height/4;

            Rectangle rec{x, y, cardWidth, cardHeight};
            Vector2 pos{(float)deck.getX(index), (float)deck.getY(index)};
            DrawTextureRec(cg.cardFaces, rec, pos, WHITE);
        }
    }
    DrawRectangleRoundedLines(rec, 0.1, 5, 0.5, BLACK);
}

bool isOverFaceUpTop(int stackIndex[12], int mx, int my, int cardWidth, int cardHeight, int cardGap) {
    return (stackIndex[0] >= 0 && mx >= 30 && mx <= 30 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
           (stackIndex[1] >= 0 && mx >= 300 && mx <= 300 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
           (stackIndex[2] >= 0 && mx >= 570 && mx <= 570 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
           (stackIndex[3] >= 0 && mx >= 840 && mx <= 840 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
           (stackIndex[4] >= 0 && mx >= 1380 && mx <= 1380 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
           (stackIndex[5] >= 0 && mx >= 30 && mx <= 30 + cardWidth && my >= 396 + (stackIndex[5])*cardGap && my <= 396 + (stackIndex[5])*cardGap + cardHeight) ||
           (stackIndex[6] >= 0 && mx >= 300 && mx <= 300 + cardWidth && my >= 396 + (stackIndex[6])*cardGap && my <= 396 + (stackIndex[6])*cardGap + cardHeight) ||
           (stackIndex[7] >= 0 && mx >= 570 && mx <= 570 + cardWidth && my >= 396 + (stackIndex[7])*cardGap && my <= 396 + (stackIndex[7])*cardGap + cardHeight) ||
           (stackIndex[8] >= 0 && mx >= 840 && mx <= 840 + cardWidth && my >= 396 + (stackIndex[8])*cardGap && my <= 396 + (stackIndex[8])*cardGap + cardHeight) ||
           (stackIndex[9] >= 0 && mx >= 1110 && mx <= 1110 + cardWidth && my >= 396 + (stackIndex[9])*cardGap && my <= 396 + (stackIndex[9])*cardGap + cardHeight) ||
           (stackIndex[10] >= 0 && mx >= 1380 && mx <= 1380 + cardWidth && my >= 396 + (stackIndex[10])*cardGap && my <= 396 + (stackIndex[10])*cardGap + cardHeight) ||
           (stackIndex[11] >= 0 && mx >= 1650 && mx <= 1650 + cardWidth && my >= 396 + (stackIndex[11])*cardGap && my <= 396 + (stackIndex[11])*cardGap + cardHeight);
}

bool isOverFaceUpMiddle(int stackIndex[12], int faceIndex[7][19][2], int mx, int my, int cardWidth, int midHeight) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 19; j++) {
            if (faceIndex[i][j][0] != 99 && mx >= 30 + (cardWidth+30)*i && mx <= 30 + cardWidth + (cardWidth+30)*i &&
                my >= faceIndex[i][j][1] && my <= faceIndex[i][j][1] + midHeight) return true;
        }
    }
    return false;
}

int topCardSelected(int returnValue, int stackIndex[12], int rowDataSheet[12][2], int mx, int my, int cardWidth, int cardHeight, int cardGap) {
    if (stackIndex[0] >= 0 && mx >= 30 && mx <= 30 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[0][returnValue];
    if (stackIndex[1] >= 0 && mx >= 300 && mx <= 300 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[1][returnValue];
    if (stackIndex[2] >= 0 && mx >= 570 && mx <= 570 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[2][returnValue];
    if (stackIndex[3] >= 0 && mx >= 840 && mx <= 840 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[3][returnValue];
    if (stackIndex[4] >= 0 && mx >= 1380 && mx <= 1380 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[4][returnValue];
    if (stackIndex[5] >= 0 && mx >= 30 && mx <= 30 + cardWidth && my >= 396 + (stackIndex[5])*cardGap && my <= 396 + (stackIndex[5])*cardGap + cardHeight) return rowDataSheet[5][returnValue];
    if (stackIndex[6] >= 0 && mx >= 300 && mx <= 300 + cardWidth && my >= 396 + (stackIndex[6])*cardGap && my <= 396 + (stackIndex[6])*cardGap + cardHeight) return rowDataSheet[6][returnValue];
    if (stackIndex[7] >= 0 && mx >= 570 && mx <= 570 + cardWidth && my >= 396 + (stackIndex[7])*cardGap && my <= 396 + (stackIndex[7])*cardGap + cardHeight) return rowDataSheet[7][returnValue];
    if (stackIndex[8] >= 0 && mx >= 840 && mx <= 840 + cardWidth && my >= 396 + (stackIndex[8])*cardGap && my <= 396 + (stackIndex[8])*cardGap + cardHeight) return rowDataSheet[8][returnValue];
    if (stackIndex[9] >= 0 && mx >= 1110 && mx <= 1110 + cardWidth && my >= 396 + (stackIndex[9])*cardGap && my <= 396 + (stackIndex[9])*cardGap + cardHeight) return rowDataSheet[9][returnValue];
    if (stackIndex[10] >= 0 && mx >= 1380 && mx <= 1380 + cardWidth && my >= 396 + (stackIndex[10])*cardGap && my <= 396 + (stackIndex[10])*cardGap + cardHeight) return rowDataSheet[10][returnValue];
    if (stackIndex[11] >= 0 && mx >= 1650 && mx <= 1650 + cardWidth && my >= 396 + (stackIndex[11])*cardGap && my <= 396 + (stackIndex[11])*cardGap + cardHeight) return rowDataSheet[11][returnValue];
    return 99;
}

int middleCardSelected(int returnValue, int stackIndex[12], int faceIndex[7][19][2], int mx, int my, int cardWidth, int midHeight) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 19; j++) {
            if (faceIndex[i][j][0] != 99 && mx >= 30 + (cardWidth+30)*i && mx <= 30 + cardWidth + (cardWidth+30)*i &&
                my >= faceIndex[i][j][1] && my <= faceIndex[i][j][1] + midHeight) {
                    if (returnValue == 0) return i + 1;
                    if (returnValue == 1) return faceIndex[i][j][0];
                }
        }
    }
    return 99;
}

int main(int argc, char const *argv[]) {
    Deck deck; // create deck of cards

    // initilize window
    const int WIN_WIDTH  = 1920;
    const int WIN_HEIGHT = 1080;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Solitaire");

    // game variables
    Node* history = NULL;
    bool newGame = true;
    bool useGraphics = true;
    // bool won = false;
    int cardWidth = 240;
    int cardHeight = 336;
    int cardGap = 30;
    Color cardBack = BLUE;
    Color cardFront = WHITE;

    // graphics
    cardGraphics cg;
    cg.cardFaces = {LoadTexture("assets/playing-card-faces.png")};
    Texture2D bg;
    bg = {LoadTexture("assets/bg01.png")};
    Rectangle bgRec{0, 0, 1920, 1080};
    Vector2 bgPos{0, 0};

    // stacks
    /******************************** 
     *            STACKS            *
     ********************************
     * [ 0][ 1][ 2][ 3]    [ 4][12] *
     *                              *
     * [ 5][ 6][ 7][ 8][ 9][10][11] *
     ********************************/
    int stacks[13][24];
    int stackIndex[13]{-1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 23}; //index of top card in stack, -1 = no cards in stack
    // int dragCards[12]{};
    
    // mouse
    int mx = GetMouseX();
    int my = GetMouseY();

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // const float dT { GetFrameTime() };
        mx = GetMouseX();
        my = GetMouseY();

        if (newGame) {
            deck.shuffle();
            history = NULL;
            int drawIndex = 0;
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 13; j++) {
                    stacks[j][i] = 99;
                }
            }
            for (int cnt = 0; cnt < 52; cnt++) {
                // set y values of cards
                if (cnt < 7) {
                    deck.setY(cnt, 396);
                } else if (cnt < 13) {
                    deck.setY(cnt, 417);
                } else if (cnt < 18) {
                    deck.setY(cnt, 438);
                } else if (cnt < 22) {
                    deck.setY(cnt, 459);
                } else if (cnt < 25) {
                    deck.setY(cnt, 480);
                } else if (cnt < 27) {
                    deck.setY(cnt, 501);
                } else if (cnt == 27) {
                    deck.setY(cnt, 522);
                } else {
                    deck.setY(cnt, 30);
                }

                // set x values of cards
                if (cnt == 0) {
                    deck.setX(cnt, 30);
                    stacks[5][0] = 0;
                } else if (cnt == 1 || cnt == 7) {
                    deck.setX(cnt, 300);
                    stacks[6][0] = 1; stacks[6][1] = 7;
                } else if (cnt == 2 || cnt == 8 || cnt == 13) {
                    deck.setX(cnt, 570);
                    stacks[7][0] = 2; stacks[7][1] = 8; stacks[7][2] = 13;
                } else if (cnt == 3 || cnt == 9 || cnt == 14 || cnt == 18) {
                    deck.setX(cnt, 840);
                    stacks[8][0] = 3; stacks[8][1] = 9; stacks[8][2] = 14; stacks[8][3] = 18;
                } else if (cnt == 4 || cnt == 10 || cnt == 15 || cnt == 19 || cnt == 22) {
                    deck.setX(cnt, 1110);
                    stacks[9][0] = 4; stacks[9][1] = 10; stacks[9][2] = 15; stacks[9][3] = 19; stacks[9][4] = 22;
                } else if (cnt == 5 || cnt == 11 || cnt == 16 || cnt == 20 || cnt == 23 || cnt == 25) {
                    deck.setX(cnt, 1380);
                    stacks[10][0] = 5; stacks[10][1] = 11; stacks[10][2] = 16; stacks[10][3] = 20; stacks[10][4] = 23; stacks[10][5] = 25;
                } else if (cnt == 6 || cnt == 12 || cnt == 17 || cnt == 21 || cnt == 24 || cnt == 26 || cnt == 27) {
                    deck.setX(cnt, 1650);
                    stacks[11][0] = 6; stacks[11][1] = 12; stacks[11][2] = 17; stacks[11][3] = 21; stacks[11][4] = 24; stacks[11][5] = 26; stacks[11][6] = 27;
                } else {
                    deck.setX(cnt, 1650);
                    stacks[12][drawIndex] = cnt;
                    drawIndex++;
                }

                // set face up
                if (cnt == 0 || cnt == 7 || cnt == 13 || cnt == 18 || cnt == 22 || cnt == 25 || cnt == 27) {
                    deck.setFaceUp(cnt, true);
                } else {
                    deck.setFaceUp(cnt, false);
                }
            }
            newGame = false;
        }

        // fullscreen
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        // undo
        if (IsKeyPressed(KEY_BACKSPACE) && history != NULL) {
            if(history->currentStack != 12) {
                stackIndex[history->previousStack]++;
                stacks[history->previousStack][stackIndex[history->previousStack]] = stacks[history->currentStack][stackIndex[history->currentStack]];
                stacks[history->currentStack][stackIndex[history->currentStack]] = 99;
                stackIndex[history->currentStack]--;
                if (history->previousStack != 12 && history->previousStack != 4) {
                    deck.setX(history->cardIndex, 30 + (history->previousStack-5)*(30+cardWidth));
                    deck.setY(history->cardIndex, 396 + cardGap*stackIndex[history->previousStack]);
                    deck.setFaceUp(history->cardIndex, history->prevFaceUp);
                } else if (history->previousStack != 4) {
                    deck.setX(history->cardIndex, 0);
                    deck.setFaceUp(history->cardIndex, false);
                } else {
                    deck.setX(history->cardIndex, 1380);
                    deck.setY(history->cardIndex, 30);
                }
                pop(&history);
            } else {
                for (int pos = stackIndex[12]; pos >= 0; pos--) {
                    stackIndex[4]++;
                    stacks[4][stackIndex[4]] = stacks[12][stackIndex[12]];
                    stacks[12][stackIndex[12]] = 99;
                    stackIndex[12]--;
                    deck.setX(stacks[4][stackIndex[4]], 1380);
                    deck.setFaceUp(stacks[4][stackIndex[4]], true);
                }
                pop(&history);
            }
        }

        // click on stacks[12]
        if (mx >= 1650 && mx <= 1650 + cardWidth && my >= 30 && my <= 30 + cardHeight && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (stackIndex[12] >= 0) {
                stackIndex[4]++;
                stacks[4][stackIndex[4]] = stacks[12][stackIndex[12]];
                deck.setX(stacks[4][stackIndex[4]], 1380);
                deck.setFaceUp(stacks[4][stackIndex[4]], true);
                stacks[12][stackIndex[12]] = 99;
                stackIndex[12]--;
                push(&history, stacks[4][stackIndex[4]], 4, 12, false);
            } else {
                push(&history, 0, 12, 4, true);
                while (stackIndex[4] >= 0) {
                    stackIndex[12]++;
                    stacks[12][stackIndex[12]] = stacks[4][stackIndex[4]];
                    deck.setFaceUp(stacks[4][stackIndex[4]], false);
                    deck.setX(stacks[4][stackIndex[4]], 1650);
                    stacks[4][stackIndex[4]] = 99;
                    stackIndex[4]--;
                }
            }
        }

        // click face-up card
        int cardInfoSheet[12][2];
        cardInfoSheet[0][0] = 1; cardInfoSheet[1][0] = 2; cardInfoSheet[2][0] = 3; cardInfoSheet[3][0] = 4;
        cardInfoSheet[4][0] = 5; cardInfoSheet[5][0] = 6; cardInfoSheet[6][0] = 7; cardInfoSheet[7][0] = 8;
        cardInfoSheet[8][0] = 9; cardInfoSheet[9][0] = 10;cardInfoSheet[10][0]=11; cardInfoSheet[11][0]=12;

        for (int i = 0; i < 12; i++) if (stackIndex[i] >= 0) cardInfoSheet[i][1] = stacks[i][stackIndex[i]];

        // top
        if (isOverFaceUpTop(stackIndex, mx, my, cardWidth, cardHeight, cardGap) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int currentStack = topCardSelected(0, stackIndex, cardInfoSheet, mx, my, cardWidth, cardHeight, cardGap);
            int currentDeckPos = topCardSelected(1, stackIndex, cardInfoSheet, mx, my, cardWidth, cardHeight, cardGap);
            int currentSuit = deck.getCardSuit(currentDeckPos);
            int currentValue = deck.getValue(currentDeckPos);
            bool currentIsRed = deck.isRed(currentDeckPos);
            bool currentIsBlack = deck.isBlack(currentDeckPos);

            int newStack;

            bool didCardMove = false;
            // foundation
            for (int i = 0; i < 4; i++) {
                if (!didCardMove && currentStack != i+1 &&
                    ((stackIndex[i] < 0 && currentValue == 1) ||
                    (stackIndex[i] >= 0 && deck.getCardSuit(stacks[i][stackIndex[i]]) == currentSuit &&
                    deck.getValue(stacks[i][stackIndex[i]])+1 == currentValue))) {
                        newStack = i;
                        stackIndex[i]++;
                        stacks[i][stackIndex[i]] = currentDeckPos;
                        deck.setX(currentDeckPos, 30 + i*(30+cardWidth));
                        deck.setY(currentDeckPos, 30);
                        didCardMove = true;
                    }
            }

            // rows
            for (int i = 5; i < 12; i++) {
                if (!didCardMove && currentStack != i+1 &&
                    ((stackIndex[i] < 0 && currentValue == 13) ||
                    (stackIndex[i] >= 0 && (deck.isRed(stacks[i][stackIndex[i]]) == currentIsBlack ||
                    deck.isBlack(stacks[i][stackIndex[i]]) == currentIsRed) &&
                    deck.getValue(stacks[i][stackIndex[i]])-1 == currentValue))) {
                        newStack = i;
                        stackIndex[i]++;
                        stacks[i][stackIndex[i]] = currentDeckPos;
                        deck.setX(currentDeckPos, 30 + (i-5)*(30+cardWidth));
                        deck.setY(currentDeckPos, 396 + stackIndex[i]*cardGap);
                        didCardMove = true;
                    }
            }

            if (didCardMove) {
                bool prevFaceUp = false;
                if (stackIndex[currentStack-1] >= 0) prevFaceUp = deck.isFaceUp(stacks[currentStack-1][stackIndex[currentStack-2]]);
                push(&history, currentDeckPos, newStack, currentStack-1, prevFaceUp);
                stacks[currentStack-1][stackIndex[currentStack-1]] = 99;
                stackIndex[currentStack-1]--;
            }
        }
 
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            int i = 0; // DEBUGGING
            if (i == 0) i = 1;
        }

        int faceIndex[7][19][2];
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 19; j++) faceIndex[i][j][0] = 99;
            for (int pos = stackIndex[i+5]-1; pos >= 0; pos--) if (deck.isFaceUp(stacks[i+5][pos])) {
                faceIndex[i][pos][0] = stacks[i+5][pos];
                faceIndex[i][pos][1] = deck.getY(stacks[i+5][pos]);
            }
        }

        int newStack;

        // middle
        if (isOverFaceUpMiddle(stackIndex, faceIndex, mx, my, cardWidth, cardGap) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int currentStack = middleCardSelected(0, stackIndex, faceIndex, mx, my, cardWidth, cardGap);
            int currentDeckPos = middleCardSelected(1, stackIndex, faceIndex, mx, my, cardWidth, cardGap);
            int currentValue = deck.getValue(currentDeckPos);
            bool currentIsRed = deck.isRed(currentDeckPos);
            bool currentIsBlack = deck.isBlack(currentDeckPos);
            int currentIndex = 0;
            for (int i = 0; i <= stackIndex[4 + currentStack]; i++) {
                if ((currentStack == 1 && stacks[5][i] == currentDeckPos) || (currentStack == 2 && stacks[6][i] == currentDeckPos) ||
                    (currentStack == 3 && stacks[7][i] == currentDeckPos) || (currentStack == 4 && stacks[8][i] == currentDeckPos) ||
                    (currentStack == 5 && stacks[9][i] == currentDeckPos) || (currentStack == 6 && stacks[10][i] == currentDeckPos) ||
                    (currentStack == 7 && stacks[11][i] == currentDeckPos)) break;
                currentIndex++;
            }
            int currentRowMax = stackIndex[4 + currentStack];

            bool didCardMove = false;
            for (int i = 1; i < 8; i++) {                   // Check stacks 5 - 11
                if (!didCardMove && currentStack != i &&    // if card has not already been moved & both stacks are different.
                    ((stackIndex[i+4] < 0 && currentValue == 13) ||  // move if new stack has no cards & current card is a King or 
                    (stackIndex[i+4] >= 0 &&                                           // new stack has cards &
                    ((deck.isRed(stacks[i+4][stackIndex[i+4]]) && currentIsBlack) ||   // ( new stacks top card is red & current card is black or
                    (deck.isBlack(stacks[i+4][stackIndex[i+4]]) && currentIsRed)) &&   // new stacks top card is black & current card is red ) and
                    deck.getValue(stacks[i+4][stackIndex[i+4]])-1 == currentValue))) { // value of current card is one less than new stacks top card.
                        
                        newStack = i;  // new stack position for history
                        // Move currently select cards and all cards on top of it to new stack.
                        for (int index = currentIndex; index <= currentRowMax; index++) {
                            stackIndex[i+4]++;
                            if (currentStack != i ) stacks[i+4][stackIndex[i+4]] = stacks[currentStack+4][index];
                            deck.setX(stacks[i+4][stackIndex[i+4]], 30 + (i-1)*(30+cardWidth));
                            deck.setY(stacks[i+4][stackIndex[i+4]], 396 + stackIndex[i+4]*cardGap);
                        }
                        didCardMove = true; // card(s) have been moved
                }
            }

            if (didCardMove) {
                bool prevFaceUp = false;
                if (stackIndex[currentStack-1] >= 0) prevFaceUp = deck.isFaceUp(stacks[currentStack-1][stackIndex[currentStack-2]]);
                push(&history, currentDeckPos, newStack, currentStack-1, prevFaceUp); // Add move to history.
                for (int pos = currentRowMax; pos >= currentIndex; pos--) {
                    stacks[currentStack + 4][pos] = 99;              // Removed any cards that have been moved &
                    stackIndex[currentStack + 4] = currentIndex - 1; // change stacks index value to match.
                }
            }
        }

            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawTextureRec(bg, bgRec, bgPos, WHITE);

            for (int p = 0; p < 5; p++) {

                if (p < 4) {
                    Rectangle rec{(float)(30 + (p*270)), 30, 240, 336};
                    DrawRectangleRoundedLines(rec, 0.1, 5, 0.5, BLACK);
                } else {
                    Rectangle rec{1650, 30, 240, 336};
                    if (stackIndex[12] >= 0) DrawRectangleRounded(rec, 0.1, 5, cardBack);
                    DrawRectangleRoundedLines(rec, 0.1, 5, 0.5, BLACK);
                }
            }

            // check rows
            for (int i = 5; i < 12; i++) {
                if (stackIndex[i] >= 0 && !deck.isFaceUp(stacks[i][stackIndex[i]])) deck.setFaceUp(stacks[i][stackIndex[i]], true);
            }

            // display cards
            for (int pos = 0; pos < 23; pos++) {
                for (int i = 0; i < 12; i++) {
                    if (i < 5) {
                        if (stackIndex[i] >= 0) drawCard(deck, stacks[i][stackIndex[i]], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                    } else {
                        if (pos <= stackIndex[i]) drawCard(deck, stacks[i][pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                    }
                }
            }

            EndDrawing();
        
    }
    CloseWindow();
}