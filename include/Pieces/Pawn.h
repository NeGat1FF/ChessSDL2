#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(Color color);
    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board &board) const override;
    char GetFEN() const override;
};