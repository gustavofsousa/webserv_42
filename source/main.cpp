#include <signal.h>

#include "./core/Webserv.hpp"
#include "./config/Utils.hpp"

#define DB "./source/config/configs/default.conf"

static void signalHandlerSigint(int signum)
{
    if (signum != SIGINT) return;
    Utils::_serverRunning = false;
}

int main(int argc, char **argv) {
    ParserServer	configServer;

    signal(SIGINT, signalHandlerSigint);
    try
    {
        if (argc > 2)
                throw Error::InvalidArg();
        if (argc == 1)
            configServer.createServer(DB);
        else
            configServer.createServer(argv[1]);

        int nbrServers = configServer.getNbrServers();
        std::vector<Server> servers;
        for (int i = 0; i < nbrServers; i++) {
            int port = configServer.getServers()[i].getPort();
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
    }
    return 0;
}
