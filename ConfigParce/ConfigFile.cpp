#include "../include/ConfigFile.hpp"
#include <string>
#include <stdlib.h>
#include <iostream>



ConfigFile::ConfigFile()
{
    // std::vector<std::string> serverOnly = {
    //     "listen",
    //     "server_name",
    //     "error_page",
    //     "client_max_body_size",
    //     "root",
    //     "index",
    //     "autoindex"
    // };
    // std::vector <std::string> locationOnly = {"allowed_methods", "upload_pass"};

    // std::vector <std::string> common = {"root", "index", "autoindex", "error_page"};

    // this->fdline = "";

    // defaultServer.listen = {"8000"};
    // defaultServer.server_name = "localhost";
    // defaultServer.root = "/var/www/html";
    // defaultServer.index = "index.html";
    // defaultServer.error_page = "/error.html";
    // defaultServer.client_max_body_size = 10000;
    // defaultServer.autoindex = false;
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

        // -------------------------
        // 1. Handle single quotes
        // -------------------------
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

        // -------------------------
        // 2. Handle double quotes
        // -------------------------
        // if (c == '\"')
        // {
        //     if (!current.empty()) {
        //         result.push_back(current);
        //         current.clear();
        //     }

        //     i++; // move past "
        //     while (i < str.size() && str[i] != '\"')
        //     {
        //         current += str[i];
        //         i++;
        //     }

        //     if (i >= str.size())
        //     {
        //         std::cerr << "ERROR: Unterminated double quote\n";
        //         exit(1);
        //     }

        //     result.push_back(current);
        //     current.clear();
        //     continue;
        // }

        // -------------------------
        // 3. Block start "{"
        // -------------------------
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

        // -------------------------
        // 4. Block end "}"
        // -------------------------
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
        // -------------------------
        // 5. Whitespace means end of token
        // -------------------------
        if (std::isspace(c))
        {
            if (!current.empty())
            {
                result.push_back(current);
                current.clear();
            }
            continue;
        }

        // -------------------------
        // 6. Normal character
        // -------------------------
        current += c;
    }

    // push last token
    if (!current.empty())
        result.push_back(current);

    return result;
}





bool ServerDirective(const std::string& token) {
    static const char* dirs[] = {
        "listen", "server_name", "root", "index",
        "error_page", "client_max_body_size", "return"
    };
    for (int i = 0; i < 7; i++)
        if (token == dirs[i])
            return true;
    return false;
}

bool LocationDirective(const std::string& token) {
    static const char* dirs[] = {
        "allow_methods", "autoindex", "upload_path",
        "cgi_extension", "cgi_path", "redirection", "alias"
    };
    for (int i = 0; i < 7; i++)
        if (token == dirs[i])
            return true;
    return false;
}




std::string Taker(const std::vector<std::string>& helo, int i)
{
    std::string result;

    // Safety check
    if (i < 0 || i >= (int)helo.size())
        return result;

    for (int k = i; k < (int)helo.size(); k++)
    {
        result += helo[k];

        // Stop as soon as we find a token ending with ';'
        if (!helo[k].empty() && helo[k].find(";") != std::string::npos)
            return result;

        result += " "; // space between tokens if needed
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


// int skipns(std::vector<std::string> strs, int index)
// {
//     for (int i =index ; strs[i] ; i++)
// }
#include <vector>
#include <string>
#include <cctype>

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
bool StartBllock(std::string str)
{
    return str =="{" ? true : false;
}
bool EndBlock(std::string str)
{
    return str =="}" ? true : false;
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

    std::vector<std::string> witout = removeEmptyLines(helo);
    for (size_t i = 0; i < witout.size(); ++i)
    {
        std::cout << "------------------>" << witout[i] << std::endl;
        
        if (isBlock(witout[i]))
        {
            
        }
        else if (isDirevative(witout[i]))
        {

        }
        else if (StartBllock(witout[i]))
        {

        }
        else if (EndBlock(witout[i]))
        {
            
        }
    }































    // for (size_t i = 0; i < helo.size(); ++i)
    // {
    //     
    //     // std::cout << "" << i << std::endl;
    //     if (helo[i] == "\n")
    //         ;//std::cout << "the hole string is [\"" << helo[i] << "\"]\n"; 
    //     // check if the  server  block first
        
    //     else if (helo[i] == "server")
    //     {
    //         size_t j = i + 1;

    //         // Check opening brace
    //         if (j < helo.size() && helo[j] == "{")
    //         {
    //             j++;

    //             // Skip empty tokens
    //             while (j < helo.size() && (helo[j].empty() || helo[j] == "\n"))
    //                 j++;

    //             // std::cout << "this sis the location or nooot->>>>>>> " << helo[j] <<  std::endl;
    //             // helo[j] = removeSpaces(helo[j]);
                


    //             // Handle directives inside server block
    //             while (j < helo.size() && helo[j] != "}")
    //             {
    //                 if (isDirevative(helo[j]))
    //                 {
    //                     std::string value = Taker(helo, j);
    //                     std::cout << "\t\t\t server --> [\"" << value << "\"]\n";
    //                 }
    //                 if (helo[j] == "location")
    //                 {
    //                     j++;
    //                     j++;
    //                     if (j < helo.size() && helo[j] == "{")
    //                     {
    //                         j++;
    //                         while (j < helo.size() && (helo[j].empty() || helo[j] == "\n"))
    //                             j++;

    //                         // Handle directives inside server block
    //                         while (j < helo.size() && helo[j] != "}")
    //                         {
    //                             if (isDirevative(helo[j]))
    //                             {
    //                                 std::string value = Taker(helo, j);
    //                                 std::cout << "\t\t\t location --> [\"" << value << "\"]\n";
    //                             }
    //                             j++;
    //                         }
    //                     }
    //                 }
    //                 j++;
    //             }

    //             i = j; // skip to the end of server block
    //         }
    //     }

    // }
    return 0;
}
