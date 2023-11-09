#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <cerrno>

# include "./Server.hpp"
# include "../config/ParserServer.hpp"
# include "./Connection.hpp"

class   Webserv {
    public:
        //Webserv();
        Webserv(std::vector<Server> const& newServers);
        ~Webserv();

        std::vector<Server> const&   servers;
        void    setup(ParserServer configFile);
        void    start();

        void    readDataClient(int i);
        void    sendDataClient();

        Connection  conn;
    private:
        bool	ableToWrite(int client);
        bool	ableToRead(int client);

        int     nbrServers;
};

#endif