#pragma once

#include "Piece.h"

class King : public Piece
{
public:
    King(Color color);

    std::vector<Position> GetMoves(Position pos, Board& board) const override;
    std::vector<Position> GetMovesWithoutChecks(Position pos, Board& board) const;
    char GetFEN() const override;
};