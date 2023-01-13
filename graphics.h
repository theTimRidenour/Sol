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

#endif