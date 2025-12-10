


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <sstream>
#include <fstream>


class Request {
    private:
        std::string     _METHOD;
        std::string     _PATH;
        std::string     _CONTENT;
        // long            _SIZE;
        

        void    GetParam(std::string line);
        void    GetRequest();
        void     validate(std::string PATH);
    public:
        Request();
        void    mainRequest(std::string line);
};


#endif
