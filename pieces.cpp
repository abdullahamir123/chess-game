#include "pieces.h"
#include"render.h"
#include<iostream>



Piece::Piece(color c, int row, int col) : pieceColor(c), position{ row, col } {}

color Piece::getColor() const{
    return pieceColor;
}

Cord Piece::getPosition() const{
    return position;
}

void Piece::setPosition(int row, int col) {
    position.row = row;
    position.col = col;
    if (!(getTypeId() == 5)) {
        return;
    }
    if (getColor() == color::white && row == 0) {
        Render::promotionPiece = Render::selectedPiece;
    }
    else if (getColor() == color::black && row == 7) {
        Render::promotionPiece = Render::selectedPiece;
    }
}


bool Piece::noFriendlyCapture(int row, int col) const{
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


bool Piece::isPathClear(int rowDiff, int colDiff) const {
    if (!rowDiff) { //check horizontal path
        if (colDiff < 0) {
            for (int i = 1; i < std::abs(colDiff); i++) {
                if (Render::board[position.row][position.col - i] != nullptr) {
                    return false;
                }
            }
        }
        else {
            for (int i = 1; i < colDiff; i++) {
                if (Render::board[position.row][position.col + i] != nullptr) {
                    return false;
                }
            }
        }
    }
    else if (!colDiff) { //check vertical path
        if (rowDiff < 0) {
            for (int i = 1; i < std::abs(rowDiff); i++) {
                if (Render::board[position.row - i][position.col] != nullptr) {
                    return false;
                }
            }
        }
        else {
            for (int i = 1; i < rowDiff; i++) {
                if (Render::board[position.row + i][position.col] != nullptr) {
                    return false;
                }
            }
        }
    }
    else { //check diagonal paths
        if (rowDiff < 0 && colDiff < 0) {
            for (int i = 1; i < std::abs(rowDiff); i++) {
                if (Render::board[position.row - i][position.col - i] != nullptr) {
                    return false;
                }
            }
        }
        else if (rowDiff < 0 && colDiff > 0) {
            for (int i = 1; i < std::abs(rowDiff); i++) {
                if (Render::board[position.row - i][position.col + i] != nullptr) {
                    return false;
                }
            }
        }
        else if (rowDiff > 0 && colDiff > 0) {
            for (int i = 1; i < std::abs(rowDiff); i++) {
                if (Render::board[position.row + i][position.col + i] != nullptr) {
                    return false;
                }
            }
        }
        else if(rowDiff > 0 && colDiff < 0) {
            for (int i = 1; i < std::abs(rowDiff); i++) {
                if (Render::board[position.row + i][position.col - i] != nullptr) {
                    return false;
                }
            }
        }
    }
    return true;
}


bool Pawn::isPawnPathClear(int row) {
    int forward = (pieceColor == color::white) ? -1 : 1;

    // check one sqaure
    if (Render::board[position.row + forward][position.col] != nullptr) {
        return false;
    }
    // check second square if moving two places
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

    //if moving(not capture)
    if (colDiff == 0) {
        bool oneStep = (rowDiff == forward);
        bool twoStep = (!hasMoved && rowDiff == 2 * forward);

        if (oneStep || twoStep) {
            return isPawnPathClear(row);
        }
    }

    if (colDiff == 1 && rowDiff == forward) {
        return Render::board[row][col] != nullptr;
    }

    return false;
}

Rook::Rook(color c, int r, int col) : Piece(c, r, col) {}
bool Rook::validmove(int row, int col) {
    if (!Piece::isPathClear(row - position.row, col - position.col)) {
        return false;
    }
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    if (rowDiff == 0 && col != position.col) {
        return true;
    }
    if (colDiff == 0 && row != position.row) {
        return true;
    }
    return false;
}


Knight::Knight(color c, int r, int col) : Piece(c, r, col) {}
bool Knight::validmove(int row, int col) {
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}


Bishop::Bishop(color c, int r, int col) : Piece(c, r, col) {}
bool Bishop::validmove(int row, int col) {
    if (!Piece::isPathClear(row - position.row, col - position.col)) {
        return false;
    }
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);
    if (rowDiff == colDiff) {
        return true;
    }
    return false;
}


Queen::Queen(color c, int r, int col) : Piece(c, r, col) {}
bool Queen::validmove(int row, int col) {
    if (!Piece::isPathClear(row - position.row, col - position.col)) {
        return false;
    }
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);

    if (rowDiff == colDiff) {
        return true;
    }
    if (rowDiff==0 && col != position.col) {
        return true;
    }
    if (colDiff==0 && row != position.row) {
        return true;
    }
    return false;
}


King::King(color c, int r, int col) : Piece(c, r, col) {}
bool King::validmove(int row, int col) {
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    return true;

}

// pawn attacking check
bool Pawn::canAttack(int row, int col)  {
    int forward = (pieceColor == color::white) ? -1 : 1;
    if (row == position.row + forward) {
        if (col == position.col + 1 || col == position.col - 1) {
            return true;
        }
    }
    return false;
}