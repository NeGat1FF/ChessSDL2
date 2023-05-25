#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>
#include <locale>
#include <codecvt>
#include <cstring>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Print "Hello World" in the center of the screen
    TTF_Font* font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 42);
    SDL_Color color = {255, 255, 255, 255};

    
    // loop
    bool running = true;
    SDL_Event event;

    // Print entered text

    std::string inputText = "";

    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;

    bool isCtrlPressed = false;

    while (running) {
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_TEXTINPUT) {
                inputText += event.text.text;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && inputText.size() > 0) {
                    if(isCtrlPressed){
                        while(inputText.size() > 0 && inputText.back() != ' '){
                            inputText.pop_back();
                        }
                    }
                    else{
                        inputText.pop_back();
                    }
                }
                else if (event.key.keysym.sym == SDLK_LCTRL) {
                    isCtrlPressed = true;
                }
                else if (event.key.keysym.sym == SDLK_v && isCtrlPressed) {
                    const char* clipboardText = SDL_GetClipboardText();
                    if (clipboardText != nullptr) {
                        inputText += clipboardText;
                        SDL_free((void*)clipboardText);
                    }
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LCTRL) {
                    isCtrlPressed = false;
                }
            }
            // Handle CTRL + BACKSPACE
        }

        surface = TTF_RenderUTF8_Solid(font, inputText.c_str(), color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        if (texture != nullptr) {
            SDL_Rect rect;
            rect.x = 320 - surface->w / 2;
            rect.y = 240 - surface->h / 2;
            rect.w = surface->w;
            rect.h = surface->h;
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
        SDL_RenderPresent(renderer);


        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // if (inputText.size())
        //     std::cout << inputText << std::endl;

        SDL_Delay(50);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
