#include "Parsing.hpp"
#include "Server.hpp"

Parsing::Parsing() {
  _nbServer = 0;
  std::cout << "Parsing default constructor called" << std::endl;
}

Parsing::Parsing(std::string path) {
  _nbServer = 0;
  std::cout << "Parsing fancy constructor called" << std::endl;
  if (path.empty() || path.length() == 0)
    throw ParsingException("Path not valid");
  //should we check more? access permissions to be checked (R_OK, W_OK, X_OK) or the existence (F_OK)?
  if (access(path.c_str(), R_OK) == -1)
    throw ParsingException("File not accessible");
  std::fstream configurationFile(path.c_str());
  if(!configurationFile || !configurationFile.is_open())
    throw ParsingException("Error opening configuration file");
  std::stringstream configStream;
  configStream << configurationFile.rdbuf();
  std::string content;
  content = configStream.str();
  if (content.empty())
    throw ParsingException("Empty file");
  trimComments(content);
  trimSpaces(content);
  extractServers(content);
  if (this->_servers.size() != this->_nbServer)
		throw ParsingException("Problem count servers");
	for (size_t i = 0; i < this->_nbServer; i++)
	{
		Server server;
		createServers(this->_servers[i], server);
		this->_servers.push_back(server);
	}
	if (this->_nbServer > 1)
		checkServers();
	return (0);
}

Parsing::Parsing(Parsing const &src) {
  if(this != &src)
  {
    *this = src;
  }
    std::cout << "Parsing copy constructor called" << std::endl;
}

Parsing &Parsing::operator=(const Parsing &rhs) {
	if (this != &rhs){
        _nbServer = rhs._nbServer;
    }

  std::cout << "Parsing copy assignment operator called" << std::endl;
	return *this;
}

Parsing::~Parsing() {
    std::cout << "Parsing destructor called" << std::endl;
}

void Parsing::trimComments(std::string &content)
{
  size_t pos_start;

	pos_start = content.find('#');
	while (pos_start != std::string::npos)
	{
		size_t pos_end;
		pos_end = content.find('\n', pos_start);
		content.erase(pos_start, pos_end - pos_start);
		pos_start = content.find('#');
	}
}

void Parsing::trimSpaces(std::string &content)
{
	size_t	i = 0;

	while (content[i] && isspace(content[i]))
		i++;
	content = content.substr(i);
	i = content.length() - 1;
	while (i > 0 && isspace(content[i]))
		i--;
	content = content.substr(0, i + 1);
}

void Parsing::extractServers(std::string &content)
{
  size_t start;
  size_t end;
  if (content.find("server") == std::string::npos)
    throw ParsingException("No server found");
  start = 0;
  end = 1;
  while (start != end && start < content.length())
	{
		start = startServer(start, content);
		end = endServer(start, content);
		if (start == end)
			throw ParsingException("Problem with scope");
		this->_servers.push_back(content.substr(start, end - start + 1));
		this->_nbServer++;
		start = end + 1;
	}
}

size_t Parsing::startServer (size_t start, std::string &content)
{
	size_t i;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  ParsingException("Wrong character");
	}
	if (!content[i])
		return (start);
	if (content.compare(i, 6, "server") != 0)
		throw ParsingException("Wrong character");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  ParsingException("Wrong character");

}

size_t Parsing::endServer (size_t start, std::string &content)
{
	size_t	i;
	size_t	size;

	size = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			size++;
		if (content[i] == '}')
		{
			if (!size)
				return (i);
			size--;
		}
	}
	return (start);
}


/*createServers
key == "listen"
key == "location" ?
key == "host"
key == "root"
key == "errorPages"
key == "clientMaxBodySize"
key == "server_names" ?
key == "index"
key == "autoindex"
//key == "ip"
//key == "ports"
// key == "methods"
// key == "redirection"
// key == "uploads"
*/
