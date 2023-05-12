#include "Utils/NetworkManager.h"

#include <iostream>

NetworkManager &NetworkManager::Instance()
{
    static NetworkManager instance;
    return instance;
}

bool NetworkManager::Init()
{
    if (SDLNet_Init() < 0)
    {
        SDL_Log("Failed to initialize SDL_net: %s", SDLNet_GetError());
        return false;
    }
    return true;
}

bool NetworkManager::AcceptTCP(){
    while (!client)
    {
        client = SDLNet_TCP_Accept(socket);
        SDL_Delay(50);
    }
    return true;
}

void NetworkManager::Quit()
{
    SDLNet_Quit();
}

bool NetworkManager::ResolveHost(const char *host, Uint16 port)
{
    if (SDLNet_ResolveHost(&ip, host, port) < 0)
    {
        SDL_Log("Failed to resolve host: %s", SDLNet_GetError());
        return false;
    }
    return true;
}

void NetworkManager::OpenTCPSocket()
{
    socket = SDLNet_TCP_Open(&ip);
    if (!socket)
    {
        SDL_Log("Failed to open TCP socket: %s", SDLNet_GetError());
    }
}

void NetworkManager::CloseTCPSocket()
{
    SDLNet_TCP_Close(socket);
}

bool NetworkManager::SendTCP(const void *data, int length)
{
    TCPsocket sock = client ? client : socket;
    if (SDLNet_TCP_Send(sock, data, length) < length)
    {
        SDL_Log("Failed to send TCP data: %s", SDLNet_GetError());
        return false;
    }
    return true;
}

bool NetworkManager::ReceiveTCP(void *data, int length)
{
    TCPsocket sock = client ? client : socket;
    int received = SDLNet_TCP_Recv(sock, data, length);
    if (received <= 0)
    {
        std::cout << "Failed to receive TCP data: " << SDLNet_GetError() << std::endl;
        SDL_Log("Failed to receive TCP data: %s", SDLNet_GetError());
        return false;
    }
    return true;
}
