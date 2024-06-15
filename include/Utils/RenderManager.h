#pragma once

#include <SDL_render.h>
#include <SDL_image.h>

#include "Board/Board.h"

class RenderManager
{
public:
    static RenderManager &Instance()
    {
        static RenderManager instance;
        return instance;
    }

    void Init(SDL_Renderer *renderer)
    {
        this->_renderer = renderer;
    }

    void DrawSquare(Square* square, int squareSize, bool flip)
    {
        // Create SDL_Rect for the square
        SDL_Rect rect = {square->GetX() * squareSize, (7-square->GetY()) * squareSize, squareSize, squareSize};
    }

private:
    SDL_Renderer *_renderer;

    RenderManager() {}
    ~RenderManager() {}

    RenderManager(const RenderManager &) = delete;
    RenderManager &operator=(const RenderManager &) = delete;

public:
    SDL_Color WhiteChessSquareColor = {0xFF, 0xD7, 0x00, 0xFF};
    SDL_Color BlackChessSquareColor = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color SelectedChessSquareColor = {0x00, 0xFF, 0x00, 0xFF};
};
