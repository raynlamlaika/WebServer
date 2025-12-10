

#include "../include/Request.hpp"


Request::Request() : _METHOD(""), _PATH(""), _CONTENT("")
{
}

void    Request::mainRequest(std::string line)
{
    GetParam(line);
    
}

void    Request::GetParam(std::string line)
{
    std::istringstream str(line);
    str >> _METHOD >> _PATH;
    if (_METHOD.empty() || _PATH.empty())
        throw "Error : paramter is empty";
    if (_METHOD != "POST" && _METHOD != "GET" && _METHOD != "DELETE")
        throw "Error : undefined bhaver";
    validate(_PATH);

    // std::cout << "METHOD : " << _METHOD << " PATH : " << _PATH << std::endl;
}

void    Request::GetRequest() {}


void    Request::validate(std::string PATH)
{
    (void)PATH;

}

