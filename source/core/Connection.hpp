#ifndef CONNECTION_HPP
# define CONNECTION_HPP

//# include "Webserv.hpp"
# include "./Server.hpp"
# include <poll.h>
# include <vector>

class Connection
{
public:
    Connection();
    ~Connection();

    void    addNewSocket(int socket_fd);
    void	addServersSockets(std::vector<Server> const& servers);
    void    closeConnection(int client);

    std::vector<pollfd> const&  getPollFd();
    pollfd const&               getFd(int i);

private:
    std::vector<pollfd> poolAllFd;
};

#endif