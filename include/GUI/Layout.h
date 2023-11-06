#pragma once

#include <memory>
#include <vector>

#include "TextElement.h"
#include "Button.h"
#include "Input.h"

class Layout {
public:
    Layout(int width, int height, int topPadding, int spacing, TTF_Font* font, std::string title, SDL_Color color, SDL_Renderer* renderer);
    void AddElement(std::unique_ptr<TextElement> element);
    void Resize(int width, int height);
    void Draw();
    void ProcessInput(SDL_Event& event);
    std::string GetText();

private:
    void _updateTextRect();
    void _updateSize();

    int _topPadding;
    int _spacing;

    TTF_Font* _font;
    std::string _title;

    SDL_Texture* _titleTexture;

    SDL_Rect _titleRect;

    SDL_Rect _rect;
    SDL_Color _color;
    SDL_Renderer* _renderer;
    std::vector<std::unique_ptr<TextElement>> _elements;
};
