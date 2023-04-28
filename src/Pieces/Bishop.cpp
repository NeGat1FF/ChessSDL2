#include "Pieces/Bishop.h"

Bishop::Bishop(Color color) : Piece(Type::Bishop, color)
{
}

Bishop::~Bishop()
{
}
std::vector<std::shared_ptr<Square>> Bishop::GetMoves(int x, int y, Board& board) const {
    return std::vector<std::shared_ptr<Square>>();
}