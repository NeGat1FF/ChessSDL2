#include "Utils/NetworkManager.h"

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

void NetworkManager::Quit()
{
    SDLNet_Quit();
}

bool NetworkManager::ResolveHost(const char *host, Uint16 port, IPaddress *address)
{
    if (SDLNet_ResolveHost(address, host, port) < 0)
    {
        SDL_Log("Failed to resolve host: %s", SDLNet_GetError());
        return false;
    }
    return true;
}

TCPsocket NetworkManager::OpenTCPSocket(IPaddress *address)
{
    TCPsocket socket = SDLNet_TCP_Open(address);
    if (!socket)
    {
        SDL_Log("Failed to open TCP socket: %s", SDLNet_GetError());
        return nullptr;
    }
    return socket;
}

void NetworkManager::CloseTCPSocket(TCPsocket socket)
{
    SDLNet_TCP_Close(socket);
}

bool NetworkManager::SendTCP(TCPsocket socket, const char *data, int length)
{
    if (SDLNet_TCP_Send(socket, data, length) < length)
    {
        SDL_Log("Failed to send TCP data: %s", SDLNet_GetError());
        return false;
    }
    return true;
}

bool NetworkManager::ReceiveTCP(TCPsocket socket, char *data, int length)
{
    int received = SDLNet_TCP_Recv(socket, data, length);
    if (received <= 0)
    {
        SDL_Log("Failed to receive TCP data: %s", SDLNet_GetError());
        return false;
    }
    return true;
}
