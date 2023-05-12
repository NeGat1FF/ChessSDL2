#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include "Board/Board.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"
#include "Utils/NetworkManager.h"

#include "GUI/Layout.h"

#include <iostream>
#include <cmath>
#include <thread>

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

void NetworkThread(Board &board)
{
    while(true){
        char data[32];
        NetworkManager::Instance().ReceiveTCP(data, 32);
        std::string from;
        from += data[0];
        from += data[1];

        std::string to;
        to += data[2];
        to += data[3];

        board.MovePiece(from, to);
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    NetworkManager::Instance().Init();

    bool inMenu = true;

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Surface *icon = IMG_Load("assets/icon/icon.ico");
    SDL_SetWindowIcon(window, icon);

    if (!AudioManager::Instance().LoadSound("assets/audio"))
    {
        SDL_Log("Failed to load sound");
        return 1;
    }
    if (!TextureManager::Instance().LoadSVG("assets/sprites", SQUARE_SIZE, renderer))
    {
        SDL_Log("Failed to load sprites");
        return 1;
    }

    TTF_Font *logoFont = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 64);

    Layout layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "ChessGame", {0, 0, 0, 200}, renderer);

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 30);

    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Singleplayer", [&inMenu](){inMenu = false;}, font, renderer));
    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Multiplayer", nullptr, font, renderer));
    layout.AddButton(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), SDL_Color(255, 0, 0, 255), "Quit", SDL_Quit, font, renderer));

    bool isPlayerWhite = false;

    if(argc > 1){
        if(strcmp(argv[1], "host") == 0){
            NetworkManager::Instance().ResolveHost(nullptr, 1234);
            NetworkManager::Instance().OpenTCPSocket();
            NetworkManager::Instance().AcceptTCP();
            char data[32];
            NetworkManager::Instance().ReceiveTCP(data, 32);
            std::cout << data << std::endl;
            NetworkManager::Instance().SendTCP("Server Connected", 32);

            isPlayerWhite = true;
        }
        else if(strcmp(argv[1], "join") == 0){
            NetworkManager::Instance().ResolveHost(argv[2], 1234);
            NetworkManager::Instance().OpenTCPSocket();
            NetworkManager::Instance().SendTCP("Client Connected", 32);
            char data[32];
            NetworkManager::Instance().ReceiveTCP(data, 32);
            std::cout << data << std::endl;
        }

    }

    Board board(isPlayerWhite);
    board.InitPieces();

    std::thread networkThread(NetworkThread, std::ref(board));
    networkThread.detach();

    SDL_Texture* targetTexture;

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
                        std::string move = board.Click(e.button.x, e.button.y);

                        if(move != "NoneNone"){
                            NetworkManager::Instance().SendTCP(move.c_str(), 32);
                        }
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

        if(!isPlayerWhite){
            targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
            SDL_SetRenderTarget(renderer, targetTexture);
        }

        board.Draw(renderer);

        if(!isPlayerWhite){
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopyEx(renderer, targetTexture, NULL, NULL, 180, NULL, SDL_FLIP_NONE);
        }

        if (inMenu)
        {
            layout.Draw();
        }

        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_CloseFont(logoFont);
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}