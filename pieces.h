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
    bool noFriendlyCapture(int row, int col) const;
    bool isPathClear(int row, int col) const;

    virtual int getTypeId() const = 0;
    virtual bool validmove(int row, int col)  = 0;

    virtual bool canAttack(int row, int col) {
        return validmove(row, col);
    }
    virtual void afterMove() {}
    virtual bool getHasMoved() const { return true; }

};


class Pawn : public Piece {
public:
    bool hasMoved;
    Pawn(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;
    bool isPawnPathClear(int row);
    bool canAttack(int row, int col) override;

    void afterMove() override;
};

class Rook : public Piece {
public:
    bool hasMoved;
    Rook(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;
    bool getHasMoved() const override { return hasMoved; }

    void afterMove() override;
};

class Knight : public Piece {
public:
    Knight(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;

};

class Bishop : public Piece {
public:
    Bishop(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;

};

class Queen : public Piece {
public:
    Queen(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;

};

class King : public Piece {
public:
    bool hasMoved;
    King(color c, int row, int col);
    int getTypeId() const override;
    bool validmove(int row, int col) override;
    void afterMove() override;
};