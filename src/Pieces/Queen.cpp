#include "Pieces/Queen.h"

#include "Board/Board.h"

#include "Pieces/Bishop.h"
#include "Pieces/Rook.h"

Queen::Queen(Color color) : Piece(Type::Queen, color) {}

std::vector<std::shared_ptr<Square>> Queen::GetMoves(int x, int y, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves;

    auto bishopMoves = Bishop::GetBishopMoves(x, y, board, GetColor());
    auto rookMoves = Rook::GetRookMoves(x, y, board, GetColor());

    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());

    return moves;
}