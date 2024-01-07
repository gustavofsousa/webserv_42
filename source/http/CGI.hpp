#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>

#include "Client.hpp"

class CGI
{
    private:
        std::string         _path; 
        int                 _pid;
        std::vector<char *> _env;
        Request const&      _request;
        std::string         _response;

    public:
        CGI(std::string path, Request const & request);
        ~CGI();
    
        void execute(void);
        void initEnvGET(std::string query);
        void initEnvPOST(void);
        void routineCheck(void);

        std::string getBody(void) const;
        

};