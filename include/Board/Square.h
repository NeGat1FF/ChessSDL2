#pragma once

#include "SDL2/SDL_render.h"
#include "Pieces/Piece.h"
#include "TextureManager/TextureManager.h"
#include "Utils/Position.h"

#define SQUARE_SIZE 45

class Square
{
public:
    Square(int x, int y, bool isWhite);

    SDL_Rect* GetRect();

    void SetPiece(std::shared_ptr<Piece> piece);
    void Draw(SDL_Renderer *renderer);

    std::shared_ptr<Piece> GetPiece() const;

    void SetSelected(bool isSelected);
    bool IsSelected() const;

    Position GetPosition() const;

    ~Square();

private:
    SDL_Rect _rect;
    bool _isSelected;
    const bool _isWhite;
    std::shared_ptr<Piece> _piece;
    Position _position;
};