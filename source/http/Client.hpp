#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Request.hpp"

class Client
{
    public:
        Client(const Request &request);
        ~Client();

        void        choiceMethod();
        std::string getMethod(void);
        void        postMethod(void);
        void        deleteMethod(void);

    private:
        Client(const Client &client);
        Client &operator=(const Client &Client);
        const Request&  _request;
        std::string     _response;
        int             _statusCode;

};

#endif