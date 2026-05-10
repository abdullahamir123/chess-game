#pragma once
#include "pieces.h"

class Check {
public:
    static Cord locate_king(color kingColor);
    static bool king_check(color kingColor);
    static bool after_move_no_checkmate(Piece* piece, int targetRow, int targetCol);
    static bool piece_move_checkmate(color kingColor);
    static bool checking_checkmate(color kingColor);
    static bool checking_stalemate(color kingColor);
};