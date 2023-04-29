#pragma once

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(Color color);
    std::vector<std::shared_ptr<Square>> GetMoves(int x, int y, Board& board) const override;
};