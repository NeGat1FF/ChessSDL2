#include "Pieces/Queen.h"

Queen::Queen(Color color) : Piece(Type::Queen, color)
{
}

Queen::~Queen()
{
}


std::vector<std::shared_ptr<Square>> Queen::GetMoves(int x, int y, Board& board) const {
    return std::vector<std::shared_ptr<Square>>();
}