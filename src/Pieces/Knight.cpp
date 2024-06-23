#include "Pieces/Knight.h"

#include "Board/Board.h"

Knight::Knight(Color color) : Piece(Type::Knight, color, 300) {}

std::vector<Position> Knight::GetMoves(Position pos, Board &board) const
{
    std::vector<Position> moves;

    // Define the possible knight moves
    std::vector<std::pair<int, int>> knightMoves = {
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};

    for (const auto &move : knightMoves)
    {
        Position newPos = {pos.x + move.first, pos.y + move.second};

        if (IsValidCoordinate(newPos))
        {
            std::shared_ptr<Square> destinationSquare = board.GetSquare(newPos);
            std::shared_ptr<Piece> destinationPiece = destinationSquare->GetPiece();

            // Add the move if the destination square is empty or has an opponent's piece
            if (!destinationPiece || destinationPiece->GetColor() != GetColor())
            {
                moves.push_back(newPos);
            }
        }
    }

    return moves;
}

char Knight::GetFEN() const
{
    return GetColor() == Color::White ? 'N' : 'n';
}
