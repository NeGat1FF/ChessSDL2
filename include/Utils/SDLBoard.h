#pragma once

#include "Board/Board.h"
#include <SDL_render.h>

class SDLBoard
{
public:
    SDLBoard(Board board, int squareSize, SDL_Renderer *renderer, SDL_Color whiteSqrColor = {238, 238, 210, 255}, SDL_Color blackSqrColor = {118, 150, 86, 255}, SDL_Color selSqrColor = {235, 52, 73, 255});

    void Draw();
    void Click(int x, int y);

private:
    void SelectSquare(int x, int y);
    void DeselectSquare(int x, int y);
    void DeselectAllSquares();
    bool IsSquareSelected(int x, int y);

    Board m_board;
    int m_squareSize;

    SDL_Color m_whiteSqrColor;
    SDL_Color m_blackSqrColor;
    SDL_Color m_selSqrColor;

    bitboard m_selectedSquares{0};

    SDL_Renderer *_renderer;
};