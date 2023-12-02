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
    int     acceptCon() const;
    void    closeCon();
    int     _fd_socket; //better be private
    void    setServerConf(const ConfigFile & server);
    const ConfigFile &   getServerConf(void);

private:
    int     _port;
    ConfigFile  _serverConf;
};

#endif
