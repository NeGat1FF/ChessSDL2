#include "Pieces/Pawn.h"

#include "Board/Board.h"

#include <SDL2/SDL_log.h>

Pawn::Pawn(Color color) : Piece(Type::Pawn, color) {}

std::vector<std::shared_ptr<Square>> Pawn::GetMoves(Position pos, Board &board) const
{
    std::vector<std::shared_ptr<Square>> moves;
    auto lastMove = board.GetLastMove();
    auto lastMovePiece = lastMove._piece;
    if (GetColor() == Color::White)
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
        if (lastMovePiece && lastMovePiece->GetType() == Type::Pawn && lastMovePiece->GetColor() != GetColor())
        {
            Position lastMoveTo = lastMove._to;
            Position lastMoveFrom = lastMove._from;
            

            if (lastMoveTo.y == pos.y && lastMoveFrom.y == pos.y - 2)
            {
                if (lastMoveTo.x == pos.x - 1)
                {
                    moves.push_back(board.GetSquare(pos.x - 1, pos.y - 1));
                }
                if (lastMoveTo.x == pos.x + 1)
                {
                    moves.push_back(board.GetSquare(pos.x + 1, pos.y - 1));
                }
            }
        }
    }
    else
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
        if (lastMovePiece && lastMovePiece->GetType() == Type::Pawn && lastMovePiece->GetColor() != GetColor())
        {
            Position lastMoveTo = lastMove._to;
            Position lastMoveFrom = lastMove._from;

            if (lastMoveTo.y == pos.y && lastMoveFrom.y == pos.y - 2)
            {
                if (lastMoveTo.x == pos.x - 1)
                {
                    moves.push_back(board.GetSquare(pos.x - 1, pos.y - 1));
                }
                if (lastMoveTo.x == pos.x + 1)
                {
                    moves.push_back(board.GetSquare(pos.x + 1, pos.y - 1));
                }
            }
        }
    }

    return moves;
}