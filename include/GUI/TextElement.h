#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

// Enum for the different types of text elements
enum TextElementType {
    TEXT,
    BUTTON,
    INPUT
};

class TextElement {
public:
    // Constructor
    TextElement(SDL_Color color, std::string text, TTF_Font* font, SDL_Renderer* renderer, bool isButton = false);

    // Render the button
    void Draw();

    void SetColor(SDL_Color color);

    void Resize(int x, int y, int width, int height);

    SDL_Rect& GetRect();

    TextElementType GetType() const;

    std::string& GetText();

    // Virtual destructor
    virtual ~TextElement();
protected:
    void _updateTextRect();

    SDL_Rect _rect;
    SDL_Rect _textRect;

    TTF_Font* _font;

    std::string _text;

    SDL_Color _color;

    bool _isButton;

    TextElementType _type;
    
    SDL_Renderer* _renderer;
    SDL_Texture* _textTexture;
};