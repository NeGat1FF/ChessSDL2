#include "Pieces/Rook.h"

#include "Board/Board.h"

Rook::Rook(Color color) : Piece(Type::Rook, color) {}

std::vector<std::shared_ptr<Square>> Rook::GetMoves(int x, int y, Board &board) const
{
    return GetRookMoves(x, y, board, GetColor());
}

std::vector<std::shared_ptr<Square>> Rook::GetRookMoves(int x, int y, Board& board, Color color){
    std::vector<std::shared_ptr<Square>> moves;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int direction = 0; direction < 4; ++direction) {
        int newX = x + dx[direction];
        int newY = y + dy[direction];

        while (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            auto square = board.GetSquare(newX, newY);

            if (square->GetPiece() == nullptr) {
                moves.push_back(square);
            } else {
                if (square->GetPiece()->GetColor() != color) {
                    moves.push_back(square);
                }
                break;
            }

            newX += dx[direction];
            newY += dy[direction];
        }
    }

    return moves;
}
