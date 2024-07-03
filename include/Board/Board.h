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

    static void setBit(bitboard &bb, int square);
    static void clearBit(bitboard &bb, int square);
    static bool getBit(bitboard bb, int square);

    inline bitboard GetWhitePawns() const { return white_pawns; }
    inline bitboard GetWhiteKnights() const { return white_knights; }
    inline bitboard GetWhiteBishops() const { return white_bishops; }
    inline bitboard GetWhiteRooks() const { return white_rooks; }
    inline bitboard GetWhiteQueens() const { return white_queens; }
    inline bitboard GetWhiteKing() const { return white_king; }

    inline bitboard GetBlackPawns() const { return black_pawns; }
    inline bitboard GetBlackKnights() const { return black_knights; }
    inline bitboard GetBlackBishops() const { return black_bishops; }
    inline bitboard GetBlackRooks() const { return black_rooks; }
    inline bitboard GetBlackQueens() const { return black_queens; }
    inline bitboard GetBlackKing() const { return black_king; }

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
