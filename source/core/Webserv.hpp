#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <cerrno>
# include <cstring> //c_string()

# include "./Server.hpp"
# include "./Connection.hpp"
# include "../config/ParserServer.hpp"
# include "../http/Client.hpp"
# include "../http/Request.hpp"
# include "../config/Utils.hpp"

extern bool serverRunning;

class   Webserv {
    public:
        //Webserv(); //VOLTAR PRA VER. TEM QUE SER CANONICO?
        Webserv(std::vector<Server> const& newServers, ParserServer const& configFile);
        ~Webserv();

        Connection                      conn;
        std::vector<Server> const&      servers;
        void    start();
        void    readDataClient(int i);
        void    sendDataClient(int i);
    private:
        std::vector<Request>    _requests;
        int                     _nbrServers;

        bool	ableToWrite(int client);
        bool	ableToRead(int client);
        bool	pollError(int i);
        bool    isRequestFromServer(int i);

        int     updateStatusPoll();

};

#endif
