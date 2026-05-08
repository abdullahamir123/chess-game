#include "board.h"
#include <iostream>
#include <iomanip>

board::board():white_pieces_count(0), black_pieces_count(0) {
    for (int i = 0; i < pieces_per_side; i++) {
        white_pieces[i] = nullptr;
        black_pieces[i] = nullptr;
    }
}
board::~board() {
    delete[] black_pieces;
    delete[] white_pieces;  
}