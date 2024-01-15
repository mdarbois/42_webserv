#include "../../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(){

  std::cout << "ServerConfig default constructor called" << std::endl;
}

ServerConfig::ServerConfig(std::stringstream &serverStream, int index, std::map<std::string, LocationConfig> locations) : _port(0), _ports(), _host (""), _root(""), _serverName(""), _clientMaxBodySize(0), _ip(0), _errorPages(), _locations(locations)
{
  	std::cout << "ServerConfig fancy constructor called" << std::endl;
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
            _serverName = extractString(line, line.find("server_name"), strlen("server_name"));
        else if (line.find("host") != std::string::npos)
            _host = extractString(line, line.find("host"), strlen("host"));
        else if (line.find("root") != std::string::npos)
            _root = extractString(line, line.find("root"), strlen("root"));
        else if (line.find("error_page") != std::string::npos)
        {
          line = extractString(line, line.find("error_page"), strlen("error_page"));
          _parseErrorPages(line);
        }
        else if (line.find("client_max_body_size") != std::string::npos)
             _clientMaxBodySize = extractInt(line, line.find("client_max_body_size"), strlen("client_max_body_size"));
        else
            throw (std::runtime_error("ServerConfig: Error, unknown keyword in server"));
    }
}

std::vector<unsigned int> ServerConfig::_extractPorts(std::string &line, size_t pos, size_t length) 
{
  if (pos != std::string::npos)
		line.erase(pos, length);
  if (line.find(";") != std::string::npos)
  	line.erase(line.find(";"), 1);
  std::vector<unsigned int>    values;
	trimSpaces(line);
  std::istringstream  lineStream(line);
  int portNumber;
  lineStream >> portNumber;
	values.push_back(portNumber);
	return values;
}

void ServerConfig::_parseErrorPages(std::string &line)
{
    std::istringstream iss(line);
    int statusCode;
    std::string path;

    if (iss >> statusCode >> path)
        _errorPages[statusCode] = path;
}

unsigned int ServerConfig::_ipConvert(const std::string host)
{
  std::vector<unsigned int>   parts;
  std::istringstream          ipStream(host);
  std::string                 part;
  unsigned int                ip;
  int                         count = 0;
  while (std::getline(ipStream, part, '.')) 
    {
        count++;
        unsigned int num = static_cast<unsigned int>(atoi(part.c_str()));
        parts.push_back(num);
    }
    if (count != 4)
        throw(std::runtime_error("ServerConfig: Error, IP address has invalid format"));
    ip = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
    return (ip);
}

void ServerConfig::_setDefault(int index)
{
  _setDefaultErrorPages();
  if (_serverName.empty())
  {
    std::stringstream ss;
    ss << index;
    _serverName = "john_doe" + ss.str();
  }
  if (_clientMaxBodySize == 0)
    _clientMaxBodySize = 42000000;
}

void ServerConfig::_setDefaultErrorPages()
{
  _errorPages[200] = "/error_pages/200.html";
  _errorPages[404] = "/error_pages/404.html";
  _errorPages[405] = "/error_pages/405.html";
  _errorPages[413] = "/error_pages/413.html";
  _errorPages[501] = "/error_pages/501.html";
}

unsigned int ServerConfig::getPort() const
{
  return (_port);
}

std::vector<unsigned int> ServerConfig::getPorts() const
{
  return (_ports);
}

std::string ServerConfig::getHost(void) const
{
    return (_host);
}

std::string ServerConfig::getRoot(void) const
{
    return (_root);
}

std::string ServerConfig::getServerName(void) const
{
    return (_serverName);
}

unsigned int ServerConfig::getClientMaxBodySize(void) const
{
    return (_clientMaxBodySize);
}

unsigned int ServerConfig::getIp(void) const
{
  return (_ip);
}

std::map<unsigned int, std::string>  ServerConfig::getErrorPages(void) const
{
  std::map<unsigned int, std::string>   copyErrorPages(_errorPages);

    for (std::map<unsigned int, std::string>::iterator it = copyErrorPages.begin();
            it != copyErrorPages.end(); ++it)
    {
        it->second = _root + it->second;
    } 
    return (copyErrorPages);
}

std::map<std::string,LocationConfig> ServerConfig::getLocations(void) const
{
    return (_locations);
}

void	ServerConfig::setPort(const int &p)
{
	_port = p;
}

std::ostream& operator<<(std::ostream& os, const ServerConfig& serverConfig) 
{
  os << "\033[34m---------- ServerConfig:-----------\033[0m\n";
    os << "Port(s):                 ";
    for (size_t i = 0; i < serverConfig.getPorts().size(); ++i) {
        os << serverConfig.getPorts()[i] << " ";
    }
    os << "\n";
    os << "Server Name:             " << serverConfig.getServerName() << "\n";
    os << "Host:                    " << serverConfig.getHost() << "\n";
    os << "IP (int):                " << serverConfig.getIp() << "\n";
    os << "Root:                    " << serverConfig.getRoot() << "\n";
    os << "Client Max Body Size:    " << serverConfig.getClientMaxBodySize() << "\n";
    os << "Error Pages:\n";
    std::map<unsigned int, std::string>   errorPageCopy(serverConfig.getErrorPages());
    for (std::map<unsigned int, std::string>::const_iterator it = errorPageCopy.begin();
            it != errorPageCopy.end(); ++it) 
    {
        os << " " << it->first << ": " << it->second << "\n";
    }
    os << "\n";
    std::map<std::string, LocationConfig> locationCopy(serverConfig.getLocations());
    for (std::map<std::string, LocationConfig>::const_iterator it = locationCopy.begin(); 
            it != locationCopy.end(); ++it) {
        os << "\033[92m---------- Location: " << it->first << " -----------\033[0m\n";
        os << it->second;
        os << std::endl;
    }
    os << "\n";
    return os;
}