#include "CGI.hpp"

CGI::CGI(std::string path, Request const & request): 
    _path(path),
    _request(request)
{
    if (_request.getMethod() == "GET")
    {
        std::cout << "Path: " << _path << std::endl;
        std::cout << "Request: " << _request.getQueryStringS() << std::endl;

        initEnvGET(_request.getQueryStringS());
        //read fd
        execute();
        //start timer
    } else if (_request.getMethod() == "POST"){
        //creat fd body;
        //creat variavel para fd de response
        //start timer
    }
}

CGI::~CGI(){ }

void CGI::initEnvGET(std::string query) {

    _env.push_back(strdup(("QUERY_STRING=" + query).c_str()));
    _env.push_back(NULL);
}

void CGI::execute() {
    
    int pipefd[2];
    // int requestFD[2];
    // int responseFD[2];

    if(pipe(pipefd) == -1){
        std::cerr << "Erro ao criar o pipe" << std::endl;
        return ;
    }

    pid_t pid = fork();

    if (pid == -1){
        std::cerr << "Error no fork" << std::endl;
        return ;
    }
    else if (pid == 0){
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        char* args[2];
        args[0] = strdup(_path.c_str());
        args[1] = NULL;

        execve(_path.c_str(), args, _env.data());

        free(args[0]);
        std::cerr << "Error ao executar execve" << std::endl;
        return;
    } else {
        close(pipefd[1]);

        char buffer[1024];
        int bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        if (bytesRead > 0) {
            std::cout << "Resposta do filho: ";
            this->_response.append(buffer, bytesRead);
            std::cout << this->_response << std::endl << std::endl;
        } else {
            std::cerr << "Erro na leitura da resposta do filho" << std::endl;
        }
        wait(NULL);
    }
    return;
}

std::string CGI::getBody(void) const{
    return this->_response;
}