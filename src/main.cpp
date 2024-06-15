#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_net.h>

#include "Board/Board.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"
#include "Utils/NetworkManager.h"
#include "Utils/AIManager.h"

#include "GUI/Layout.h"

#include <iostream>
#include <cmath>
#include <thread>

int SQUARE_SIZE = 64;

int WINDOW_WIDTH = SQUARE_SIZE * 8;
int WINDOW_HEIGHT = SQUARE_SIZE * 8;

bool isMultiplayer = false;
bool shouldRun = true;

Uint16 port = 5353;

int setWindowSizeSquare(SDL_Window *window, int newSize)
{
    if (newSize % 8 != 0)
    {
        newSize = (newSize / 8) * 8;
    }
    SDL_SetWindowSize(window, newSize, newSize);

    return newSize / 8;
}

void hostNetwork()
{
    NetworkManager::Instance().ResolveHost(nullptr, port);
    NetworkManager::Instance().OpenTCPSocket();
    NetworkManager::Instance().AcceptTCP();
    char data[32];
    NetworkManager::Instance().ReceiveTCP(data, 32);
    std::cout << data << std::endl;
    NetworkManager::Instance().SendTCP("Server Connected", 32);
}

void joinNetwork(const char *host)
{
    NetworkManager::Instance().ResolveHost(host, port);
    NetworkManager::Instance().OpenTCPSocket();
    NetworkManager::Instance().SendTCP("Client Connected", 32);
    char data[32];
    NetworkManager::Instance().ReceiveTCP(data, 32);
    std::cout << data << std::endl;
}

void NetworkThread(Board *&board, const char *host)
{
    if(!NetworkManager::Instance().Init()){
        SDL_Log("Failed to init network");
    }
    if (host != nullptr)
    {
        joinNetwork(host);
    }
    else
    {
        hostNetwork();
    }
    while (true)
    {
        char data[32];
        NetworkManager::Instance().ReceiveTCP(data, 32);
        std::string from;
        from += data[0];
        from += data[1];

        std::string to;
        to += data[2];
        to += data[3];

        board->MovePiece(from, to);
    }
}

void createNetworkThread(std::thread *thread, Board *&board, const char *host)
{
    thread = new std::thread(NetworkThread, std::ref(board), host);
    thread->detach();

    isMultiplayer = true;
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    
    SDL_Window *window = SDL_CreateWindow("SDL2 Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
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

    return 0;
}