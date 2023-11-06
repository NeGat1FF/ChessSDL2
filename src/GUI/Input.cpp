#include "GUI\Input.h"

Input::Input(SDL_Color color, std::string text, TTF_Font* font, SDL_Renderer* renderer) : TextElement(color, text, font, renderer)
{

    this->_type = TextElementType::INPUT;
    SDL_SetTextInputRect(&_rect);
}

void Input::onClicked()
{
    isSelected = true;
    SDL_StartTextInput();
}

void Input::handleEvent(SDL_Event &e)
{
    if(e.type == SDL_TEXTINPUT){
        _text += e.text.text;
    }
    else if(e.type == SDL_KEYDOWN){
        if(e.key.keysym.sym == SDLK_BACKSPACE && _text.length() > 0){
            _text.pop_back();
        }
    }
    _updateTextRect();
}
