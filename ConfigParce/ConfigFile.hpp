#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



class ConfigFile
{
// cofig file collected data
private:
    std::string fdline;
    std::string include;
    std::string log_format;
    std::string listen;
    std::string server_name;
    


    

public:
    ConfigFile();
    ~ConfigFile();


    int OpeningFile(const char *FileName);
    int TakeData();
    void TakerD(std::vector<std::string>helo, int IndexStart ,int IndexEnd );
};


#endif

