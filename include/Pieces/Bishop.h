#pragma once

#include "Piece.h"

class Bishop : public Piece
{
public:
    Bishop(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(int x, int y, Board& board) const override;

    ~Bishop();
};
