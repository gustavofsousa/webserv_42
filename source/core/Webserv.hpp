#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <poll.h>
# include <vector>
# include <cerrno>

# include "Server.hpp"

class   Webserv {
    public:
        //Webserv();
        Webserv(std::vector<Server> const& newServers);
        ~Webserv();

        std::vector<Server> const&   servers;
        void    setup();
        void    start();

        void    readDataClient(int i);
        void    sendDataClient();

        std::vector<pollfd> poolAllFd;

    private:
        bool	ableToWrite(int client);
        bool	ableToRead(int client);

        void	addNewSocket(int socket_fd);
        void	addServersSockets();
        void    closeConnection(int client);
        int     num_servers;
};

#endif