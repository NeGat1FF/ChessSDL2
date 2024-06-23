#pragma once

#include <cstdint>

using bitboard = uint64_t;

namespace MoveGenerator {

    static bitboard generateAllWhitePawnSinglePushes(bitboard white_pawns, bitboard empty_squares);
    static bitboard generateAllWhitePawnDoublePushes(bitboard white_pawns, bitboard empty_squares);
    static bitboard generateAllWhitePawnAttacks(bitboard white_pawns, bitboard black_pieces);
    static bitboard generateAllWhitePawnEnPassantAttacks(bitboard white_pawns, int enPassantSquare);
    static bitboard generateAllWhitePawnMoves(bitboard white_pawns, bitboard empty_squares, bitboard black_pieces, int enPassantSquare);

    static bitboard generateAllBlackPawnSinglePushes(bitboard black_pawns, bitboard empty_squares);
    static bitboard generateAllBlackPawnDoublePushes(bitboard black_pawns, bitboard empty_squares);
    static bitboard generateAllBlackPawnAttacks(bitboard black_pawns, bitboard white_pieces);
    static bitboard generateAllBlackPawnEnPassantAttacks(bitboard black_pawns, int enPassantSquare);
    static bitboard generateAllBlackPawnMoves(bitboard black_pawns, bitboard empty_squares, bitboard white_pieces, int enPassantSquare);

    static bitboard generateAllKnightMoves(bitboard knights, bitboard empty_squares, bitboard own_pieces);
    static bitboard generateAllBishopMoves(bitboard bishops, bitboard empty_squares, bitboard own_pieces);
    static bitboard generateAllRookMoves(bitboard rooks, bitboard empty_squares, bitboard own_pieces);
    static bitboard generateAllQueenMoves(bitboard queens, bitboard empty_squares, bitboard own_pieces);
    static bitboard generateAllKingMoves(bitboard king, bitboard empty_squares, bitboard own_pieces);
    

}