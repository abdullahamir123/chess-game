#include "pieces.h"
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
}

int Rook::getTypeId() const { return 0; }
int Knight::getTypeId() const { return 1; }
int Bishop::getTypeId() const { return 2; }
int Queen::getTypeId() const { return 3; }
int King::getTypeId() const { return 4; }
int Pawn::getTypeId() const { return 5; }


Pawn::Pawn(color c, int r, int col) : Piece(c, r, col), hasMoved(false) {}
bool Pawn::validmove(int row, int col) {
    if (col == position.col) {
        if (pieceColor == color::black) {
            if (!hasMoved) {
                hasMoved = true;
                return row == position.row + 1 || row == position.row + 2;
            }
            return row == position.row + 1;
        }

        if (pieceColor == color::white) {
            if (!hasMoved) {
                //makes sure moving 2 pieces only possible on first turn of piece
                hasMoved = true;
                return row == position.row - 1 || row == position.row - 2;
            }
            return row == position.row - 1;
        }
        return false;
    }
    return false;
}

Rook::Rook(color c, int r, int col) : Piece(c, r, col) {}
bool Rook::validmove(int row, int col) {
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
    int rowDiff = std::abs(row - position.row);
    int colDiff = std::abs(col - position.col);
    if (rowDiff == colDiff) {
        return true;
    }
    return false;
}


Queen::Queen(color c, int r, int col) : Piece(c, r, col) {}
bool Queen::validmove(int row, int col) {
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