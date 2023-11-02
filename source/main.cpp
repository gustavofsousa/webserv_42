#include "./core/Webserv.hpp"

int main() {
    int port = 8080;

    Server server(port);
    server.initialize();

    Webserv webserv(server);
    webserv.setup();
    webserv.start();
    server.closeCon();

    return 0;
}
