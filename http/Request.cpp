

#include "../include/Request.hpp"


Request::Request() : _METHOD(""), _PATH(""), _CONTENT("")
{
}

void    Request::mainRequest(std::string line)
{
    // int fd =0;
    GetParam(line);
    // if (_METHOD == "GET")
    //     GetRequest(fd);
    // else if (_METHOD == "POST")
    //     PostRequest(fd, line);
}

void    Request::GetParam(std::string line)
{
    std::istringstream str(line);
    str >> _METHOD >> _PATH;
    if (_METHOD.empty() || _PATH.empty())
        throw "Error : paramter is empty";
    if (_METHOD != "POST" && _METHOD != "GET" && _METHOD != "DELETE")
        throw "Error : undefined bhaver";
    // validate();

    // std::cout << "METHOD : " << _METHOD << " PATH : " << _PATH << std::endl;
}

// void    Request::GetRequest() {}


// void    Request::validate(std::string PATH)
// {
//     (void)PATH;

// }

// <<<<<<< HEAD
// =======
// void     Request::Readfile()
// {
//     std::ifstream inputFile(_PATH.c_str());
//     getline(inputFile, _CONTENT, '\0');
//     inputFile.close();
// }

// void    Request::validate()
// {
//     std::cout << "DEBUG PATH = [" << _PATH << "]" << std::endl;
//     struct stat file;
//     if (stat(_PATH.c_str(), &file) != 0)
//         throw std::runtime_error("Error : in stat validation");
//     if ((file.st_mode & S_IFMT) != S_IFREG)
//         throw std::runtime_error("Error : in stat validation not Regular file");
//     if (access(_PATH.c_str(), F_OK) < 0)
//         throw std::runtime_error("Error : i can't access this file");
//     _SIZE = file.st_size;
// }



// void    Request::PostRequest(int fd, std::string line)
// {
//     // char buffer[4096];
//     // memset(buffer, 0, sizeof(buffer));

//     // // Read body from client
//     // int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
//     // if (bytes <= 0)
//     // {
//     //     std::cerr << "Error : failed to read POST body" << std::endl;
//     //     return;
//     // }

//     _CONTENT = line;

//     std::ofstream out(_PATH.c_str());
//     if (!out.is_open())
//     {
//         std::cerr << "Error : cannot write file for POST" << std::endl;
//         return;
//     }
//     out << _CONTENT;
//     out.close();

//     std::string body = "<h1>POST Received</h1><p>Saved to file.</p>";

//     std::stringstream ss;
//     ss << body.size();
//     std::string sizeStr = ss.str();

//     std::string header =
//         "HTTP/1.1 200 OK\r\n"
//         "Content-Type: text/html\r\n"
//         "Content-Length: " + sizeStr + "\r\n"
//         "\r\n";

//     std::string response = header + body;
//     send(fd, response.c_str(), response.size(), 0);
// }
// >>>>>>> 816019f (update post request)
