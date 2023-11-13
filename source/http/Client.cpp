#include "Client.hpp"

Client::Client(const Request &request) :
    _request(request)
{

}

Client::~Client()
{
}

std::string Client::getMethod(void){
    std::string pagePath("./index.html");

    //I need to receive information from the header
    std::ifstream file(pagePath.c_str());
    if (!file) {
        _response = "HTTP/1.1 404 Not Found\r\n\r\n";
        return "";
    }

    std::ostringstream page;
    page << file.rdbuf();
    file.close();

    std::ostringstream resp;
    resp << "HTTP/1.1 200 OK\n";
    resp << "Content-Type: text/html; charset=UTF-8\n";
    resp << "Content-Length: "  << page.str().length()  << " \n";
    resp << page.str();
    
    _response = resp.str();
    _statusCode = 200;
    return (_response);
}