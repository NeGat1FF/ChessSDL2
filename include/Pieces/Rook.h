#pragma once

#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(Color color);

    std::vector<Position> GetMoves(Position pos, Board &board) const override;
    char GetFEN() const override;

    static std::vector<Position> GetRookMoves(Position pos, Board& board, Color color);
};