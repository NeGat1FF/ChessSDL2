#include "GUI/Button.h"

Button::Button(SDL_Color color, SDL_Color _hoverColor, std::string text, std::function<void()> func, TTF_Font* font, SDL_Renderer* renderer) {
    this->_color = color;
    this->_text = text;
    this->_font = font;

    this->_hoverColor = _hoverColor;

    this->_rect = {0, 0, 0, 0};

    this->_isMouseOver = false;

    this->_renderer = renderer;

    this->_textTexture = nullptr;

    this->_func = func;

    _updateTextRect();
}

void Button::Execute() {
    if(_func){
        _func();
    }
}

void Button::_updateTextRect() {
    float aspectRatio = float(_rect.h) / float(_rect.w);  // Button aspect ratio is 3:1
    float fontScale = 1.5f;  // Font size is 80% of button size

    int fontSize = static_cast<int>(_rect.h * aspectRatio * fontScale);

    // Set the font size
    TTF_SetFontSize(_font, fontSize);

    // Create the text surface using the font and text color
    SDL_Surface* textSurface = TTF_RenderText_Blended(_font, _text.c_str(), {255, 255, 255, 255});
    
    // Calculate the text position based on the button size and text size
    int textX = (_rect.w - textSurface->w) / 2; // center the text horizontally
    int textY = (_rect.h - textSurface->h) / 2; // center the text vertically

    // Update the text rectangle
    this->_textRect = {_rect.x + textX, _rect.y + textY, textSurface->w, textSurface->h};

    // Destroy the old text texture if it exists
    if (_textTexture != nullptr) {
        SDL_DestroyTexture(_textTexture);
    }

    // Create a new text texture from the text surface
    this->_textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

    // Free the text surface
    SDL_FreeSurface(textSurface);
}


void Button::SetMouseOver(bool isMouseOver) {
    this->_isMouseOver = isMouseOver;
}

void Button::Resize(int x, int y, int width, int height) {
    _rect.x = x;
    _rect.y = y;
    _rect.w = width;
    _rect.h = height;

    _updateTextRect();
}

void Button::Draw() {
    // Draw the shadow first
    SDL_Rect shadowRect = {_rect.x + 2, _rect.y + 2, _rect.w, _rect.h};
    SDL_Color shadowColor = {static_cast<Uint8>(_color.r * 0.7), static_cast<Uint8>(_color.g * 0.7), static_cast<Uint8>(_color.b * 0.7), _color.a};
    SDL_SetRenderDrawColor(_renderer, shadowColor.r, shadowColor.g, shadowColor.b, shadowColor.a);
    SDL_RenderFillRect(_renderer, &shadowRect);

    // Draw the button
    if(_isMouseOver) {
        SDL_SetRenderDrawColor(_renderer, _hoverColor.r, _hoverColor.g, _hoverColor.b, _hoverColor.a);
    } else {
        SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    }
    SDL_RenderFillRect(_renderer, &_rect);

    // Draw the text
    SDL_RenderCopy(_renderer, _textTexture, NULL, &_textRect);
}

SDL_Rect& Button::GetRect() {
    return _rect;
}

void Button::SetColor(SDL_Color color) {
    this->_color = color;
}

Button::~Button() {
    //TTF_CloseFont(_font);
}