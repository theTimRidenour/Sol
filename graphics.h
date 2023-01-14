#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>

Texture2D checkSize;

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
    if (!(checkSize.width > 0)) checkSize = {LoadTexture("assets/cards/playing-card-faces1.png")};

    char fileName[50];
    std::sprintf(fileName, "assets/cards/playing-card-faces%d.png", option + 1);
    cg.cardFaces = {LoadTexture(fileName)};
    if (cg.cardFaces.height != checkSize.height) cg.customBackground = true;

    return cg;
}

backGroundGraphics selectBG(int option) {
    backGroundGraphics bg;

    char fileName[50];
    std::sprintf(fileName, "assets/backgrounds/bg%d.png", option + 1);
    bg.image = {LoadTexture(fileName)};
//    if (option == 1) {
//        bg.image = {LoadTexture("assets/bg01.png")};
//    }

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

Texture2D loadFaceCard(int i) {
    Texture2D faceCard;

    char name[50];
    std::sprintf(name, "assets/cards/playing-card-faces%d.png", i);
    Image cardFacesImages{LoadImage(name)};
    ImageResize(&cardFacesImages, cardFacesImages.width/3, cardFacesImages.height/3);
    faceCard = LoadTextureFromImage(cardFacesImages);
    UnloadImage(cardFacesImages);
    
    return faceCard;
}

backGroundGraphics loadBg(int i) {
    backGroundGraphics load;

    char fileName[50];
    std::sprintf(fileName, "assets/backgrounds/bg%d.png", i);
    Image backgroundImage{LoadImage(fileName)};
    ImageResize(&backgroundImage, 199, 112);
    load.image = {LoadTextureFromImage(backgroundImage)};
    UnloadImage(backgroundImage);

    return load;
}

#endif