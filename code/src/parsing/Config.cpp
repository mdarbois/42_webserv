#include "../../includes/Config.hpp"
#include "../../includes/ServerConfig.hpp"
#include "../../includes/LocationConfig.hpp"

Config::Config() {

  std::cout << "Config default constructor called" << std::endl;
}

Config::Config(std::string path) {
	_foundServer = false;
  std::cout << "Config fancy constructor called" << std::endl;
  if (path.empty() || path.length() == 0 || path.find(".txt") == std::string::npos)
    throw ConfigException("Path not valid");
  //should we check more? access permissions to be checked (R_OK, W_OK, X_OK) or the existence (F_OK)?
  if (access(path.c_str(), R_OK) == -1)
    throw ConfigException("File not accessible");
  std::ifstream configurationFile(path.c_str());
  if(!configurationFile || !configurationFile.is_open())
    throw ConfigException("Error opening configuration file");

  _parse(configurationFile);
  
  configurationFile.close();

}

Config::Config(Config const &src) {
  if(this != &src)
  {
    *this = src;
  }
    std::cout << "Config copy constructor called" << std::endl;
}

Config &Config::operator=(const Config &rhs) {
	if (this != &rhs){
        _servers = rhs._servers;
    }

  std::cout << "Config copy assignment operator called" << std::endl;
	return *this;
}

Config::~Config() {
    std::cout << "Config destructor called" << std::endl;
}

void Config::_parse(std::ifstream &configurationFile)
{
	std::string line;
	std::stringstream serverStream;
	std::map<std::string, LocationConfig> locations;
	while (std::getline(configurationFile, line))
	{
		if (line.empty())
			continue;
		else if (line.find("#") != std::string::npos)
			trimComments(line);
		trimSpaces(line);
		else if (line.find("server {") != std::string::npos)
		{
			foundServer = true;
			serverStream.str("");
			serverStream.clear();
		}
		else if (foundServer && line.find("location") != std::string::npos)
		{
			std::string path = _extractPath(line, line.find("location"), strlen("location"));
			std::stringstream elements(_extractElements(line, configurationFile));
			locations.insert(std::make_pair(path, LocationConfig(elements)));
		}
		else if (foundServer && line.find("}") != std::string::npos)
		{
			ServerConfig server(serverStream, _servers.size(), locations);
			_servers.push_back(server);
			locations.clear();
		}
		else if (foundServer)
			serverStream << line << std::endl;
		else
			throw ConfigException("Wrong keyword found");
	}
	if (!foundServer)
    	throw ConfigException("No server found");
}

std::string Config::_extractElements(std::string &line, std::ifstream &configurationFile)
{
    size_t start = 0;
    size_t  end = 0;
    std::string substr;

    start = line.find_first_of("{", start) + 1;
    substr = line.substr(start, end - start);
    std::getline(configurationFile, line, '}');
    return (substr + line);
}

std::string Config::_extractPath(std::string &line, size_t pos, size_t length)
{
	if (pos != std::string::npos)
		line.erase(pos, length);
	trimSpaces(line);
	return (line);
}



/* void Config::extractServers(std::string &content)
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
} */

/* size_t Config::startServer (size_t start, std::string &content)
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

} */

/* size_t Config::endServer (size_t start, std::string &content)
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
 */

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
