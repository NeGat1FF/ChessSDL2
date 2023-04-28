#pragma once

#include <string>
#include <memory>
#include <vector>

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

    virtual std::vector<std::shared_ptr<Square>> GetMoves(int x, int y, Board& board) const = 0;

    ~Piece();
private:
    Type _type;
    Color _color;
};

