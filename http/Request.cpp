#include "../include/Request.hpp"


Request::Request() : _METHOD(""), _PATH(""), _CONTENT("")
{
}

void    Request::mainRequest(std::string line, int fd, ServerConfig servers)
{

    // (void)servers;

    // std::cout << "sigwiefwihefiwebioiwegfhwoiewowoif-----------------" << std::endl;

    std::cout << servers.root << std::endl;
    std::cout << servers.error_page << std::endl;
    std::cout << servers.index << std::endl;

    GetParam(line, fd);
    if (_METHOD == "GET")
        GetRequest(fd);
    if (_METHOD == "POST")
        PostRequest(fd, line);
}

void Request::SendError(int fd)
{
    std::string error =
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>404 Not Found</title></head>"
        "<body style='font-family:sans-serif;text-align:center;margin-top:20%'>"
        "<h1>404</h1>"
        "<p>Page Not Found</p>"
        "</body>"
        "</html>";
    std::stringstream ss;
    ss << error.size();
    std::string header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + ss.str() + "\r\n"
        "\r\n";
    send(fd, header.c_str(), header.size(), 0);
}

void    Request::GetParam(std::string line, int fd)
{
    std::istringstream str(line);
    str >> _METHOD >> _PATH;
    std::string update = "." + _PATH;
    _PATH = update;
    if (_METHOD.empty() || _PATH.empty())
        throw std::runtime_error("Error : paramter is empty");
    if (_METHOD != "POST" && _METHOD != "GET" && _METHOD != "DELETE")
        throw std::runtime_error("Error : undefined bhaver");
    try {
        validate();
    } catch (const std::exception &e)
    {
        // std::cout << e.what() << std::endl;
        SendError(fd);
    }
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
}

void    Request::GetRequest(int fd)
{
    
    Readfile();
    if (_PATH.find("/cgi/") != std::string::npos)
    {
        Cgi helper;
        Holde helper2 =  helper.execCgi(_PATH, _CONTENT);
    }



    std::stringstream ss;
    ss << _SIZE;
    std::string sizeStr = ss.str();

    std::string header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + sizeStr + "\r\n"
        "\r\n";

    std::string response = header + _CONTENT;

    send(fd, response.c_str(), response.size(), 0);
}

void     Request::Readfile()
{
    std::ifstream inputFile(_PATH.c_str());
    getline(inputFile, _CONTENT, '\0');
    inputFile.close();
}

void    Request::validate()
{
    std::cout << "DEBUG PATH = [" << _PATH << "]" << std::endl;
    struct stat file;
    if (stat(_PATH.c_str(), &file) != 0)
        throw std::runtime_error("Error : in stat validation");
    if ((file.st_mode & S_IFMT) != S_IFREG)
        throw std::runtime_error("Error : in stat validation not Regular file");
    if (access(_PATH.c_str(), F_OK) < 0)
        throw std::runtime_error("Error : i can't access this file");
    _SIZE = file.st_size;
}



void    Request::PostRequest(int fd, std::string line)
{
    // char buffer[4096];
    // memset(buffer, 0, sizeof(buffer));

    // // Read body from client
    // int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    // if (bytes <= 0)
    // {
    //     std::cerr << "Error : failed to read POST body" << std::endl;
    //     return;
    // }
    (void)line;
    Readfile();
    // _CONTENT = line;

    // std::ofstream out(_PATH.c_str());
    // if (!out.is_open())
    // {
    //     std::cerr << "Error : cannot write file for POST" << std::endl;
    //     return;
    // }
    // out << _CONTENT;
    // out.close();

    std::string body = "<h1>POST Received</h1><p>Saved to file.</p>";

    std::stringstream ss;
    ss << _CONTENT.size();
    std::string sizeStr = ss.str();

    std::string header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + sizeStr + "\r\n"
        "\r\n";

    std::string response = header + _CONTENT;
    send(fd, response.c_str(), response.size(), 0);
}