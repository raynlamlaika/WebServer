

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <arpa/inet.h>

#define READINGBUFFER 1024
#define MAXFD 2048

// first handel the server side
int  server(int port)
{
	// socket it allow thw commenection out of tow programs over the network or localy
	// is the door to resv or pass the 
	// params is the domain , the  type , protocol
	// domain he tails to the sys the rang  expect
	// type UDP/TCP
	// SOCK_STREAM (TCP) | SOCK_DGRAM (UDP)
	// protocol 0 to auto spicify protocol UDP ? TCP 

	int socketfd =  socket(AF_INET, SOCK_STREAM, 0);
	int  opt = 1;
	setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if (socketfd < 0)
		return	perror("bref error in the socket creation: "),12;

	struct sockaddr_in firs;
	memset(&firs, 0, sizeof(firs));

	firs.sin_family =  AF_INET;
	firs.sin_port = htons(port);
	firs.sin_addr.s_addr = inet_addr("127.0.0.1");
	// firs.sin_zero ;

	// next is to bind to attach the socket to the port and ip we just put on
	
	int checkBind = bind(socketfd, (struct sockaddr *) &firs , sizeof(firs));
	if (checkBind < 0)
	{
		std::cerr << "bind: error can't bind the server"<< std::endl;
		exit(1);
	}

	listen(socketfd, 5);// i think here it will be the max fd
	

	struct pollfd first;
	first.events =  POLLIN;
	first.fd  = socketfd;
	first.revents = 0;

	std::vector<struct pollfd> clients;
	clients.push_back(first);
	while (true)
	{
		int event =	poll(&clients[0], clients.size(),-1);
		if (event > 0)
		{
			if (clients[0].revents & POLLIN)
			{
				std::cout << "newww clainer" << std::endl;
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int clientfd = accept(socketfd, (struct sockaddr *) &client_addr , &client_len);
				if (clientfd < 0)
				{
					perror("can't tke the client with accest");
					continue;
				}

				struct pollfd cli;
				cli.fd  = clientfd;
				cli.events =  POLLIN;
				cli.revents = 0;
				clients.push_back(cli);
			}
			// Check all connected clients
			for (size_t i = 1; i < clients.size(); i++)
			{
				std::cout << "client is connected" << std::endl;
				if (clients[i].revents & POLLIN)
				{
					char buffer[READINGBUFFER];
					int r = recv(clients[i].fd, buffer, sizeof(buffer)-1, 0);

					if (r <= 0)
					{
						std::cout << "Client disconnected\n";
						close(clients[i].fd);
						clients.erase(clients.begin() + i);
						i--;
						continue;
					}

					buffer[r] = '\0';
					std::cout << "Received: " << buffer << std::endl;

					const char* reply = "Hello from server\n";
					send(clients[i].fd, reply, strlen(reply), 0);
				}
			}

			
		}
		if (event < 0)
			;
		if (event == 0)
			;
		
		// close(acceptint);
	}

    close(socketfd);

	return 1;
}


// handel the client side



int main(int ac, char **av)
{
	if (ac < 2)
		return (std::cerr <<"specify the port first to listen \n",1);
	std::string arg = av[1];
	// check_valid_port(fd);
	int fd = atoi(arg.c_str());
	server(fd);

}

