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
        executeGET();
        //start timer
    } else if (_request.getMethod() == "POST"){
        //creat fd body;
        if(!writeFD(_request.returnBody()))
            return;
        //creat variavel para fd de response
       // initEnvPOST(_request.getQueryStringS());
        //start timer

    }
}

CGI::~CGI(){}

void CGI::initEnvGET(std::string queryString){

    _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
    _env.push_back(NULL);
}

// void CGI::initEnvPOST(std::string queryString){

//     _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
//     _env.push_back(NULL);
// }

// void CGI::initEnvPOST(std::string queryString){
//     _env.push_back(strdup(("QUERY_STRING=" + queryString).c_str()));
//     _env.push_back(strdup(("CONTENT_TYPE=" + _request.getContentType()).c_str()));
//     _env.push_back(strdup(("CONTENT_LENGTH=" + _request.totalLengthS()).c_string()));
//     _env.push_back(strdup(("PATH_INFO=" + this->_path()).c_str()));
//     //_env.push_back(strdup("HTTP_USER_AGENT=").c_str());
//     //_env.push_back(strdup("SCRIPT_NAME=" + );
// }

void CGI::executeGET(){
    
    int pipefd[2];

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
        readFD(pipefd[0]);
        wait(NULL);
    }
    return;
}

// void CGI::executePOST(void){
//     int requestFD[2];
//     int responseFD[2];

// }

void CGI::readFD(int fd){
    char buffer[BUFFER_SIZE_CGI];
    int bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        // std::cout << "Resposta do filho: ";
        this->_response.append(buffer, bytesRead);
        // std::cout << this->_response << std::endl;
        // std::cout << std::endl;
    } else {
        std::cerr << "Erro na leitura da resposta do filho" << std::endl;
        //fazer uma classe de log
    }
}

std::string CGI::getBody(void) const{
    return this->_response;
}

// void CGI::routineCheck(void){
//     time_t current_time = time(NULL);

//     while(isActive){
//         if(current_time - this->_start_time >= TIME_LIMIT){
//             kill(this->_cgi_pid, SIGKILL);
//         }
//     }
// }

bool CGI::writeFD(std::string body){
    ssize_t bytesWritten = write(_fdOut, body.c_str(), body.length());

    if (bytesWritten == -1){
        std::cerr << "error";
        return false;
    }
    return true;
}
