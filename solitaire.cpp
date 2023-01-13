#include "history.h"
#include "klondike.h"

int main(int argc, char const *argv[]) {
    // initilize window
    const int WIN_WIDTH  = 1920;
    const int WIN_HEIGHT = 1080;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Solitaire");

    // graphics
/*    cardGraphics cg;
    cg.cardFaces = {LoadTexture("assets/playing-card-faces2.png")};
    cg.customBackground = true;
    Texture2D bg;
    bg = {LoadTexture("assets/bg01.png")};
    Rectangle bgRec{0, 0, 1920, 1080};
    Vector2 bgPos{0, 0};*/
    
    cardGraphics cg = selectCards(2);
    backGroundGraphics bg = selectBG(1);

    Klondike game;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        game.start(cg, bg);       
    }
    CloseWindow();
}