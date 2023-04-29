#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
    Bishop(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board& board) const override;

    static std::vector<std::shared_ptr<Square>> GetBishopMoves(Position pos, Board& board, Color color);
};
