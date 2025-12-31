

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include "cgi.hpp"
#include "ConfigFile.hpp"

// : public Cgi
class Request 
{
    private:
        std::string     _METHOD;
        std::string     _PATH;
        std::string     _CONTENT;
        size_t            _SIZE;

        void    GetParam(std::string line, int fd);
        void    GetRequest(int fd);
        void    PostRequest(int fd, std::string line);
        void     validate();
        void     Readfile();
        void    SendError(int fd);
    public:
        Request();
        void    mainRequest(std::string line, int fd, ServerConfig servers);
};


#endif
