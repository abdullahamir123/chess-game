#pragma once
#include <iostream>
#include <string>

using namespace std;

enum class color{white,black};
enum class type_of_pieces{queen,king,knight,bishop,pawn,rook};
class pieces {
protected:
	color  type_colors;
	type_of_pieces type_piece;
	int row;
	int col;
	int piece_number;
public:
	pieces(color c, type_of_pieces p, int rows, int cols, int num);
	 color get_color() const;
	type_of_pieces get_type_piece() const;
	 int get_row() const;
	 int get_col() const;
	int get_piece_number() const;
	virtual bool check_mover_validity(int r, int c) const = 0;
	virtual char piece_icon()const = 0;
	bool if_alive();
};
class pawn:public pieces{
public:
	pawn(color c, int row, int col, int num);
	bool check_mover_validity(int r,int c)const override;
	char piece_icon()const override;
};

class rook :public pieces {
public:
	rook(color c, int row, int col, int num);
	bool check_mover_validity(int r, int c)const override;
	char piece_icon()const override;
};

class king :public pieces {
public:
	king(color c, int row, int col, int num);
	bool check_mover_validity(int r, int c) const override;
	char piece_icon()const override;
};

class queen :public pieces {
public:
	queen(color c, int row, int col, int num);
	bool check_mover_validity(int r, int c)const override;
	char piece_icon()const override;
};

class knight :public pieces {
public:
	knight(color c, int row, int col, int num);
	bool check_mover_validity(int r, int c)const override;
	char piece_icon()const override;
};

class bishop :public pieces {
public:
	bishop(color c, int row, int col, int num);
	bool check_mover_validity(int r, int c)const override;
	char piece_icon()const override;
};