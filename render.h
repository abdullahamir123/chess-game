#pragma once
#include "raylib.h"
#include "pieces.h"

class Render {
public:
    static Piece* board[8][8];
    static Texture2D textures[12];
    //end popup background for funsies :)
    static Texture2D endGameTexture;
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
    static bool white_check;
    static bool black_check;
    static bool white_checkmate;
    static bool black_checkmate;


    // for pop up after game end
    static void drawPopup();
    //stalemate, render
    static bool white_stalemate;
    static bool black_stalemate;
    //this will update after every move, checking if its checkmate or check by calling those functions-if so then pop up
    static void updateState();
};