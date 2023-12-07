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
# include "../Utils.hpp"
//# include "../config/Utils.hpp"

class   Webserv {
    public:
        Webserv(std::vector<Server> const& newServers, ParserServer const& configFile);
        ~Webserv();

        Connection                      conn;
        std::vector<Server>             servers;
//        std::vector<Server> const&      servers;
        void    start();

    private:
        std::vector<Request>    _requests;
        int                     _nbrServers;

        bool	ableToWrite(int client);
        bool	ableToRead(int client);
        bool    readDataClient(int i, int indReq);
        bool    sendDataClient(int i, int indReq);
        bool	pollError(int i);
        bool    isRequestFromServer(int i);
        int     updateStatusPoll();
        bool	openNewConnection(int i);
        bool	deleteConnection(int i, int indReq);
};

#endif