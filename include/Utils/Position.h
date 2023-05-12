#pragma once

#include <string>

struct Position
{
    Position() : x(-1), y(-1) {}
    Position(int x, int y) : x(x), y(y) {}
    Position(const std::string& pos): x(pos[0] - 'a'), y(pos[1] - '1'){}

    int x;
    int y;

    bool operator==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }

    std::string ToString() const
    {
        if(x == -1 || y == -1)
            return "None";
        return std::string(1, x + 'a') + std::string(1, y + '1');
    }
};
