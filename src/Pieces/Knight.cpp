#include "Pieces/Knight.h"

#include "Board/Board.h"

Knight::Knight(Color color) : Piece(Type::Knight, color)
{
}

Knight::~Knight()
{
}

std::vector<std::shared_ptr<Square>> Knight::GetMoves(int x, int y, Board& board) const {
    std::vector<std::shared_ptr<Square>> moves;

    // Define the possible knight moves
    std::vector<std::pair<int, int>> knightMoves = {
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1},
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1}
    };

    for (const auto& move : knightMoves) {
        int newX = x + move.first;
        int newY = y + move.second;

        if (board.IsValidCoordinate(newX, newY)) {
            std::shared_ptr<Square> destinationSquare = board.GetSquare(newX, newY);
            std::shared_ptr<Piece> destinationPiece = destinationSquare->GetPiece();

            // Add the move if the destination square is empty or has an opponent's piece
            if (!destinationPiece || destinationPiece->GetColor() != GetColor()) {
                moves.push_back(destinationSquare);
            }
        }
    }

    return moves;
}
