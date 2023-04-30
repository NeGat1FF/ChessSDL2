#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include <SDL2/SDL_render.h>

#include "Utils/Move.h"
#include "Square.h"

// Piece headers
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

    void Draw(SDL_Renderer *renderer);
    void InitPieces();

    bool IsValidCoordinate(int x, int y) const;
    void MovePiece(const std::shared_ptr<Square>& fromSquare, const std::shared_ptr<Square>& toSquare);
    void SelectPiece(const std::shared_ptr<Square>& square);

    std::shared_ptr<Square> GetSquare(int x, int y);
    void Click(int x, int y);
    const Move& GetLastMove() const;

    void LoadFEN(const std::string& fen);
    bool IsTarget(const Position& pos, Color color);

    std::shared_ptr<Square> GetEnPassantSquare() const;
    std::string GetFEN() const;


private:

    void _unselectAll();
    std::shared_ptr<Piece> _getPieceFromFEN(char fenChar);

    Move _lastMove;

    Color _turnColor;

    int _halfMoveClock;
    int _fullMoveNumber;

    bool _canWhiteCastleKingside;
    bool _canWhiteCastleQueenside;

    bool _canBlackCastleKingside;
    bool _canBlackCastleQueenside;

    std::shared_ptr<Square> _selectedSquare;
    std::shared_ptr<Square> _enPassantSquare;

    std::vector<std::vector<std::shared_ptr<Square>>> _board;
};
