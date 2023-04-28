#include "Pieces/Pawn.h"

Pawn::Pawn(Color color) : Piece(Type::Pawn, color)
{
}

Pawn::~Pawn()
{
}


std::vector<std::shared_ptr<Square>> Pawn::GetMoves(int x, int y, Board& board) const {
    return std::vector<std::shared_ptr<Square>>();
}