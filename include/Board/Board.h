#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

#include <SDL2/SDL_render.h>

#include "Utils/Move.h"
#include "Utils/AudioManager.h"
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
    Board(unsigned int size = 64);

    void InitPieces();
    void Resize(int size);
    void Draw(SDL_Renderer *renderer);

    bool IsValidCoordinate(int x, int y) const;
    bool IsValidCoordinate(const Position& pos) const;
    void MovePiece(const std::shared_ptr<Square>& fromSquare, const std::shared_ptr<Square>& toSquare);
    void SelectPiece(const std::shared_ptr<Square>& square);

    std::shared_ptr<Square> GetSquare(int x, int y);
    std::shared_ptr<Square> GetSquare(const Position& pos);
    void Click(int x, int y);
    const Move& GetLastMove() const;

    void FilterMoves(std::vector<std::shared_ptr<Square>>& moves, const std::shared_ptr<Square>& square, Color color);
    void VirtualMove(const std::shared_ptr<Square>& fromSquare,const std::shared_ptr<Square>& toSquare, const std::shared_ptr<Piece>& piece);

    void LoadFEN(const std::string& fen);
    bool IsTarget(const Position& pos, Color color);

    std::shared_ptr<Square> GetEnPassantSquare() const;
    std::string GetFEN() const;

    void UpdateCheckStatus();


private:

    void _unselectAll();
    std::shared_ptr<Piece> _getPieceFromFEN(char fenChar);

    Move _lastMove;

    Color _turnColor;

    int _halfMoveClock;
    int _fullMoveNumber;

    unsigned int _size;

    bool _canWhiteCastleKingside;
    bool _canWhiteCastleQueenside;

    bool _canBlackCastleKingside;
    bool _canBlackCastleQueenside;

    bool _isWhiteChecked;
    bool _isBlackChecked;

    std::shared_ptr<Square> _whiteKingSquare;
    std::shared_ptr<Square> _blackKingSquare;

    std::shared_ptr<Square> _selectedSquare;
    std::shared_ptr<Square> _enPassantSquare;

    std::vector<std::vector<std::shared_ptr<Square>>> _board;
};
