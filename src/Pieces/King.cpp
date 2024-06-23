#include "Pieces/King.h"

#include "Board/Board.h"

King::King(Color color) : Piece(Type::King, color, 10000) {}

std::vector<Position> King::GetMoves(Position pos, Board &board) const
{
    std::vector<Position> moves = GetMovesWithoutChecks(pos, board);

    moves.erase(std::remove_if(moves.begin(), moves.end(), [&](const Position& position) {
        return board.IsTarget(position, GetColor());
    }), moves.end());

    // Add castling moves if the king hasn't moved 
    if (!HasMoved())
    {
        // King-side castling
        auto rookSquare = board.GetSquare(pos.x + 3, pos.y);
        if (rookSquare->GetPiece() && rookSquare->GetPiece()->GetType() == Type::Rook && !rookSquare->GetPiece()->HasMoved())
        {
            if (!board.GetSquare(pos.x + 1, pos.y)->GetPiece() && !board.GetSquare(pos.x + 2, pos.y)->GetPiece() && !board.IsTarget(Position(pos.x + 1, pos.y), GetColor()) && !board.IsTarget(Position(pos.x + 2, pos.y), GetColor()))
            {
                moves.push_back({pos.x + 2, pos.y});
            }
        }
        // Queen-side castling
        rookSquare = board.GetSquare(pos.x - 4, pos.y);
        if (rookSquare->GetPiece() && rookSquare->GetPiece()->GetType() == Type::Rook && !rookSquare->GetPiece()->HasMoved())
        {
            if (!board.GetSquare(pos.x - 1, pos.y)->GetPiece() && !board.GetSquare(pos.x - 2, pos.y)->GetPiece() && !board.GetSquare(pos.x - 3, pos.y)->GetPiece() && !board.IsTarget(Position(pos.x - 1, pos.y), GetColor()) && !board.IsTarget(Position(pos.x - 2, pos.y), GetColor()))
            {
                moves.push_back({pos.x - 2, pos.y});
            }
        }
    }

    return moves;
}

std::vector<Position> King::GetMovesWithoutChecks(Position pos, Board &board) const
{
    std::vector<Position> moves;
    int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int i = 0; i < 8; ++i)
    {
        Position newPos(pos.x + dx[i], pos.y + dy[i]);

        if (IsValidCoordinate(newPos))
        {
            auto nextSquare = board.GetSquare(newPos);
            if (nextSquare->GetPiece())
            {
                if (nextSquare->GetPiece()->GetColor() != GetColor())
                {
                    moves.push_back(newPos);
                }
            }
            else
            {
                moves.push_back(newPos);
            }
        }
    }

    return moves;
}

char King::GetFEN() const
{
    return GetColor() == Color::White ? 'K' : 'k';
}