#include <signal.h>

#include "./core/Webserv.hpp"
//#include "./config/Utils.hpp"
#include "Utils.hpp"

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
            servers[i].setServerConf(configServer.getServers()[i]);
            servers[i].initialize();
        }
/*
        int i = 0;
        size_t j;
        size_t k;
        while (i < nbrServers)
        {
            j = 0;
            while (j < servers[i].getServerConf().getLocation().size())
            {
                k = 0;
                while ( k < servers[i].getServerConf().getLocation()[j].getIndex().size())
                {
                    std::cout << "in server " << i << " Location: " << j << " index para k: " << k << " é: " << servers[i].getServerConf().getLocation()[j].getIndex()[k] << std::endl;
                    k++;
                }
                j++;
            }
//            std::cout << "in server " << (i + 1) << " Port: " << servers[i].getServerConf().getPort() << std::endl;
            i++;
        }
        return (0);
*/
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
