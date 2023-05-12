#pragma once


#include <SDL2/SDL_net.h>


class NetworkManager
{
public:
    static NetworkManager &Instance();
    bool Init();

    void Quit();

    bool ResolveHost(const char *host, Uint16 port);
    void OpenTCPSocket();
    void CloseTCPSocket();
    bool SendTCP(const void *data, int length);
    bool AcceptTCP();
    bool ReceiveTCP(void *data, int length);
private:
    IPaddress ip;
    TCPsocket socket;
    TCPsocket client;
};