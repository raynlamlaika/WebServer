#ifndef SERVER_HPP
# define SERVER_HPP

#include <poll.h>
#include <map>
#include "../ConfigParce/ConfigFile.hpp"


#define READINGBUFFER 1000

class Server
{
private:
    int i;

public:
    Server();
    ~Server();


    int ServersPortsLoop(std::vector<ServerConfig>& cfg);// add const later
    int createServerSocket(ServerConfig& cfg);// add const later
    void RunServer(int server_fd);
};


#endif