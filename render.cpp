#include "render.h"

const int width = 1600;
const int height = 1100;
const float cellSize = (width - 500.0f) / 8.0f;

int Render::selectedRow = -1;
int Render::selectedCol = -1;
Piece* Render::selectedPiece = nullptr;

Piece* Render::board[8][8] = { nullptr };
Texture2D Render::textures[12];


void Render::LoadTextures() {
    textures[0] = LoadTexture("assets/white-rook.png");
    textures[1] = LoadTexture("assets/white-knight.png");
    textures[2] = LoadTexture("assets/white-bishop.png");
    textures[3] = LoadTexture("assets/white-queen.png");
    textures[4] = LoadTexture("assets/white-king.png");
    textures[5] = LoadTexture("assets/white-pawn.png");

    textures[6] = LoadTexture("assets/black-rook.png");
    textures[7] = LoadTexture("assets/black-knight.png");
    textures[8] = LoadTexture("assets/black-bishop.png");
    textures[9] = LoadTexture("assets/black-queen.png");
    textures[10] = LoadTexture("assets/black-king.png");
    textures[11] = LoadTexture("assets/black-pawn.png");
}


void Render::initBoard() {
    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(color::black, 1, i);
        board[6][i] = new Pawn(color::white, 6, i);
    }

    board[0][0] = new Rook(color::black, 0, 0);
    board[0][1] = new Knight(color::black, 0, 1);
    board[0][2] = new Bishop(color::black, 0, 2);
    board[0][3] = new Queen(color::black, 0, 3);
    board[0][4] = new King(color::black, 0, 4);
    board[0][5] = new Bishop(color::black, 0, 5);
    board[0][6] = new Knight(color::black, 0, 6);
    board[0][7] = new Rook(color::black, 0, 7);

    board[7][0] = new Rook(color::white, 7, 0);
    board[7][1] = new Knight(color::white, 7, 1);
    board[7][2] = new Bishop(color::white, 7, 2);
    board[7][3] = new Queen(color::white, 7, 3);
    board[7][4] = new King(color::white, 7, 4);
    board[7][5] = new Bishop(color::white, 7, 5);
    board[7][6] = new Knight(color::white, 7, 6);
    board[7][7] = new Rook(color::white, 7, 7);
}

void Render::window() {
    InitWindow(width, height, "chess game");

    LoadTextures();
    initBoard();

    while (!WindowShouldClose()) {
        mouse();

        BeginDrawing();
        mainGrid();
        leftGrid();
        rightGrid();
        EndDrawing();
    }

    CloseWindow();
}

void Render::mouse() {
    Vector2 mouse = GetMousePosition();

    float offsetX = 250.0f;

    if ((mouse.x - offsetX) / cellSize < 0.0){
        return;
    }

    int col = (mouse.x - offsetX) / cellSize;
    int row = mouse.y / cellSize;

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;

    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return;

    // NEW: if already selected and clicking same square → unselect
    if (selectedPiece != nullptr && selectedRow == row && selectedCol == col){
        selectedPiece = nullptr;
        selectedRow = -1;
        selectedCol = -1;
        return;
    }

    if (selectedPiece == nullptr) {
        if (board[row][col] != nullptr) {
            selectedPiece = board[row][col];
            selectedRow = row;
            selectedCol = col;
        }
        return;
    }

    if (selectedPiece != nullptr) {
        if (selectedPiece->validmove(row, col)) {
            board[selectedRow][selectedCol] = nullptr;

            selectedPiece->setPosition(row, col);
            board[row][col] = selectedPiece;

            selectedPiece = nullptr;
            selectedRow = -1;
            selectedCol = -1;
        }
    }
}

void Render::mainGrid() {
    float offsetX = 250.0f;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            float x = offsetX + j * cellSize;
            float y = i * cellSize;

            Rectangle cell = { x, y, cellSize, cellSize };

            Color col = ((i + j) % 2) ? BROWN : DARKBROWN;
            if (i == selectedRow && j == selectedCol) {
                col = YELLOW;
            }
            DrawRectangleRec(cell, col);


            Piece* p = board[i][j];
            if (p != nullptr) {
                int base = (p->getColor() == color::white) ? 0 : 6;
                int texIndex = base + p->getTypeId();

                DrawTexture(textures[texIndex], x, y, WHITE);
            }
        }
    }
}


void Render::leftGrid(){
    DrawRectangle(0, 0, 250, height, LIGHTGRAY);
}

void Render::rightGrid(){
    DrawRectangle(width - 250, 0, 250, height, LIGHTGRAY);
}