#pragma once

#include <memory>

#include <SDL_render.h>
#include <SDL_image.h>

#include "Board/Square.h"
#include "Board/Board.h"

class SDLBoard
{
public:
    SDLBoard(std::shared_ptr<Board> board, unsigned int squareSize);

    void Draw(SDL_Renderer* renderer, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor);

    void Resize(unsigned int squareSize);

    void SetSelectedSquare(int x, int y);

    void ClearSelectedSquare();

    void SetBoard(std::shared_ptr<Board> board);
};