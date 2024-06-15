#pragma once

#include "Pieces/Piece.h"
#include "Utils/TextureManager.h"
#include "Utils/Position.h"

class Square
{
public:
    Square(int x, int y);

    std::shared_ptr<Piece> GetPiece() const;
    void SetPiece(const std::shared_ptr<Piece>& piece);

    int GetX() const;
    int GetY() const;

    Position GetPosition() const;

    void Resize(unsigned int size);

private:
    int _x;
    int _y;
    Position _position;
    std::shared_ptr<Piece> _piece;
};