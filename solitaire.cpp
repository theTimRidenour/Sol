#include "klondike.h"

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

char* fileName(char* name, int i) {
    std::sprintf(name, "assets/playing-card-faces%d.png", i);
    return name;
}

int main(int argc, char const *argv[]) {
    // initilize window
    const int WIN_WIDTH  = 1920;
    const int WIN_HEIGHT = 1080;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Solitaire");

    // load graphics
    cardGraphics cg = selectCards(1);
    int isSelectedCard = 1;
    backGroundGraphics bg = selectBG(1);
    menuGraphics menu = loadMenuGraphics();

    // initialize game(s)
    Klondike klondike;

    int option = 0;
    int menuOption = 0;
    bool exit = false; // end game & close window.
    bool cardFacesLoaded = false;
    Texture2D menuFaceCards[50];
    Color cardBack = PURPLE;

    SetTargetFPS(60);
    SetExitKey(0);
    while(!exit) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        if (IsKeyPressed(KEY_ESCAPE)) {
            option = 0;
            menuOption = 0;
        }

        if (WindowShouldClose()) exit = true;
        
        if (option == 1) {
            klondike.start(cg, bg, cardBack);
        } else if (option == 0) { 
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
                    int i = 1;
                    if (!cardFacesLoaded) {
                        Image cardFacesImages;
                        while (true) {
                            char name[50];
                            std::sprintf(name, "assets/playing-card-faces%d.png", i);
                            cardFacesImages = LoadImage(name);
                            ImageResize(&cardFacesImages, cardFacesImages.width/3, cardFacesImages.height/3);
                            menuFaceCards[i] = LoadTextureFromImage(cardFacesImages);
                            UnloadImage(cardFacesImages);
                            if (!(menuFaceCards[i].width > 0)) { cardFacesLoaded = true; break; }
                            i++;
                        }
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
                // card face
                DrawText("CARD FACE", 837, 28, 20, BLACK);
                for (int pos = 1; pos < 50; pos++) {
                    if (!(menuFaceCards[pos].width > 0)) break;
                    int bgImage = 0;
                    if (menuFaceCards[pos].height == 448) bgImage = 0;
                    else bgImage = 1;
                    if (isSelectedCard == pos) DrawRectangleLines((float)837-3 + (pos -1)*(5 + menuFaceCards[pos].width/13), (float)58-3, (float)menuFaceCards[1].width/13 + 6, menuFaceCards[1].height/4 + 6, BLACK);
                    if (bgImage == 1) DrawTextureRec(menuFaceCards[pos], {0, (float)menuFaceCards[pos].height/5*4, (float)menuFaceCards[pos].width/13, (float)menuFaceCards[pos].height/5}, {(float)837 + (pos -1)*(5 + menuFaceCards[pos].width/13), (float)58}, WHITE);
                    else DrawRectangleRounded({(float)837 + (pos -1)*(5 + menuFaceCards[pos].width/13), (float)58, (float)menuFaceCards[pos].width/13, (float)menuFaceCards[pos].height/4}, .1, 5, WHITE);
                    DrawTextureRec(menuFaceCards[pos], {0, 0, (float)menuFaceCards[pos].width/13, (float)menuFaceCards[pos].height/(4 + bgImage)}, {(float)837 + (pos-1)*(5 + menuFaceCards[pos].width/13), (float)58}, WHITE);

                    // card face selection
                    if (GetMouseX() > 837 + (pos-1)*(5 + menuFaceCards[pos].width/13) && GetMouseX() < 837 + menuFaceCards[pos].width/13 + (pos-1)*(5 + menuFaceCards[pos].width/13) && GetMouseY() > 58 && GetMouseY() < 58 + menuFaceCards[pos].height/(4 + bgImage) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        cg = selectCards(pos);
                        isSelectedCard = pos;
                    }
                }

                // card back
                float sPos = menuFaceCards[1].height/4 + 58 + 40;
                float width = menuFaceCards[1].width/13;
                float height = menuFaceCards[1].height/4;
                int colorCnt = 0;
                int colorRow = 0;
                DrawText("CARD BACK", 837, sPos - 30, 20, BLACK);
                for (Color c: {BEIGE, BLACK, BLUE, BROWN, DARKBLUE, DARKBROWN, DARKGRAY, DARKGREEN, DARKPURPLE, GOLD, GRAY, GREEN, LIGHTGRAY, LIME, MAGENTA, MAROON, ORANGE, PINK, PURPLE, RED, SKYBLUE, VIOLET, WHITE, YELLOW}) {
                    if (cardBack.a == c.a && cardBack.b == c.b && cardBack.g == c.g && cardBack.r == c.r) DrawRectangleLines(837 - 3 + colorCnt*(width+5), sPos - 3 + colorRow*(height+5), width + 6, height + 6, BLACK);
                    DrawRectangleRounded({837 + colorCnt*(width+5), sPos + colorRow*(height+5), width, height}, 0.1, 5, c);
                    if (GetMouseX() > 837 + colorCnt*(width+5) && GetMouseX() < 837 + width + colorCnt*(width+5) && GetMouseY() > sPos + colorRow*(height+5) && GetMouseY() < sPos + height + colorRow*(height+5) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) cardBack = c;
                    colorCnt++;
                    if (colorCnt > 11) {
                        colorCnt = 0;
                        colorRow++;
                    }
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
}