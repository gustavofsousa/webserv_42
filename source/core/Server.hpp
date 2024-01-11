#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <cstdio> //perror
# include <stdlib.h> //exit()
//# include <fcntl.h> //fcntl()
#include "../config/ConfigFile.hpp"

class Server {
public:
    Server();
    ~Server();
    Server(int port);

    void    initialize();
    void    closeCon();
    int     _fd_socket; //better be private
    void    setServerConf(const ConfigFile & server);
    const ConfigFile &   getServerConf(void);
    struct clientInfo       acceptCon() const;

private:
    int     _port;
    ConfigFile  _serverConf;
};

struct clientInfo {
    int                 socket;
    int                 port;
    uint32_t            ip;
    socklen_t           addressLen; 
    std::string         ipString;
    struct sockaddr_in  address;
};

#endif
