


#include "ConfigParce/ConfigFile.hpp"

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
    delete Config;
    return 0;
}

