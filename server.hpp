#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

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
