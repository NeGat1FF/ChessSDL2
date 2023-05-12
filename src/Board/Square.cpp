#include "Board/Square.h"

Square::Square(int x, int y, unsigned int size, bool isWhite): _isWhite(isWhite), _isSelected(false), _size(size)
{
    this->_rect.x = x * _size;
    this->_rect.y = (7 - y) * _size;

    this->_rect.w = this->_rect.h = _size;

    this->_piece = nullptr;

    this->_position.x = x;
    this->_position.y = y;
}

SDL_Rect *Square::GetRect()
{
    return &this->_rect;
}

void Square::Resize(unsigned int size){

    this->_rect.h = this->_rect.w = this->_size = size;

    this->_rect.x = this->_position.x * _size;
    this->_rect.y = (7 - this->_position.y) * _size;
}

void Square::Draw(SDL_Renderer *renderer, bool isPlayerWhite)
{
    if (_isSelected)
    {
        SDL_SetRenderDrawColor(renderer, 235, 52, 73, 255);
        SDL_RenderFillRect(renderer, &this->_rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &this->_rect);
    }
    else
    {
        if (this->_isWhite)
        {
            SDL_SetRenderDrawColor(renderer, 238, 238, 210, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 118, 150, 86, 255);
        }

        SDL_RenderFillRect(renderer, &this->_rect);
    }


    if (this->_piece)
    {
        if(isPlayerWhite){
            TextureManager::Instance().Draw(this->_piece->ToString(), &this->_rect, renderer);
        }
        else{
            TextureManager::Instance().Draw(this->_piece->ToString(), &this->_rect, renderer, SDL_FLIP_VERTICAL);
        }
    }
}

std::shared_ptr<Piece> Square::GetPiece() const
{
    return this->_piece;
}

void Square::SetPiece(const std::shared_ptr<Piece>& piece)
{
    this->_piece = piece;
}

bool Square::IsSelected() const
{
    return this->_isSelected;
}

void Square::SetSelected(bool isSelected)
{
    this->_isSelected = isSelected;
}

Position Square::GetPosition() const
{
    return this->_position;
}
