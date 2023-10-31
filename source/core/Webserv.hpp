#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <poll.h>
# include <vector>
# include <cerrno>

# include "Server.hpp"

class   Webserv {
    public:
        //Webserv();
        Webserv(Server const& server);
        ~Webserv();

        Server const&   server;
        void    setup();
        void    start();

        void    readDataClient();
        void    sendDataClient();

        std::vector<pollfd> clientSockets;

    private:
        bool	is_available_to_write(int client);
        bool	is_available_to_read(int client);
};

#endif