#include "Board/Board.h"

#include <sstream>
#include <vector>

Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
}

Board::Board(const std::string &fen)
{
    LoadFen(fen);
}

void Board::LoadFen(const std::string &fen)
{
    white_pawns = 0;
    white_knights = 0;
    white_bishops = 0;
    white_rooks = 0;
    white_queens = 0;
    white_king = 0;

    black_pawns = 0;
    black_knights = 0;
    black_bishops = 0;
    black_rooks = 0;
    black_queens = 0;
    black_king = 0;

    white_pieces = 0;
    black_pieces = 0;

    all_pieces = 0;

    std::istringstream iss(fen);
    std::string board;
    iss >> board;

    int square = 56; // Start from the top-left corner of the board (A8)
    for (char c : board)
    {
        if (c == '/')
        {
            square -= 16; // Move to the start of the next rank
        }
        else if (isdigit(c))
        {
            square += c - '0'; // Move forward by the number of empty squares
        }
        else
        {
            switch (c)
            {
            case 'P':
                setBit(white_pawns, square);
                break;
            case 'N':
                setBit(white_knights, square);
                break;
            case 'B':
                setBit(white_bishops, square);
                break;
            case 'R':
                setBit(white_rooks, square);
                break;
            case 'Q':
                setBit(white_queens, square);
                break;
            case 'K':
                setBit(white_king, square);
                break;
            case 'p':
                setBit(black_pawns, square);
                break;
            case 'n':
                setBit(black_knights, square);
                break;
            case 'b':
                setBit(black_bishops, square);
                break;
            case 'r':
                setBit(black_rooks, square);
                break;
            case 'q':
                setBit(black_queens, square);
                break;
            case 'k':
                setBit(black_king, square);
                break;
            }
            square++;
        }
    }

    white_pieces = white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_king;
    black_pieces = black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_king;
    all_pieces = white_pieces | black_pieces;
    empty_squares = ~all_pieces;

    std::string turn;
    iss >> turn;

    whiteToMove = turn == "w";

    std::string castlingRights;
    iss >> castlingRights;

    whiteCastleKingside = castlingRights.find('K') != std::string::npos;
    whiteCastleQueenside = castlingRights.find('Q') != std::string::npos;
    blackCastleKingside = castlingRights.find('k') != std::string::npos;
    blackCastleQueenside = castlingRights.find('q') != std::string::npos;

    std::string enPassant;
    iss >> enPassant;
    // If there is no en passant square, set it to -1
    enPassantSquare = enPassant == "-" ? -1 : (enPassant[0] - 'a') + (enPassant[1] - '1') * 8;

    iss >> halfMoveClock;
    iss >> fullMoveCounter;
}

void Board::setBit(bitboard &bb, int square)
{
    bb |= 1ULL << square;
}

void Board::clearBit(bitboard &bb, int square)
{
    bb &= ~(1ULL << square);
}

bool Board::getBit(bitboard bb, int square)
{
    return bb & (1ULL << square);
}