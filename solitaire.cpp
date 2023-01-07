#include "raylib.h"
#include <string>

class Deck {
    struct card {
        int value = 0;
        int suit;           // 0 = Clubs, 1 = Spades, 2 = Diamonds, 3 = Hearts
        char name[20]{};
        bool faceUp = false;
        bool isRed = false;
        bool isBlack = false;
        int x = 0;
        int y = 0;
    };

    card cards[52];

    public:    

        Deck() {
            int cnt = 0;
            for (auto s: {"CLUBS", "SPADES", "DIAMONDS", "HEARTS"}) {
                for (auto v: {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"}) {
                    if (cnt < 13) {
                        cards[cnt].value = cnt + 1;
                        cards[cnt].suit = 0;
                    } else if (cnt < 26) {
                        cards[cnt].value = cnt - 12;
                        cards[cnt].suit = 1;
                    } else if (cnt < 39) {
                        cards[cnt].value = cnt - 25;
                        cards[cnt].suit = 2;
                    } else {
                        cards[cnt].value = cnt - 38;
                        cards[cnt].suit = 3;
                    }
                    std::sprintf(cards[cnt].name, "%s OF %s", v, s);
                    if (cnt < 26) {
                        cards[cnt].isRed = false;
                        cards[cnt].isBlack = true;
                    } else {
                        cards[cnt].isRed = true;
                        cards[cnt].isBlack = false;
                    }
                    cnt++;
                }
            }
            shuffle();
        }

        char* getCardName(int i) { return cards[i].name; }

        int getValue(int i) { 
            if (i > 51) return 0;
            return cards[i].value;
        }

        int getCardSuit(int i) { return cards[i].suit; }

        int getX(int i) { return cards[i].x; }

        int getY(int i) { return cards[i].y; }

        bool isRed(int i) { return cards[i].isRed; }

        bool isBlack(int i) { return (i != 99 && cards[i].isBlack); }

        bool isFaceUp(int i) { return cards[i].faceUp; }

        void setX(int index, int xValue) { cards[index].x = xValue; }

        void setY(int index, int yValue) { cards[index].y = yValue; }

        void setFaceUp(int index, bool isFaceUp) { cards[index].faceUp = isFaceUp; }

        void shuffle() {
            card newOrder[52];
            int cnt = 0;
            while (cnt < 52) {
                int random = rand()%51;
                bool insertedCard = false;
                while (!insertedCard) {
                    if (newOrder[random].isBlack == true || newOrder[random].isRed == true) {
                        if (random < 51) random++;
                        else random = 0;
                    } else {
                        newOrder[random] = cards[cnt];
                        cnt++;
                        insertedCard = true;
                    }
                }
            }
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
    // float gameTime = 0;
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
    int stackIndex[13]{-1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 23};
    int foundation1[13]{}; // stackIndex[0]
    int foundation2[13]{}; // stackIndex[1]
    int foundation3[13]{}; // stackIndex[2]
    int foundation4[13]{}; // stackIndex[3]
    int stack[24]{};       // stackIndex[12]
    int discard[24]{};     // stackIndex[4]
    int row1[13]{};        // stackIndex[5]
    int row2[14]{};        // stackIndex[6]
    int row3[15]{};        // stackIndex[7]
    int row4[16]{};        // stackIndex[8]
    int row5[17]{};        // stackIndex[9]
    int row6[18]{};        // stackIndex[10]
    int row7[19]{};        // stackIndex[11]
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
            int drawIndex = 0;
            for (int i = 0; i < 24; i++) {
                discard[i] = 99;
                if (i < 13) {
                    foundation1[i] = 99;
                    foundation2[i] = 99;
                    foundation3[i] = 99;
                    foundation4[i] = 99;
                    // if (i < 12) dragCards[i] = 99;
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
                    row1[0] = 0;
                    for(int i = 1; i < 13; i++) row1[i] = 99;
                } else if (cnt == 1 || cnt == 7) {
                    deck.setX(cnt, 300);
                    row2[0] = 1; row2[1] = 7;
                    for (int i = 2; i < 14; i++) row2[i] = 99;
                } else if (cnt == 2 || cnt == 8 || cnt == 13) {
                    deck.setX(cnt, 570);
                    row3[0] = 2; row3[1] = 8; row3[2] = 13;
                    for (int i = 3; i < 15; i++) row3[i] = 99;
                } else if (cnt == 3 || cnt == 9 || cnt == 14 || cnt == 18) {
                    deck.setX(cnt, 840);
                    row4[0] = 3; row4[1] = 9; row4[2] = 14; row4[3] = 18;
                    for (int i = 4; i < 16; i++) row4[i] = 99;
                } else if (cnt == 4 || cnt == 10 || cnt == 15 || cnt == 19 || cnt == 22) {
                    deck.setX(cnt, 1110);
                    row5[0] = 4; row5[1] = 10; row5[2] = 15; row5[3] = 19; row5[4] = 22;
                    for (int i = 5; i < 17; i++) row5[i] = 99;
                } else if (cnt == 5 || cnt == 11 || cnt == 16 || cnt == 20 || cnt == 23 || cnt == 25) {
                    deck.setX(cnt, 1380);
                    row6[0] = 5; row6[1] = 11; row6[2] = 16; row6[3] = 20; row6[4] = 23; row6[5] = 25;
                    for (int i = 6; i < 18; i++) row6[i] = 99;
                } else if (cnt == 6 || cnt == 12 || cnt == 17 || cnt == 21 || cnt == 24 || cnt == 26 || cnt == 27) {
                    deck.setX(cnt, 1650);
                    row7[0] = 6; row7[1] = 12; row7[2] = 17; row7[3] = 21; row7[4] = 24; row7[5] = 26; row7[6] = 27;
                    for (int i = 7; i < 19; i++) row7[i] = 99;
                } else {
                    deck.setX(cnt, 1650);
                    stack[drawIndex] = cnt;
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

        // click on stack
        if (mx >= 1650 && mx <= 1650 + cardWidth && my >= 30 && my <= 30 + cardHeight && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (stackIndex[12] >= 0) {
                stackIndex[4]++;
                discard[stackIndex[4]] = stack[stackIndex[12]];
                deck.setX(discard[stackIndex[4]], 1380);
                deck.setFaceUp(discard[stackIndex[4]], true);
                stack[stackIndex[12]] = 99;
                stackIndex[12]--;
            } else {
                while (stackIndex[4] >= 0) {
                    stackIndex[12]++;
                    stack[stackIndex[12]] = discard[stackIndex[4]];
                    deck.setFaceUp(discard[stackIndex[4]], false);
                    deck.setX(discard[stackIndex[4]], 1650);
                    discard[stackIndex[4]] = 99;
                    stackIndex[4]--;
                }
            }
        }

        // click face-up card
        int cardInfoSheet[12][2];
        cardInfoSheet[0][0] = 1; cardInfoSheet[1][0] = 2; cardInfoSheet[2][0] = 3; cardInfoSheet[3][0] = 4;
        cardInfoSheet[4][0] = 5; cardInfoSheet[5][0] = 6; cardInfoSheet[6][0] = 7; cardInfoSheet[7][0] = 8;
        cardInfoSheet[8][0] = 9; cardInfoSheet[9][0] = 10;cardInfoSheet[10][0]=11; cardInfoSheet[11][0]=12;

        if (stackIndex[0] >= 0) cardInfoSheet[0][1] = foundation1[stackIndex[0]];
        if (stackIndex[1] >= 0) cardInfoSheet[1][1] = foundation2[stackIndex[1]];
        if (stackIndex[2] >= 0) cardInfoSheet[2][1] = foundation3[stackIndex[2]];
        if (stackIndex[3] >= 0) cardInfoSheet[3][1] = foundation4[stackIndex[3]];
        if (stackIndex[4] >= 0) cardInfoSheet[4][1] = discard[stackIndex[4]];
        if (stackIndex[5] >= 0) cardInfoSheet[5][1] = row1[stackIndex[5]];
        if (stackIndex[6] >= 0) cardInfoSheet[6][1] = row2[stackIndex[6]];
        if (stackIndex[7] >= 0) cardInfoSheet[7][1] = row3[stackIndex[7]];
        if (stackIndex[8] >= 0) cardInfoSheet[8][1] = row4[stackIndex[8]];
        if (stackIndex[9] >= 0) cardInfoSheet[9][1] = row5[stackIndex[9]];
        if (stackIndex[10] >= 0) cardInfoSheet[10][1] = row6[stackIndex[10]];
        if (stackIndex[11] >= 0) cardInfoSheet[11][1] = row7[stackIndex[11]];

        // top
        if (isOverFaceUpTop(stackIndex, mx, my, cardWidth, cardHeight, cardGap) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int currentStack = topCardSelected(0, stackIndex, cardInfoSheet, mx, my, cardWidth, cardHeight, cardGap);
            int currentDeckPos = topCardSelected(1, stackIndex, cardInfoSheet, mx, my, cardWidth, cardHeight, cardGap);
            int currentSuit = deck.getCardSuit(currentDeckPos);
            int currentValue = deck.getValue(currentDeckPos);
            bool currentIsRed = deck.isRed(currentDeckPos);
            bool currentIsBlack = deck.isBlack(currentDeckPos);

            bool didCardMove = false;
            // foundation
            if (!didCardMove && currentStack != 1 && 
                ((stackIndex[0] < 0 && currentValue == 1) ||
                (stackIndex[0] >= 0 && deck.getCardSuit(foundation1[stackIndex[0]]) == currentSuit && 
                deck.getValue(foundation1[stackIndex[0]])+1 == currentValue))) {
                    stackIndex[0]++;
                    foundation1[stackIndex[0]] = currentDeckPos;
                    deck.setX(currentDeckPos, 30);
                    deck.setY(currentDeckPos, 30);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 2 && 
                ((stackIndex[1] < 0 && currentValue == 1) || 
                (stackIndex[1] >= 0 && deck.getCardSuit(foundation2[stackIndex[1]]) == currentSuit &&
                deck.getValue(foundation2[stackIndex[1]])+1 == currentValue))) { 
                    stackIndex[1]++;
                    foundation2[stackIndex[1]] = currentDeckPos;
                    deck.setX(currentDeckPos, 300);
                    deck.setY(currentDeckPos, 30);
                    didCardMove = true; 
            }
            if (!didCardMove && currentStack != 3 && 
                ((stackIndex[2] < 0 && currentValue == 1) || 
                (stackIndex[2] >= 0 && deck.getCardSuit(foundation3[stackIndex[2]]) == currentSuit &&
                deck.getValue(foundation3[stackIndex[2]])+1 == currentValue))) { 
                    stackIndex[2]++;
                    foundation3[stackIndex[2]] = currentDeckPos;
                    deck.setX(currentDeckPos, 570);
                    deck.setY(currentDeckPos, 30);
                    didCardMove = true; 
            }
            if (!didCardMove && currentStack != 4 && 
                ((stackIndex[3] < 0 && currentValue == 1) || 
                (stackIndex[3] >= 0 && deck.getCardSuit(foundation4[stackIndex[3]]) == currentSuit &&
                deck.getValue(foundation4[stackIndex[3]])+1 == currentValue))) { 
                    stackIndex[3]++;
                    foundation4[stackIndex[3]] = currentDeckPos;
                    deck.setX(currentDeckPos, 840);
                    deck.setY(currentDeckPos, 30);
                    didCardMove = true; 
            }
            // rows
            if (!didCardMove && currentStack != 6 && 
                ((stackIndex[5] < 0 && currentValue == 13) || 
                (stackIndex[5] >= 0 && (deck.isRed(row1[stackIndex[5]]) == currentIsBlack ||
                deck.isBlack(row1[stackIndex[5]]) == currentIsRed) &&
                deck.getValue(row1[stackIndex[5]])-1 == currentValue))) {
                    stackIndex[5]++;
                    row1[stackIndex[5]] = currentDeckPos;
                    deck.setX(currentDeckPos, 30);
                    deck.setY(currentDeckPos, 396 + stackIndex[5]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 7 &&
                ((stackIndex[6] < 0 && currentValue == 13) || 
                (stackIndex[6] >= 0 && (deck.isRed(row2[stackIndex[6]]) == currentIsBlack || 
                deck.isBlack(row2[stackIndex[6]]) == currentIsRed) &&
                deck.getValue(row2[stackIndex[6]])-1 == currentValue))) {
                    stackIndex[6]++;
                    row2[stackIndex[6]] = currentDeckPos;
                    deck.setX(currentDeckPos, 300);
                    deck.setY(currentDeckPos, 396 + stackIndex[6]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 8 && 
                ((stackIndex[7] < 0 && currentValue == 13) || 
                (stackIndex[7] >= 0 && (deck.isRed(row3[stackIndex[7]]) == currentIsBlack ||
                deck.isBlack(row3[stackIndex[7]]) == currentIsRed) &&
                deck.getValue(row3[stackIndex[7]])-1 == currentValue))) {
                    stackIndex[7]++;
                    row3[stackIndex[7]] = currentDeckPos;
                    deck.setX(currentDeckPos, 570);
                    deck.setY(currentDeckPos, 396 + stackIndex[7]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 9 && 
                ((stackIndex[8] < 0 && currentValue == 13) ||
                (stackIndex[8] >= 0 && (deck.isRed(row4[stackIndex[8]]) == currentIsBlack ||
                deck.isBlack(row4[stackIndex[8]]) == currentIsRed) &&
                deck.getValue(row4[stackIndex[8]])-1 == currentValue))) {
                    stackIndex[8]++;
                    row4[stackIndex[8]] = currentDeckPos;
                    deck.setX(currentDeckPos, 840);
                    deck.setY(currentDeckPos, 396 + stackIndex[8]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 10 && 
                ((stackIndex[9] < 0 && currentValue == 13) || 
                (stackIndex[9] >= 0 && (deck.isRed(row5[stackIndex[9]]) == currentIsBlack || 
                deck.isBlack(row5[stackIndex[9]]) == currentIsRed) &&
                deck.getValue(row5[stackIndex[9]])-1 == currentValue))) {
                    stackIndex[9]++;
                    row5[stackIndex[9]] = currentDeckPos;
                    deck.setX(currentDeckPos, 1110);
                    deck.setY(currentDeckPos, 396 + stackIndex[9]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 11 && 
                ((stackIndex[10] < 0 && currentValue == 13) ||
                (stackIndex[10] >= 0 && (deck.isRed(row6[stackIndex[10]]) == currentIsBlack ||
                deck.isBlack(row6[stackIndex[10]]) == currentIsRed) &&
                deck.getValue(row6[stackIndex[10]])-1 == currentValue))) {
                    stackIndex[10]++;
                    row6[stackIndex[10]] = currentDeckPos;
                    deck.setX(currentDeckPos, 1380);
                    deck.setY(currentDeckPos, 396 + stackIndex[10]*cardGap);
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 12 && 
                ((stackIndex[11] < 0 && currentValue == 13) ||
                (stackIndex[11] >= 0 && (deck.isRed(row7[stackIndex[11]]) == currentIsBlack ||
                deck.isBlack(row7[stackIndex[11]]) == currentIsRed) &&
                deck.getValue(row7[stackIndex[11]])-1 == currentValue))) {
                    stackIndex[11]++;
                    row7[stackIndex[11]] = currentDeckPos;
                    deck.setX(currentDeckPos, 1650);
                    deck.setY(currentDeckPos, 396 + stackIndex[11]*cardGap);
                    didCardMove = true;
            }

            if (didCardMove) {
                if (currentStack == 1) { foundation1[stackIndex[0]] = 99; stackIndex[0]--; }
                if (currentStack == 2) { foundation2[stackIndex[1]] = 99; stackIndex[1]--; }
                if (currentStack == 3) { foundation3[stackIndex[2]] = 99; stackIndex[2]--; }
                if (currentStack == 4) { foundation4[stackIndex[3]] = 99; stackIndex[3]--; }
                if (currentStack == 5) { discard[stackIndex[4]] = 99; stackIndex[4]--; }
                if (currentStack == 6) { row1[stackIndex[5]] = 99; stackIndex[5]--; }
                if (currentStack == 7) { row2[stackIndex[6]] = 99; stackIndex[6]--; }
                if (currentStack == 8) { row3[stackIndex[7]] = 99; stackIndex[7]--; }
                if (currentStack == 9) { row4[stackIndex[8]] = 99; stackIndex[8]--; }
                if (currentStack == 10) { row5[stackIndex[9]] = 99; stackIndex[9]--; }
                if (currentStack == 11) { row6[stackIndex[10]] = 99; stackIndex[10]--; }
                if (currentStack == 12) { row7[stackIndex[11]] = 99; stackIndex[11]--; }
            }
        }
 
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            int i = 0; // DEBUGGING
            if (i == 0) i = 1;
        }

        int faceIndex[7][19][2];
        for (int i = 0; i < 7; i++) for (int j = 0; j < 19; j++) faceIndex[i][j][0] = 99;
        for (int pos = stackIndex[5]-1; pos >= 0; pos--) if (deck.isFaceUp(row1[pos])) { faceIndex[0][pos][0] = row1[pos]; faceIndex[0][pos][1] = deck.getY(row1[pos]); }
        for (int pos = stackIndex[6]-1; pos >= 0; pos--) if (deck.isFaceUp(row2[pos])) { faceIndex[1][pos][0] = row2[pos]; faceIndex[1][pos][1] = deck.getY(row2[pos]); }
        for (int pos = stackIndex[7]-1; pos >= 0; pos--) if (deck.isFaceUp(row3[pos])) { faceIndex[2][pos][0] = row3[pos]; faceIndex[2][pos][1] = deck.getY(row3[pos]); }
        for (int pos = stackIndex[8]-1; pos >= 0; pos--) if (deck.isFaceUp(row4[pos])) { faceIndex[3][pos][0] = row4[pos]; faceIndex[3][pos][1] = deck.getY(row4[pos]); }
        for (int pos = stackIndex[9]-1; pos >= 0; pos--) if (deck.isFaceUp(row5[pos])) { faceIndex[4][pos][0] = row5[pos]; faceIndex[4][pos][1] = deck.getY(row5[pos]); }
        for (int pos = stackIndex[10]-1; pos >= 0; pos--) if (deck.isFaceUp(row6[pos])) { faceIndex[5][pos][0] = row6[pos]; faceIndex[5][pos][1] = deck.getY(row6[pos]); }
        for (int pos = stackIndex[11]-1; pos >= 0; pos--) if (deck.isFaceUp(row7[pos])) { faceIndex[6][pos][0] = row7[pos]; faceIndex[6][pos][1] = deck.getY(row7[pos]); }

        // middle
        if (isOverFaceUpMiddle(stackIndex, faceIndex, mx, my, cardWidth, cardGap) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int currentStack = middleCardSelected(0, stackIndex, faceIndex, mx, my, cardWidth, cardGap);
            int currentDeckPos = middleCardSelected(1, stackIndex, faceIndex, mx, my, cardWidth, cardGap);
            int currentValue = deck.getValue(currentDeckPos);
            bool currentIsRed = deck.isRed(currentDeckPos);
            bool currentIsBlack = deck.isBlack(currentDeckPos);
            int currentIndex = 0;
            for (int i = 0; i <= stackIndex[4 + currentStack]; i++) {
                if ((currentStack == 1 && row1[i] == currentDeckPos) || (currentStack == 2 && row2[i] == currentDeckPos) ||
                    (currentStack == 3 && row3[i] == currentDeckPos) || (currentStack == 4 && row4[i] == currentDeckPos) ||
                    (currentStack == 5 && row5[i] == currentDeckPos) || (currentStack == 6 && row6[i] == currentDeckPos) ||
                    (currentStack == 7 && row7[i] == currentDeckPos)) break;
                currentIndex++;
            }
            int currentRowMax = stackIndex[4 + currentStack];

            bool didCardMove = false;
            if (!didCardMove && currentStack != 1 && 
                ((stackIndex[5] < 0 && currentValue == 13) || 
                (stackIndex[5] >= 0 &&
                ((deck.isRed(row1[stackIndex[5]]) && currentIsBlack) || 
                (deck.isBlack(row1[stackIndex[5]]) && currentIsRed)) &&
                deck.getValue(row1[stackIndex[5]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[5]++;
                        if (currentStack == 2) row1[stackIndex[5]] = row2[index];
                        if (currentStack == 3) row1[stackIndex[5]] = row3[index];
                        if (currentStack == 4) row1[stackIndex[5]] = row4[index];
                        if (currentStack == 5) row1[stackIndex[5]] = row5[index];
                        if (currentStack == 6) row1[stackIndex[5]] = row6[index];
                        if (currentStack == 7) row1[stackIndex[5]] = row7[index];
                        
                        deck.setX(row1[stackIndex[5]], 30);
                        deck.setY(row1[stackIndex[5]], 396 + stackIndex[5]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 2 && 
                ((stackIndex[6] < 0 && currentValue == 13) || 
                (stackIndex[6] >= 0 &&
                ((deck.isRed(row2[stackIndex[6]]) && currentIsBlack) || 
                (deck.isBlack(row2[stackIndex[6]]) && currentIsRed)) &&
                deck.getValue(row2[stackIndex[6]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[6]++;
                        if (currentStack == 1) row2[stackIndex[6]] = row1[index];
                        if (currentStack == 3) row2[stackIndex[6]] = row3[index];
                        if (currentStack == 4) row2[stackIndex[6]] = row4[index];
                        if (currentStack == 5) row2[stackIndex[6]] = row5[index];
                        if (currentStack == 6) row2[stackIndex[6]] = row6[index];
                        if (currentStack == 7) row2[stackIndex[6]] = row7[index];
                        
                        deck.setX(row2[stackIndex[6]], 300);
                        deck.setY(row2[stackIndex[6]], 396 + stackIndex[6]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 3 && 
                ((stackIndex[7] < 0 && currentValue == 13) || 
                (stackIndex[7] >= 0 &&
                ((deck.isRed(row3[stackIndex[7]]) && currentIsBlack) || 
                (deck.isBlack(row3[stackIndex[7]]) && currentIsRed)) &&
                deck.getValue(row3[stackIndex[7]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[7]++;
                        if (currentStack == 2) row3[stackIndex[7]] = row2[index];
                        if (currentStack == 1) row3[stackIndex[7]] = row1[index];
                        if (currentStack == 4) row3[stackIndex[7]] = row4[index];
                        if (currentStack == 5) row3[stackIndex[7]] = row5[index];
                        if (currentStack == 6) row3[stackIndex[7]] = row6[index];
                        if (currentStack == 7) row3[stackIndex[7]] = row7[index];
                        
                        deck.setX(row3[stackIndex[7]], 570);
                        deck.setY(row3[stackIndex[7]], 396 + stackIndex[7]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 4 &&
                ((stackIndex[8] < 0 && currentValue == 13) || 
                (stackIndex[8] >= 0 &&
                ((deck.isRed(row4[stackIndex[8]]) && currentIsBlack) || 
                (deck.isBlack(row4[stackIndex[8]]) && currentIsRed)) &&
                deck.getValue(row4[stackIndex[8]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[8]++;
                        if (currentStack == 2) row4[stackIndex[8]] = row2[index];
                        if (currentStack == 3) row4[stackIndex[8]] = row3[index];
                        if (currentStack == 1) row4[stackIndex[8]] = row1[index];
                        if (currentStack == 5) row4[stackIndex[8]] = row5[index];
                        if (currentStack == 6) row4[stackIndex[8]] = row6[index];
                        if (currentStack == 7) row4[stackIndex[8]] = row7[index];
                        
                        deck.setX(row4[stackIndex[8]], 840);
                        deck.setY(row4[stackIndex[8]], 396 + stackIndex[8]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 5 && 
                ((stackIndex[9] < 0 && currentValue == 13) ||
                (stackIndex[9] >= 0 &&
                ((deck.isRed(row5[stackIndex[9]]) && currentIsBlack) ||
                (deck.isBlack(row5[stackIndex[9]]) && currentIsRed)) &&
                deck.getValue(row5[stackIndex[9]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[9]++;
                        if (currentStack == 2) row5[stackIndex[9]] = row2[index];
                        if (currentStack == 3) row5[stackIndex[9]] = row3[index];
                        if (currentStack == 4) row5[stackIndex[9]] = row4[index];
                        if (currentStack == 1) row5[stackIndex[9]] = row1[index];
                        if (currentStack == 6) row5[stackIndex[9]] = row6[index];
                        if (currentStack == 7) row5[stackIndex[9]] = row7[index];
                        
                        deck.setX(row5[stackIndex[9]], 1110);
                        deck.setY(row5[stackIndex[9]], 396 + stackIndex[9]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 6 &&
                ((stackIndex[10] < 0 && currentValue == 13) || 
                (stackIndex[10] >= 0 &&
                ((deck.isRed(row6[stackIndex[10]]) && currentIsBlack) ||
                (deck.isBlack(row6[stackIndex[10]]) && currentIsRed)) &&
                deck.getValue(row6[stackIndex[10]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[10]++;
                        if (currentStack == 2) row6[stackIndex[10]] = row2[index];
                        if (currentStack == 3) row6[stackIndex[10]] = row3[index];
                        if (currentStack == 4) row6[stackIndex[10]] = row4[index];
                        if (currentStack == 5) row6[stackIndex[10]] = row5[index];
                        if (currentStack == 1) row6[stackIndex[10]] = row1[index];
                        if (currentStack == 7) row6[stackIndex[10]] = row7[index];
                        
                        deck.setX(row6[stackIndex[10]], 1380);
                        deck.setY(row6[stackIndex[10]], 396 + stackIndex[10]*cardGap);
                    }
                    didCardMove = true;
            }
            if (!didCardMove && currentStack != 7 && 
                ((stackIndex[11] < 0 && currentValue == 13) ||
                (stackIndex[11] >= 0 && 
                ((deck.isRed(row7[stackIndex[11]]) && currentIsBlack) || 
                (deck.isBlack(row7[stackIndex[11]]) && currentIsRed)) &&
                deck.getValue(row7[stackIndex[11]])-1 == currentValue))) {
                    for (int index = currentIndex; index <= currentRowMax; index++) {
                        stackIndex[11]++;
                        if (currentStack == 2) row7[stackIndex[11]] = row2[index];
                        if (currentStack == 3) row7[stackIndex[11]] = row3[index];
                        if (currentStack == 4) row7[stackIndex[11]] = row4[index];
                        if (currentStack == 5) row7[stackIndex[11]] = row5[index];
                        if (currentStack == 6) row7[stackIndex[11]] = row6[index];
                        if (currentStack == 1) row7[stackIndex[11]] = row1[index];
                        
                        deck.setX(row7[stackIndex[11]], 1650);
                        deck.setY(row7[stackIndex[11]], 396 + stackIndex[11]*cardGap);
                    }
                    didCardMove = true;
            }

            if (didCardMove) {
                for (int pos = currentRowMax; pos >= currentIndex; pos--) {
                    if (currentStack == 1) row1[pos] = 99;
                    if (currentStack == 2) row2[pos] = 99;
                    if (currentStack == 3) row3[pos] = 99;
                    if (currentStack == 4) row4[pos] = 99;
                    if (currentStack == 5) row5[pos] = 99;
                    if (currentStack == 6) row6[pos] = 99;
                    if (currentStack == 7) row7[pos] = 99;
                }
                stackIndex[currentStack + 4] = currentIndex - 1;
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
            if (stackIndex[5] >= 0 && !deck.isFaceUp(row1[stackIndex[5]])) deck.setFaceUp(row1[stackIndex[5]], true);
            if (stackIndex[6] >= 0 && !deck.isFaceUp(row2[stackIndex[6]])) deck.setFaceUp(row2[stackIndex[6]], true);
            if (stackIndex[7] >= 0 && !deck.isFaceUp(row3[stackIndex[7]])) deck.setFaceUp(row3[stackIndex[7]], true);
            if (stackIndex[8] >= 0 && !deck.isFaceUp(row4[stackIndex[8]])) deck.setFaceUp(row4[stackIndex[8]], true);
            if (stackIndex[9] >= 0 && !deck.isFaceUp(row5[stackIndex[9]])) deck.setFaceUp(row5[stackIndex[9]], true);
            if (stackIndex[10] >= 0 && !deck.isFaceUp(row6[stackIndex[10]])) deck.setFaceUp(row6[stackIndex[10]], true);
            if (stackIndex[11] >= 0 && !deck.isFaceUp(row7[stackIndex[11]])) deck.setFaceUp(row7[stackIndex[11]], true);

            // display cards
            for (int pos = 0; pos < 23; pos++) {
                if (pos <= stackIndex[0]) drawCard(deck, foundation1[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[1]) drawCard(deck, foundation2[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[2]) drawCard(deck, foundation3[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[3]) drawCard(deck, foundation4[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[4]) drawCard(deck, discard[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[5]) drawCard(deck, row1[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[6]) drawCard(deck, row2[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[7]) drawCard(deck, row3[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[8]) drawCard(deck, row4[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[9]) drawCard(deck, row5[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[10]) drawCard(deck, row6[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
                if (pos <= stackIndex[11]) drawCard(deck, row7[pos], cardWidth, cardHeight, cardBack, cardFront, useGraphics, cg);
            }

            EndDrawing();
        
    }
    CloseWindow();
}