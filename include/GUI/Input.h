#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "TextElement.h"

class Input : public TextElement
{
public:
    // Constructor
    Input(SDL_Color color, std::string text, TTF_Font* font, SDL_Renderer* renderer);

    void onClicked();
    void handleEvent(SDL_Event& e);

private:
    bool isSelected = false;
};