#pragma once
#include "raylib.h"

class Menu {
public:
    static Texture2D catTextures[7];
    static int selectionIndex;
    static int whiteChoice;
    static int blackChoice;
    static bool choosingWhite;
    static bool menuActive;

    static void LoadAssets();
    static void Update();
    static void Draw();
};