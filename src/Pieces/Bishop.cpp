#include "Pieces/Bishop.h"

#include "Board/Board.h"

Bishop::Bishop(Color color) : Piece(Type::Bishop, color, 300) {}

std::vector<Position> Bishop::GetMoves(Position pos, Board &board) const
{
    return GetBishopMoves(pos, board, GetColor());
}

std::vector<Position> Bishop::GetBishopMoves(Position pos, Board& board, Color color){
    std::vector<Position> moves;

    int dx[] = {1, 1, -1, -1};
    int dy[] = {1, -1, 1, -1};

    for (int direction = 0; direction < 4; ++direction) {
        Position newPos = {pos.x + dx[direction], pos.y + dy[direction]};

        while (IsValidCoordinate(newPos)) {
            auto square = board.GetSquare(newPos);

            if (square->GetPiece() == nullptr) {
                moves.push_back(newPos);
            } else {
                if (square->GetPiece()->GetColor() != color) {
                    moves.push_back(newPos);
                }
                break;
            }

            newPos.x += dx[direction];
            newPos.y += dy[direction];
        }
    }

    return moves;
}

char Bishop::GetFEN() const
{
    return GetColor() == Color::White ? 'B' : 'b';
}