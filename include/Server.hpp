#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <cstdio> //perror
# include <stdlib.h> //exit()

class Server {
public:
    Server();
    ~Server();
    Server(int port);
    void    initialize();
    int     acceptCon();
    void    closeCon();

private:
    int     _fd_socket;
    int     _port;
};

#endif
