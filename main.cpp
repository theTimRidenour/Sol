#include "klondike.h"
#include "menus.h"

int main(int argc, char const *argv[]) {
    // initilize window
    const int WIN_WIDTH  = 1920;
    const int WIN_HEIGHT = 1080;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Solitaire");

    int option = 0;
    bool endGame = false;
    int isSelectedCard = 0;
    int isSelectedBG = 0;
    Color cardBack = PURPLE;
    cardGraphics cg;
    backGroundGraphics bg;

    // initialize graphics
    cg = selectCards(0);
    bg = selectBG(0);

    // initialize menus
    Menus menu;

    // initialize game(s)
    Klondike klondike;

    SetTargetFPS(60);
    SetExitKey(0);
    while(!endGame) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        if (IsKeyPressed(KEY_ESCAPE)) {
            option = 0;
        }

        if (WindowShouldClose()) endGame = true;
        
        if (option == 0) menu.start(option, endGame, isSelectedCard, isSelectedBG, cardBack, cg, bg);
        else if (option == 1) klondike.start(cg, bg, cardBack);

        EndDrawing();
    }
    CloseWindow();
}