#include <Board/Board.h>
#include <iostream>


void printBitboardAsBoard(uint64_t bitboard)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            std::cout << ((bitboard & (1ULL << square)) ? '1' : '0') << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    Board board;
    printBitboardAsBoard(board.generateWhitePawnMoves());
    return 0;
}