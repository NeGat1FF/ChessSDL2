#pragma once

#include "TextElement.h"

#include <SDL2/SDL_ttf.h>
#include <string>

#include <functional>

class Button : public TextElement
{
public:
    // Constructor
    Button(SDL_Color color, std::string text, std::function<void()> func, TTF_Font* font, SDL_Renderer* renderer);
    
    void Execute();

    ~Button() override;
private:

    std::function<void()> _func;
};
