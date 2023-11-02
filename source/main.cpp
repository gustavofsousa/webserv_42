#include "./core/Webserv.hpp"

int main() {
    // ConfigFile config_file;
    int port = 8080;
    int num_servers = 1;

    std::vector<Server> servers;
    for (int i = 0; i < num_servers; i++) {
        servers.push_back(Server(port + i));
        servers[i].initialize();
    }

    Webserv webserv(servers);
    webserv.setup();
    webserv.start();
    for (int i = 0; i < num_servers; i++) {
        servers[i].closeCon();
    }
    return 0;
}
