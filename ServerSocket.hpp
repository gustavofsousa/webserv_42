#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ServerSocket {
public:
    ServerSocket(int port);
    void Initialize();
    int Accept();
    void Close();

private:
    int server_socket;
    int port;
};

#endif
