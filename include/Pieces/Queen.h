#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board& board) const override;
};