


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>

class Request
{
    private:
        std::string     _METHOD;
        std::string     _PATH;
        std::string     _CONTENT;
        size_t            _SIZE;

        void    GetParam(std::string line);
        void    GetRequest(int fd);
        void     validate();
        void     Readfile();
    public:
        Request();
        void    mainRequest(std::string line, int fd);
};


#endif
