#include "./Response.hpp"
#include <fstream>
#include <iostream>

Response::Response() {}

Response::~Response() {}

std::string Response::toString(int number) {
    std::stringstream ss;
    ss << number;
    std::string str = ss.str();
    return str;
}

void Response::setBody(std::string body) {
    this->_body = body;
}

void Response::createHTTPHeader(int statusCode, std::string contentType, int contentLength) {
    std::string header;

    this->_statusCode = statusCode;
    header.append("HTTP/1.1 ");
    header.append(toString(statusCode));
    header.append(" ");
    header.append("OK");
    header.append("\r\n");
    header.append("Content-Type: ");
    header.append(contentType);
    header.append("\r\n");
    header.append("Content-Length: ");
    header.append(toString(contentLength));
    header.append("\r\n");
    header.append("\r\n");

    this->_header = header;
}

void Response::send() {
   this->httpMessage = this->_header;
   this->httpMessage.append(this->_body);
}

void Response::processFileForHTTPResponse(std::string &file, int statusCode)
{
    std::ifstream fileStream(file.c_str());
    
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    std::string content = buffer.str();

    this->_body = content;
    this->createHTTPHeader(statusCode, "text/html", this->_body.size());
}

//create header and body for timeout error
void Response::sendErrorTimeOut()
{

    std::string body = "<html><body><h1>408 Request Timeout</h1></body></html>";
    this->setBody(body);
    this->createHTTPHeader(408, "text/html",  body.size());
    this->send();
}