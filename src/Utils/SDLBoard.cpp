#include "Utils/SDLBoard.h"

SDLBoard::SDLBoard(std::shared_ptr<Board> board, unsigned int squareSize) : _board(board), _squareSize(squareSize)
{
}

void SDLBoard::Draw(SDL_Renderer *renderer, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor)
{
}

void SDLBoard::Resize(unsigned int squareSize)
{
}

void SDLBoard::SetSelectedSquare(int x, int y)
{
}

void SDLBoard::ClearSelectedSquare()
{
}

void SDLBoard::SetBoard(std::shared_ptr<Board> board)
{
}
