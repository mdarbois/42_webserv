#include "../../includes/CGI.hpp"

CGI::CGI(){}

CGI::CGI(ParserHTTP &parsing) :  _length (0),  _timeOut (false), _body ("")
{
    _env = mapToArray(parsing.getHeaderMap());
    _addArgs(parsing);
    _php = "/usr/bin/php";
    if (pipe(input_pipefd) == -1)
        throw std::runtime_error("CGI error: input pipe failed");
    if (pipe(output_pipefd) == -1)
        throw std::runtime_error("CGI error: output pipe failed");
    time_t	startTime = time(0);
    std::string output;
    int pid = fork();
    _argsArray = vectorToCharArray(_args);
    _envArray = vectorToCharArray(_env);
    if (pid == 0)
        _childProcess(input_pipefd, output_pipefd);
    else if (pid > 0)
        _parentProcess(parsing, pid, input_pipefd, output_pipefd, 0, startTime);
    else
    {
        deleteArray(_argsArray);
        deleteArray(_envArray);
        throw std::runtime_error("CGI error: fork failed");
    }
    if (!_timeOut)
		_body = _readOutput(output_pipefd);
    _print(*this);
    deleteArray(_argsArray);
    deleteArray(_envArray);
}

CGI::~CGI()
{}

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

    _args.push_back(_php);
    std::string folder = "./cgi";
    _args.push_back(folder + parsing.getPath());

}

void CGI::_childProcess(int *input_pipefd, int *output_pipefd)
{
    close(input_pipefd[1]);
    close(output_pipefd[0]);
    if (dup2(output_pipefd[0], 0) == -1)
    {
        deleteArray(_argsArray);
        deleteArray(_envArray);
        throw std::runtime_error("CGI error: dup2 output pipe in child process failed");
    }
    close(input_pipefd[0]);
    if (dup2(input_pipefd[1], 1) == -1)
    {
        deleteArray(_argsArray);
        deleteArray(_envArray);
        throw std::runtime_error("CGI error: dup2 input pipe in child process failed");
    }
    close(output_pipefd[1]);
    if (execve(_php, _argsArray, _envArray) == -1)
    {
        deleteArray(_argsArray);
        deleteArray(_envArray);
        throw std::runtime_error("Error: execve failed");
    }
    std::exit(0);
}
void CGI::_parentProcess(ParserHTTP parsing, int pid, int *input_pipefd, int *output_pipefd, pid_t pidWait, time_t startTime)
{
    close(input_pipefd[0]);
    close(output_pipefd[1]);
    std::string body = parsing.getBody();
    if ( !write(input_pipefd[1], body.c_str(), body.length()) )
		throw std::runtime_error("CGI error: write body");
    close(input_pipefd[1]);
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


std::string	CGI::_readOutput(int output_pipefd[2])
{
    const int BUFFSIZE = 4096;
	char buffer[BUFFSIZE];
    bzero(buffer, BUFFSIZE);
	int bytesRead = 0;
	bytesRead = read(output_pipefd[0], buffer, BUFFSIZE);
	if (bytesRead < 0)
    {
        close(output_pipefd[0]);
		throw std::runtime_error("Error: CGI read");
    }
    else if (bytesRead == 0)
    {
        close(output_pipefd[0]);
        return "";
    }
    std::vector<char> bufferVector(buffer, buffer + bytesRead);
    close(output_pipefd[0]);
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
    std::cout  << "ENV:                 ";
    for (size_t i = 0; i < cgi.getEnv().size(); ++i) {
        std::cout  << cgi.getEnv()[i] << " ";
    }
    std::cout  << "\n";
    std::cout  << "ARGS:                 ";
    for (size_t i = 0; i < cgi.getArgs().size(); ++i) {
        std::cout  << cgi.getArgs()[i] << " ";
    }
    std::cout  << "\n";
}