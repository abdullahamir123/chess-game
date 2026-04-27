#include "pieces.h"
#include <cmath>
#include <string>
#include <cstring>

pieces::pieces(color c, type_of_pieces p, int rows, int cols, int num) {
	type_colors = c;
	type_piece = p;
	row = rows;
	col = cols;
	piece_number = num;
}
int pieces::get_col()const {
	return col;
}
int pieces::get_row()const {
	return row;
}
int pieces::get_piece_number() const {
	return piece_number;
}
color pieces::get_color() const {
	return type_colors;
}
type_of_pieces pieces::get_type_piece() const {
	return type_piece;
}
pawn::pawn(color c, int row, int col, int num) :pieces(c, type_of_pieces::pawn, row, col, num){};
king::king(color c, int row, int col, int num) :pieces(c, type_of_pieces::king, row, col, num) {};
rook::rook(color c, int row, int col, int num) :pieces(c, type_of_pieces::rook, row, col, num) {};
bishop::bishop(color c, int row, int col, int num) :pieces(c, type_of_pieces::bishop, row, col, num) {};
queen::queen(color c, int row, int col, int num) :pieces(c, type_of_pieces::queen, row, col, num) {};
knight::knight(color c, int row, int col, int num) :pieces(c, type_of_pieces::knight, row, col, num) {};
