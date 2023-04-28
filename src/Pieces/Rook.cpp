#include "Pieces/Rook.h"

#include "Board/Board.h"

Rook::Rook(Color color) : Piece(Type::Rook, color)
{
}

Rook::~Rook()
{
}


std::vector<std::shared_ptr<Square>> Rook::GetMoves(int x, int y, Board& board) const {
    std::vector<std::shared_ptr<Square>> moves;
    
    // Check all squares in the same row as the rook
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            std::shared_ptr<Square> square = board.GetSquare(i, y);
            if (square && (!square->GetPiece() || square->GetPiece()->GetColor() != this->GetColor())) {
                moves.push_back(square);
            }
        }
    }

    // Check all squares in the same column as the rook
    for (int j = 0; j < 8; j++) {
        if (j != y) {
            std::shared_ptr<Square> square = board.GetSquare(x, j);
            if (square && (!square->GetPiece() || square->GetPiece()->GetColor() != this->GetColor())) {
                moves.push_back(square);
            }
        }
    }

    return moves;
}
