#pragma once
#include "pieces.h"
const int pieces_per_side = 16;
const int board_size = 8;
class board {
protected:
	pieces* black_pieces[pieces_per_side];
	pieces* white_pieces[pieces_per_side];
	board* chess_board[board_size][board_size];
	int white_pieces_count = 0;
	int black_pieces_count = 0;
public:
	board();
	void display_board() const;
	void show_pieces() const;
	pieces* mover_piece_to(int row, int col);
	~board();
};