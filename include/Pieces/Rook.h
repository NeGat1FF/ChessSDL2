#pragma once

#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(Color color);

    std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board &board) const override;
    char GetFEN() const override;

    static std::vector<std::shared_ptr<Square>> GetRookMoves(Position pos, Board& board, Color color);
};