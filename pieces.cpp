#include "pieces.h"

Piece::Piece(color c, int row, int col) : pieceColor(c), position{ row, col } {}

color Piece::getColor() const{
    return pieceColor;
}

Cord Piece::getPosition() const{
    return position;
}

void Piece::setPosition(int row, int col){
    position.row = row;
    position.col = col;
}


int Rook::getTypeId() const { return 0; }
int Knight::getTypeId() const { return 1; }
int Bishop::getTypeId() const { return 2; }
int Queen::getTypeId() const { return 3; }
int King::getTypeId() const { return 4; }
int Pawn::getTypeId() const { return 5; }


Pawn::Pawn(color c, int r, int col) : Piece(c, r, col) {}
Rook::Rook(color c, int r, int col) : Piece(c, r, col) {}
Knight::Knight(color c, int r, int col) : Piece(c, r, col) {}
Bishop::Bishop(color c, int r, int col) : Piece(c, r, col) {}
Queen::Queen(color c, int r, int col) : Piece(c, r, col) {}
King::King(color c, int r, int col) : Piece(c, r, col) {}