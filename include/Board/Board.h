#pragma once

#include <string>
#include <cstdint>

using bitboard = uint64_t;

class Board
{
public:
    Board();
    Board(const std::string &fen);

    void LoadFen(const std::string &fen);

    void setBit(bitboard &bb, int square);
    void clearBit(bitboard &bb, int square);

    bitboard generateWhitePawnMoves();

private:
    int halfMoveClock;
    int fullMoveCounter;

    bool whiteToMove;
    bool whiteCastleKingside;
    bool whiteCastleQueenside;
    bool blackCastleKingside;
    bool blackCastleQueenside;
    int enPassantSquare;

    bitboard white_pawns;
    bitboard white_knights;
    bitboard white_bishops;
    bitboard white_rooks;
    bitboard white_queens;
    bitboard white_king;

    bitboard black_pawns;
    bitboard black_knights;
    bitboard black_bishops;
    bitboard black_rooks;
    bitboard black_queens;
    bitboard black_king;

    bitboard white_pieces;
    bitboard black_pieces;

    bitboard all_pieces;
    bitboard empty_squares;
};
