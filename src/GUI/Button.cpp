#include "GUI/Button.h"

Button::Button(SDL_Color color, std::string text, std::function<void()> func, TTF_Font* font, SDL_Renderer* renderer) : TextElement(color, text, font, renderer, true)
{
    this->_func = func;
    this->_type = TextElementType::BUTTON;
}

void Button::Execute() {
    if(_func){
        _func();
    }
}

Button::~Button() {
    // Nothing to do here
}