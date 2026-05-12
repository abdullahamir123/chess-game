#pragma once
#include "raylib.h"
#include "pieces.h"
#include <vector>

//i made the var and funcs var. makes it easier. no need to have an obj since only one game can run at a time.

class Render {
public:
    //GAME VARIABLES
    static Piece* board[8][8];
    static bool whiteTurn;

    static int selectedRow;
    static int selectedCol;
    static Piece* selectedPiece;
    static Piece* promotionPiece;

    static int enPassantCol;
    static int enPassantRow;

    static bool white_check;
    static bool black_check;
    static bool white_checkmate;
    static bool black_checkmate;
    static bool white_stalemate;
    static bool black_stalemate;

    static std::vector<int> whiteCaptured;
    static std::vector<int> blackCaptured;

    //TEXTURES
    static Texture2D textures[12];
    static Texture2D endGameTexture;

    //INIT
    static void LoadTextures();
    static void initBoard();
    static void window();

    //LOGIC & INPUT FUNCTIONS
    static void mainGridMouse();
    static void promotionMouse(float offsetX, float offsetY, float popupCell);
    static void executeMove(Piece* piece, int targetRow, int targetCol);

    //CHECK IF MOVES CAUSES KING CHECK(INVALID)
    static bool move_making_king_check(Piece* piece, int row, int col);
    //UPDATE STATES
    static void updateState();

    //DRAWING FUNCTIONS
    static void mainGrid();
    static void leftGrid();
    static void rightGrid();
    static void promotionGrid(Piece* promotionPiece);
    static void drawPopup();
    static void drawCapture();
};