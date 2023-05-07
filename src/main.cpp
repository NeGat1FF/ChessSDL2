#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include "Board/Board.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"

#include "GUI/Layout.h"

#include <cmath>

int SQUARE_SIZE = 64;

int WINDOW_WIDTH = SQUARE_SIZE * 8;
int WINDOW_HEIGHT = SQUARE_SIZE * 8;

int setWindowSizeSquare(SDL_Window *window, int newSize)
{
    if (newSize % 8 != 0)
    {
        newSize = (newSize / 8) * 8;
    }
    SDL_SetWindowSize(window, newSize, newSize);

    return newSize / 8;
}

bool inMenu = true;

void SwitchToGame()
{
    inMenu = false;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *icon = IMG_Load("../assets/icon/icon.ico");
    SDL_SetWindowIcon(window, icon);

    if (!AudioManager::Instance().LoadSound("../assets/audio"))
    {
        SDL_Log("Failed to load sound");
        return 1;
    }
    if (!TextureManager::Instance().LoadSVG("../assets/sprites", SQUARE_SIZE, renderer))
    {
        SDL_Log("Failed to load sprites");
        return 1;
    }

    TTF_Font *logoFont = TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 64);

    Layout layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "ChessGame", {0, 0, 0, 200}, renderer);

    TTF_Font *font = TTF_OpenFont("../assets/fonts/OpenSans-Regular.ttf", 30);

    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Singleplayer", SwitchToGame, font, renderer));
    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Multiplayer", nullptr, font, renderer));
    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Quit", SDL_Quit, font, renderer));

    Board board;
    board.InitPieces();

    while (true)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (inMenu)
            {
                layout.ProcessInput(e);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (!inMenu)
                    {
                        board.Click(e.button.x, e.button.y);
                    }
                }
            }
            if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    int newWidth = e.window.data1;
                    int newHeight = e.window.data2;
                    int newSize = std::min(newWidth, newHeight);

                    SQUARE_SIZE = setWindowSizeSquare(window, newSize);

                    TextureManager::Instance().Cleanup();
                    TextureManager::Instance().LoadSVG("../assets/sprites", SQUARE_SIZE, renderer);

                    WINDOW_WIDTH = WINDOW_HEIGHT = SQUARE_SIZE * 8;

                    layout.Resize(WINDOW_WIDTH, WINDOW_HEIGHT);

                    board.Resize(SQUARE_SIZE);
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                    board.InitPieces();
                }
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    inMenu = !inMenu;
                }
            }
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }
        board.Draw(renderer);
        if (inMenu)
        {
            layout.Draw();
        }
        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(logoFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}