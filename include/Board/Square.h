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

    void Draw(SDL_Renderer *renderer);

    std::shared_ptr<Piece> GetPiece() const;
    void SetPiece(const std::shared_ptr<Piece>& piece);

    bool IsSelected() const;
    void SetSelected(bool isSelected);

    Position GetPosition() const;

private:
    SDL_Rect _rect;
    bool _isSelected;
    Position _position;
    const bool _isWhite;
    std::shared_ptr<Piece> _piece;
};