#include "CGI.hpp"

CGI::CGI(std::string path, Request const & request): 
    _path(path),
    _request(request)
{
}

CGI::~CGI(){}

void CGI::initEnvGET(std::string queryString){

    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(NULL);
}

void CGI::initEnvPOST(std::string queryString){
    
    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(strdup(("CONTENT_TYPE=" + _request.getContentType()).c_str()));
    _env.push_back(strdup(("CONTENT_LENGTH=" + _request.totalLengthS()).c_str()));
    _env.push_back(strdup(("PATH_INFO=" + this->_path).c_str()));
    _env.push_back(strdup("AUTH_TYPE=Basic"));
    _env.push_back(strdup("REQUEST_METHOD=POST"));
    _env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
    _env.push_back(strdup("SERVER_SOFTWARE=Webserv/1.0"));
    _env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
    _env.push_back(strdup("REDIRECT_STATUS=200"));
    _env.push_back(strdup("DOCUMENT_ROOT=./"));
    _env.push_back(strdup("TRANSLATED_PATH_INFO=.//"));
    _env.push_back(strdup(("SERVER_PORT=" + _request.returnPort()).c_str()));
    _env.push_back(strdup(("PATH_TRANSLATED=" + _path).c_str()));
    _env.push_back(strdup(("SCRIPT_NAME=" + _path).c_str()));
    _env.push_back(strdup(("REQUEST_URI=" + _request.getHost()).c_str()));
    _env.push_back(NULL);
}


bool CGI::executeCGI(){
    if (_request.getMethod() == "GET")
    {
        initEnvGET(_request.getQueryStringS());
        int result = executeGET();
        if (result == 1)
            return true;
        else if (result == -1)
            return false;
    } else if (_request.getMethod() == "POST"){
        initEnvPOST(_request.getQueryStringS());
        executePOST();
        //alterar para verificar se o filho esta ativo
        return true;
    }
    return false;
}

int CGI::executeGET(){
    
    int pipefd[2];

    if(pipe(pipefd) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return 0;
    }

    fcntl(pipefd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(pipefd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    
    this->_cgi_pid = fork();
    this->_isActive = true;
    if (this->_cgi_pid == -1){
        std::cerr << "Error no fork" << std::endl;
        return 0;
    }
    else if (this->_cgi_pid == 0){
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        this->_isActive = true;

        char* args[2];
        args[0] = strdup(_path.c_str());
        args[1] = NULL;

        execve(_path.c_str(), args, _env.data());

        free(args[0]);
        std::cerr << "Error ao executar execve" << std::endl;
        return 0;
    } else {
        close(pipefd[1]);
       int bytes = readFD(pipefd[0]);
       if (bytes <= 0){
           routineCheck(bytes, pipefd[0]);
           return -1;
       }
       return 1;
    }
    return 0;
}

int CGI::executePOST(void){
    int responseFD[2];

    if(pipe(_requestFD) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return 0;
    }

    if(pipe(responseFD) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return 0;
    }

    configurePipesAsNonBlocking(responseFD);

    if(!writeFD(_request.returnBody()))
            return 0;

    this->_cgi_pid = fork();

    this->_isActive = true;

    if (this->_cgi_pid == -1){
        std::cerr << "Error no fork" << std::endl;
        return 0;
    }
    else if (this->_cgi_pid == 0){
        close(_requestFD[1]);
        close(responseFD[0]);

        dup2(_requestFD[0], STDIN_FILENO);
        

        close(_requestFD[0]);

        dup2(responseFD[1], STDOUT_FILENO);
        close(responseFD[1]);

        char* args[2];
        args[0] = strdup(_path.c_str());
        args[1] = NULL;

        execve(_path.c_str(), args, _env.data());

        free(args[0]);
        std::cerr << "Error ao executar execve" << std::endl;
        return 0;
    } else {
        close(_requestFD[0]);
        close(responseFD[1]);
        int bytes =   readFD(responseFD[0]);
        if (bytes <= 0){
           routineCheck(bytes, responseFD[0]);
           return -1;
        }
       return 1;
    }
    return 0;
}

int CGI::readFD(int fd){
    char buffer[BUFFER_SIZE_CGI];
    int bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        this->_response.append(buffer, bytesRead);
        return bytesRead;
    } 
    return 0;
}

std::string CGI::getBody(void) const{
    return this->_response;
}

void CGI::routineCheck(int bytes, int pipefd){
    time_t now = time(NULL);

    while (bytes <= 0 || !this->_isActive){
        bytes = readFD(pipefd);
      //  std::cout << "Time limit: " << difftime(now, this->_request.getStartTime()) << std::endl;
        if (difftime(now, this->_request.getStartTime()) >= TIME_LIMIT){
            kill(this->_cgi_pid, SIGKILL);
            this->_isActive = false;
            return;
        }
        now = time(NULL);
    }
    return;
}

bool CGI::writeFD(std::string body){
    size_t bytesWritten = 0;

    while (bytesWritten < body.length()){
        int bytes = write(this->_requestFD[1], body.c_str() + bytesWritten, body.length() - bytesWritten);
        if (bytes == -1){
            std::cerr << "error";
            return false;
        }
        bytesWritten += bytes;
    }
    return true;

}


void CGI::configurePipesAsNonBlocking(int* pip) {
	fcntl(pip[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(pip[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->_requestFD[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(this->_requestFD[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
}
