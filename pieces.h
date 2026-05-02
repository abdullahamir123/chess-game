#pragma once

enum class color { white, black };

struct Cord {
    int row;
    int col;
};


class Piece {
protected:
    color pieceColor;
    Cord position;

public:
    Piece(color c, int row, int col);
    virtual ~Piece() = default;

    color getColor() const;
    Cord getPosition() const;

    void setPosition(int row, int col);

    virtual int getTypeId() const = 0;
};


class Pawn : public Piece {
public:
    Pawn(color c, int row, int col);
    int getTypeId() const override;
};

class Rook : public Piece {
public:
    Rook(color c, int row, int col);
    int getTypeId() const override;
};

class Knight : public Piece {
public:
    Knight(color c, int row, int col);
    int getTypeId() const override;
};

class Bishop : public Piece {
public:
    Bishop(color c, int row, int col);
    int getTypeId() const override;
};

class Queen : public Piece {
public:
    Queen(color c, int row, int col);
    int getTypeId() const override;
};

class King : public Piece {
public:
    King(color c, int row, int col);
    int getTypeId() const override;
};