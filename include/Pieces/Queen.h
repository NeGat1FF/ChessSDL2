#pragma once

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(int x, int y, Board& board) const override;

    ~Queen();
};