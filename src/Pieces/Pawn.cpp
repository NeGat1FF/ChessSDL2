#include "Pieces/Pawn.h"
#include "Board/Board.h"

Pawn::Pawn(Color color) : Piece(Type::Pawn, color, 100) {}

std::vector<Position> Pawn::GetMoves(Position pos, Board &board) const
{
    std::vector<Position> moves;
    auto lastMove = board.GetLastMove();
    auto lastMovePiece = lastMove._piece;

    auto enPassantSquare = board.GetEnPassantSquare();
    if (GetColor() == Color::White)
    {
        auto nextSquare = board.GetSquare(pos.x, pos.y + 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare->GetPosition());

            // Double move
            if (pos.y == 1)
            {
                auto doubleSquare = board.GetSquare(pos.x, pos.y + 2);
                if (doubleSquare && !doubleSquare->GetPiece())
                {
                    moves.push_back(doubleSquare->GetPosition());
                }
            }
        }

        nextSquare = board.GetSquare(pos.x - 1, pos.y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare->GetPosition());
        }
        nextSquare = board.GetSquare(pos.x + 1, pos.y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare->GetPosition());
        }

        // Check for en passant
        if (enPassantSquare && enPassantSquare->GetPosition().y == pos.y + 1)
        {
            if (enPassantSquare->GetPosition().x == pos.x - 1 || enPassantSquare->GetPosition().x == pos.x + 1)
            {
                moves.push_back(enPassantSquare->GetPosition());
            }
        }
    }
    else
    {
        auto nextSquare = board.GetSquare(pos.x, pos.y - 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare->GetPosition());

            // Double move
            if (pos.y == 6)
            {
                auto doubleSquare = board.GetSquare(pos.x, pos.y - 2);
                if (doubleSquare && !doubleSquare->GetPiece())
                {
                    moves.push_back(doubleSquare->GetPosition());
                }
            }
        }

        nextSquare = board.GetSquare(pos.x - 1, pos.y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare->GetPosition());
        }
        nextSquare = board.GetSquare(pos.x + 1, pos.y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare->GetPosition());
        }

        // Check for en passant
        if (enPassantSquare && enPassantSquare->GetPosition().y == pos.y - 1)
        {
            if (enPassantSquare->GetPosition().x == pos.x - 1 || enPassantSquare->GetPosition().x == pos.x + 1)
            {
                moves.push_back(enPassantSquare->GetPosition());
            }
        }
    }

    return moves;
}

std::vector<Position> Pawn::GetAttackSquares(Position pos, Board &board) const
{
    std::vector<Position> moves;

    if (GetColor() == Color::White)
    {
        Position leftAttack(pos.x - 1, pos.y + 1);
        Position rightAttack(pos.x + 1, pos.y + 1);

        if (IsValidCoordinate(leftAttack))
        {
            moves.push_back(leftAttack);
        }
        if (IsValidCoordinate(rightAttack))
        {
            moves.push_back(rightAttack);
        }
    }
    else
    {
        Position leftAttack(pos.x - 1, pos.y - 1);
        Position rightAttack(pos.x + 1, pos.y - 1);

        if (IsValidCoordinate(leftAttack))
        {
            moves.push_back(leftAttack);
        }
        if (IsValidCoordinate(rightAttack))
        {
            moves.push_back(rightAttack);
        }
    }

    return moves;
}

char Pawn::GetFEN() const
{
    return GetColor() == Color::White ? 'P' : 'p';
}