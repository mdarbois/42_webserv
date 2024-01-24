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
        void getArgs(ParserHTTP &parsing);
        void _childProcess(int *input_pipefd, int *output_pipefd);
        void _parentProcess(ParserHTTP parsing, int pid, int *input_pipefd, int *output_pipefd, pid_t pidWait, time_t startTime);
        std::string	_readOutput(int output_pipefd[2]);
        std::string getBody() const;
        size_t	getLength() const;
        bool	timeOut() const;
    private:
        std::vector<const char *> _env;
        std::vector<const char *> _args;
        const char *_php;
        bool _timeOut;
        size_t _length;
        std::string _body;
};
#endif