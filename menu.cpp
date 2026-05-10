#include "menu.h"

Texture2D Menu::catTextures[7];
int Menu::selectionIndex = 0;
int Menu::whiteChoice = -1;
int Menu::blackChoice = -1;
bool Menu::choosingWhite = true;
bool Menu::menuActive = true;

void Menu::LoadAssets() {
    catTextures[0] = LoadTexture("assets/cat1.jpeg");
    catTextures[1] = LoadTexture("assets/cat2.jpg");
    catTextures[2] = LoadTexture("assets/cat3.jpg");
    catTextures[3] = LoadTexture("assets/cat4.jpg");
    catTextures[4] = LoadTexture("assets/cat5.jpg");
    catTextures[5] = LoadTexture("assets/cat6.jpg");
    catTextures[6] = LoadTexture("assets/cat7.jpg");
}

void Menu::Update() {
    if (IsKeyPressed(KEY_RIGHT))
        selectionIndex = (selectionIndex + 1) % 7;
    if (IsKeyPressed(KEY_LEFT))
        selectionIndex = (selectionIndex + 6) % 7;

    if (IsKeyPressed(KEY_ENTER)) {
        if (choosingWhite) {
            whiteChoice = selectionIndex;
            choosingWhite = false;
            selectionIndex = 0;
        }
        else {
            blackChoice = selectionIndex;
            menuActive = false;
        }
    }
}

void Menu::Draw() {
    ClearBackground(RAYWHITE);

    const char* title = choosingWhite ? "WHITE: CHOOSE YOUR CAT" : "BLACK CHOOSE YOUR CAT";
    DrawText(title, 550, 100, 40, BLACK);

    for (int i = 0; i < 7; i++) {
        float x = 50 + (float)i * 220;
        float y = 400;

        if (i == selectionIndex) {
            DrawRectangle((int)x - 10, (int)y - 10, 220, 220, BLACK);
        }

        DrawTexturePro(catTextures[i],
            { 0, 0, (float)catTextures[i].width, (float)catTextures[i].height },
            { x, y, 200, 200 },
            { 0,0 }, 0, WHITE);
    }

    DrawText("use ARROW KEYS to navigate and ENTER to select", 500, 800, 20, GRAY);
}