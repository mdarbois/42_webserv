#include "../../includes/CGI.hpp"

CGI::CGI(){}

CGI::CGI(ParserHTTP &parsedRequest) : _php(""), _length (0),  _timeOut (false), _body ("")
{
	//Set up the CGI
	_parsedRequest = parsedRequest;
	_addEnv(parsedRequest);
	_addArgs(parsedRequest);
	//if (pipe(input_pipefd) == -1)
	//	throw std::runtime_error("CGI error: input pipe failed");
	if (pipe(output_pipe) == -1)
		throw std::runtime_error("CGI error: output pipe failed");
	std::cerr << "Pipe set up write end: " << output_pipe[1] << std::endl;
	std::cerr << "Pipe set up read end: " << output_pipe[0] << std::endl;
	//Build the response in the child and write it to the pipe
	//In order to use poll, this must be executed in the next poll cycle
	//therefore put in another function
	/* 
	time_t	startTime = time(0);
	int pid = fork();
	_print(*this);
	_argsArray = vectorToCharArray(_args);
	_envArray = vectorToCharArray(_env);
	if (pid == 0)
		_childProcess(input_pipefd, output_pipe);
	else if (pid > 0)
		_parentProcess(parsing, pid, input_pipefd, output_pipe, 0, startTime);
	else
	{
		deleteArray(_argsArray);
		deleteArray(_envArray);
		throw std::runtime_error("CGI error: fork failed");
	} */

	//Read the response from the pipe
	/* if (!_timeOut)
		_body = _readOutput(output_pipe);
	deleteArray(_argsArray);
	deleteArray(_envArray); */
}

void	CGI::writeCGIToPipe()
{
	time_t	startTime = time(0);
	int		pid = fork();
	_argsArray = vectorToCharArray(_args);
	_envArray = vectorToCharArray(_env);
	if (pid == 0)
		_childProcess(output_pipe);
	else if (pid > 0)
		_parentProcess(pid, output_pipe, 0, startTime);
	else
	{
		deleteArray(_argsArray);
		deleteArray(_envArray);
		throw std::runtime_error("CGI error: fork failed");
	}
}

//TODO: What happens if timeout?
void	CGI::readBodyFromPipe()
{
	if (!_timeOut)
		_body = _readOutput(output_pipe);
	deleteArray(_argsArray);
	deleteArray(_envArray);
}

CGI::~CGI() {}

CGI::CGI(CGI const &src) {
	this->_env = src._env;
	this->_args = src._args;
	this->_php = src._php;
	this->_timeOut = src._timeOut;
	this->_length = src._length;
	this->_body = src._body;

}

CGI &CGI::operator=(const CGI &rhs) {
	if (this != &rhs){
		this->_env = rhs._env;
		this->_args = rhs._args;
		this->_php = rhs._php;
		this->_timeOut = rhs._timeOut;
		this->_length = rhs._length;
		this->_body = rhs._body;
		//this->input_pipefd[0] = rhs.input_pipefd[0];
		//this->input_pipefd[1] = rhs.input_pipefd[1];
		this->output_pipe[0] = rhs.output_pipe[0];
		this->output_pipe[1] = rhs.output_pipe[1];
	}
	return *this;
}

std::string CGI::getBody() const
{
	return(_body);
}
size_t	CGI::getLength() const
{
	return(_length);
}
bool	CGI::timeOut() const
{
	return (_timeOut);
}


void CGI::_addArgs(ParserHTTP &parsing)
{
	_php = "/usr/bin/php";
	_args.push_back(_php);
	std::string phpScript = ".";
	_args.push_back(phpScript + parsing.getPath());
}

void CGI::_addEnv(ParserHTTP &parsing)
{
	if (parsing.getBody().empty())
		_env.push_back("CONTENT_LENGTH = 0");
	else
	{
		size_t length = parsing.getBody().length();
		std::stringstream ss;
		ss << length;
		_env.push_back("CONTENT_LENGTH = " + ss.str());
	}
	std::string method;
	if (parsing.getMethod() == GET)
		method = "GET";
	else if (parsing.getMethod() == POST)
		method = "POST";
	else
		method = "DELETE";
	_env.push_back("METHOD = " + method);
	_env.push_back("UPLOAD_PATH = ." + parsing.getPath());
	std::map<std::string, std::string> cgiParams = parsing.getCGIParamMap();
		std::map<std::string, std::string>::iterator it;
		for (it = cgiParams.begin(); it != cgiParams.end(); ++it) {
			std::cout << "\t-" << it->first << ": " << it->second << std::endl;
			std::string envVar = pairToString(it->first, it->second);
			std::cout << envVar << "\n";
			_env.push_back(envVar);
			//putenv(const_cast<char*>(envVar.c_str()));
		}
	/* std::map<std::string, std::string>::iterator iter;
	std::cout << parsing << std::endl;
	for (iter = parsing.getCGIParamMap().begin(); iter != parsing.getCGIParamMap().end(); ++iter) {
		std::cout << "first=" << iter->first;
		std::cout << "second=" << iter->second << "\n";
		//std::string envVar = pairToString(iter->first, iter->second);
		//std::cout << envVar << "\n";
		//putenv(const_cast<char*>(envVar.c_str()));
	} */
} 

void CGI::_childProcess(int *output_pipe)
{
	//close(input_pipefd[1]);
	close(output_pipe[0]);
	/* if (dup2(input_pipefd[0], 0) == -1)
	{
		deleteArray(_argsArray);
		deleteArray(_envArray);
		throw std::runtime_error("CGI error: dup2 input pipe in child process failed");
	}
	close(input_pipefd[0]); */
	if (dup2(output_pipe[1], 1) == -1)
	{
		deleteArray(_argsArray);
		deleteArray(_envArray);
		throw std::runtime_error("CGI error: dup2 output pipe in child process failed");
	}
	close(output_pipe[1]);
	std::cerr << "Execute child" << std::endl;
	if (execve(_php, _argsArray, _envArray) == -1)
	{
		deleteArray(_argsArray);
		deleteArray(_envArray);
		throw std::runtime_error("Error: execve failed");
	}
	else
	{
		//close(input_pipefd[0]);
		close(output_pipe[1]);
	}
}
void CGI::_parentProcess(int pid, int *output_pipe, pid_t pidWait, time_t startTime)
{
	//close(input_pipefd[0]);
	close(output_pipe[1]);
	//std::string body = parsing.getBody();
	//if ( !write(input_pipefd[1], body.c_str(), body.length()) && !write(input_pipefd[1], "coucou", 7 ))
		//throw std::runtime_error("CGI error: write body"); 
	//close(input_pipefd[1]);
	while (pidWait == 0 && time(0) - startTime <= 6) 
		pidWait = waitpid(pid, NULL, WNOHANG);
	if (pidWait == 0) {
		kill(1, SIGKILL);
		_timeOut = true;
	}
	if (pidWait == -1)	{
		throw std::runtime_error("CGI error: waitpid failed");
	}
}


std::string	CGI::_readOutput(int output_pipe[2])
{
	const int BUFFSIZE = 4096;
	char buffer[BUFFSIZE];
	bzero(buffer, BUFFSIZE);
	int bytesRead = 0;
	bytesRead = read(output_pipe[0], buffer, BUFFSIZE);
	if (bytesRead < 0)
	{
		close(output_pipe[0]);
		throw std::runtime_error("Error: CGI read");
	}
	else if (bytesRead == 0)
	{
		close(output_pipe[0]);
		return "";
	}
	std::vector<char> bufferVector(buffer, buffer + bytesRead);
	close(output_pipe[0]);
	std::string output(bufferVector.begin(), bufferVector.end());
	_length += bytesRead;
	return output;
}

std::vector<std::string> CGI::getEnv() const
{
	return (_env);
}

std::vector<std::string> CGI::getArgs() const
{
	return (_args);
}

void CGI::_print(CGI cgi)
{
	std::cout  << "\033[34m---------- CGI:-----------\033[0m\n";
	std::cout  << "BODY: " << cgi.getBody() << "\n";
	std::cout  << "LENGTH: " << cgi.getLength() << "\n";
	std::cout  << "ENV: ";
	for (size_t i = 0; i < cgi.getEnv().size(); ++i) {
		std::cout << i <<":";
		std::cout  << cgi.getEnv()[i];
	}
	std::cout  << "\n";
	std::cout  << "ARGS: ";
	for (size_t i = 0; i < cgi.getArgs().size(); ++i) {
		std::cout << i <<":";
		std::cout  << cgi.getArgs()[i];
	}
	std::cout  << "\n";
}