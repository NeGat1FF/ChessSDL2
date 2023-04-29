#include "Pieces/Pawn.h"

#include "Board/Board.h"

#include <SDL2/SDL_log.h>

Pawn::Pawn(Color color) : Piece(Type::Pawn, color) {}

std::vector<std::shared_ptr<Square>> Pawn::GetMoves(int x, int y, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves;
    auto lastMove = board.GetLastMove();
    auto lastMovePiece = lastMove._piece;
    if (GetColor() == Color::White)
    {
        auto nextSquare = board.GetSquare(x, y - 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare);
        }
        if (!HasMoved())
        {
            nextSquare = board.GetSquare(x, y - 2);
            if (nextSquare && !nextSquare->GetPiece())
            {
                moves.push_back(nextSquare);
            }
        }

        nextSquare = board.GetSquare(x - 1, y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }
        nextSquare = board.GetSquare(x + 1, y - 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }


        // Check for en passant
        if (lastMovePiece && lastMovePiece->GetType() == Type::Pawn && lastMovePiece->GetColor() != GetColor())
        {
            Position lastMoveTo = lastMove._to;
            Position lastMoveFrom = lastMove._from;
            

            if (lastMoveTo.y == y && lastMoveFrom.y == y - 2)
            {
                if (lastMoveTo.x == x - 1)
                {
                    moves.push_back(board.GetSquare(x - 1, y - 1));
                }
                if (lastMoveTo.x == x + 1)
                {
                    moves.push_back(board.GetSquare(x + 1, y - 1));
                }
            }
        }
    }
    else
    {
        auto nextSquare = board.GetSquare(x, y + 1);
        if (nextSquare && !nextSquare->GetPiece())
        {
            moves.push_back(nextSquare);
        }
        if (!HasMoved())
        {
            nextSquare = board.GetSquare(x, y + 2);
            if (nextSquare && !nextSquare->GetPiece())
            {
                moves.push_back(nextSquare);
            }
        }

        nextSquare = board.GetSquare(x - 1, y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }
        nextSquare = board.GetSquare(x + 1, y + 1);
        if (nextSquare && nextSquare->GetPiece() && nextSquare->GetPiece()->GetColor() != GetColor())
        {
            moves.push_back(nextSquare);
        }

        // Check for en passant
        if (lastMovePiece && lastMovePiece->GetType() == Type::Pawn && lastMovePiece->GetColor() != GetColor())
        {
            Position lastMoveTo = lastMove._to;
            Position lastMoveFrom = lastMove._from;

            if (lastMoveTo.y == y && lastMoveFrom.y == y - 2)
            {
                if (lastMoveTo.x == x - 1)
                {
                    moves.push_back(board.GetSquare(x - 1, y - 1));
                }
                if (lastMoveTo.x == x + 1)
                {
                    moves.push_back(board.GetSquare(x + 1, y - 1));
                }
            }
        }
    }

    return moves;
}