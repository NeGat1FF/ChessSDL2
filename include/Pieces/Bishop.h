#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
    Bishop(Color color);

    std::vector<Position> GetMoves(Position pos, Board& board) const override;
    char GetFEN() const override;

    static std::vector<Position> GetBishopMoves(Position pos, Board& board, Color color);
};
