#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_net.h>

#include "Utils/SDLBoard.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"

#include "Utils/AIManager.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    SDL_Window *window = SDL_CreateWindow("SDL2 Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*8, 64*8, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *icon = IMG_Load("assets/icon/icon.ico");
    SDL_SetWindowIcon(window, icon);

    if (!AudioManager::Instance().LoadSound("assets/audio"))
    {
        SDL_Log("Failed to load sound");
        return 1;  
    }
    
    if (!TextureManager::Instance().LoadSVG("assets/sprites", 64, renderer))
    {
        SDL_Log("Failed to load sprites");
        return 1;
    }

    SDLBoard board(std::make_shared<Board>(), SDL_Color{238, 238, 210, 255}, SDL_Color{118, 150, 86, 255}, SDL_Color{255, 0, 0, 255}, 64);

    bool running = true;

    while (running)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                board.Click(x, y);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    board.Resize(event.window.data1 / 8);
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_SPACE)
                {
                    board.GetBoard()->UndoMove();
                    std::cout << board.GetBoard()->GetFEN() << std::endl;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        board.Draw(renderer);

        SDL_RenderPresent(renderer);
    }

    return 0;
}