#include "GUI\TextElement.h"

TextElement::TextElement(SDL_Color color, std::string text, TTF_Font *font, SDL_Renderer *renderer, bool isButton)
{
    _color = color;
    _text = text;
    _font = font;
    _renderer = renderer;

    _isButton = isButton;

    _textTexture = nullptr;

    _type = TextElementType::TEXT;

    _updateTextRect();
}

TextElementType TextElement::GetType() const
{
    return _type;
}

void TextElement::Draw()
{
    SDL_Rect shadowRect = {_rect.x + 2, _rect.y + 2, _rect.w, _rect.h};
    SDL_Color shadowColor = {static_cast<Uint8>(_color.r * 0.7), static_cast<Uint8>(_color.g * 0.7), static_cast<Uint8>(_color.b * 0.7), _color.a};
    SDL_SetRenderDrawColor(_renderer, shadowColor.r, shadowColor.g, shadowColor.b, shadowColor.a);
    SDL_RenderFillRect(_renderer, &shadowRect);

    // Draw the button
    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderFillRect(_renderer, &_rect);

    SDL_RenderCopy(_renderer, _textTexture, NULL, &_textRect);
}

void TextElement::SetColor(SDL_Color color)
{
    _color = color;
}

std::string &TextElement::GetText()
{
    return _text;
}

void TextElement::Resize(int x, int y, int width, int height)
{
    _rect.x = x;
    _rect.y = y;
    _rect.w = width;
    _rect.h = height;

    _updateTextRect();
}

SDL_Rect &TextElement::GetRect()
{
    return _rect;
}

TextElement::~TextElement()
{
}

void TextElement::_updateTextRect()
{
    float aspectRatio = float(_rect.h) / float(_rect.w); // Button aspect ratio is 3:1
    float fontScale = 1.5f;                              // Font size is 80% of button size

    int fontSize = static_cast<int>(_rect.h * aspectRatio * fontScale);

    // Set the font size
    TTF_SetFontSize(_font, fontSize);

    // Create the text surface using the font and text color
    SDL_Surface *textSurface = TTF_RenderText_Blended(_font, _text.c_str(), {255, 255, 255, 255});

    if(textSurface == nullptr){
        std::cout << "Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Calculate the text position based on the button size and text size
    int textX = (_rect.w - textSurface->w) / 2; // center the text horizontally
    int textY = (_rect.h - textSurface->h) / 2; // center the text vertically

    // Update the text rectangle
    this->_textRect = {_rect.x + textX, _rect.y + textY, textSurface->w, textSurface->h};

    // Destroy the old text texture if it exists
    if (_textTexture != nullptr)
    {
        SDL_DestroyTexture(_textTexture);
    }

    // Create a new text texture from the text surface
    this->_textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

    // Free the text surface
    SDL_FreeSurface(textSurface);
}