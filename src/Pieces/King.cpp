#include "Pieces/King.h"

#include "Board/Board.h"

King::King(Color color) : Piece(Type::King, color) {}

std::vector<std::shared_ptr<Square>> King::GetMoves(Position pos, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves = GetMovesWithoutChecks(pos, board);

    moves.erase(std::remove_if(moves.begin(), moves.end(), [&](const std::shared_ptr<Square>& sq) {
        return board.IsTarget(sq->GetPosition(), GetColor());
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
                moves.push_back(board.GetSquare(pos.x + 2, pos.y));
            }
        }
        // Queen-side castling
        rookSquare = board.GetSquare(pos.x - 4, pos.y);
        if (rookSquare->GetPiece() && rookSquare->GetPiece()->GetType() == Type::Rook && !rookSquare->GetPiece()->HasMoved())
        {
            if (!board.GetSquare(pos.x - 1, pos.y)->GetPiece() && !board.GetSquare(pos.x - 2, pos.y)->GetPiece() && !board.GetSquare(pos.x - 3, pos.y)->GetPiece() && !board.IsTarget(Position(pos.x - 1, pos.y), GetColor()) && !board.IsTarget(Position(pos.x - 2, pos.y), GetColor()))
            {
                moves.push_back(board.GetSquare(pos.x - 2, pos.y));
            }
        }
    }

    return moves;
}

std::vector<std::shared_ptr<Square>> King::GetMovesWithoutChecks(Position pos, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves;
    int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int i = 0; i < 8; ++i)
    {
        Position newPos(pos.x + dx[i], pos.y + dy[i]);

        if (newPos.x >= 0 && newPos.x < 8 && newPos.y >= 0 && newPos.y < 8)
        {
            auto nextSquare = board.GetSquare(newPos.x, newPos.y);
            if (nextSquare->GetPiece())
            {
                if (nextSquare->GetPiece()->GetColor() != GetColor())
                {
                    moves.push_back(nextSquare);
                }
            }
            else
            {
                moves.push_back(nextSquare);
            }
        }
    }

    return moves;
}

char King::GetFEN() const
{
    return GetColor() == Color::White ? 'K' : 'k';
}