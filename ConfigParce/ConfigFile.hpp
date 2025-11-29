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



    

public:
    ConfigFile();
    ~ConfigFile();


    int OpeningFile(const char *FileName);
    int TakeData();
    void TakerD(std::vector<std::string>helo, int IndexStart ,int IndexEnd );
};


#endif