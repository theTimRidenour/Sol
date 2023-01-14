#ifndef MENUS_H
#define MENUS_H

#include "raylib.h"
#include "graphics.h"

bool drawButton(int mouseX, int mouseY, bool mouseDown, bool mouseReleased, menuGraphics menu, const char *name, int buttonNumber) {
    if (!mouseDown && !mouseReleased && mouseX > 837 && mouseX < 837 + menu.button.width && mouseY > 28 + buttonNumber*(15 + menu.button.height) && mouseY < 28 + menu.button.height + buttonNumber*(15 + menu.button.height)) {
        DrawTexture(menu.buttonHover, 837, 28 + buttonNumber*(15 + menu.button.height), WHITE);
    }

    if (mouseDown && !mouseReleased && mouseX > 837 && mouseX < 837 + menu.button.width && mouseY > 28 + buttonNumber*(15 + menu.button.height) && mouseY < 28 + menu.button.height + buttonNumber*(15 + menu.button.height)) {
        DrawTexture(menu.buttonPressed, 837, 28 + buttonNumber*(15 + menu.button.height), WHITE);
    }

    if (!mouseDown && mouseReleased && mouseX > 837 && mouseX < 837 + menu.button.width && mouseY > 28 + buttonNumber*(15 + menu.button.height) && mouseY < 28 + menu.button.height + buttonNumber*(15 + menu.button.height)) {
        return true;
    }

    if (!(mouseX > 837 && mouseX < 837 + menu.button.width) || !(mouseY > 28 + buttonNumber*(15 + menu.button.height) && mouseY < 28 + menu.button.height + buttonNumber*(15 + menu.button.height))) {
        DrawTexture(menu.button, 837, 28 + buttonNumber*(15 + menu.button.height), WHITE);
    }

    DrawText(name, 837 + menu.button.width/2 - MeasureText(name, 30)/2, 28 + buttonNumber*(15 + menu.button.height) + menu.button.height/2 - 10, 30, BLACK);

    return false;
}

class Menus {
    public:
    menuGraphics menu = loadMenuGraphics();
    int menuOption = 0;
    bool cardFacesLoaded = false;
    Texture2D menuFaceCards[50];
    backGroundGraphics menuBgImages[50];

    void start(int &option, bool &exit, int &isSelectedCard, int &isSelectedBG, Color &cardBack, cardGraphics &cg, backGroundGraphics &bg) {
                    DrawTexture(menu.bg, 0, 0, WHITE);

            // button effects & actions
            if (menuOption == 0) {
                // Klondike
                if (drawButton(GetMouseX(), GetMouseY(), IsMouseButtonDown(MOUSE_LEFT_BUTTON), IsMouseButtonReleased(MOUSE_LEFT_BUTTON), menu, "KLONDIKE", 0)) {
                    option = 1;
                }
                // Options
                if (drawButton(GetMouseX(), GetMouseY(), IsMouseButtonDown(MOUSE_LEFT_BUTTON), IsMouseButtonReleased(MOUSE_LEFT_BUTTON), menu, "OPTIONS", 1)) {
                    menuOption = 1;
                    if (!cardFacesLoaded) {
                        bool faceDone = false;
                        bool bgDone = false;
                        for (int i = 1; i < 50; i++) {
                            if (!faceDone) menuFaceCards[i - 1] = loadFaceCard(i);
                            if (!bgDone) menuBgImages[i - 1] = loadBg(i);
                            if (!(menuFaceCards[i - 1].width > 0)) faceDone = true;
                            if (!(menuBgImages[i - 1].image.width > 0)) bgDone = true;
                            if (faceDone && bgDone) break;
                        }
                        cardFacesLoaded = true;
                    }
                }    
                // Exit
                if (drawButton(GetMouseX(), GetMouseY(), IsMouseButtonDown(MOUSE_LEFT_BUTTON), IsMouseButtonReleased(MOUSE_LEFT_BUTTON), menu, "EXIT", 2)) {
                    exit = true;
                }
                // button previews
                if (GetMouseX() > 837 && GetMouseX() < 837 + menu.button.width && GetMouseY() > 28 && GetMouseY() < 28 + menu.button.height)
                    DrawTexture(menu.klondike, 27, 606, WHITE);
                if (GetMouseX() > 837 && GetMouseX() < 837 + menu.button.width && GetMouseY() > 28 + 15 + menu.button.height && GetMouseY() < 28 + 15 + 2*menu.button.height)
                    DrawTexture(menu.options, 27, 606, WHITE);
                    
            } else if (menuOption == 1) {
                if (IsKeyPressed(KEY_ESCAPE)) menuOption = 0;

                // option menu variables
                float cardWidth = menuFaceCards[0].width/13;
                float cardHeight = menuFaceCards[0].height/4;
                float facePos = 58;
                float backPos = 58;
                float bgPos = 58;
                int faceCnt = 0;
                int faceRow = 0;
                int backCnt = 0;
                int backRow = 0;
                int bgCnt = 0;
                int bgRow = 0;

                // card face
                DrawText("CARD FACE", 837, facePos - 30, 20, BLACK);
                for (int pos = 0; pos < 50; pos++) {
                    if (!(menuFaceCards[pos].width > 0)) break;
                    int bgImage = 0;
                    if (menuFaceCards[pos].height == menuFaceCards[0].height) bgImage = 0;
                    else bgImage = 1;
                    if (isSelectedCard == pos) DrawRectangleLines(837-3 + faceCnt*(5 + cardWidth), facePos-3 + faceRow*(cardHeight + 5), cardWidth + 6, cardHeight + 6, BLACK);
                    if (bgImage == 1) DrawTextureRec(menuFaceCards[pos], {0, cardHeight*4, cardWidth, cardHeight}, {837 + faceCnt*(5 + cardWidth), facePos + faceRow*(cardHeight + 5)}, WHITE);
                    else DrawRectangleRounded({837 + faceCnt*(5 + cardWidth), facePos + faceRow*(cardHeight + 5), cardWidth, cardHeight}, .1, 5, WHITE);
                    DrawTextureRec(menuFaceCards[pos], {0, 0, cardWidth, cardHeight}, {837 + faceCnt*(5 + cardWidth), facePos + faceRow*(cardHeight + 5)}, WHITE);

                    // card face selection
                    if (GetMouseX() > 837 + faceCnt*(5 + cardWidth) && GetMouseX() < 837 + cardWidth + faceCnt*(5 + cardWidth) && GetMouseY() > facePos + faceRow*(cardHeight + 5) && GetMouseY() < facePos + faceRow*(cardHeight + 5) + cardHeight && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        cg = selectCards(pos);
                        isSelectedCard = pos;
                    }
                    faceCnt++;
                    if (faceCnt > 11) {
                        faceCnt = 0;
                        faceRow++;
                    }
                }

                // card back
                backPos = cardHeight + facePos + faceRow*(cardHeight + 5) + 40;
                DrawText("CARD BACK", 837, backPos - 30, 20, BLACK);
                for (Color c: {BEIGE, BLACK, BLUE, BROWN, DARKBLUE, DARKBROWN, DARKGRAY, DARKGREEN, DARKPURPLE, GOLD, GRAY, GREEN, LIGHTGRAY, LIME, MAGENTA, MAROON, ORANGE, PINK, PURPLE, RED, SKYBLUE, VIOLET, WHITE, YELLOW}) {
                    if (cardBack.a == c.a && cardBack.b == c.b && cardBack.g == c.g && cardBack.r == c.r) DrawRectangleLines(837 - 3 + backCnt*(cardWidth + 5), backPos - 3 + backRow*(cardHeight+5), cardWidth + 6, cardHeight + 6, BLACK);
                    DrawRectangleRounded({837 + backCnt*(cardWidth + 5), backPos + backRow*(cardHeight + 5), cardWidth, cardHeight}, 0.1, 5, c);
                    if (GetMouseX() > 837 + backCnt*(cardWidth + 5) && GetMouseX() < 837 + cardWidth + backCnt*(cardWidth + 5) && GetMouseY() > backPos + backRow*(cardHeight + 5) && GetMouseY() < backPos + cardHeight + backRow*(cardHeight + 5) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cardBack = c;
                    backCnt++;
                    if (backCnt > 11) {
                        backCnt = 0;
                        backRow++;
                    }
                }

                // backgrounds
                bgPos = backPos + backRow*(cardHeight + 5) + 40;
                int bgWidth = 199;
                int bgHeight = 112;
                DrawText("BACKGROUND", 837, bgPos - 30, 20, BLACK);
                for (int pos = 0; pos < 50; pos++) {
                    if (!(menuBgImages[pos].image.width > 0)) break;
                    if (isSelectedBG == pos) DrawRectangleLines(837-3 + bgCnt*(bgWidth + 5), bgPos-3 + bgRow*(bgHeight+5), bgWidth+6, bgHeight+6, BLACK);
                    DrawTexture(menuBgImages[pos].image, 837 + bgCnt*(bgWidth+5), bgPos + bgRow*(bgHeight+5), WHITE);
                    if (GetMouseX() > 837 + bgCnt*(bgWidth+5) && GetMouseX() < 837 + bgWidth + bgCnt*(bgWidth+5) && GetMouseY() > bgPos + bgRow*(bgHeight+5) && GetMouseY() < bgPos + bgHeight + bgRow*(bgHeight+5) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        bg = selectBG(pos);
                        isSelectedBG = pos;
                    }

                    bgCnt++;
                    if (bgCnt > 4) {
                        bgCnt = 0;
                        bgRow++;
                    }
                }
            }

    }

};

#endif