#include "pieces.h"
#include "render.h"
#include "check.h"
#include <iostream>
#include <cmath>

Piece::Piece(color c, int row, int col) : pieceColor(c), position{ row, col } {}

color Piece::getColor() const { return pieceColor; }
Cord Piece::getPosition() const { return position; }

void Piece::setPosition(int row, int col) {
    position.row = row;
    position.col = col;
}

void Pawn::afterMove() {
    hasMoved = true;
    if ((pieceColor == color::white && position.row == 0) ||
        (pieceColor == color::black && position.row == 7)) {
        Render::promotionPiece = this;
    }
}

bool Piece::noFriendlyCapture(int row, int col) const {
    if (Render::board[row][col] != nullptr) {
        if (this->getColor() == Render::board[row][col]->getColor()) {
            return false;
        }
        return true;
    }
    return true;
}

int Rook::getTypeId() const { return 0; }
int Knight::getTypeId() const { return 1; }
int Bishop::getTypeId() const { return 2; }
int Queen::getTypeId() const { return 3; }
int King::getTypeId() const { return 4; }
int Pawn::getTypeId() const { return 5; }

bool Piece::isPathClear(int row, int col) const {
    int rowDiff = row - position.row;
    int colDiff = col - position.col;

    int rowStep = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
    int colStep = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

    int checkRow = position.row + rowStep;
    int checkCol = position.col + colStep;

    while (checkRow != row || checkCol != col) {
        if (Render::board[checkRow][checkCol] != nullptr) {
            return false;
        }
        checkRow += rowStep;
        checkCol += colStep;
    }
    return true;
}

bool Pawn::isPawnPathClear(int row) {
    int forward = (pieceColor == color::white) ? -1 : 1;

    if (Render::board[position.row + forward][position.col] != nullptr) {
        return false;
    }
    if (std::abs(row - position.row) == 2) {
        if (Render::board[row][position.col] != nullptr) {
            return false;
        }
    }
    return true;
}

Pawn::Pawn(color c, int r, int col) : Piece(c, r, col), hasMoved(false) {}
bool Pawn::validmove(int row, int col) {
    int forward = (pieceColor == color::white) ? -1 : 1;
    int rowDiff = row - position.row;
    int colDiff = std::abs(col - position.col);

    if (colDiff == 0) {
        bool oneStep = (rowDiff == forward);
        bool twoStep = (!hasMoved && rowDiff == 2 * forward);

        if (oneStep || twoStep) {
            return isPawnPathClear(row);
        }
    }

    if (colDiff == 1 && rowDiff == forward) {
        if (Render::board[row][col] != nullptr) {
            return true;
        }
        if (col == Render::enPassantCol && row == Render::enPassantRow + forward) {
            return true;
        }
    }
    return false;
}

Rook::Rook(color c, int r, int col) : Piece(c, r, col), hasMoved(false) {}
void Rook::afterMove() { hasMoved = true; }
bool Rook::validmove(int row, int col) {
    if (!Piece::isPathClear(row, col)) return false;
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    return (rowDiff == 0 && col != position.col) || (colDiff == 0 && row != position.row);
}

Knight::Knight(color c, int r, int col) : Piece(c, r, col) {}
bool Knight::validmove(int row, int col) {
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

Bishop::Bishop(color c, int r, int col) : Piece(c, r, col) {}
bool Bishop::validmove(int row, int col) {
    if (!Piece::isPathClear(row, col)) return false;
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);
    return rowDiff == colDiff;
}

Queen::Queen(color c, int r, int col) : Piece(c, r, col) {}
bool Queen::validmove(int row, int col) {
    if (!Piece::isPathClear(row, col)) return false;
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    if (rowDiff == colDiff) return true;
    if (rowDiff == 0 && col != position.col) return true;
    if (colDiff == 0 && row != position.row) return true;
    return false;
}

King::King(color c, int r, int col) : Piece(c, r, col), hasMoved(false) {}
void King::afterMove() { hasMoved = true; }
bool King::validmove(int row, int col) {
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    if (!hasMoved && rowDiff == 0 && colDiff == 2) {
        int rookCol = (col > position.col) ? 7 : 0;
        Piece* ptr = Render::board[row][rookCol];

        if (ptr && ptr->getTypeId() == 0 && !ptr->getHasMoved()) {
            if (rookCol == 7) {
                if (Render::board[row][5] == nullptr && Render::board[row][6] == nullptr) {
                    if (!Check::king_check(pieceColor) && !Render::move_making_king_check(this, row, 5)) {
                        return true;
                    }
                }
            }
            else {
                if (Render::board[row][1] == nullptr && Render::board[row][2] == nullptr && Render::board[row][3] == nullptr) {
                    if (!Check::king_check(pieceColor) && !Render::move_making_king_check(this, row, 3)) {
                        return true;
                    }
                }
            }
        }
    }

    return rowDiff <= 1 && colDiff <= 1;
}

bool King::canAttack(int row, int col) {
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);
    return rowDiff <= 1 && colDiff <= 1;
}

bool Pawn::canAttack(int row, int col) {
    int forward = (pieceColor == color::white) ? -1 : 1;
    if (row == position.row + forward) {
        if (col == position.col + 1 || col == position.col - 1) {
            return true;
        }
    }
    return false;
}