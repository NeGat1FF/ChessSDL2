#pragma once


#include <SDL2/SDL_net.h>


class NetworkManager
{
public:
    static NetworkManager &Instance()
    {
        static NetworkManager instance;
        return instance;
    }

    bool Init()
    {
        if (SDLNet_Init() < 0)
        {
            SDL_Log("Failed to initialize SDL_net: %s", SDLNet_GetError());
            return false;
        }
        return true;
    }

    void Quit()
    {
        SDLNet_Quit();
    }

    bool ResolveHost(const char *host, Uint16 port, IPaddress *address)
    {
        if (SDLNet_ResolveHost(address, host, port) < 0)
        {
            SDL_Log("Failed to resolve host: %s", SDLNet_GetError());
            return false;
        }
        return true;
    }

    TCPsocket OpenTCPSocket(IPaddress *address)
    {
        TCPsocket socket = SDLNet_TCP_Open(address);
        if (!socket)
        {
            SDL_Log("Failed to open TCP socket: %s", SDLNet_GetError());
            return nullptr;
        }
        return socket;
    }

    void CloseTCPSocket(TCPsocket socket)
    {
        SDLNet_TCP_Close(socket);
    }

    bool SendTCP(TCPsocket socket, const char *data, int length)
    {
        if (SDLNet_TCP_Send(socket, data, length) < length)
        {
            SDL_Log("Failed to send TCP data: %s", SDLNet_GetError());
            return false;
        }
        return true;
    }

    bool ReceiveTCP(TCPsocket socket, char *data, int length)
    {
        int received = SDLNet_TCP_Recv(socket, data, length);
        if (received <= 0)
        {
            SDL_Log("Failed to receive TCP data: %s", SDLNet_GetError());
            return false;
        }
        return true;
    }
};