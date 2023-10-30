#include "../include/Webserv.hpp"

// Webserv::Webserv() {}

Webserv::Webserv(Server const& newServer) : server(newServer) {}

Webserv::~Webserv() {}

// Im not saving new fd in poll_fd.
void    Webserv::readDataClient() {
    int sock_fd_client = this->server.acceptCon();

    char buffer[1024];
    ssize_t bytes_received;

    bytes_received = recv(sock_fd_client, buffer, sizeof(buffer), 0);

    if (bytes_received < 0) {
        std::cerr << "Error reading request from client." << std::endl;
        return;
    }
    if (bytes_received == 0) {
        std::cerr << "Client closed the connection." << std::endl;
        return;
    }
    std::cout << "Received " << bytes_received << " bytes from client." << std::endl;
    std::cout << "Message: " << buffer << std::endl;

    buffer[bytes_received] = '\0';
}

void    Webserv::sendDataClient() {
    std::cout << "I'm sending data back" << std::endl;
}

void    Webserv::start() {
    //int statusPoll;

    pollfd pfd;
    pfd.fd = this->server._fd_socket;
    pfd.events = POLLIN | POLLOUT;
    this->clientSockets.push_back(pfd);

    while (42) {
        std::cout << "Ready to poll" << std::endl;
        // statusPoll = poll(this->clientSockets.data(), this->clientSockets.size(), -1);
        // if (statusPoll == -1) {
        //     std::cerr << "Error in webserv.cpp: " << strerror(errno) << std:endl;
        // }


        // for (size_t i = 0; i < this->clientSockets.size(); i++)
        // {
        //     if (this->clientSockets[i].revents & POLLIN)
        //         this->readDataClient();
        //     else if (this->clientSockets[i].revents & POLLOUT)
        //         this->sendDataClient();
        //     else
        //         std::cer << "error in receive poll" << std::endl;
        // }
    }
}

void    Webserv::setup() {
    std::cout << "Doing the setup with config" << std::endl;
}


// Transform in auxiliar functions for better redability (do it in if's)