#pragma once

#include <vector>
#include <memory>
#include <string>
#include <sstream>

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
    Board(Color playerColor = Color::White, std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    void MakeMove(Position from, Position to);
    void MakeMove(std::string from, std::string to);
    void MakeMove(const std::shared_ptr<Square>& fromSquare, const std::shared_ptr<Square>& toSquare);

    void UndoMove();

    void SelectPiece(const std::shared_ptr<Square>& square);

    inline Color GetPlayerColor() const { return _playerColor;}
    inline Color GetTurnColor() const { return _turnColor;}
    inline void SetPlayerColor(Color color) { _playerColor = color;}

    inline std::shared_ptr<Square> GetSquare(int x, int y);
    inline std::shared_ptr<Square> GetSquare(const Position& pos);

    inline Move GetLastMove() const { return _lastMove;}

    void FilterMoves(std::vector<Position>& moves, Position square, Color color);
    void VirtualMove(Position fromPos,Position toPos, const std::shared_ptr<Piece>& piece);

    void LoadFEN(const std::string& fen);
    bool IsTarget(const Position& pos, Color color);

    std::shared_ptr<Square> GetEnPassantSquare() const;
    std::string GetFEN() const;

    std::vector<std::vector<std::shared_ptr<Square>>> GetBoard() const;

    void UpdateCheckStatus();

    bool IsChecked(){return _isWhiteChecked || _isBlackChecked;}


private:
    std::shared_ptr<Piece> _getPieceFromFEN(char fenChar);

    Move _lastMove;

    Color _turnColor;
    Color _playerColor;

    int _halfMoveClock;
    int _fullMoveNumber;

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
