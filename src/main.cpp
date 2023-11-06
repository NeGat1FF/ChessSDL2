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

void NetworkThread(Board* &board)
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

        board->MovePiece(from, to);
    }
}

void hostNetwork() {
    NetworkManager::Instance().ResolveHost(nullptr, 1234);
    NetworkManager::Instance().OpenTCPSocket();
    NetworkManager::Instance().AcceptTCP();
    char data[32];
    NetworkManager::Instance().ReceiveTCP(data, 32);
    std::cout << data << std::endl;
    NetworkManager::Instance().SendTCP("Server Connected", 32);
}

void joinNetwork(const char* host) {
    NetworkManager::Instance().ResolveHost(host, 1234);
    NetworkManager::Instance().OpenTCPSocket();
    NetworkManager::Instance().SendTCP("Client Connected", 32);
    char data[32];
    NetworkManager::Instance().ReceiveTCP(data, 32);
    std::cout << data << std::endl;
}

void createNetworkThread(std::thread* thread, Board* &board){
    thread = new std::thread(NetworkThread, std::ref(board));
    thread->detach();
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    NetworkManager::Instance().Init();

    bool isPlayerWhite = true;

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

    Board* board = new Board;

    std::thread* networkThread = nullptr;

    Layout* currentLayout = nullptr;

    TTF_Font *logoFont = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 64);

    Layout* mainLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "ChessGame", {0, 0, 0, 200}, renderer);
    Layout* multiplayerLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "Multiplayer", {0, 0, 0, 200}, renderer);
    Layout* joinLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "Join", {0, 0, 0, 200}, renderer);

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 30);

    mainLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Singleplayer", [&currentLayout]{currentLayout = nullptr;}, font, renderer));
    mainLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Multiplayer", [&currentLayout, &multiplayerLayout]{currentLayout = multiplayerLayout;}, font, renderer));
    mainLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Quit", SDL_Quit, font, renderer));

    multiplayerLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Host", [&currentLayout, &networkThread, &board]{currentLayout = nullptr; hostNetwork(); createNetworkThread(networkThread, board);}, font, renderer));
    multiplayerLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &joinLayout]{currentLayout = joinLayout;}, font, renderer));

    joinLayout->AddElement(std::make_unique<Input>(SDL_Color(32, 32, 32, 255), "", font, renderer));
    joinLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &board, &isPlayerWhite, &networkThread]{isPlayerWhite = false; std::string ip = currentLayout->GetText(); currentLayout = nullptr; board = new Board(Color::Black); joinNetwork(ip.c_str()); createNetworkThread(networkThread, board);}, font, renderer));

    currentLayout = mainLayout;

    SDL_Texture* targetTexture;

    while (true)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if(currentLayout != nullptr){
                currentLayout->ProcessInput(e);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (!currentLayout)
                    {
                        std::string move = board->Click(e.button.x, e.button.y);

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

                    mainLayout->Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
                    multiplayerLayout->Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
                    joinLayout->Resize(WINDOW_WIDTH, WINDOW_HEIGHT);

                    board->Resize(SQUARE_SIZE);
                }
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_r)
                {
                }
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    if(currentLayout){
                        currentLayout = nullptr;
                    }
                    else{
                        currentLayout = mainLayout;
                    }
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

        board->Draw(renderer);

        if(!isPlayerWhite){
            SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopyEx(renderer, targetTexture, NULL, NULL, 180, NULL, SDL_FLIP_NONE);
        }

        if(currentLayout != nullptr){
            currentLayout->Draw();
        }

        SDL_RenderPresent(renderer);
    }
    delete mainLayout;
    delete multiplayerLayout;
    delete joinLayout;
    TTF_CloseFont(font);
    TTF_CloseFont(logoFont);
    if(!isPlayerWhite){
        SDL_DestroyTexture(targetTexture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}