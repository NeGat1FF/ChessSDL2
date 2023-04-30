#include "Pieces/Pawn.h"

#include "Board/Board.h"

#include <SDL2/SDL_log.h>

Pawn::Pawn(Color color) : Piece(Type::Pawn, color) {}

std::vector<std::shared_ptr<Square>> Pawn::GetMoves(Position pos, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves;
    auto lastMove = board.GetLastMove();
    auto lastMovePiece = lastMove._piece;


    auto enPassantSquare = board.GetEnPassantSquare();
    if (GetColor() == Color::White)
    {
        auto nextSquare = board.GetSquare(pos.x, pos.y + 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare);
        }
        if (!HasMoved())
        {
            nextSquare = board.GetSquare(pos.x, pos.y + 2);
            if (nextSquare && !nextSquare->GetPiece())
            {
                moves.push_back(nextSquare);
            }
        }

        nextSquare = board.GetSquare(pos.x - 1, pos.y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }
        nextSquare = board.GetSquare(pos.x + 1, pos.y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }

        // Check for en passant
        if (enPassantSquare && enPassantSquare->GetPosition().y == pos.y + 1)
        {
            if (enPassantSquare->GetPosition().x == pos.x - 1 || enPassantSquare->GetPosition().x == pos.x + 1)
            {
                moves.push_back(enPassantSquare);
            }
        }
    }
    else
    {
        auto nextSquare = board.GetSquare(pos.x, pos.y - 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare);
        }
        if (!HasMoved())
        {
            nextSquare = board.GetSquare(pos.x, pos.y - 2);
            if (nextSquare && !nextSquare->GetPiece())
            {
                moves.push_back(nextSquare);
            }
        }

        nextSquare = board.GetSquare(pos.x - 1, pos.y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }
        nextSquare = board.GetSquare(pos.x + 1, pos.y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }

        // Check for en passant
        if (enPassantSquare && enPassantSquare->GetPosition().y == pos.y - 1)
        {
            if (enPassantSquare->GetPosition().x == pos.x - 1 || enPassantSquare->GetPosition().x == pos.x - 1)
            {
                moves.push_back(enPassantSquare);
            }
        }
    }

    return moves;
}

char Pawn::GetFEN() const
{
    return GetColor() == Color::White ? 'P' : 'p';
}