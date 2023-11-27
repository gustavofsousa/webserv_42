#include "./core/Webserv.hpp"

#define DB "./source/config/configs/default.conf"

int main(int argc, char **argv) {
    ParserServer	configServer;

    try
    {
        if (argc > 2)
                throw Error::InvalidArg();
        if (argc == 1)
            configServer.createServer(DB);
        else
            configServer.createServer(argv[1]);

        int nbrServers = configServer.getNbrServers();
        // int nbrServers = 1;
        std::vector<Server> servers;
        for (int i = 0; i < nbrServers; i++) {
            int port = configServer.getServers()[i].getPort();
            // int port = 8080;
            std::cout << "Initializing server number " << i << " on port " << port << std::endl;
            servers.push_back(Server(port));
            servers[i].initialize();
        }

        Webserv webserv(servers, configServer);
        webserv.start();
        for (int i = 0; i < nbrServers; i++) {
            servers[i].closeCon();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
//        std::cerr << e.what() << '\n'; alguma justificativa para usar '\n' ao invés de std::endl?
    }
    return 0;
}
