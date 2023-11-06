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
    for(auto &element : _elements){
        if(element->GetType() == TextElementType::INPUT){
            dynamic_cast<Input*>(element.get())->handleEvent(event);
        }
    }
    if (event.type == SDL_MOUSEMOTION)
    {
        for (auto &element : _elements)
        {
            SDL_Point mousePos = {event.button.x, event.button.y};
            if (element->GetType() == TextElementType::BUTTON){
                Button* button = dynamic_cast<Button*>(element.get());
                if (SDL_PointInRect(&mousePos, &button->GetRect()))
                {
                    button->SetColor(SDL_Color{64, 64, 64, 255});
                }
                else
                {
                    button->SetColor(SDL_Color{32, 32, 32, 255});
                }
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT){
            for (auto &element : _elements)
            {
                SDL_Point mousePos = {event.button.x, event.button.y};
                if (element->GetType() == TextElementType::BUTTON && SDL_PointInRect(&mousePos, &element->GetRect()))
                {
                    dynamic_cast<Button*>(element.get())->Execute();
                }
                if (element->GetType() == TextElementType::INPUT && SDL_PointInRect(&mousePos, &element->GetRect())){
                    dynamic_cast<Input*>(element.get())->onClicked();
                }
            }
        }
    }

    Draw();
}

std::string Layout::GetText()
{
    for(auto &element : _elements){
        if(element->GetType() == TextElementType::INPUT){
            return dynamic_cast<Input*>(element.get())->GetText();
        }
    }

    return "";
}
void Layout::AddElement(std::unique_ptr<TextElement> element)
{
    _elements.push_back(std::move(element));

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

    for (auto &element : _elements)
    {
        element->Draw();
    }

    SDL_RenderPresent(_renderer);
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

    size_t elementCount = _elements.size();
    if (elementCount == 0)
    {
        return;
    }

    int elementWidth = static_cast<int>(_rect.w * 0.4);
    int elementHeight = static_cast<int>(_rect.h * 0.1);

    int spaceBetweenElements = _spacing; // Use the specified spacing between buttons
    int elementY = _titleRect.y + _titleRect.h + spaceBetweenElements;

    for (auto &element : _elements)
    {
        int elementX = (_rect.w - elementWidth) / 2;
        element->Resize(elementX, elementY, elementWidth, elementHeight);
        elementY += elementHeight + spaceBetweenElements;
    }
}
