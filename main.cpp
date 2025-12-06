

#include "unistd.h"
#include "ConfigParce/ConfigFile.hpp"
#include "Server/Server.hpp"


std::vector<ServerConfig> hard_code(std::vector<ServerConfig> servers)
{
    

    // ============================
    // SERVER 1
    // ============================
    ServerConfig s1;

    s1.listen.push_back("8099");
    s1.listen.push_back("8080");
    s1.server_name = "mysite.com";
    s1.root = "/var/www/html";
    s1.index = "index.html";
    s1.error_page = "404.html";
    s1.client_max_body_size = 10000; // 1MB
    s1.autoindex = false;

    // Location 1 inside server 1
    LocationConfig l1;
    l1.path = "/images";
    l1.root = "/var/www/images";
    l1.index = "index.html";
    l1.autoindex = true;

    s1.locations.push_back(l1);

    // ============================
    // SERVER 2
    // ============================
    ServerConfig s2;

    s2.listen.push_back("3000");
    s2.listen.push_back("3001");
    s2.server_name = "api.local";
    s2.root = "/var/www/api";
    s2.index = "api_index.html";
    s2.error_page = "500.html";
    s2.client_max_body_size = 5000000; // 5MB
    s2.autoindex = true;

    // Location 1 inside server 2
    LocationConfig l2;
    l2.path = "/upload";
    l2.root = "/var/www/api/upload";
    l2.index = "upload.html";
    l2.autoindex = false;

    s2.locations.push_back(l2);

    // Add both servers to container
    servers.push_back(s1);
    servers.push_back(s2);
    return (servers);
}
#include <iostream>

void printServers(const std::vector<ServerConfig>& servers)
{
    for (size_t si = 0; si < servers.size(); si++)
    {
        const ServerConfig& srv = servers[si];

        std::cout << "=====================================\n";
        std::cout << "SERVER #" << si << "\n";
        std::cout << "=====================================\n\n";

        // ------------------------------------
        // Listen Ports with indexes
        // ------------------------------------
        std::cout << "Listen Ports:\n";
        for (size_t pi = 0; pi < srv.listen.size(); pi++)
        {
            std::cout << "   [" << pi << "] " << srv.listen[pi] << "\n";
        }
        if (srv.listen.empty())
            std::cout << "   (none)\n";
        std::cout << "\n";

        // ------------------------------------
        // Server fields
        // ------------------------------------
        std::cout << "Server Configuration:\n";
        std::cout << "   server_name: " << srv.server_name << "\n";
        std::cout << "   root: " << srv.root << "\n";
        std::cout << "   index: " << srv.index << "\n";
        std::cout << "   error_page: " << srv.error_page << "\n";
        std::cout << "   client_max_body_size: " << srv.client_max_body_size << "\n";
        std::cout << "   autoindex: " << (srv.autoindex ? "on" : "off") << "\n\n";

        // ------------------------------------
        // Locations with indexes
        // ------------------------------------
        std::cout << "Locations:\n";
        if (srv.locations.empty())
        {
            std::cout << "   (none)\n";
        }
        else
        {
            for (size_t li = 0; li < srv.locations.size(); li++)
            {
                const LocationConfig& loc = srv.locations[li];

                std::cout << "   Location #" << li << ":\n";
                std::cout << "       path: " << loc.path << "\n";
                std::cout << "       root: " << loc.root << "\n";
                std::cout << "       index: " << loc.index << "\n";
                std::cout << "       autoindex: " << (loc.autoindex ? "on" : "off") << "\n\n";
            }
        }

        std::cout << "\n";
    }
}

int main(int ac, char **av)
{
    ConfigFile *Config = new ConfigFile();

    // std::cout << "----------------------->" << av[1] << std::endl;
    if (1 == Config->OpeningFile(av[1]))
    {
        std::cerr << " config file error" << std::endl;
        return 0;
    }   
    if(Config->TakeData())
        ;

    // work with the defaut values
    ConfigFile Conf;
    std::vector<ServerConfig> k;
    std::vector<ServerConfig> p =hard_code(k);
    printServers(p);

    Server *hh  = new Server();
    hh->ServersPortsLoop(p);    
    // ServersPortsLoop(k);
    // int server_fd = k.createServerSocket(bb);

    // std::cout << "Server running...\n";
    // k.RunServer(server_fd);

    // close(server_fd);

    delete Config;
    return 0;
}

