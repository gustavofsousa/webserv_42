#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <map>

# include "Request.hpp"
# include "Response.hpp"
# include "CGI.hpp"

class Client
{
    public:
        Client(const Request &request, Response & response);
        ~Client();

        void        choiceMethod();
        void        handleHTTPMethod(void);
        void        postMethod(void);
        void        deleteMethod(void);
        int         getStatusCode(void);
        std::string readFile(std::string name); 

    private:
        Client(const Client &client);
        Client &operator=(const Client &Client);
        Request const&      _request;
        Response &          _response;
        int                 _statusCode;
        bool                _isCGI;
        std::string	        fileRequested(void);
        void                buildDeleteFile(const std::string & path, const std::string & idValue);
};

#endif