#pragma once
#include "raylib.h"
#include "pieces.h"

class Render {
public:
    static Piece* board[8][8];
    static Texture2D textures[12];

    static void LoadTextures();

    static void initBoard();
    static void window();

    static void mainGrid();
    static void leftGrid();
    static void rightGrid();

    static void mouse();

    static int selectedRow;
    static int selectedCol;
    static Piece* selectedPiece;
};