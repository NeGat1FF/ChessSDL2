#pragma once

#include "Piece.h"

class Knight : public Piece
{
public:
    Knight(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board& board) const override;
};