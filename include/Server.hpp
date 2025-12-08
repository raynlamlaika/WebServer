#ifndef SERVER_HPP
# define SERVER_HPP

#include <poll.h>
#include <map>
#include "ConfigFile.hpp"
#include "Request.hpp"

#define READINGBUFFER 1000

class Server : Request
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
