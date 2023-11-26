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

class   Webserv {
    public:
        //Webserv(); //VOLTAR PRA VER. TEM QUE SER CANONICO?
        Webserv(std::vector<Server> const& newServers);
        ~Webserv();

        std::vector<Server> const&   servers;
        void    setup(ParserServer configFile);
        void    start();

        void    readDataClient(int i);
        void    sendDataClient(int i);

        Connection  conn;
    private:

        int                     _nbrServers;
        Response                _response;
        std::vector<Request>    _requests;

        bool	ableToWrite(int client);
        bool	ableToRead(int client);
        bool	pollError(int i);
        bool    isRequestFromServer(int i);

        int     updateStatusPoll();

};

#endif
