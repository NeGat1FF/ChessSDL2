#pragma once

#include <memory>

#include <SDL_render.h>
#include <SDL_image.h>

#include "Board/Square.h"
#include "Board/Board.h"
#include "Utils/SDLSquare.h"

class SDLBoard
{
public:
    SDLBoard(std::shared_ptr<Board> board, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor, unsigned int squareSize);

    void Draw(SDL_Renderer* renderer);

    void Resize(unsigned int squareSize);

    void Click(int x, int y);

    void SelectPiece(std::shared_ptr<SDLSquare> square);

    void DeselectAll();

    void LoadFEN(std::string fen);

    std::shared_ptr<Board> GetBoard() { return _board; }

    std::shared_ptr<SDLSquare> GetSquare(const Position &position);


private:
    std::vector<std::vector<std::shared_ptr<SDLSquare>>> _squares;

    std::shared_ptr<SDLSquare> _selectedSquare;

    std::shared_ptr<Board> _board;

    SDL_Color _whiteSquareColor;
    SDL_Color _blackSquareColor;
    SDL_Color _selectedSquareColor;

    unsigned int _squareSize;
};