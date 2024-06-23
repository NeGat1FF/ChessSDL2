#include "Pieces/Rook.h"

#include "Board/Board.h"

Rook::Rook(Color color) : Piece(Type::Rook, color, 500) {}

std::vector<Position> Rook::GetMoves(Position pos, Board &board) const
{
    return GetRookMoves(pos, board, GetColor());
}

std::vector<Position> Rook::GetRookMoves(Position pos, Board& board, Color color){
    std::vector<Position> moves;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

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

char Rook::GetFEN() const
{
    return GetColor() == Color::White ? 'R' : 'r';
}