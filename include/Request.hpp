
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <map>
#include <unistd.h>
#include <iostream>
#include "ConfigFile.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstring>
  // Make sure this header defines ServerConfig

class Request
{
public:
    // Constructor
    Request();

    // Main request handler
    void MainRequest(const std::string &line, int fd, const ServerConfig &servers);

    // Send file contents as HTTP GET response
    // void GetRequest(int fd, const ServerConfig &servers);

    // // Send error response (like 404)
    // void SendError(int fd);

    // // Parse the HTTP request line and build the file path
    void GetParam(const std::string &line, int fd, const ServerConfig &servers);

private:
    // Read file contents into _CONTENT
    // void ReadFile();

    // // Validate the file exists and is readable
    // void Validate() const;

private:
    std::string _METHOD;     // HTTP Method: GET, POST, DELETE
    std::string _PATH;       // Full path to the file on disk
    std::string _OLDPATH;    // Original requested path (for logging)
    std::string _CONTENT;    // File content
    // size_t      _SIZE;       // File size
};

#endif // REQUEST_HPP
