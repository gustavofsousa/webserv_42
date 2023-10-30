#include "../include/Webserv.hpp"

Webserv::Webserv() {


}

Webserv::~Webserv() {}


void    Webserv::readDataClient() {
    this->server.acceptCon();
    // need to accept client conection
    // save the new fd
}

void    Webserv::sendDataClient() {
    // prepare to send
}

void    Webserv::start() {
    int statusPoll

    this->clientSockets.push_back(this->server._fd_socket);

    while (42) {
        statusPoll = poll(this->clientSockets.data(), this->clientSockets.size(), -1);
        if (statusPoll == -1) {
            std::cerr << "Error in webserv.cpp: " << strerror(errno) << std:endl;
        }


        for (size_t i = 0; i < this->clientSockets.size(); i++)
        {
            if (this->clientSockets[i].revents & POLLIN)
                this->readDataClient();
            else if (this->clientSockets[i].revents & POLLOUT)
                this->sendDataClient();
            else
                std::cer << "error in receive poll" << std::endl;
        }
    }
}

void    Webserv::setup() {
    int i = 0;
    i++;
}