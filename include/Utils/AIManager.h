#pragma once

#include "Board/Board.h"

class AIManager
{
public:

    AIManager() = delete;

    static std::vector<std::pair<Position, std::vector<Position>>> GenerateLegalMoves(Board &board);

    static void generatePositions(int depth, int& positions, std::string fen);

};