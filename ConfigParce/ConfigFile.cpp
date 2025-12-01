#include "ConfigFile.hpp"
#include <string>
#include <iostream>
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
        // if (std::isspace(c))
        // {
        //     if (!current.empty())
        //     {
        //         result.push_back(current);
        //         current.clear();
        //     }
        //     continue;
        // }

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

    for (int i = 0; i < helo.size(); ++i)
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
            size_t start = StartIndex.back();
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
        for (auto &b : stack) std::cerr << b << " ";
        std::cerr << std::endl;
        exit(1);
    }
    // check the ; at the end of string

    for (int i = 0; i < helo.size(); ++i)
    {
        helo[i] = removeSpaces(helo[i]);
        // std::cout << "" << i << std::endl;
        if (helo[i] == "\n")
            ;//std::cout << "the hole string is [\"" << helo[i] << "\"]\n"; 
        // check if the  server  block first
        
        else if ((helo[i]).find("server") != std::string::npos)
        {
            // check if there is{ in the next token
            int j = i + 1;
            if (helo[j] == "{")
            {
                j++;
                while (helo[j] == "\n")
                    j++;
                if (isDirevative(helo[j]))
                {
                    std::string value = Taker(helo, j);
                    std::cout << "\t\t\t server -->[\"" << value << "\"]\n"; 
                }
            }
            
        }
    }
    return 0;
}
