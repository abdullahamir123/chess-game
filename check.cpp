#include "check.h"
#include "render.h"

Cord Check::locate_king(color kingColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* p = Render::board[i][j];
            if (p != nullptr) {
                if (p->getColor() == kingColor) {
                    if (p->getTypeId() == 4) {
                        Cord kingPosition;
                        kingPosition.row = i;
                        kingPosition.col = j;
                        return kingPosition;
                    }
                }
            }
        }
    }
    Cord not_found;
    not_found.row = -1;
    not_found.col = -1;
    return not_found;
}

bool Check::king_check(color color_king) {
    Cord location_king = locate_king(color_king);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* enemy = Render::board[i][j];
            if (enemy == nullptr) {
                continue;
            }
            if (enemy->getColor() == color_king) {
                continue;
            }

            if (enemy->canAttack(location_king.row, location_king.col)) {
                return true;
            }
        }
    }
    return false;
}

bool Check::piece_move_checkmate(color kingColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = Render::board[i][j];
            if (piece == nullptr) continue;
            if (piece->getColor() != kingColor) continue;

            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (!piece->validmove(row, col)) continue;
                    if (!piece->noFriendlyCapture(row, col)) continue;
                    if (after_move_no_checkmate(piece, row, col)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Check::after_move_no_checkmate(Piece* piece, int targetRow, int targetCol) {
    Cord position_king = piece->getPosition();
    Piece* capture_piece = Render::board[targetRow][targetCol];

    bool isEnPassant = false;
    Piece* enPassantPawn = nullptr;
    int epRow = -1, epCol = -1;

    if (piece->getTypeId() == 5 && capture_piece == nullptr && targetCol != position_king.col) {
        isEnPassant = true;
        epRow = position_king.row;
        epCol = targetCol;
        enPassantPawn = Render::board[epRow][epCol];
        Render::board[epRow][epCol] = nullptr;
    }

    Render::board[position_king.row][position_king.col] = nullptr;
    Render::board[targetRow][targetCol] = piece;

    piece->setPosition(targetRow, targetCol);
    bool safe = !king_check(piece->getColor());

    Render::board[position_king.row][position_king.col] = piece;
    Render::board[targetRow][targetCol] = capture_piece;
    piece->setPosition(position_king.row, position_king.col);

    if (isEnPassant) {
        Render::board[epRow][epCol] = enPassantPawn;
    }

    return safe;
}

bool Check::checking_stalemate(color kingColor) {
    if (king_check(kingColor))
        return false;

    if (piece_move_checkmate(kingColor))
        return false;

    return true;
}

bool Check::checking_checkmate(color kingColor) {
    if (!king_check(kingColor)) {
        return false;
    }
    if (piece_move_checkmate(kingColor)) {
        return false;
    }
    return true;
}