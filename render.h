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
    static void promotionGrid(Piece* promotionPiece);

    static void mainGridMouse();
    static void promotionMouse(float offsetX, float OffsetY, float popupCell);

    static int selectedRow;
    static int selectedCol;
    static Piece* selectedPiece;

    static bool whiteTurn;

    static Piece* promotionPiece;

    // by rayan for the checkmate (check.cpp)
    static bool move_making_king_check(Piece* piece, int row, int col);
    static void check_if_check_king();
    static bool white_check;
    static bool black_check;
    static bool white_checkmate;
    static bool black_checkmate;

    static int enPassantCol;
    static int enPassantRow;
    
};