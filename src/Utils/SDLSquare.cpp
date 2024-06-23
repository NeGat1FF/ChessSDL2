#include "Utils/SDLSquare.h"

#include "Utils/TextureManager.h"

SDLSquare::SDLSquare(std::shared_ptr<Square> Square, int x, int y, unsigned int size, bool isWhite): _square(Square), _x(x), _y(y), _isWhite(isWhite), _isSelected(false)
{
    Resize(size);
}

SDL_Rect& SDLSquare::GetRect()
{
    return _rect;
}

void SDLSquare::Resize(unsigned int size)
{
    _rect.h = _rect.w = size;

    _rect.x = _square->GetX() * size;
    _rect.y = (7 - _square->GetY()) * size;
}

void SDLSquare::Draw(SDL_Renderer* renderer, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor, bool isPlayerWhite)
{
    if (this->_isSelected)
    {
        SDL_SetRenderDrawColor(renderer, SelectedSquareColor.r, SelectedSquareColor.g, SelectedSquareColor.b, SelectedSquareColor.a);
        SDL_RenderFillRect(renderer, &this->_rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &this->_rect);
    }
    else
    {
        if (this->_isWhite)
        {
            SDL_SetRenderDrawColor(renderer, WhiteSquareColor.r, WhiteSquareColor.g, WhiteSquareColor.b, WhiteSquareColor.a);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, BlackSquareColor.r, BlackSquareColor.g, BlackSquareColor.b, BlackSquareColor.a);
        }

        SDL_RenderFillRect(renderer, &this->_rect);
    }

    if (this->_square->GetPiece())
    {
        if(isPlayerWhite){
            TextureManager::Instance().Draw(this->_square->GetPiece()->ToString(), &this->_rect, renderer);
        }
        else{
            TextureManager::Instance().Draw(this->_square->GetPiece()->ToString(), &this->_rect, renderer, static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL));
        }
    }
}
