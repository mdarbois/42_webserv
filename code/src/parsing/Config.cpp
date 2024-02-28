#include "../../includes/Config.hpp"
#include "../../includes/ServerConfig.hpp"
#include "../../includes/LocationConfig.hpp"

Config::Config() {

  //std::cout << "Config default constructor called" << std::endl;
}

Config::Config(std::string path) {
	_foundServer = false;
  //std::cout << "Config fancy constructor called" << std::endl;
  if (path.empty() || path.length() == 0 || path.find(".txt") == std::string::npos)
    throw std::runtime_error("Config: Path not valid");
  //should we check more? access permissions to be checked (R_OK, W_OK, X_OK) or the existence (F_OK)?
  if (access(path.c_str(), R_OK) == -1)
    throw std::runtime_error("Config: File not accessible");
  std::ifstream configurationFile(path.c_str());
  if(!configurationFile || !configurationFile.is_open())
    throw std::runtime_error("Config: Error opening configuration file");
  _parse(configurationFile);
	/* std::map< unsigned int, std::string>::iterator it;
    for (it = _servers[0].getErrorPages().begin(); it != _servers[0].getErrorPages().end(); ++it) {
        std::cout << "CONFIG CONSTRUCTOR Key: " << it->first << ", Value: " << it->second << std::endl;
    } */
  configurationFile.close();
	if(!_arePortsDifferent(_servers))
		throw std::runtime_error("Config: servers can't listen to the same ports");
//_print();

}


Config::Config(Config const &src) {
  if(this != &src)
  {
	_servers = src._servers;
  }
    *this = src;
   // std::cout << "Config copy constructor called" << std::endl;
}

Config &Config::operator=(const Config &rhs) {
	if (this != &rhs){
        _servers = rhs._servers;
    }

  //std::cout << "Config copy assignment operator called" << std::endl;
	return *this;
}

Config::~Config() {
    //std::cout << "Config destructor called" << std::endl;
}

void Config::_parse(std::ifstream &configurationFile)
{
	std::string line;
	std::stringstream serverStream;
	std::map<std::string, LocationConfig> locations;
	while (std::getline(configurationFile, line))
	{
		trimSpaces(line);
		if (!checkSemiColon(line))
			throw std::runtime_error("Server: missing semicolons at the end of the lines");
		if (line.empty())
			continue;
		else if (line.find("#") != std::string::npos)
			trimComments(line);
		else if (line.find("server {") != std::string::npos)
		{
			_foundServer = true;
			serverStream.str("");
			serverStream.clear();
		}
		else if (_foundServer)
		{
			if(line.find("location") != std::string::npos)
			{
				std::string path = _extractPath(line, line.find("location"), strlen("location"));
				if (path.empty())
					throw std::runtime_error("Config: Location path can't be empty");
				std::stringstream elements(_extractElements(line, configurationFile));
				locations.insert(std::make_pair(path, LocationConfig(elements)));
				
			}
			else if (_foundServer && line.find("}") != std::string::npos)
			{
				ServerConfig server(serverStream, _servers.size(), locations);
				/* std::map< unsigned int, std::string>::iterator it;
				for (it = server.getErrorPages().begin(); it != server.getErrorPages().end(); ++it) {
					std::cout << "SERVER Key: " << it->first << ", Value: " << it->second << std::endl;
				} */
				_servers.push_back(server);
				locations.clear();
			}
			else if (_foundServer)
				serverStream << line << std::endl;
			else
				throw std::runtime_error("Config: Wrong keyword found");
		}
	}
	if (!_foundServer)
    	throw std::runtime_error("Config: No server found");
}

std::string Config::_extractElements(std::string &line, std::ifstream &configurationFile)
{
    size_t start = 0;
    size_t  end = 0;
    std::string substr;

    start = line.find_first_of("{", start) + 1;
    substr = line.substr(start, end - start);
    std::getline(configurationFile, line, '}');
	//printf("line=%s\n", (substr + line).c_str());

    return (substr + line);
}

std::string Config::_extractPath(std::string &line, size_t pos, size_t length)
{
	//if (pos != std::string::npos)
	line.erase(pos, length);
	line.resize(line.size() - 1);
	trimSpaces(line);
	return (line);
}

void    Config::_print(void) const
{
    for (size_t i = 0; i < servers().size(); ++i) 
    {
        std::cout   << servers()[i] << "\n";
    }
}

std::vector<ServerConfig> Config::getServers() const
{
	return (_servers);
}

std::vector<ServerConfig> Config::servers(void) const
{
    std::vector<ServerConfig>   extendedServers;

    for (size_t i = 0; i < _servers.size(); i++)
    {
        for (size_t j = 0; j < _servers[i].getPorts().size(); j++)
        {
            ServerConfig    tmp(_servers[i]);
			int port;

			port = _servers[i].getPorts()[j];
			tmp.setPort(port);
            extendedServers.push_back(tmp);
        }
    }
    return (extendedServers);
}

bool Config::_arePortsDifferent(const std::vector<ServerConfig>& servers) {
    std::vector<unsigned int> allPorts;

    // Collect all ports from all servers into a single vector
    for (size_t i = 0; i < servers.size(); ++i) {
        const std::vector<unsigned int>& ports = servers[i].getPorts();
        for (size_t j = 0; j < ports.size(); ++j) {
            allPorts.push_back(ports[j]);
        }
    }

    // Check for duplicate ports
    for (size_t i = 0; i < allPorts.size(); ++i) {
        unsigned int port = allPorts[i];
        for (size_t j = i + 1; j < allPorts.size(); ++j) {
            if (port == allPorts[j]) {
				//std::cout << port << std::endl;
                return false; // Found a duplicate port
            }
        }
    }

    // If no duplicate ports were found, return true
    return true;
}


