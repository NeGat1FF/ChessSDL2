#include "GUI/Layout.h"

Layout::Layout(int width, int height, int topPadding, int spacing, TTF_Font *font, std::string text, SDL_Color color, SDL_Renderer *renderer)
{
    _color = color;

    _rect.x = 0;
    _rect.y = 0;
    _rect.w = width;
    _rect.h = height;

    _renderer = renderer;

    _topPadding = topPadding;
    _spacing = spacing;

    _font = font;
    _title = text;

    _titleTexture = nullptr;
}

void Layout::ProcessInput(SDL_Event &event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        for (auto &button : _buttons)
        {
            SDL_Point mousePos = {event.button.x, event.button.y};
            if (SDL_PointInRect(&mousePos, &button->GetRect()))
            {
                button->SetMouseOver(true);
            }
            else
            {
                button->SetMouseOver(false);
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT){
            for (auto &button : _buttons)
            {
                SDL_Point mousePos = {event.button.x, event.button.y};
                if (SDL_PointInRect(&mousePos, &button->GetRect()))
                {
                    button->Execute();
                }
            }
        }
    }
}

void Layout::AddButton(std::unique_ptr<Button> button)
{
    _buttons.push_back(std::move(button));

    _updateSize();
}

void Layout::Resize(int width, int height)
{
    _rect.w = width;
    _rect.h = height;

    _updateSize();
}

void Layout::Draw()
{
    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderFillRect(_renderer, &_rect);

    SDL_RenderCopy(_renderer, _titleTexture, nullptr, &_titleRect);

    for (auto &button : _buttons)
    {
        button->Draw();
    }
}

void Layout::_updateTextRect()
{
    float aspectRatio = 1.0f / 3.0f; // Button aspect ratio is 3:1
    float fontScale = 1.5f;          // Font size is 80% of button size

    int fontSize = static_cast<int>(_rect.h * 0.2f * aspectRatio * fontScale);

    // Set the font size
    TTF_SetFontSize(_font, fontSize);

    // Create the text surface using the font and text color
    SDL_Surface *textSurface = TTF_RenderText_Blended(_font, _title.c_str(), {255, 255, 255, 255});

    // Calculate the text position based on the button size and text size
    int textX = (_rect.w - textSurface->w) / 2; // center the text horizontally
    int textY = (_rect.h - textSurface->h) / 2; // center the text vertically

    // Update the text rectangle
    this->_titleRect.x = _rect.x + textX;
    this->_titleRect.y = _topPadding;
    this->_titleRect.w = textSurface->w;
    this->_titleRect.h = textSurface->h;

    // Destroy the old text texture if it exists
    if (_titleTexture != nullptr)
    {
        SDL_DestroyTexture(_titleTexture);
    }

    // Create a new text texture from the text surface
    this->_titleTexture = SDL_CreateTextureFromSurface(_renderer, textSurface);

    // Free the text surface
    SDL_FreeSurface(textSurface);
}

void Layout::_updateSize()
{
    _updateTextRect();

    int buttonCount = _buttons.size();
    if (buttonCount == 0)
    {
        return;
    }

    int buttonWidth = static_cast<int>(_rect.w * 0.4);
    int buttonHeight = static_cast<int>(_rect.h * 0.1);

    int spaceBetweenButtons = _spacing; // Use the specified spacing between buttons
    int buttonY = _titleRect.y + _titleRect.h + spaceBetweenButtons;

    for (auto &button : _buttons)
    {
        int buttonX = (_rect.w - buttonWidth) / 2;
        button->Resize(buttonX, buttonY, buttonWidth, buttonHeight);
        buttonY += buttonHeight + spaceBetweenButtons;
    }
}
