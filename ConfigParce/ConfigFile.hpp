#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


struct LocationConfig {
    std::string path;              // e.g. "/images"
    std::string root;
    std::string index;
    bool autoindex;
    std::vector<LocationConfig> locations;
};


struct ServerConfig {
    std::vector<std::string> listen;  
    std::string server_name;
    std::string root;
    std::string index;
    std::string error_page;
    int client_max_body_size;
    bool autoindex;
    std::vector<LocationConfig> locations;
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

