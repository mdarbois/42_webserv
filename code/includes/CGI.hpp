#ifndef CGI_HPP
#define CGI_HPP

#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "./ParserHTTP.hpp"
#include "utils.hpp"

class CGI {
    public:
        CGI();
        CGI(ParserHTTP &parsing);
        ~CGI();
        CGI(CGI const &src);
        CGI &operator=(CGI const &rhs);
        int input_pipefd[2];  // Pipe for sending input to child
        int output_pipefd[2]; // Pipe for receiving output from child
        void _addArgs(ParserHTTP &parsing);
        void _childProcess(int *input_pipefd, int *output_pipefd);
        void _parentProcess(ParserHTTP parsing, int pid, int *input_pipefd, int *output_pipefd, pid_t pidWait, time_t startTime);
        std::string	_readOutput(int output_pipefd[2]);
        std::string getBody() const;
        size_t	getLength() const;
        std::vector<std::string> getEnv() const;
        std::vector<std::string> getArgs() const;
        bool	timeOut() const;
    private:
        std::vector<std::string> _env;
        std::vector<std::string> _args;
        char** _argsArray;
        char** _envArray;
        const char *_php;
        size_t _length;
        bool _timeOut;
        std::string _body;
        void _print(CGI cgi);
};
std::ostream& operator<<(std::ostream& os, const CGI& cgi);
#endif