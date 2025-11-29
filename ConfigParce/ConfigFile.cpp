#include "ConfigFile.hpp"

ConfigFile::ConfigFile()
{
    this->fdline = "";
    std::cout << "constracture is called \n";
}

ConfigFile::~ConfigFile()
{
    std::cout << "deconstracture is called \n";
}



int ConfigFile::OpeningFile(const char *FileName)
{
    if (!FileName)
        return(std::cerr << "file is empty", 1);
    std::ifstream fdConfigF;

    fdConfigF.open(FileName, std::ios::in);
    if (fdConfigF)
    {
        std::cout << "file is opened! -> name :"<< FileName << std::endl;
        fdline = std::string(
        (std::istreambuf_iterator<char>(fdConfigF)),
        std::istreambuf_iterator<char>());

        fdConfigF.close();
        std::cout << "file is closed! -> name :"<< FileName << std::endl;
        return (0);
    }
    else
    {
        std::cerr << "error in opning the config file -> name:" << FileName <<  "|"<< std::endl;
        return (1);
    }
    return (0);
}

std::vector<std::string> splitingConfg(const std::string &str, std::string delims)
{
    std::vector<std::string> splited;
    std::stringstream ss(str);
    std::string tokenTaker;

    size_t start = str.find_first_not_of(delims);
    while (start != std::string::npos)// check o delimeters
    {
        size_t end = str.find_first_of(delims, start);
        splited.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(delims, end);
    }
    return(splited);
}



void ConfigFile::TakerD(std::vector<std::string>helo, int IndexStart ,int IndexEnd )
{
    while (IndexEnd >= IndexStart)
    {
        std::cout << helo[IndexStart];
        IndexStart++;
        std::cout << std::endl;
    }
}

int ConfigFile::TakeData()
{
    // remove the commantes
    for (size_t i = 0; i < fdline.length();)
    {
        if (fdline[i] == '#')
        {
            size_t from = i;
            size_t toReplace = from;
            while (toReplace < fdline.length() && fdline[toReplace] != '\n')
                toReplace++;
            fdline.erase(from, toReplace - from);
            continue;
        }
        i++;
    }
    // std::cout << fdline;

    // now it need to be tokenzed into vector
    std::vector<std::string> helo = splitingConfg(fdline, " \n\t");
    // so tokens is here
    int flag = 0;
    int IndexStart = 0;
    int IndexEnd = 0;
    for (size_t i = 0; i < helo.size(); ++i)
    {
        if (helo[i] == "server")
        {
            std::cout << "start  " << helo[i] << "  index is" << i<< std::endl;
            if (helo[++i] == "{")
            {
                flag = 1;
                IndexStart  = i - 1;
            }
        }
        else if (flag && helo[i] == "}")
        {
            IndexEnd = i;
            std::cout << "end  ->" << IndexEnd << "  index is ->" << IndexStart << std::endl;
            flag = 0;
            // handel the server pointer start / end (data where it will be taking)
            TakerD(helo, IndexStart , IndexEnd );
        }
    }
    return 0;

}
