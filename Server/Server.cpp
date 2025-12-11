#include "../include/Server.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>


Server::Server() : Request() {}
Server::~Server(){}


// int Server::createServerSocket(ServerConfig& cfg)
// {
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd == 0)
//     {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     int opt = 1;
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
//     {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }

//     struct sockaddr_in address;
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY; // bind to all interfaces
//     address.sin_port = htons(std::stoi(cfg.listen)); // convert port string to int

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     if (listen(server_fd, 10) < 0)
//     {
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }

//     std::cout << "Server listening on port " << cfg.listen << "\n";
//     return server_fd;
// }

// void Server::RunServer(int server_fd)
// {
    
// }

// int ServerListen(int port)
// {
//     // server socket
//     int ServSocketFd = socket(AF_INET, SOCK_STREAM, 0);
//     int  opt = 1;
// 	setsockopt(ServSocketFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
//     if (ServSocketFd < 0)
// 		return	perror("bref error in the socket creation: "),12;
    
//     struct sockaddr_in serverInfo;
// 	memset(&serverInfo, 0, sizeof(serverInfo));

//     serverInfo.sin_family =  AF_INET;
// 	serverInfo.sin_port = htons(port);
// 	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
//     // serverInfo.sin_zero search about this

//     int checkBind = bind(ServSocketFd, (struct sockaddr *) &serverInfo , sizeof(serverInfo));
// 	if (checkBind < 0)
// 	{
// 		std::cerr << "bind: error can't bind the server"<< std::endl;
// 		exit(1);
// 	}


//     listen(ServSocketFd, 5); // change 5 with max fd

//     struct pollfd ServActions;
// 	ServActions.events =  POLLIN;
// 	ServActions.fd  = ServSocketFd;
// 	ServActions.revents = 0;

//     std::vector<struct pollfd> clients;
// 	clients.push_back(ServActions);


//     while (42)
//     {
//         // wait for the connection
//         int event =	poll(&clients[0], clients.size(),-1);
//         if (event > 0)
//         {
//             if (clients[0].revents & POLLIN)
// 			{
// 				std::cout << "newww clainer" << std::endl;
// 				struct sockaddr_in client_addr;
// 				socklen_t client_len = sizeof(client_addr);
// 				int clientfd = accept(ServSocketFd, (struct sockaddr *) &client_addr , &client_len);
// 				if (clientfd < 0)
// 				{
// 					perror("can't tke the client with accest");
// 					continue;
// 				}

// 				struct pollfd cli;
// 				cli.fd  = clientfd;
// 				cli.events =  POLLIN;
// 				cli.revents = 0;
// 				clients.push_back(cli);
// 			}
//             // Check all connected clients
// 			for (size_t i = 1; i < clients.size(); i++)
// 			{
// 				std::cout << "client is connected" << std::endl;
// 				if (clients[i].revents & POLLIN)
// 				{
// 					char buffer[READINGBUFFER];
// 					int r = recv(clients[i].fd, buffer, sizeof(buffer)-1, 0);

// 					if (r <= 0)
// 					{
// 						std::cout << "Client disconnected\n";
// 						close(clients[i].fd);
// 						clients.erase(clients.begin() + i);
// 						i--;
// 						continue;
// 					}

// 					buffer[r] = '\0';
// 					std::cout << "Received: " << buffer << std::endl;

// 					const char* reply = "Hello from server\n";
// 					send(clients[i].fd, reply, strlen(reply), 0);
// 				}
// 			}
//         }
//         if (event > 0)
//             ;
//         if (event > 0)
//             ;
//     }
// }

int ServerListen(int port)
{
    // server socket
    int ServSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    int  opt = 1;
	setsockopt(ServSocketFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (ServSocketFd < 0)
		return	perror("bref error in the socket creation: "),12;
    
    struct sockaddr_in serverInfo;
	memset(&serverInfo, 0, sizeof(serverInfo));

    serverInfo.sin_family =  AF_INET;
	serverInfo.sin_port = htons(port);
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    // serverInfo.sin_zero search about this

    int checkBind = bind(ServSocketFd, (struct sockaddr *) &serverInfo , sizeof(serverInfo));
	if (checkBind < 0)
	{
		std::cerr << "bind: error can't bind the server"<< std::endl;
		exit(1);
	}


    listen(ServSocketFd, SOMAXCONN); // change 5 with max fd

    return (ServSocketFd);

}


int Server::ServersPortsLoop(std::vector<ServerConfig>& cfg)
{
    std::vector<pollfd> fds;                 // All listening + client FDs here
    std::map<int, int>  fdToServerIndex;  // rememeber the port come from every server
    for (size_t i = 0; i < cfg.size() ; i++)
    {
        // std::cout <<   "server :" << i << std::endl;
        //servers will take the port from every one
        for (size_t j = 0;j < cfg[i].listen.size() ; j++)
        {
            //every port in server
            // printf()
            // std::cout <<   "helooo lo portr : "<<cfg[i].listen[j] <<std::endl;
            //creation on the port listen
            int  port =  atoi(cfg[i].listen[j].c_str());
            int FdSocket = ServerListen(port);
            
            struct pollfd ServActions;
            ServActions.events =  POLLIN;
            ServActions.fd  = FdSocket;
            ServActions.revents = 0;

            // std::vector<struct pollfd> clients;
            fds.push_back(ServActions);
            fdToServerIndex[FdSocket] = i;
            std::cout << "[Listening] Server " << i
                      << " on port " << port
                      << " (fd=" << FdSocket << ")\n";
        }
    }


    while (42)
    {
        // wait for the connection
        int event =	poll(fds.data(), fds.size(),-1);
        if (event < 0)
        {
            perror("poll");
            continue;
        }
        for (int i = 0 ; i < (int)fds.size(); i++)
        {
            // I LISTENING SOCKET (incoming client)
            if (fdToServerIndex.count(fds[i].fd) && (fds[i].revents & POLLIN))
            {
                int ServerId = fdToServerIndex[fds[i].fd];
                int listeningFd = fds[i].fd;
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);


                int clientfd = accept(listeningFd, (struct sockaddr *) &client_addr , &client_len);
                if (clientfd < 0)
                {
                    perror("can't tke the client with accest");
                    continue;
                }
                struct pollfd cli;
                cli.fd  = clientfd;
                cli.events =  POLLIN;
                cli.revents = 0;

                fds.push_back(cli);
                std::cout << "[ACCEPT] Client FD " << clientfd
                      << " for server #" << ServerId << "\n";
                continue;
            }
            
            // II CLIENT SOCKET READY TO READ
            if (fds[i].revents & POLLIN)
            {
                char buffer[4096];
                int n = recv(fds[i].fd, buffer, sizeof(buffer)-1, 0);
                if (n <= 0)
                {
                    // Client disconnected
                    std::cout << "[DISCONNECT] client " << fds[i].fd << "\n";
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    i--;
                    continue;
                }
                buffer[n] = '\0';

                // the parcing should be in this buffer 'buffer'
                // mainRequest(buffer, fds[i].fd);
                std::cout << "[DATA FROM " << fds[i].fd << "]\n"
                        << buffer << "\n";
                // SIMPLE RESPONSE (for now)
                // string = parce();

                const char* reply = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nWorldHwllo";
                send(fds[i].fd, reply, strlen(reply), 0);
                close(fds[i].fd);
            }
        }
    }
    return 1;

}
