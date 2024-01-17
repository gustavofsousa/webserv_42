#pragma once

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string.h>
#include <ctime>
#include <fcntl.h>

#include "Client.hpp"
#include "Request.hpp"

#define BUFFER_SIZE_CGI 64 * 1024
#define TIME_LIMIT 2

class CGI
{
    private:
        std::string         _path; 
        std::vector<char *> _env;
        Request const&      _request;
        std::string         _response;
        pid_t               _cgi_pid;
        int                 _requestFD[2];
        bool                _isActive;
        void                configurePipesAsNonBlocking(int* pip);
        void                initEnvGET(std::string queryString);
        void                initEnvPOST(std::string queryString);
        void                routineCheck(int bytes, int pipefd);


    public:
        CGI(std::string path, Request const & request);
        ~CGI();
    
        bool        executeCGI(void);
        int         executeGET(void);
        int         executePOST(void);

        int         readFD(int fd);
        bool        writeFD(std::string body);

        std::string getBody(void) const;

        

};