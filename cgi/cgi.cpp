

#include "../include/cgi.hpp"
#include <fcntl.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
// switched to map
#define BUFFER_SIZE 2048
Cgi::Cgi(){

}

Cgi::~Cgi(){}

Holde Cgi::execCgi(std::string path, std::string Data)
{
    this->dataholder.flag =  true;
    this->dataholder.data =  "" ;
    this->dataholder.len_size =  0;
    this->dataholder.fd = 0;

    // check content valid 
    std::cout << "----------> CGI PATh is : " << path << std::endl;
    std::cout << "---------------> CGI content is : " << Data << std::endl;


    // open pipe
    char buffer[BUFFER_SIZE];
    int Pipefd[2];
    int PipefdOut[2];
    if (pipe(Pipefd) == -1 || pipe(PipefdOut) == -1)
    {
        std::cout << "the pipe is faildd\n";
        exit(1);
    }
    // fork to exec the php  
    pid_t i = fork();
    if (i  < 0)
    {
        std::cout << "the fork is faildd\n";
        exit(1);
    }
    
    if (i == 0)
    {
        // child process is here

        if (dup2(Pipefd[0], STDIN_FILENO) == -1 )
        {
            std::cout << "the dup2 is faildd\n";
            close(Pipefd[1]);
            close(Pipefd[0]);
            exit(1);
        }

        if (dup2(PipefdOut[1], STDOUT_FILENO) == -1 )
        {
            std::cout << "the dup2 is faildd\n";
            close(Pipefd[1]);
            close(Pipefd[0]);
            exit(1);
        }
        close(Pipefd[1]);
        close(PipefdOut[0]);

        const char *script = path.c_str();
        char *const argv[] = {(char *)"php", (char *)script, NULL};
        execve("/usr/bin/php", argv , NULL);
        perror("execve");
    }
    else
    {
        // WIFEXITED(status);
        // close(PipefdOut[0]);
        //  write into the data out pipe
        close(Pipefd[0]);
        close(PipefdOut[1]);
        write(Pipefd[1], Data.c_str(), Data.size());
        close(Pipefd[1]);

        ssize_t bytes;
        while ((bytes = read(PipefdOut[0], buffer, sizeof(buffer))) > 0)
        {
            std::cout.write(buffer, bytes);
        }
        close(PipefdOut[0]);

        // wait for the child
        int status;
        int w = waitpid(i, &status, WUNTRACED | WCONTINUED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
         if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            (void) exit_status;
            // check of the compalier pass any probleme in the executing part
            // return ;
        }
        else if (WIFSIGNALED(status)) {
            // check of the compalier pass any SIG in the executing part
            // return ;
        }

        

    }

    return dataholder;
    // check exec fail exit child 1

    // other whi
}