#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>

struct LocationConfig {
    std::string path;              // e.g. "/images"
    std::string root;
    std::vector<std::string> _return;
    std::string methods;
    std::string index;
    bool autoindex;
    std::vector<LocationConfig> locations;
    
    std::string upload_store;               // Path to save uploaded files
    std::map<std::string, std::string> cgi; // Map extension -> CGI interpreter path, e.g., {".php": "/usr/bin/php-cgi"}
    std::string redirection;                // URL to redirect to, if any
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
    
    std::string fdline;
    std::vector<std::string> serverOnly;
    std::vector<std::string> locationOnly;
    std::vector<std::string> common;



public:
    std::vector<ServerConfig> servers;
    ConfigFile();
    ~ConfigFile();


    int OpeningFile(const char *FileName);
    int TakeData();
    void TakerD(std::vector<std::string>helo, int IndexStart ,int IndexEnd );
    int helper(const std::vector<std::string>& tokens, size_t &i, LocationConfig & Holder);
    int serverHelper(const std::vector<std::string>& tokens, size_t &i);

};



void printServers(const std::vector<ServerConfig>& servers);
#endif

