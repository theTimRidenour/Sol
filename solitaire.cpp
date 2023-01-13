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

int main(int argc, char const *argv[]) {
    // initilize window
    const int WIN_WIDTH  = 1920;
    const int WIN_HEIGHT = 1080;
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Solitaire");

    // load graphics
    cardGraphics cg = selectCards(1);
    backGroundGraphics bg = selectBG(1);
    menuGraphics menu = loadMenuGraphics();

    // initialize game(s)
    Klondike klondike;

    int option = 0;
    int menuOption = 0;
    bool exit = false; // end game & close window.

    SetTargetFPS(60);
    SetExitKey(0);
    while(!exit) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            option = 0;
            menuOption = 0;
        }
        
        if (option == 0) { 
            BeginDrawing();
            ClearBackground(LIGHTGRAY);

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
                }
                // Exit
                if (drawButton(GetMouseX(), GetMouseY(), IsMouseButtonDown(MOUSE_LEFT_BUTTON), IsMouseButtonReleased(MOUSE_LEFT_BUTTON), menu, "EXIT", 2) || WindowShouldClose()) {
                    exit = true;
                }
                
                
            } else if (menuOption == 1) {
                std::cout << "menu option 1\n";
            }

            // button previews
            if (GetMouseX() > 837 && GetMouseX() < 837 + menu.button.width && GetMouseY() > 28 && GetMouseY() < 28 + menu.button.height)
                DrawTexture(menu.klondike, 27, 606, WHITE);

            EndDrawing();}
        if (option == 1) klondike.start(cg, bg);  // run klondike game
    }
    CloseWindow();
}