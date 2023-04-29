#pragma once

#include "Piece.h"

class King : public Piece
{
public:
    King(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board& board) const override;
};