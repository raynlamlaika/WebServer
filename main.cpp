


#include "ConfigParce/ConfigFile.hpp"

void printServerConfig(const ServerConfig& cfg)
{
    std::cout << "----- Server Configuration -----\n";
    std::cout << "listen: " << cfg.listen << "\n";
    std::cout << "server_name: " << cfg.server_name << "\n";
    std::cout << "root: " << cfg.root << "\n";
    std::cout << "index: " << cfg.index << "\n";
    std::cout << "error_page: " << cfg.error_page << "\n";
    std::cout << "client_max_body_size: " << cfg.client_max_body_size << "\n";
    std::cout << "autoindex: " << (cfg.autoindex ? "on" : "off") << "\n";
    std::cout << "--------------------------------\n";
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
    ServerConfig bb = Conf.defaultServer;
    printServerConfig(bb);
    delete Config;
    return 0;
}

