#include "Server.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


Server::Server(){}
Server::~Server(){}


int Server::createServerSocket(ServerConfig& cfg)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // bind to all interfaces
    address.sin_port = htons(std::stoi(cfg.listen)); // convert port string to int

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << cfg.listen << "\n";
    return server_fd;
}

void Server::RunServer(int server_fd)
{
    while (true)
    {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
        if (client_fd < 0)
        {
            perror("accept failed");
            continue;
        }
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!\n";
        send(client_fd, response.c_str(), response.length(), 0);
        close(client_fd);
    }
}
