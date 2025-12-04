#ifndef SERVER_HPP
# define SERVER_HPP

#include "../ConfigParce/ConfigFile.hpp"

class Server
{
private:
    int i;

public:
    Server();
    ~Server();


    int createServerSocket(ServerConfig& cfg);// add const later
    void RunServer(int server_fd);
};


#endif