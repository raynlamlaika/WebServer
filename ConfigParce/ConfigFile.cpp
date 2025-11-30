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
        // if (c == ';')
        // {
        //     if (!current.empty())
        //     {
        //         result.push_back(current);
        //         current.clear();
        //     }
        //     result.push_back(";");
        //     continue;
        // }
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


bool isDirevative(std::string str)
{
    if (str.find("listen") != std::string::npos)
        return 1;
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
                std::cerr << " eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n";

    // check the ; at the end of string

    for (int i = 0; i < helo.size(); ++i)
    {
        // std::cout << "" << i << std::endl;
        std::cout << helo[i] << std::endl;
        if (isDirevative(helo[i]))
        {
            // loop into the next string and see the if there is ; if it come to } it throw exaception
            std::cerr << " eeeeeeeeeeerrrrrrrrrrrrrrrrrrrrrrreeeeeeeeeeeeeeeeeeeeeeeeeee\n";
            std::cout << "heheheh->:" << helo[i] << std::endl;
            
            std::string value = Taker(helo, i);
            std::cout << "heheheh VAllle->:" << value << std::endl;
            exit(1);
        }
    }
    return 0;
}
