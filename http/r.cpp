#include "../include/Request.hpp"


Request::Request() : _METHOD(""), _PATH(""), _OLDPATH(""), _CONTENT(""), _SIZE(0) {}

void Request::ReadFile()
{
    std::ifstream file(_PATH.c_str());
    if (!file.is_open())
        throw std::runtime_error("Error: unable to open file " + _PATH);

    std::ostringstream buffer;
    buffer << file.rdbuf();
    _CONTENT = buffer.str();
    _SIZE = _CONTENT.size();
}

void Request::Validate() const
{
    struct stat fileInfo;
    if (stat(_PATH.c_str(), &fileInfo) != 0)
        throw std::runtime_error("Error: failed to stat file " + _PATH);

    if (!S_ISREG(fileInfo.st_mode))
        throw std::runtime_error("Error: not a regular file " + _PATH);

    if (access(_PATH.c_str(), R_OK) < 0)
        throw std::runtime_error("Error: file not accessible " + _PATH);
}

void Request::MainRequest(const std::string &line, int fd, const ServerConfig &servers)
{
    GetParam(line, fd, servers);
    std::cout << "-> Method " << _METHOD
              << " [\033[32mhttp://localhost:" 
              << servers.listen[0].substr(0, servers.listen[0].length() - 1)
              << _OLDPATH << "]\033[0m" << std::endl;
    std::cerr << "["<< servers.root << "]" << std::endl;


    if (_METHOD == "GET")
        GetRequest(fd, servers);
}

void Request::SendError(int fd)
{
    ReadFile();
    std::ostringstream ss;
    ss << _SIZE;

    std::string header =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + ss.str() + "\r\n"
        "\r\n";

    send(fd, header.c_str(), header.size(), 0);
}

void Request::GetParam(const std::string &line, int fd, const ServerConfig &servers)
{
    std::cout << line << std::endl;
    std::istringstream stream(line);
    stream >> _METHOD >> _PATH;
    _OLDPATH = _PATH;

    std::string rootPath = servers.root;
    if (!rootPath.empty()) //  && rootPath[rootPath.size() - 1] == '/'
        rootPath.resize(rootPath.size() - 1);
    std::string indexFile = servers.index;
    if (_PATH == "/")
    {
        // std::cout << "((" << indexFile[indexFile.size() - 1] << "))" << std::endl;
        // if (!indexFile.empty() && indexFile[indexFile.size() - 1] == ';')
        indexFile.resize(indexFile.size() - 1);

        _PATH = "./www/" + indexFile;
    }
    else
    {
        _PATH = "./www/" + indexFile;
    }

    if (_METHOD.empty() || _PATH.empty())
        throw std::runtime_error("Error: method or path is empty");

    if (_METHOD != "GET" && _METHOD != "POST" && _METHOD != "DELETE")
        throw std::runtime_error("Error: undefined HTTP method");

    try
    {
        Validate();
    }
    catch (const std::exception &)
    {
        SendError(fd);
    }
}


void Request::GetRequest(int fd, const ServerConfig &)
{
    try {
        ReadFile();
        std::ostringstream ss;
        ss << _SIZE;
        std::cout << " -> " << _PATH << std::endl;
        std::string header =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " + ss.str() + "\r\n"
            "\r\n";
    
        std::string response = header + _CONTENT;
        send(fd, response.c_str(), response.size(), 0);
    } catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}
