#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Utils\Position.h"

class Square;
class Board;

enum class Type
{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

enum class Color
{
    White,
    Black
};

class Piece
{
public:
    Piece(Type type, Color color);

    Type GetType() const;
    Color GetColor() const;

    std::string ToString() const;

    virtual std::vector<std::shared_ptr<Square>> GetMoves(Position pos, Board& board) const = 0;
    virtual char GetFEN() const = 0;

    void Move();
    bool HasMoved() const;

private:
    bool _hasMoved = false;
    Type _type;
    Color _color;
};

