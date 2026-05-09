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

    if (getColor() == color::black) {
        if (Render::board[position.row+1][position.col] != nullptr) {
            return false;
        }
        if (!hasMoved) {
            if (std::abs(position.row - row) == 2) {
                if (Render::board[position.row + 2][position.col] != nullptr) {
                    return false;
                }
            }
            else {
                if (Render::board[position.row + 1][position.col] != nullptr) {
                    return false;
                }
            }
        }
    }
    if (getColor() == color::white) {
        if (Render::board[position.row - 1][position.col] != nullptr) {
            return false;
        }
        if (!hasMoved) {
            if (std::abs(position.row - row) == 2) {
                if (Render::board[position.row - 2][position.col] != nullptr) {
                    return false;
                }
            }
            else {
                if (Render::board[position.row - 1][position.col] != nullptr) {
                    return false;
                }
            }
        }
    }
    return true;
}


Pawn::Pawn(color c, int r, int col) : Piece(c, r, col), hasMoved(false) {}
bool Pawn::validmove(int row, int col) {
    if (pieceColor == color::white) {
        // diagonal attacking of pawn when a enemy piece near its diognal
        if (row == position.row - 1) {
            if (col == position.col - 1 || col == position.col + 1) {
                if (Render::board[row][col] != nullptr) {
                    if (Render::board[row][col]->getColor() != pieceColor) {
                        return true;
                    }
                }
            }
        }
        // moving forward
        if (col == position.col) {
            if (!isPawnPathClear(row)) {
                return false;
            }
            // it onyl allwoing that pawn can only move 2 steps only once
            if (!hasMoved) {
                if (row == position.row - 1 || row == position.row - 2) {
                    return true;
                }
            }
            if (row == position.row - 1) {
                return true;
            }
        }
    }
    if (pieceColor == color::black) {
        // diagonal attacking of pawn when a enemy piece near its diognal
        if (row == position.row + 1) {
            if (col == position.col - 1 || col == position.col + 1) {
                if (Render::board[row][col] != nullptr) {
                    if (Render::board[row][col]->getColor() != pieceColor) {
                        return true;
                    }
                }
            }
        }
        // moving forward
        if (col == position.col) {
            if (!isPawnPathClear(row)) {
                return false;
            }
            // it onyl allwoing that pawn can only move 2 steps only once
            if (!hasMoved) {
                if (row == position.row + 1 || row == position.row + 2) {
                    return true;
                }
            }
            if (row == position.row + 1) {
                return true;
            }
        }
    }
    if (!isPawnPathClear(row)) { 
        return false;
    }
    if (col == position.col) {
        if (pieceColor == color::black) {
            if (!hasMoved) {
                if (std::abs(row - position.row) > 2) {
                    return false;
                }
                hasMoved = true;
                return row == position.row + 1 || row == position.row + 2;
            }
            return row == position.row + 1;
        }

        if (pieceColor == color::white) {
            if (!hasMoved) {
                if (std::abs(row - position.row) > 2) {
                    return false;
                }
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
bool Pawn::pawn_attack(int row, int col) const {
    if (pieceColor == color::white) {
        if (row == position.row - 1) {
            if (col == position.col - 1 || col == position.col + 1) {
                return true;
            }
        }
    }
    if (pieceColor == color::black) {
        if (row == position.row + 1) {
            if (col == position.col - 1 || col == position.col + 1) {
                return true;
            }
        }
    }
    return false;
}
