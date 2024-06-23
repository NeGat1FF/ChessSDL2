#pragma once

#include "Piece.h"

class Knight : public Piece
{
public:
    Knight(Color color);

    std::vector<Position> GetMoves(Position pos, Board& board) const override;
    char GetFEN() const override;
};