#include "render.h"
#include "check.h"
#include "menu.h"
#include "pieces.h"
#include <iostream>
#include <cmath>

//GLOBALS
const int width = 1600;
const int height = 1100;
const float cellSize = (width - 500.0f) / 8.0f;

int Render::selectedRow = -1;
int Render::selectedCol = -1;
Piece* Render::selectedPiece = nullptr;

bool Render::whiteTurn = true;

int Render::enPassantCol = -1;
int Render::enPassantRow = -1;

Piece* Render::promotionPiece = nullptr;

Font font;

std::vector<int> Render::whiteCaptured;
std::vector<int> Render::blackCaptured;

Texture2D Render::endGameTexture;

Piece* Render::board[8][8] = { nullptr };

Texture2D Render::textures[12];

bool Render::white_check = false;
bool Render::black_check = false;

bool Render::white_checkmate = false;
bool Render::black_checkmate = false;

bool Render::white_stalemate = false;
bool Render::black_stalemate = false;


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

    endGameTexture = LoadTexture("assets/cat endpopup.png");
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
    InitWindow(1600, 1100, "Chess Game");
    font = LoadFontEx("assets/FreeSans.otf", 32, NULL, 0);

    LoadTextures();
    Menu::LoadAssets();
    initBoard();

    while (!WindowShouldClose()) {
        if (Menu::menuActive) {
            Menu::Update();
        }
        else {
            if (!promotionPiece) mainGridMouse();
        }

        BeginDrawing();
        if (Menu::menuActive) {
            Menu::Draw();
        }
        else {
            ClearBackground(RAYWHITE);
            mainGrid();
            leftGrid();
            rightGrid();
            drawCapture();
            if (promotionPiece) promotionGrid(promotionPiece);
            drawPopup();
        }
        EndDrawing();
    }
    CloseWindow();
}


void Render::mainGridMouse() {
    Vector2 mouse = GetMousePosition();
    float offsetX = 250.0f;

    // CALCULATE GRID CORDS
    if ((mouse.x - offsetX) / cellSize < 0.0) return;
    int col = (mouse.x - offsetX) / cellSize;
    int row = mouse.y / cellSize;

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return;

    //DESELECT PIECE
    if (selectedPiece && selectedRow == row && selectedCol == col) {
        selectedPiece = nullptr;
        selectedRow = -1;
        selectedCol = -1;
        return;
    }

    //SELECT A NEW PIECE
    if (board[row][col] && ((board[row][col]->getColor() == color::white && whiteTurn) ||
        (board[row][col]->getColor() == color::black && !whiteTurn))) {
        selectedPiece = board[row][col];
        selectedRow = row;
        selectedCol = col;
        return;
    }

    if (selectedPiece == nullptr) return;

    //MOVE VALIDITY
    if (selectedPiece->validmove(row, col)) {
        if (move_making_king_check(selectedPiece, row, col)) {
            return; // Move is illegal (King would be in check)
        }

        executeMove(selectedPiece, row, col);
    }
}


void Render::executeMove(Piece* piece, int row, int col) {
    //EN PASSANT CAPTURE
    if (piece->getTypeId() == 5 && col == enPassantCol && row == enPassantRow + (piece->getColor() == color::white ? -1 : 1)) {
        if (board[enPassantRow][enPassantCol] != nullptr) {
            int capturedId = board[enPassantRow][enPassantCol]->getTypeId();
            color capturedColor = board[enPassantRow][enPassantCol]->getColor();
            if (capturedColor == color::white) whiteCaptured.push_back(capturedId);
            else blackCaptured.push_back(capturedId);

            delete board[enPassantRow][enPassantCol];
            board[enPassantRow][enPassantCol] = nullptr;
        }
    }

    //CASTLING
    if (piece->getTypeId() == 4 && std::abs(col - selectedCol) == 2) {
        int rookOldCol = (col > selectedCol) ? 7 : 0;
        int rookNewCol = (col > selectedCol) ? 5 : 3;

        if (board[row][rookOldCol] != nullptr) {
            board[row][rookNewCol] = board[row][rookOldCol];
            board[row][rookOldCol] = nullptr;
            board[row][rookNewCol]->setPosition(row, rookNewCol);
            board[row][rookNewCol]->afterMove();
        }
    }

    //EN PASSANT AVAILABLE
    int currentSelectedRow = selectedRow;
    if (piece->getTypeId() == 5 && std::abs(row - currentSelectedRow) == 2) {
        enPassantCol = col;
        enPassantRow = row;
    }
    else {
        enPassantCol = -1;
    }

    //STANDARD CAPTURE
    if (board[row][col] != nullptr) {
        int capturedId = board[row][col]->getTypeId();
        color capturedColor = board[row][col]->getColor();

        if (capturedColor == color::white) whiteCaptured.push_back(capturedId);
        else blackCaptured.push_back(capturedId);

        delete board[row][col];
    }

    //DO THE MOVE
    board[selectedRow][selectedCol] = nullptr;
    board[row][col] = piece;
    piece->setPosition(row, col);
    piece->afterMove();

    //RESET
    selectedPiece = nullptr;
    selectedRow = -1;
    selectedCol = -1;

    //SWITCH TURN AND CHECK IF GAME STILL GOING
    whiteTurn = !whiteTurn;
    updateState();
}


bool Render::move_making_king_check(Piece* piece, int row, int col) {
    Cord king_position = piece->getPosition();
    Piece* old_piece = Render::board[row][col];

    bool isEnPassant = false;
    Piece* enPassantPawn = nullptr;
    int epRow = -1, epCol = -1;

    //seperately for en passant
    if (piece->getTypeId() == 5 && old_piece == nullptr && col != king_position.col) {
        isEnPassant = true;
        epRow = king_position.row;
        epCol = col;
        enPassantPawn = Render::board[epRow][epCol];
        Render::board[epRow][epCol] = nullptr;
    }

    Render::board[king_position.row][king_position.col] = nullptr;
    Render::board[row][col] = piece;
    piece->setPosition(row, col);

    bool check = Check::king_check(piece->getColor());

    Render::board[king_position.row][king_position.col] = piece;
    Render::board[row][col] = old_piece;
    piece->setPosition(king_position.row, king_position.col);

    if (isEnPassant) {
        Render::board[epRow][epCol] = enPassantPawn;
    }

    return check;
}


void Render::updateState() {
    white_checkmate = Check::checking_checkmate(color::white);
    white_stalemate = Check::checking_stalemate(color::white);

    black_checkmate = Check::checking_checkmate(color::black);
    black_stalemate = Check::checking_stalemate(color::black);

    white_check = Check::king_check(color::white);
    black_check = Check::king_check(color::black);
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
            if (board[i][j] && board[i][j]->getTypeId() == 4) {
                if (board[i][j]->getColor() == color::white && white_check) col = RED;
                if (board[i][j]->getColor() == color::black && black_check) col = RED;
            }
            DrawRectangleRec(cell, col);

            Piece* p = board[i][j];
            if (p) {
                int base = (p->getColor() == color::white) ? 0 : 6;
                int texIndex = base + p->getTypeId();
                DrawTexture(textures[texIndex], x, y, WHITE);
            }
        }
    }
}


void drawCatPFP(Texture2D cat, float x, float y) {
    float size = 200.0f;
    Rectangle source = { 0, 0, (float)cat.width, (float)cat.height };
    Rectangle dest = { x, y, size, size };

    DrawTexturePro(cat, source, dest, { 0, 0 }, 0, WHITE);
}


void Render::leftGrid() {
    DrawRectangle(0, 0, 250, 1100, LIGHTGRAY);
    DrawText("WHITE", 70, 500, 30, WHITE);

    //Draw the PFP
    Texture2D chosenCat = Menu::catTextures[Menu::whiteChoice];
    drawCatPFP(chosenCat, 25, 850);
    
}

void Render::rightGrid() {
    DrawRectangle(1350, 0, 250, 1100, LIGHTGRAY);
    DrawText("BLACK", 1420, 500, 30, BLACK);

    //Draw the PFP
    Texture2D chosenCat = Menu::catTextures[Menu::blackChoice];
    drawCatPFP(chosenCat, 1375, 50);
}


void Render::promotionGrid(Piece* selectedPiece) {
    float offsetX = 400;
    float offsetY = 500;
    float popupWidth = 800;
    float popupHeight = 180;
    float popupCell = popupWidth / 4;
    Vector2 pos = { offsetX + 200, offsetY };

    Rectangle popup = { offsetX, offsetY, popupWidth, popupHeight };
    DrawRectangleRounded(popup, 0.5, 10, LIGHTGRAY);
    DrawTextEx(font, "Choose a piece to promote to", pos, 32, 2, BLACK);
    int textureIndex = (selectedPiece->getColor() == color::white) ? 0 : 6;
    for (int i = 0; i < 4; i++) {
        DrawTexture(textures[i + textureIndex], offsetX + i * popupCell + 30, offsetY + 30, WHITE);
    }
    promotionMouse(offsetX, offsetY, popupCell);
}


void Render::promotionMouse(float offsetX, float OffsetY, float popupCell) {
    Vector2 mouse = GetMousePosition();
    if (mouse.x < offsetX || mouse.x > 1200 || mouse.y < 500 || mouse.y > 680) {
        return;
    }
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return;
    }

    int choice = (mouse.x - offsetX) / popupCell;

    color promotionColor = promotionPiece->getColor();
    Cord promotionCord = promotionPiece->getPosition();
    delete board[promotionCord.row][promotionCord.col];

    switch (choice) {
    case 0:
        board[promotionCord.row][promotionCord.col] = new Rook(promotionColor, promotionCord.row, promotionCord.col);
        break;
    case 1:
        board[promotionCord.row][promotionCord.col] = new Knight(promotionColor, promotionCord.row, promotionCord.col);
        break;
    case 2:
        board[promotionCord.row][promotionCord.col] = new Bishop(promotionColor, promotionCord.row, promotionCord.col);
        break;
    case 3:
        board[promotionCord.row][promotionCord.col] = new Queen(promotionColor, promotionCord.row, promotionCord.col);
        break;
    }

    promotionPiece = nullptr;
    updateState();
}


void Render::drawCapture() {
    float scale = 0.4f;
    float spacingX = 38.0f;
    float spacingY = 46.0f;
    for (int i = 0; i < whiteCaptured.size(); i++) {
        int texIndex = whiteCaptured[i];
        float x = 10 + (i % 6) * spacingX;
        float y = 10 + (i / 6) * spacingY;
        DrawTextureEx(textures[texIndex], { x, y }, 0, scale, WHITE);
    }
    for (int i = 0; i < blackCaptured.size(); i++) {
        int texIndex = blackCaptured[i] + 6;
        float x = 1360 + (i % 6) * spacingX;
        float y = 1050 - (i / 6) * spacingY;
        DrawTextureEx(textures[texIndex], { x, y }, 0, scale, WHITE);
    }
}


void Render::drawPopup() {
    float W = 640, H = 280;
    bool isStalemate = (white_stalemate || black_stalemate);
    if (!white_checkmate && !black_checkmate && !isStalemate) {
        return;
    }

    DrawRectangle(0, 0, 1600, 1100, Fade(BLACK, 0.75f));

    float x = (1600 - W) / 2.0f;
    float y = (1100 - H) / 2.0f;
    Rectangle source = { 0, 0, (float)endGameTexture.width, (float)endGameTexture.height };
    Rectangle dest = { x, y, W, H };
    DrawTexturePro(endGameTexture, source, dest, { 0, 0 }, 0.0f, WHITE);
    const char* subtext = "";
    Color highlight = BLACK;
    if (white_checkmate) {
        subtext = "Black Wins";
    }
    else if (black_checkmate) {
        subtext = "White Wins";
    }
    else if (isStalemate) {
        subtext = "Stalemate";
    }
    Vector2 headSize = MeasureTextEx(font, "Match Ended", 42, 2);
    Vector2 subSize = MeasureTextEx(font, subtext, 28, 1);
    DrawTextEx(font, "Match Ended", { x + (W - headSize.x) / 2, y + 50 }, 42, 2, BLACK);
    DrawTextEx(font, subtext, { x + (W - subSize.x) / 2, y + 130 }, 28, 1, highlight);
    const char* closeHint = "Press ESC to exit";
    Vector2 hintSize = MeasureTextEx(font, closeHint, 18, 1);
    DrawTextEx(font, closeHint, { x + (W - hintSize.x) / 2, y + 220 }, 18, 1, RED);
}