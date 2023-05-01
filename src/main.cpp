#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Board/Board.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"


const int DIVISOR = 8;

int SQUARE_SIZE = 64;

int setWindowSizeSquare(SDL_Window* window, int newSize) {
    if (newSize % DIVISOR != 0) {
        newSize = (newSize / DIVISOR) * DIVISOR;
    }
    SDL_SetWindowSize(window, newSize, newSize);

    return newSize / 8;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *icon = IMG_Load("../assets/icon/icon.ico");
    SDL_SetWindowIcon(window, icon);


    if(!AudioManager::Instance().LoadSound("../assets/audio")){
        SDL_Log("Failed to load sound");
        return 1;
    }
    if(!TextureManager::Instance().LoadSVG("../assets/sprites", SQUARE_SIZE, renderer)){
        SDL_Log("Failed to load sprites");
        return 1;
    }

    Board board;
    board.InitPieces();

    while (true)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                board.Click(e.button.x, e.button.y);
            }

            if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    int newWidth = e.window.data1;
                    int newHeight = e.window.data2;
                    int newSize = std::min(newWidth, newHeight);

                    newSize = setWindowSizeSquare(window, newSize);

                    TextureManager::Instance().Cleanup();
                    TextureManager::Instance().LoadSVG("../assets/sprites", newSize, renderer);

                    board.Resize(newSize);
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    // SDL_Log(board.GetFEN().c_str());
                }
                if (e.key.keysym.sym == SDLK_r)
                {
                    board.InitPieces();
                }
                if (e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
            }

            if (e.type == SDL_QUIT)
            {
                break;
            }
        }

        board.Draw(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}