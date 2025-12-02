#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



struct ServerConfig {
    std::string listen;
    std::string server_name;
    std::string root;
    std::string index;
    std::string error_page;
    std::string client_max_body_size;
    bool autoindex;
};


class ConfigFile
{
// cofig file collected data
private:
    std::vector<ServerConfig> servers;

    

    std::string fdline;
    std::vector<std::string> serverOnly;
    std::vector<std::string> locationOnly;
    std::vector<std::string> common;

    


    

public:
    ServerConfig defaultServer;
    ConfigFile();
    ~ConfigFile();


    int OpeningFile(const char *FileName);
    int TakeData();
    void TakerD(std::vector<std::string>helo, int IndexStart ,int IndexEnd );
};


#endif

