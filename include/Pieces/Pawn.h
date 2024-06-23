#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(Color color);
    std::vector<Position> GetMoves(Position pos, Board &board) const override;
    std::vector<Position> GetAttackSquares(Position pos, Board &board) const;
    char GetFEN() const override;
};