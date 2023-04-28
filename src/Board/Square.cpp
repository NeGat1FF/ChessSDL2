#include "Board/Square.h"

Square::Square(int x, int y, bool isWhite): _isWhite(isWhite), _isSelected(false)
{
    this->_rect.x = x;
    this->_rect.y = y;

    this->_rect.w = this->_rect.h = SQUARE_SIZE;

    this->_piece = nullptr;
}

SDL_Rect *Square::GetRect()
{
    return &this->_rect;
}

void Square::SetPiece(std::shared_ptr<Piece> piece)
{
    this->_piece = piece;
}

void Square::SetSelected(bool isSelected)
{
    this->_isSelected = isSelected;
}

void Square::Draw(SDL_Renderer *renderer)
{
    if (_isSelected)
    {
        SDL_SetRenderDrawColor(renderer, 235, 52, 73, 255);
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
    }

    SDL_RenderFillRect(renderer, &this->_rect);

    if (this->_piece)
    {
        TextureManager::Instance().Draw(this->_piece->ToString(), &this->_rect, renderer);
    }
}

std::shared_ptr<Piece> Square::GetPiece() const
{
    return this->_piece;
}

bool Square::IsSelected() const
{
    return this->_isSelected;
}

Square::~Square()
{
}
