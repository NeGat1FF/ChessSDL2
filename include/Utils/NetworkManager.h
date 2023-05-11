#pragma once

#include <SDL2/SDL_net.h>

class NetworkManager
{
public:
    static NetworkManager &Instance();
    bool Init();
    void Quit();
    bool ResolveHost(const char *host, Uint16 port, IPaddress *address);
    TCPsocket OpenTCPSocket(IPaddress *address);
    void CloseTCPSocket(TCPsocket socket);
    bool SendTCP(TCPsocket socket, const char *data, int length);
    bool ReceiveTCP(TCPsocket socket, char *data, int length);
};
