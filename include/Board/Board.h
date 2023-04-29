#pragma once

#include <vector>
#include <memory>

#include "SDL2/SDL_render.h"

#include "Utils/Move.h"

#include "Square.h"

#include "Pieces/Piece.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Knight.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"


class Board
{
public:
    Board();
    ~Board();

    void Draw(SDL_Renderer *renderer);
    void InitPieces();

    bool IsValidCoordinate(int x, int y) const;

    void MovePiece(const std::shared_ptr<Square>& fromSquare, const std::shared_ptr<Square>& toSquare);
    void SelectPiece(const std::shared_ptr<Square>& square);

    std::shared_ptr<Square> GetSquare(int x, int y);
    void Click(int x, int y);
    const Move& GetLastMove() const;

    bool IsTarget(const Position& pos, Color color);

    std::shared_ptr<Square> GetEnPassantSquare() const;

    std::string GetFEN() const;

private:
    void UnselectAll();
    std::vector<std::vector<std::shared_ptr<Square>>> _board;
    std::shared_ptr<Square> _selectedSquare;

    Move _lastMove;

    bool _isWhiteTurn;

    std::shared_ptr<Square> _enPassantSquare;

    int _halfMoveClock;
    int _fullMoveNumber;
    
    bool _canWhiteCastleKingside;
    bool _canWhiteCastleQueenside;

    bool _canBlackCastleKingside;
    bool _canBlackCastleQueenside;
};
