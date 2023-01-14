#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

struct cardGraphics {
    Texture2D cardFaces;
    Texture2D cardBacks;
    bool customBackground = false;
};

struct backGroundGraphics {
    Texture2D image;
    Rectangle rec{0, 0, 1920, 1080};
    Vector2 pos{0, 0};
};

struct menuGraphics {
    Texture2D bg;
    Texture2D button;
    Texture2D buttonHover;
    Texture2D buttonPressed;
    Texture2D klondike;
    Texture2D options;
};

cardGraphics selectCards(int option) {
    cardGraphics cg;

    if (option == 1) {
        cg.cardFaces = {LoadTexture("assets/playing-card-faces.png")};
        cg.customBackground = false;
    } else if (option == 2) {
        cg.cardFaces = {LoadTexture("assets/playing-card-faces2.png")};
        cg.customBackground = true;
    }

    return cg;
}

backGroundGraphics selectBG(int option) {
    backGroundGraphics bg;

    if (option == 1) {
        bg.image = {LoadTexture("assets/bg01.png")};
    }

    return bg;
}

menuGraphics loadMenuGraphics() {
    menuGraphics load;
    load.bg = {LoadTexture("assets/menu/background.png")};
    load.button = {LoadTexture("assets/menu/button.png")};
    load.buttonHover = {LoadTexture("assets/menu/button-hover.png")};
    load.buttonPressed = {LoadTexture("assets/menu/button-pressed.png")};
    load.klondike = {LoadTexture("assets/menu/klondike.png")};
    load.options = {LoadTexture("assets/menu/options.png")};

    return load;
}

#endif