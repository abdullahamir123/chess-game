#pragma once
#include <iostream>
#include <string>

using namespace std;

enum class color{white,black};
enum class type_of_pieces{queen,king,knight,bishop,pawn,rook};


struct Cord {
	int row;
	int col;
};


class pieces {
protected:
	color  type_colors;
	type_of_pieces type_piece;
	Cord cordinates;
	int piece_number;
public:
	pieces(color c, type_of_pieces p, int rows, int cols, int num);
	 color get_color() const;
	type_of_pieces get_type_piece() const;
	Cord get_cordinates() const;
	int get_piece_number() const;
};
class pawn:public pieces{
public:
	pawn(color c, int row, int col, int num);
};

class rook :public pieces {
public:
	rook(color c, int row, int col, int num);
};

class king :public pieces {
public:
	king(color c, int row, int col, int num);
};

class queen :public pieces {
public:
	queen(color c, int row, int col, int num);
};

class knight :public pieces {
public:
	knight(color c, int row, int col, int num);
};

class bishop :public pieces {
public:
	bishop(color c, int row, int col, int num);
};