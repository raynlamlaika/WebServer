#include "../include/Request.hpp"

Request::Request() : _METHOD(""), _PATH(""), _OLDPATH(""), _CONTENT("")//, _SIZE(0) 
{}


void Request::MainRequest(const std::string &line, int fd, const ServerConfig &servers)
{
    GetParam(line, fd, servers);
    std::cout << "-> Method " << _METHOD
              << " [\033[32mhttp://localhost:" 
              << servers.listen[0].substr(0, servers.listen[0].length() - 1)
              << _OLDPATH << "]\033[0m" << std::endl;
    std::cerr << "["<< servers.root << "]" << std::endl;


    // if (_METHOD == "GET")
    //     GetRequest(fd, servers);
}

void Request::GetParam(const std::string &line, int fd, const ServerConfig &servers)
{
    (void)fd;
    (void)servers;
    std::cout << line << std::endl;
}
