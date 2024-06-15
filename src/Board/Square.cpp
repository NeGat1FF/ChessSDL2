#include "Board/Square.h"

Square::Square(int x, int y): _x(x), _y(y)
{
    this->_piece = nullptr;

    this->_position.x = x;
    this->_position.y = y;
}


std::shared_ptr<Piece> Square::GetPiece() const
{
    return this->_piece;
}

void Square::SetPiece(const std::shared_ptr<Piece>& piece)
{
    this->_piece = piece;
}

int Square::GetX() const
{
    return this->_x;
}

int Square::GetY() const
{
    return this->_y;
}

Position Square::GetPosition() const
{
    return this->_position;
}
