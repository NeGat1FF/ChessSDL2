#include "Pieces/Piece.h"

Piece::Piece(Type type, Color color) : _type(type), _color(color) {}

Type Piece::GetType() const
{
    return _type;
}

std::string Piece::ToString() const
{
    std::string res;
    switch (_type)
    {
    case Type::Pawn:
        res = "p";
        break;
    case Type::Rook:
        res = "r";
        break;
    case Type::Knight:
        res = "n";
        break;
    case Type::Bishop:
        res = "b";
        break;
    case Type::Queen:
        res = "q";
        break;
    case Type::King:
        res = "k";
        break;
    default:
        break;
    }
    switch (_color)
    {
    case Color::White:
        res += "l";
        break;
    case Color::Black:
        res += "d";
        break;
    default:
        break;
    }

    res += "t45.png";
    return res;
}

void Piece::Move()
{
    _hasMoved = true;
}

bool Piece::HasMoved() const
{
    return _hasMoved;
}

Color Piece::GetColor() const
{
    return _color;
}