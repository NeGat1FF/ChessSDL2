#include "Pieces/Bishop.h"

#include "Board/Board.h"

Bishop::Bishop(Color color) : Piece(Type::Bishop, color) {}

std::vector<std::shared_ptr<Square>> Bishop::GetMoves(int x, int y, Board &board) const
{
    return GetBishopMoves(x, y, board, GetColor());
}

std::vector<std::shared_ptr<Square>> Bishop::GetBishopMoves(int x, int y, Board& board, Color color){
    std::vector<std::shared_ptr<Square>> moves;

    int dx[] = {1, 1, -1, -1};
    int dy[] = {1, -1, 1, -1};

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