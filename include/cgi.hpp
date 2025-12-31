#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>


typedef struct Holde
{
    // dta/
    bool flag;
    int fd;
    int len_size;
    std::string data;
}    Holde;



class  Cgi
{
private:
    Holde dataholder; 



public:
    Cgi();
    virtual ~Cgi();


    // helper function
    Holde execCgi(std::string path, std::string Data);
    char **helperSplit(std::string str);

    // geters and setter
};



#endif