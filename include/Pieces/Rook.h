#pragma once

#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(int x, int y, Board &board) const override;

    static std::vector<std::shared_ptr<Square>> GetRookMoves(int x, int y, Board& board, Color color);
};