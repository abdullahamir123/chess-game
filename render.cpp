#include "render.h"
#include "check.h"
#include "pieces.h"

const int width = 1600;
const int height = 1100;
const float cellSize = (width - 500.0f) / 8.0f;

int Render::selectedRow = -1;
int Render::selectedCol = -1;
Piece* Render::selectedPiece = nullptr;
bool Render::whiteTurn = true;

Piece* Render::promotionPiece = nullptr;

Font font;


Piece* Render::board[8][8] = { nullptr };
Texture2D Render::textures[12];
bool Render::white_check = false;
bool Render::black_check = false;
bool Render::white_checkmate = false;
bool Render::black_checkmate = false;

//loads all assets to memory
//for white variant, 0 to 5. for the black variant of same type, +6
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


//initialize specific indices of the board[][] as piece type pointers. remaining will stay nullptr
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
    font = LoadFontEx("assets/FreeSans.otf", 32, NULL, 0);

    LoadTextures();
    initBoard();

    while (!WindowShouldClose()) {
        if (Render::promotionPiece == nullptr) {
            mainGridMouse();
        }

        BeginDrawing();
        mainGrid();
        leftGrid();
        rightGrid();
        if (Render::promotionPiece != nullptr) {
            Render::promotionGrid(Render::promotionPiece);
        }
        EndDrawing();
    }

    CloseWindow();
}

void Render::mainGridMouse() {
    Vector2 mouse = GetMousePosition();

    float offsetX = 250.0f;

    if ((mouse.x - offsetX) / cellSize < 0.0){
        return;
    }

    int col = (mouse.x - offsetX) / cellSize;
    int row = mouse.y / cellSize;

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;

    //checks out of bound
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return;

    //deselect an already selected piece by clicking on it again
    if (selectedPiece != nullptr && selectedRow == row && selectedCol == col){
        selectedPiece = nullptr;
        selectedRow = -1;
        selectedCol = -1;
        return;
    }

    //if no selection yet, and a new selection occurs, first check if new selection is not nullptr. if not then point selected piece towards it
    if (selectedPiece == nullptr) {
        if (board[row][col] != nullptr) {
            //check if the black or whites turn
            if ((board[row][col]->getColor() == color::white && whiteTurn) || (board[row][col]->getColor() == color::black && !whiteTurn)) {
                selectedPiece = board[row][col];
                selectedRow = row;
                selectedCol = col;
            }
        }
        return;
    }
    //if a piece is already selected and another selection occurs(moving a piece)
    if (selectedPiece != nullptr) {
        //first check if the move is valid moveset of the piece type
        if (selectedPiece->validmove(row, col)) {
            //check if not capturing it's own piece
            if (selectedPiece->noFriendlyCapture(row, col)) {
                if (move_making_king_check(selectedPiece, row, col)) {
                    return;
                }
                board[selectedRow][selectedCol] = nullptr;
                //move selectedpiece after deleting old piece from there
                delete board[row][col];
                selectedPiece->setPosition(row, col);
                board[row][col] = selectedPiece;
                // this is stopping the pawn to move 2 steps as if it has alredy moved
                Pawn* pawn = dynamic_cast<Pawn*>(selectedPiece);
                if (pawn != nullptr) {
                    pawn->hasMoved = true;
                }
                selectedPiece = nullptr;
                selectedRow = -1;
                selectedCol = -1;
                //gives turn to the other side
                whiteTurn = !whiteTurn;
                check_if_check_king();
            }
        }
    }
}

// check.cpp linking it to this render.cpp......
bool Render::move_making_king_check(Piece* piece, int row, int col) {

    Cord king_position = piece->getPosition();
    Piece* old_piece = Render::board[row][col];

    // temporary move
    Render::board[king_position.row][king_position.col] = nullptr;
    Render::board[row][col] = piece;
    piece->setPosition(row, col);

    // it is checking if king becomes checked or not
    bool check = Check::king_check(piece->getColor());

    // now we undoing the move we did
    Render::board[king_position.row][king_position.col] = piece;
    Render::board[row][col] = old_piece;
    piece->setPosition(king_position.row, king_position.col);
    return check;
}
void Render::check_if_check_king() {
    white_check = Check::king_check(color::white);
    black_check = Check::king_check(color::black);
    white_checkmate = Check::checking_checkmate(color::white);
    black_checkmate = Check::checking_checkmate(color::black);
}

//creates a main grid where each rectangle is seperately drawn, keeping their width and height same(cellsize)
void Render::mainGrid() {
    float offsetX = 250.0f;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            float x = offsetX + j * cellSize;
            float y = i * cellSize;

            Rectangle cell = { x, y, cellSize, cellSize };

            Color col = ((i + j) % 2) ? BROWN : DARKBROWN;

            //selected rectangle becomes yellow
            if (i == selectedRow && j == selectedCol) {
                col = YELLOW;
            }
            // this for checkmate and check 
            if (board[i][j] != nullptr && board[i][j]->getTypeId() == 4) {
                if (board[i][j]->getColor() == color::white && white_check) {
                    col = RED;
                }
                if (board[i][j]->getColor() == color::black && black_check) {
                    col = RED;
                }
            }
            DrawRectangleRec(cell, col);

            //draws the pieces
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
    if (white_check) {
        DrawText("white check", width - 240, 100, 30, RED);
    }

    if (black_check) {
        DrawText("black check", width - 240, 150, 30, RED);
    }

    if (white_checkmate) {
        DrawText("white checkmate", width - 240, 250, 30, RED);
    }

    if (black_checkmate) {
        DrawText("black checkmate", width - 240, 300, 30, RED);
    }
}


void Render::promotionGrid(Piece* selectedPiece) {

    float offsetX = 400;
    float offsetY = 500;
    float popupWidth = 800;
    float popupHeight = 180;
    float popupCell = popupWidth / 4;
    Vector2 pos = { offsetX + 200, offsetY };

    Rectangle popup = {
        offsetX, offsetY,
        popupWidth, popupHeight
    };
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
    if (mouse.x < offsetX || mouse.x>1200 || mouse.y < 500 || mouse.y>680) {
        return;
    }
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    return;
    }
    int choice = (mouse.x - offsetX) / popupCell;
    Cord cordinates = (promotionPiece->getPosition());
    switch (choice) {
    case 0:
        board[cordinates.row][cordinates.col] = new Rook(promotionPiece->getColor(), cordinates.row, cordinates.col);
        break;
    case 1:
        board[cordinates.row][cordinates.col] = new Knight(promotionPiece->getColor(), cordinates.row, cordinates.col);
        break;
    case 2:
            board[cordinates.row][cordinates.col] = new Bishop(promotionPiece->getColor(), cordinates.row, cordinates.col);
            break;
    case 3:
            board[cordinates.row][cordinates.col] = new Queen(promotionPiece->getColor(), cordinates.row, cordinates.col);
            break;
    }
    promotionPiece = nullptr;

}
