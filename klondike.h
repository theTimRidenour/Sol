#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "raylib.h"
#include "deck.h"
#include "graphics.h"
#include "history.h"

#include <iostream>

void drawCard(Deck deck, int index, float cardWidth, float cardHeight, Color cardBack, Color cardFront, bool usingGraphics = false, cardGraphics cg = cardGraphics()) {
    Rectangle rec {(float)deck.getX(index), (float)deck.getY(index), cardWidth, cardHeight};
    if (!deck.isFaceUp(index)) {
        DrawRectangleRounded(rec, 0.1, 5, cardBack);
    } else {
        if (!cg.customBackground) DrawRectangleRounded(rec, 0.1, 5, cardFront);
        if (!usingGraphics) {
            Color color;
            if (deck.isRed(index)) color = RED;
            else color = BLACK;
            DrawText(deck.getCardName(index), rec.x + 5, rec.y + 5, 7, color);
        } else {
            float x = (deck.getValue(index)-1) * cg.cardFaces.width/13;

            float y = deck.getCardSuit(index);
            if (cg.customBackground) y *= cg.cardFaces.height/5;
            else y *= cg.cardFaces.height/4;

            Rectangle rec{x, y, cardWidth, cardHeight};
            Vector2 pos{(float)deck.getX(index), (float)deck.getY(index)};
            if (cg.customBackground) {
                Rectangle bgRec{0, (float)(cg.cardFaces.height/5*4), cardWidth, cardHeight};
                DrawTextureRec(cg.cardFaces, bgRec, pos, WHITE);
            }
            DrawTextureRec(cg.cardFaces, rec, pos, WHITE);
        }
    }
    DrawRectangleRoundedLines(rec, 0.1, 5, 0.5, BLACK);
}

bool selectedCardFaceUp(int stackIndex[12], int mx, int my, int cardWidth, int cardHeight, int cardGap, int faceIndex[7][19][2] = NULL) {
    bool topFaceUp, middleFaceUp = false;
    if (faceIndex == NULL) {
        topFaceUp = (stackIndex[0] >= 0 && mx >= 30 && mx <= 30 + cardWidth && my >= 30 && my <= 30 + cardHeight) ||
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
    } else {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 19; j++) {
                if (faceIndex[i][j][0] != 99 && mx >= 30 + (cardWidth+30)*i && mx <= 30 + cardWidth + (cardWidth+30)*i &&
                    my >= faceIndex[i][j][1] && my <= faceIndex[i][j][1] + cardGap) middleFaceUp = true;
            }
        }
    }
    return topFaceUp || middleFaceUp;
}

int selectedCardData(int returnValue, int stackIndex[12], int mx, int my, int cardWidth, int cardHeight, int cardGap, int rowDataSheet[12][2] = NULL, int faceIndex[7][19][2] = NULL) {
    if (faceIndex == NULL) {
        int pos = 0;
        while (pos < 5) {
            if (pos < 4 && stackIndex[pos] >= 0 && mx >= 30 + pos*(cardWidth+30) && mx <= 30 + cardWidth + pos*(cardWidth+30) &&
                my >= 30 && my <= 30 + cardHeight) return rowDataSheet[pos][returnValue];
            if (stackIndex[pos] >= 0 && mx >= 1380 && mx <= 1380 + cardWidth && my >= 30 && my <= 30 + cardHeight) return rowDataSheet[pos][returnValue];
            pos++;
        }
        while (pos < 12) {
            if (stackIndex[pos] >= 0 && mx >= 30 + (pos-5)*(30+cardWidth) && mx <= 30 + cardWidth + (pos-5)*(30+cardWidth) && 
                my >= 396 + (stackIndex[pos])*cardGap && my <= 396 + (stackIndex[pos])*cardGap + cardHeight) return rowDataSheet[pos][returnValue];
            pos++;
        }
    } else {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 19; j++) {
                if (faceIndex[i][j][0] != 99 && mx >= 30 + (cardWidth+30)*i && mx <= 30 + cardWidth + (cardWidth+30)*i &&
                    my >= faceIndex[i][j][1] && my <= faceIndex[i][j][1] + cardGap) {
                        if (returnValue == 0) return i + 1;
                        if (returnValue == 1) return faceIndex[i][j][0];
                    }
            }
        }
    }
    return 99;
}

class Klondike {
    public:
        Deck deck; // create deck of cards

        // game variables
        History *history = NULL;
        bool newGame = true;
        bool useGraphics = true;
        // bool won = false;
        int cardWidth = 240;
        int cardHeight = 336;
        int cardGap = 50;
        Color cardFront = WHITE;

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
        int refIndex[13]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // reference for updating positions.
        // int dragCards[12]{};
        
        // mouse
        int mx = GetMouseX();
        int my = GetMouseY();

        void start(cardGraphics cg, backGroundGraphics bg, Color cardBack) {
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
                    // put cards in stacks
                    if (cnt == 0) {
                        stacks[5][0] = 0;
                    } else if (cnt == 1 || cnt == 7) {
                        stacks[6][0] = 1; stacks[6][1] = 7;
                    } else if (cnt == 2 || cnt == 8 || cnt == 13) {
                        stacks[7][0] = 2; stacks[7][1] = 8; stacks[7][2] = 13;
                    } else if (cnt == 3 || cnt == 9 || cnt == 14 || cnt == 18) {
                        stacks[8][0] = 3; stacks[8][1] = 9; stacks[8][2] = 14;
                        stacks[8][3] = 18;
                    } else if (cnt == 4 || cnt == 10 || cnt == 15 || cnt == 19 || cnt == 22) {
                        stacks[9][0] = 4;  stacks[9][1] = 10; stacks[9][2] = 15;
                        stacks[9][3] = 19; stacks[9][4] = 22;
                    } else if (cnt == 5 || cnt == 11 || cnt == 16 || cnt == 20 || cnt == 23 || cnt == 25) {
                        stacks[10][0] = 5;  stacks[10][1] = 11; stacks[10][2] = 16;
                        stacks[10][3] = 20; stacks[10][4] = 23; stacks[10][5] = 25;
                    } else if (cnt == 6 || cnt == 12 || cnt == 17 || cnt == 21 || cnt == 24 || cnt == 26 || cnt == 27) {
                        stacks[11][0] = 6;  stacks[11][1] = 12; stacks[11][2] = 17;
                        stacks[11][3] = 21; stacks[11][4] = 24; stacks[11][5] = 26;
                        stacks[11][6] = 27;
                    } else {
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
/*
            // undo
            if (IsKeyPressed(KEY_BACKSPACE) && history != NULL) {
                if(history->currentStack != 12) {
                    //stackIndex[history->previousStack]++;
                    //stacks[history->previousStack][stackIndex[history->previousStack]] = stacks[history->currentStack][stackIndex[history->currentStack]];
                    //stacks[history->currentStack][stackIndex[history->currentStack]] = 99;
                    //stackIndex[history->currentStack]--;
                    //if (history->previousStack != 12 && history->previousStack != 4) {
                    //    deck.setX(history->cardIndex, 30 + (history->previousStack-5)*(30+cardWidth));
                    //    deck.setY(history->cardIndex, 396 + cardGap*stackIndex[history->previousStack]);
                    //    deck.setFaceUp(stacks[history->previousStack][stackIndex[history->previousStack - 1]], history->prevFaceUp);
                    //} else if (history->previousStack != 4) {
                    //    deck.setX(history->cardIndex, 0);
                    //    deck.setFaceUp(history->cardIndex, false);
                    //} else {
                    //    deck.setX(history->cardIndex, 1380);
                    //    deck.setY(history->cardIndex, 30);
                    //}
                    //pop(&history);if(history->currentStack != 12) {
                    int cardStackIndex = -1;
                    bool foundIndex = false;
                    bool firstMove = false;
                    while (!foundIndex) {
                        cardStackIndex++;
                        if (stacks[history->currentStack][cardStackIndex] == 99) {
                            cardStackIndex--;
                            foundIndex = true;
                        }
                    }
                    if (stacks[history->currentStack][cardStackIndex] == history->cardIndex) {
                        stackIndex[history->previousStack]++;
                        stacks[history->previousStack][stackIndex[history->previousStack]] = stacks[history->currentStack][stackIndex[history->currentStack]];
                        stacks[history->currentStack][stackIndex[history->currentStack]] = 99;
                        stackIndex[history->currentStack]--;
                        if (history->previousStack != 12 && history->previousStack != 4) {
                            deck.setX(history->cardIndex, 30 + (history->previousStack-5)*(30+cardWidth));
                            deck.setY(history->cardIndex, 396 + cardGap*stackIndex[history->previousStack]);
                            deck.setFaceUp(stacks[history->previousStack][stackIndex[history->previousStack - 1]], history->prevFaceUp);
                        } else if (history->previousStack != 4) {
                            deck.setX(history->cardIndex, 0);
                            deck.setFaceUp(history->cardIndex, false);
                        } else {
                            deck.setX(history->cardIndex, 1380);
                            deck.setY(history->cardIndex, 30);
                        }
                    } else {
                        int stopPos = stackIndex[history->previousStack] + 1;
                        while (stacks[history->previousStack][stopPos] != history->cardIndex) {
                            stackIndex[history->previousStack]++;
                            stacks[history->previousStack][stackIndex[history->previousStack]] = stacks[history->currentStack][cardStackIndex];
                            stacks[history->currentStack][cardStackIndex] == 99;
                            stackIndex[history->currentStack]--;
                            if (history->previousStack != 12 && history->previousStack != 4) {
                                deck.setX(stacks[history->previousStack][stackIndex[history->previousStack]], 30 + (history->previousStack-5)*(30+cardWidth));
                                deck.setY(stacks[history->previousStack][stackIndex[history->previousStack]], 396 + cardGap*stackIndex[history->previousStack]);
                                if (stacks[history->previousStack][stackIndex[history->previousStack]] == history->cardIndex) deck.setFaceUp(stacks[history->previousStack][stackIndex[history->previousStack - 1]], history->prevFaceUp);
                            } else if (history->previousStack != 4) {
                                deck.setX(history->cardIndex, 0);
                                deck.setFaceUp(history->cardIndex, false);
                            } else {
                                deck.setX(history->cardIndex, 1380);
                                deck.setY(history->cardIndex, 30);
                            }
                            cardStackIndex--;
                        }
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
*/
            // click on stacks[12]
            if (mx >= 1650 && mx <= 1650 + cardWidth && my >= 30 && my <= 30 + cardHeight && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (stackIndex[12] >= 0) {
                    stackIndex[4]++;
                    stacks[4][stackIndex[4]] = stacks[12][stackIndex[12]];
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
            if (selectedCardFaceUp(stackIndex, mx, my, cardWidth, cardHeight, cardGap) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int currentStack = selectedCardData(0, stackIndex, mx, my, cardWidth, cardHeight, cardGap, cardInfoSheet);
                int currentDeckPos = selectedCardData(1, stackIndex, mx, my, cardWidth, cardHeight, cardGap, cardInfoSheet);
                int currentSuit = deck.getCardSuit(currentDeckPos);
                int currentValue = deck.getValue(currentDeckPos);
                bool currentIsRed = deck.isRed(currentDeckPos);
                bool currentIsBlack = deck.isBlack(currentDeckPos);

                int newStack;

                bool didCardMove = false;
                for (int i = 0; i < 12; i++) {
                    int j;
                    if (i < 4) j = 1;
                    else j = -1;
                    if (!didCardMove && currentStack != i+1 && i != 4 &&
                        ((stackIndex[i] < 0 && ((i < 4 && currentValue == 1) || (i > 4 && currentValue == 13))) ||
                        ((stackIndex[i] >= 0 && deck.getValue(stacks[i][stackIndex[i]])+j == currentValue &&
                        ((i < 4 && deck.getCardSuit(stacks[i][stackIndex[i]]) == currentSuit) || (i > 4 &&
                        (deck.isRed(stacks[i][stackIndex[i]]) == currentIsBlack ||
                        deck.isBlack(stacks[i][stackIndex[i]]) == currentIsRed))))))) {
                            newStack = i;
                            stackIndex[i]++;
                            stacks[i][stackIndex[i]] = currentDeckPos;
                            didCardMove = true;
                        }
                }

                if (didCardMove) {
                    bool prevFaceUp = false;
                    if (stackIndex[currentStack-1] >= 0) prevFaceUp = deck.isFaceUp(stacks[currentStack][stackIndex[currentStack-1]]);
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
            if (selectedCardFaceUp(stackIndex, mx, my, cardWidth, cardHeight, cardGap, faceIndex) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int currentStack = selectedCardData(0, stackIndex, mx, my, cardWidth, cardHeight, cardGap, NULL, faceIndex);
                int currentDeckPos = selectedCardData(1, stackIndex, mx, my, cardWidth, cardHeight, cardGap, NULL, faceIndex);
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
                            }
                            didCardMove = true; // card(s) have been moved
                    }
                }

                if (didCardMove) {
                    bool prevFaceUp = false;
                    if (stackIndex[currentStack-1] >= 0) prevFaceUp = deck.isFaceUp(stacks[currentStack][stackIndex[currentStack-1]]);
                    push(&history, currentDeckPos, newStack, currentStack-1, prevFaceUp); // Add move to history.
                    for (int pos = currentRowMax; pos >= currentIndex; pos--) {
                        stacks[currentStack + 4][pos] = 99;              // Removed any cards that have been moved &
                        stackIndex[currentStack + 4] = currentIndex - 1; // change stacks index value to match.
                    }
                }
            }

            DrawTextureRec(bg.image, bg.rec, bg.pos, WHITE);

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

            // update positions
            for (int i = 0; i < 13; i++) {
                if (stackIndex[i] != refIndex[i]) {
                    int pos = 0;
                    refIndex[i] = stackIndex[i];
                    int tempCardGap = cardGap;
                    if (stackIndex[i] > 6) tempCardGap = 340/stackIndex[i];
                    while (stacks[i][pos] != 99 && pos < 24) {
                        if (i == 4) {
                            deck.setX(stacks[i][pos], 1380);
                            deck.setY(stacks[i][pos], 30);
                        } else if (i == 12) {
                            deck.setX(stacks[i][pos], 0);
                            deck.setY(stacks[i][pos], 0);
                        } else if (i < 4) {
                            deck.setX(stacks[i][pos], 30 + i*(cardWidth + 30));
                            deck.setY(stacks[i][pos], 30);
                        } else {
                            deck.setX(stacks[i][pos], 30 + (i -5 )*(cardWidth + 30));
                            deck.setY(stacks[i][pos], 396 + pos*(tempCardGap));
                        }
                        pos++;
                    }
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
        }

};

#endif