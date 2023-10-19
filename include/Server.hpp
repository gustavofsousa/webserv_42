#ifndef SERVER_SOCKET_HPP
# define SERVER_SOCKET_HPP

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>

class ServerSocket {
public:
    ServerSocket(int port);
    void    initialize();
    int     acceptCon();
    void    close();

private:
    int fd_socket;
    int port;
};

#endif