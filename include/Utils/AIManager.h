#pragma once

#include <string>
#include <utility>
#include "Board/Board.h"

class AIManager
{
public:
    AIManager(Board* board);

    void MakeMove();

    std::vector<std::pair<std::shared_ptr<Square>, std::vector<std::shared_ptr<Square>>>> GenerateLegalMoves();

    void generatePositions(int depth, int &positions);
private:
    Board* _board;
};