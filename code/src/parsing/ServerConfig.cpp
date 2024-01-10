#include "../../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(){

  std::cout << "ServerConfig default constructor called" << std::endl;
}

ServerConfig::ServerConfig(std::stringstream &serverStream, int index, std::map<std::string, LocationConfig> locations) : _port(0), _ports(), _host (""), _root(""), _serverName(""), _clientMaxBodySize(0), _index(""), _ip(""), _errorPages(""), _location = locations;
{
  _parseServer(serverStream);
  _ip = _ipConvert(_host);
  _setDefault(index);
 // validation
}

ServerConfig::ServerConfig(ServerConfig const &src) {
    this->_port = src._port;
    this->_ports = src._ports;
    this->_host = src._host;
    this->_root = src._root;
    this->_serverName = src._serverName;
    this->_clientMaxBodySize = src._clientMaxBodySize;
    this->_index = src._index;
    this->_ip = src._ip;
    this->_errorPages = src._errorPages;
    this->_locations = src._locations;
  std::cout << "ServerConfig copy constructor called" << std::endl;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &rhs) {
	if (this != &rhs){
      this->_port = rhs._port;
      this->_ports = rhs._ports;
      this->_host = rhs._host;
      this->_root = rhs._root;
      this->_serverName = rhs._serverName;
      this->_clientMaxBodySize = rhs._clientMaxBodySize;
      this->_index = rhs._index;
      this->_ip = rhs._ip;
      this->_errorPages = rhs._errorPages;
      this->_locations = rhs._locations;
    }
  std::cout << "ServerConfig copy assignment operator called" << std::endl;
	return *this;
}

ServerConfig::~ServerConfig() {
    std::cout << "ServerConfig destructor called" << std::endl;
}

void ServerConfig::_parseServer(std::stringstream &serverStream)
{
   std::string line;

    while (std::getline(serverStream, line, ';')) 
    {
        trimSpaces(line);
        if (line.empty())
            continue;
        if (line.find("listen") != std::string::npos)
            _ports = _extractPorts(line, line.find("listen"), strlen("listen"));
        else if (line.find("server_name") != std::string::npos)
            _serverName = _extractString(line, line.find("server_name"), strlen("server_name"));
        else if (line.find("host") != std::string::npos)
            _host = _extractString(line, line.find("host"), strlen("host"));
        else if (line.find("root") != std::string::npos)
            _root = _extractString(line, line.find("root"), strlen("root"));
        else if (line.find("index") != std::string::npos)
            _index = _extractString(line, line.find("index"), strlen("index"))
        else if (line.find("error_page") != std::string::npos)
            _parseErrorPages(line);
        else if (line.find("client_max_body_size") != std::string::npos)
             _clientMaxBodySize = _extractInt(line, line.find("client_max_body_size"), strlen("client_max_body_size"));
        else
            throw (std::exception("Error, unknown keyword"));
    }
}

std::vector<int> ServerConfig::_extractPorts(std::string &line, size_t pos, size_t length) 
{
  if (pos != std::string::npos)
		line.erase(pos, length);
	trimSpaces(line);
  line.erase(line.find(";"), 1);
	std::vector<int>    values;
  std::istringstream  lineStream(line);
  int portNumber;
  lineStream >> portNumber;
	values.push_back(portNumber);
	return values;
}

std::string ServerConfig::_extractString(std::string &line, size_t pos, size_t length)
{
  if (pos != std::string::npos)
		line.erase(pos, length);
  line.erase(line.find(";"), 1);
 return (line);
}

unsigned int ServerConfig::_extractInt(std::string &line, size_t pos, size_t length)
{
   if (pos != std::string::npos)
		line.erase(pos, length);
	trimSpaces(line);
  line.erase(line.find(";"), 1);
  std::istringstream  lineStream(line);
  int intNumber;
  lineStream >> intNumber;
  return intNumber;
}

void ServerConfig::_parseErrorPages()