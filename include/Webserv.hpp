#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <poll.h>
# include <vector>
# include <cerrno>

# include "Server.hpp"

class   Webserv {
    public:
        Webserv();
        Webserv(Server const& server);
        ~Webserv();

        Server const&   server;
        void    setup();
        void    start();

        std::vector<pollfd> clientSockets;
};

#endif