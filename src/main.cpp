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

    Board *board = new Board(renderer);

    std::thread *networkThread = nullptr;

    Layout *currentLayout = nullptr;

    TTF_Font *logoFont = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 64);

    Layout *mainLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "ChessGame", {0, 0, 0, 200}, renderer);
    Layout *multiplayerLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "Multiplayer", {0, 0, 0, 200}, renderer);
    Layout *joinLayout = new Layout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, logoFont, "Join", {0, 0, 0, 200}, renderer);

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 30);

    mainLayout->AddElement(std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Singleplayer", [&currentLayout]
        { currentLayout = nullptr; },
        font, renderer));
    mainLayout->AddElement(std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Multiplayer", [&currentLayout, &multiplayerLayout]
        { currentLayout = multiplayerLayout; },
        font, renderer));
    mainLayout->AddElement(std::make_unique<Button>(SDL_Color(32, 32, 32, 255), "Quit", [](){shouldRun = false;}, font, renderer));

    multiplayerLayout->AddElement(std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Host", [&currentLayout, &networkThread, &board]
        {currentLayout = nullptr; createNetworkThread(networkThread, board, nullptr); },
        font, renderer));
    multiplayerLayout->AddElement(std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &joinLayout]
        { currentLayout = joinLayout; },
        font, renderer));

    std::shared_ptr<Button> backButton = std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Back", [&currentLayout, &mainLayout]
        { currentLayout = mainLayout; },
        font, renderer);
    multiplayerLayout->AddElement(backButton);

    joinLayout->AddElement(std::make_unique<Input>(SDL_Color(32, 32, 32, 255), "", font, renderer));
    joinLayout->AddElement(std::make_unique<Button>(
        SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &board, &networkThread, &renderer]
        {std::string ip = currentLayout->GetText(); currentLayout = nullptr; board = new Board(renderer, Color::Black); createNetworkThread(networkThread, board, ip.c_str()); },
        font, renderer));
    joinLayout->AddElement(backButton);

    currentLayout = mainLayout;

    while (shouldRun)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (currentLayout != nullptr)
            {
                currentLayout->ProcessInput(e);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    if (!currentLayout)
                    {
                        std::string move = board->Click(e.button.x, e.button.y);

                        if (move != "NoneNone")
                        {
                            if (isMultiplayer)
                            {
                                NetworkManager::Instance().SendTCP(move.c_str(), 32);
                            }
                            else
                            {
                                // TODO: AI move
                            }
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
                    if (currentLayout)
                    {
                        currentLayout = nullptr;
                    }
                    else
                    {
                        currentLayout = mainLayout;
                    }
                }
            }
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }

        board->Draw();
        if (currentLayout)
        {
            currentLayout->Draw();
        }

        SDL_RenderPresent(renderer);
    }

    NetworkManager::Instance().Quit();

    delete mainLayout;
    delete multiplayerLayout;
    delete joinLayout;
    delete board;
    delete networkThread;
    TTF_CloseFont(font);
    TTF_CloseFont(logoFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}