#include <SDL.h>
#include <SDL_net.h>
#include <SDL_mixer.h>

#include "Utils/SDLBoard.h"
#include "Utils/AIManager.h"
#include "Utils/AudioManager.h"
#include "Utils/TextureManager.h"
#include "Utils/NetworkManager.h"

#include "GUI/Layout.h"

int main(int argc, char *argv[])
{
    int SQUARE_SIZE = 64;

    int WINDOW_WIDTH = SQUARE_SIZE * 8;
    int WINDOW_HEIGHT = SQUARE_SIZE * 8;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("SDL2 Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    NetworkManager::Instance().Init();

    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 24);

    Layout *currentLayout = nullptr;

    bool running = true;
    bool isMultiplayer = false;

    SDLBoard sdlBoard(SDL_Color{238, 238, 210, 255}, SDL_Color{118, 150, 86, 255}, SDL_Color{255, 0, 0, 255}, 64);

    Layout mainLayout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, font, "Chess", SDL_Color{0, 0, 0, 200}, renderer);
    Layout multiplayerLayout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, font, "Multiplayer", SDL_Color{0, 0, 0, 200}, renderer);
    Layout joinLayout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, font, "Join", SDL_Color{0, 0, 0, 200}, renderer);
    Layout hostLayout(WINDOW_WIDTH, WINDOW_HEIGHT, 32, 24, font, "Host", SDL_Color{0, 0, 0, 200}, renderer);

    mainLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Singleplayer", [&currentLayout, &sdlBoard, &renderer]
                                                   { currentLayout = nullptr; sdlBoard.Draw(renderer); }, font, renderer));
    mainLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Multiplayer", [&currentLayout, &multiplayerLayout]
                                                   { currentLayout = &multiplayerLayout; }, font, renderer));
    mainLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Quit", [&running]
                                                   { running = false; }, font, renderer));

    multiplayerLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &joinLayout]
                                                          { currentLayout = &joinLayout; }, font, renderer));
    multiplayerLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Host", [&currentLayout, &hostLayout]
                                                          { currentLayout = &hostLayout; }, font, renderer));
    multiplayerLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Back", [&currentLayout, &mainLayout]
                                                          { currentLayout = &mainLayout; }, font, renderer));

    joinLayout.AddElement(std::make_shared<Input>(SDL_Color(32, 32, 32, 255), "", font, renderer));
    joinLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Join", [&currentLayout, &isMultiplayer, &sdlBoard]
                                                   { std::string ip = currentLayout->GetText(); NetworkManager::Instance().ResolveHost(ip.c_str(), 12345); NetworkManager::Instance().OpenTCPSocket(); currentLayout = nullptr; isMultiplayer = true; sdlBoard.GetBoard().SetPlayerColor(Color::Black);}, font, renderer));
    joinLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Back", [&currentLayout, &multiplayerLayout]
                                                   { currentLayout = &multiplayerLayout; }, font, renderer));

    hostLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Host", [&currentLayout, &isMultiplayer]
                                                   { currentLayout = nullptr; NetworkManager::Instance().ResolveHost(nullptr, 12345); NetworkManager::Instance().OpenTCPSocket(); isMultiplayer = true; }, font, renderer));
    hostLayout.AddElement(std::make_shared<Button>(SDL_Color(32, 32, 32, 255), "Back", [&currentLayout, &multiplayerLayout]
                                                   { currentLayout = &multiplayerLayout; }, font, renderer));

    currentLayout = &mainLayout;

    while (running)
    {
        SDL_Event event;
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (currentLayout)
            {
                currentLayout->ProcessInput(event);
            }
            else
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                {
                    auto move = sdlBoard.Click(event.button.x, event.button.y);
                    if (move.has_value() && isMultiplayer)
                    {
                        std::string moveString = move.value().first.ToString() + move.value().second.ToString();
                        NetworkManager::Instance().SendTCP(moveString.c_str(), 5);
                    }
                    break;
                }
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        int WINDOW_WIDTH = WINDOW_HEIGHT = std::min(event.window.data1, event.window.data2) / 8 * 8;
                        int SQUARE_SIZE = WINDOW_WIDTH / 8;
                        TextureManager::Instance().Cleanup();
                        TextureManager::Instance().LoadSVG("assets/sprites", SQUARE_SIZE, renderer);
                        sdlBoard.Resize(SQUARE_SIZE);
                        SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_r)
                    {
                        sdlBoard.LoadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        sdlBoard.Draw(renderer);

        if (currentLayout)
        {
            currentLayout->Draw();
        }

        SDL_RenderPresent(renderer);

        if (!sdlBoard.IsPlayerTurn() && isMultiplayer)
        {
            char buffer[5];
            NetworkManager::Instance().ReceiveTCP(buffer, 5);
            std::string from;
            from.push_back(buffer[0]);
            from.push_back(buffer[1]);

            std::string to;
            to.push_back(buffer[2]);
            to.push_back(buffer[3]);

            sdlBoard.GetBoard().MakeMove(from, to);
        }
    }

    SDL_Quit();

    return 0;
}