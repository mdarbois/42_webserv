#ifndef CGI_HPP
#define CGI_HPP

#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "./ParserHTTP.hpp"
#include "./ServerConfig.hpp"
#include "utils.hpp"

class CGI {
	public:
		CGI();
		CGI(ParserHTTP &parsedRequest, ServerConfig &config);
		~CGI();
		CGI(CGI const &src);
		CGI &operator=(CGI const &rhs);
		int output_pipe[2];

		std::string					getBody() const;
		size_t						getLength() const;
		std::vector<std::string>	getEnv() const;
		std::vector<std::string>	getArgs() const;
		bool						timeOut() const;
		void						writeCGIToPipe();
		void						readBodyFromPipe();

	private:
		std::vector<std::string>	_env;
		std::vector<std::string>	_args;
		ParserHTTP					_parsedRequest;
		char**						_argsArray;
		char**						_envArray;
		const char*					_php;
		size_t						_length;
		bool						_timeOut;
		std::string					_body;

		void						_print(CGI cgi);
		void						_addArgs(ParserHTTP &parsing, ServerConfig &config);
		void						_addEnv(ParserHTTP &parsing);
		void						_childProcess(int *output_pipe);
		void						_parentProcess(int pid, int *output_pipe, pid_t pidWait, time_t startTime);
		std::string					_readOutput(int output_pipe[2]);
};

std::ostream& operator<<(std::ostream& os, const CGI& cgi);
#endif