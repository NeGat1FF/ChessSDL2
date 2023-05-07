#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

#include <functional>

class Button {
public:
    // Constructor
    Button(SDL_Color color, SDL_Color hoverColor, std::string text, std::function<void()> func, TTF_Font* font, SDL_Renderer* renderer);

    // Render the button
    void Draw();

    void SetColor(SDL_Color color);
    void SetMouseOver(bool isMouseOver);

    void Resize(int x, int y, int width, int height);

    void Execute();

    SDL_Rect& GetRect();

    ~Button();

private:
    void _updateTextRect();

    bool _isMouseOver;

    std::function<void()> _func;

    SDL_Rect _rect;
    SDL_Rect _textRect;

    TTF_Font* _font;

    std::string _text;

    SDL_Color _color;
    SDL_Color _hoverColor;
    
    SDL_Renderer* _renderer;
    SDL_Texture* _textTexture;
};
