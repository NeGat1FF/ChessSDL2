#pragma once

#include <memory>

#include <SDL_render.h>
#include <SDL_image.h>

#include "Board/Square.h"

class SDLSquare
{
public:
    SDLSquare(std::shared_ptr<Square> Square, int x, int y, unsigned int size, bool isWhite);

    SDL_Rect& GetRect();

    inline bool IsWhite() const { return _isWhite; }
    inline bool IsSelected() const { return _isSelected; }

    inline void SetSelected(bool isSelected) { _isSelected = isSelected; }

    void Resize(unsigned int size);

    void Draw(SDL_Renderer* renderer, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor);

private:
    int _x;
    int _y;
    SDL_Rect _rect;
    
    bool _isWhite;
    bool _isSelected;

    std::shared_ptr<Square> _square;
};