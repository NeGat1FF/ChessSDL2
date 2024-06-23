#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(Color color);

    std::vector<Position> GetMoves(Position pos, Board& board) const override;
    char GetFEN() const override;
};