#include "../include/ConfigFile.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>

ConfigFile::ConfigFile()
{
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

std::vector<std::string> splitingConfg(const std::string &str)
{
    std::vector<std::string> result;
    std::string current;

    for (size_t i = 0; i < str.size(); ++i)
    {
        char c = str[i];
        if (c == '\'')
        {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }

            i++; // move past ' 
            while (i < str.size() && str[i] != '\'')
            {
                current += str[i];
                i++;
            }

            if (i >= str.size())
            {
                std::cerr << "ERROR: Unterminated single quote\n";
                exit(1);
            }

            // push completed quoted token
            result.push_back(current);
            current.clear();
            continue;
        }
        if (c == '{')
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            result.push_back("{");
            continue;
        }

        if (c == '}')
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            result.push_back("}");
            continue;
        }
        if (c == '\n')
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            result.push_back("\n");
            continue;
        }

        if (std::isspace(c))
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            continue;
        }

        current += c;
    }

    // push last token
    if (!current.empty())
        result.push_back(current);

    return result;
}



std::string Taker(const std::vector<std::string>& helo, size_t & i)
{
    std::string result;

    // Safety check
    std::cout << "Taker the arges   |" << helo[i]  << std::endl;
    if (i < 0 || i >= helo.size())
        return result;

    for (size_t k = i; k < helo.size(); k++)
    {
        result += helo[k];
        

        // Stop as soon as we find a token ending with ';'
        if (!helo[k].empty() && helo[k].find(";") != std::string::npos)
            return result;

        result += " "; // space between tokens if needed
        i++;
    }

    return result; // if no ';' found, return full concatenation
}

std::string removeSpaces(const std::string& input) {
    std::string result = "";
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] != ' ') {
            result += input[i];
        }
    }
    return result;
}

std::vector<std::string> removeEmptyLines(const std::vector<std::string> &strs)
{
    std::vector<std::string> cleaned;

    for (size_t i = 0; i < strs.size(); i++)
    {
        const std::string &s = strs[i];
        bool onlySpaces = true;

        for (size_t j = 0; j < s.size(); j++)
        {
            if (!std::isspace(s[j]))   // found real character
            {
                onlySpaces = false;
                break;
            }
        }

        if (!onlySpaces)               // keep only real lines
            cleaned.push_back(s);
    }

    return cleaned;
}


bool isBlock(std::string str)
{
    if (str == "http" || str == "server" || str == "location")
        return true;
    return false;
}


typedef struct t_block
{
    std::string BlockName;
    std::string rrrr;
}   block;

int checkBlock(std::string token)
{
    // std::cout << token.c_str() <<"<------this sisthe innd \n";
    if (strcmp(token.c_str(), "server") == 0) // need to fix later
        return (2);
    else if (strcmp(token.c_str(), "http") == 0)
        return (1);
    return false;
}


int isDirevative(std::string str)
{
     static const char* dirs[] =
     {
        "listen", "server_name", "root", "index",
        "error_page", "client_max_body_size", "return"};
    static const char* loc[] = {
        "allow_methods", "autoindex", "upload_path",
        "cgi_extension", "cgi_path", "redirection", "alias","proxy_pass", "proxy_set_header", "try_files"
    };
    for (int i = 0; dirs[i] ; i++)
    {

        if (str.find(dirs[i]) != std::string::npos)
            return  1;
    }
    for (int i = 0; loc[i] ; i++)
    {

        if (str.find(loc[i]) != std::string::npos)
            return  2;
    }
    return 0;
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
    //std::cout << fdline;

    // split by " \n\t"
    std::vector<std::string> helo = splitingConfg(fdline);

    std::vector<std::string> stack;
    std::vector<size_t> StartIndex;

    for (size_t i = 0; i < helo.size(); ++i)
    {
        std::string token = helo[i];
        // std::cout << token << std::endl;

        if (token == "{")
        {
            // block start, push a placeholder (could store the preceding directive if exists)
            std::string blockName = (i > 0) ? helo[i - 1] : "anonymous";
            stack.push_back(blockName);
            StartIndex.push_back(i);
            continue;
        }

        if (token == "}")
        {
            if (stack.empty())
            {
                std::cerr << "ERROR: Unexpected '}' at index " << i << std::endl;
                exit(1);
            }
            std::string finished = stack.back();
            // size_t start = StartIndex.back();
            stack.pop_back();
            StartIndex.pop_back();

            // std::cout << "END BLOCK: " << finished << " from " << start << " to " << i << std::endl;
            continue;
        }
    }

    // At the end, check for unclosed blocks
    if (!stack.empty())
    {
        std::cerr << "ERROR: Unclosed block(s) detected: ";
        // for (auto &b : stack) std::cerr << b << " ";
        // std::cerr << std::endl;
        exit(1);
    }
    



    // check the ; at the end of string
    std::vector<std::string> tokens = removeEmptyLines(helo);
    size_t i = 0;


    while (i < tokens.size())
    {
        int y = checkBlock(tokens[i]);
        if (y == 0)
        {
            std::cerr << "Unexpected token: " << tokens[i] << "\n";
            exit(1);
        }
        if (y == 1)// handel the http bloock
        {
            if (tokens[i+1] != "{")
                return std::cerr << "Error: expected '{' after http\n", exit(1), 1;

            i += 2; // skip "http {"
            int depth = 1;

            while (i < tokens.size() && depth > 0)
            {
                if (tokens[i] == "{")
                {
                    depth++;
                    i++;
                }
                else if (tokens[i] == "}")
                {
                    depth--;
                    i++;
                }
                else
                {
                    int inner = checkBlock(tokens[i]);
                    // std::cout << inner <<"<------this sisthe innd \n";
                    if (inner == 2) // server inside http
                    {
                        // parse server block
                        ServerConfig servinfo;

                        if (tokens[i+1] != "{")
                            return std::cerr << "Error: expected '{' after server\n", exit(1), 1;

                        i += 2; // skip "server {"
                        int srvDepth = 1;

                        while (i < tokens.size() && srvDepth > 0)
                        {
                            if (tokens[i] == "{")
                                srvDepth++;
                            else if (tokens[i] == "}")
                                srvDepth--;
                            else
                            {
                                while (isDirevative(tokens[i]))
                                {
                                    std::string name = tokens[i];

                                    // std::string FullLine = // full line in here;
                                    if (name == "listen")
                                    {
                                        
                                        std::string listen = Taker(tokens, i);
                                        std::cout << listen << "\n";
                                        // servinfo.listen = Taker(tokens, i);
                                    }
                                    else if (name == "server_name")
                                    {
                                        std::string  server_name = Taker(tokens, i);
                                        std::cout << server_name << "\n";
                                        // servinfo.server_name = Taker();
                                    }
                                    else if (name == "root")
                                    {
                                        std::string root = Taker(tokens, i);
                                        std::cout << root << "\n";
                                        
                                        // servinfo.root = Taker(tokens, i);
                                    }
                                    else if (name == "error_page")
                                    {
                                        std::string  error_page = Taker(tokens, i);
                                        std::cout << error_page << "\n";

                                        // servinfo.error_page = Taker(tokens, i);
                                    }
                                    else if (name == "client_max_body_size")
                                    {
                                        std::string  client_max_body_size = Taker(tokens, i);
                                        std::cout << client_max_body_size << "\n";
                                        
                                        // servinfo.client_max_body_size = Taker(tokens, i);
                                    }
                                    else if (name == "index")
                                    {
                                        std::string  index = Taker(tokens, i);
                                        std::cout << index << "\n";
                                        
                                        // servinfo.client_max_body_size = Taker(tokens, i);
                                    }
                                    i++;

                                }
                                if (tokens[i] == "location")
                                {
                                    // check aregs first
                                    i++;
                                    std::string name = tokens[i];
                                    if (name != "{")
                                    {
                                        std::cout << "the path is :" << name << std::endl;
                                        i++;
                                        if (tokens[i] == "{")
                                            i++;
                                        else
                                            return(std::cout << "Unexpected tokeeen " << tokens[i]  << std::endl, exit(1), 1);
                                        std::cout << "|" << tokens[i] <<"|" << "\n";
                                        
                                    }
                                    while (isDirevative(tokens[i]))
                                    {
                                        name = tokens[i];
                                        if (name == "methods")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "root")
                                        {
                                            std::cout << "sddddd\n";
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "index")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "autoindex")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "upload_store")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "cgi")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        else if (name == "return")
                                        {
                                            std::string  tokenn = Taker(tokens, i);
                                            std::cout << tokenn << "\n";
                                        }
                                        i++;
                                    }
                                }
                                else
                                {
                                    std::cerr << "Error: only server blocks are allowed inside http. Found: " << tokens[i] << "\n";
                                    exit(1);
                                }   
                            }
                            i++;
                        }

                        // push the fully filled server to your ConfigFile servers vector
                        std::cout << "server is finished" << std::endl;
                        servers.push_back(servinfo);
                    }
                    else
                    {
                        std::cerr << "Error: only server blocks are allowed inside http. Found: " << tokens[i] << "\n";
                        exit(1);
                    }
                }
            }
        }
        else if (y == 2) // handel the server block
        {
            ServerConfig servinfo;

            if (tokens[i+1] != "{")
                return std::cerr << "Error: expected '{' after server\n", exit(1), 1;
            i += 2; // skip "server {"
            int depth = 1;

            while (i < tokens.size() && depth > 0)
            {
                if (tokens[i] == "{") depth++;
                else if (tokens[i] == "}") depth--;
                else
                {
                    // parse server-level directive or location
                    // fill servinfo
                    
                }
                i++;
            }

            servers.push_back(servinfo);
        }
    }
   











    return 0;
}
