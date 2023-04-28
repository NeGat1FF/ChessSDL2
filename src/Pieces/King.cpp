#include "Pieces/King.h"

King::King(Color color) : Piece(Type::King, color)
{
}

King::~King()
{
}

std::vector<std::shared_ptr<Square>> King::GetMoves(int x, int y, Board& board) const {
    return std::vector<std::shared_ptr<Square>>();
}