#include "../../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(){

  //std::cout << "ServerConfig default constructor called" << std::endl;
}

ServerConfig::ServerConfig(std::stringstream &serverStream, int index, std::map<std::string, LocationConfig> locations) : _port(0), _ports(), _host (""), _root(""), _serverName(""), _clientMaxBodySize(0), _ip(0), _locations(locations)
{
  	//std::cout << "ServerConfig fancy constructor called" << std::endl;
  _parseServer(serverStream);
  _ip = _ipConvert(_host);
  _setDefault(index);
  _validation();
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
    //std::cout << _locations["/"]<< std::endl;
  //std::cout << "ServerConfig copy constructor called" << std::endl;
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
  //std::cout << "ServerConfig copy assignment operator called" << std::endl;
	return *this;
}

ServerConfig::~ServerConfig() {
    //std::cout << "ServerConfig destructor called" << std::endl;
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
          _extractPorts(line, line.find("listen"), strlen("listen"));
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
          /*   std::map< unsigned int, std::string>::iterator it;
				for (it = getErrorPages().begin(); it != getErrorPages().end(); ++it) {
					std::cout << "PARSE SERVER Key: " << it->first << ", Value: " << it->second << std::endl;
				} */
        }
        else if (line.find("client_max_body_size") != std::string::npos)
             _clientMaxBodySize = extractInt(line, line.find("client_max_body_size"), strlen("client_max_body_size"));
        else
            throw (std::runtime_error("ServerConfig: Error, unknown keyword in server"));
    }
}

void ServerConfig::_extractPorts(std::string &line, size_t pos, size_t length)
{
  if (pos != std::string::npos)
		line.erase(pos, length);
  if (line.find(";") != std::string::npos)
  	line.erase(line.find(";"), 1);
  trimSpaces(line);
  std::istringstream  lineStream(line);
  int portNumber;
  lineStream >> portNumber;
	_ports.push_back(portNumber);
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
  //_setDefaultLocations();
  if (_serverName.empty())
  {
    std::stringstream ss;
    ss << index;
    _serverName = "john_doe" + ss.str();
  }
  if (_clientMaxBodySize == 0)
    _clientMaxBodySize = 42000000;
}

void ServerConfig::_setDefaultLocations()
{
  std::map<std::string, LocationConfig>::const_iterator   it;

    it = _locations.find("/");
    if (it == _locations.end())
    {
        LocationConfig          rootLocation;
        std::vector<std::string> methods;

        methods.push_back("GET");
        methods.push_back("POST");
        //rootLocation.setMethods(methods);
        _locations.insert(std::make_pair("/", rootLocation));
    }
    it = _locations.find("/uploads/");
    if (it == _locations.end())
    {
        LocationConfig           uploadsLocation;
        std::vector<std::string>  methods;

        methods.push_back("GET");
        methods.push_back("POST");
        methods.push_back("DELETE");
        //newLocation.setMethods(methods);
        _locations.insert(std::make_pair("/uploads/", uploadsLocation));
    }
}
void ServerConfig::_setDefaultErrorPages()
{
  if (_errorPages[400] == "")
    _errorPages[400] = "/400.html";
  if (_errorPages[403] == "")
    _errorPages[403] = "/403.html";
  if (_errorPages[404] == "")
    _errorPages[404] = "/404.html";
  if (_errorPages[408] == "")
    _errorPages[408] = "/408.html";
  if (_errorPages[413] == "")
    _errorPages[413] = "/413.html";
  if (_errorPages[500] == "")
    _errorPages[500] = "/500.html";
}

void ServerConfig::_validation(void) const
{
  if (_ports.size() == 0)
    throw std::runtime_error("no port specified");
  for (size_t i = 0; i < _ports.size(); ++i) {
    if (_ports[i] < 1 || _ports[i] > 65535)
      throw std::runtime_error("Port needs to be between 1 and 65.535");
  }
  if (_host.empty())
    throw std::runtime_error("no IP (host) specified");
  if (_root.empty())
        throw std::runtime_error("no root specified");
  for (std::vector<unsigned int>::const_iterator it = _ports.begin(); it != _ports.end(); it++)
    {
      std::vector<unsigned int>::const_iterator duplicate = std::find(it + 1, _ports.end(), *it);
      if (duplicate != _ports.end())
        throw std::runtime_error("duplicate ports");
    }
  if (_locations.empty())
    throw std::runtime_error("please provide at least one location");
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
  /* std::map<unsigned int, std::string>   copyErrorPages(_errorPages);

    for (std::map<unsigned int, std::string>::iterator it = copyErrorPages.begin();
            it != copyErrorPages.end(); ++it)
    {
        it->second = (it->second).c_str();
    }
    return (copyErrorPages); */
    return(_errorPages);
}

std::map<std::string,LocationConfig> ServerConfig::getLocations(void) const
{
    return (_locations);
}

std::string ServerConfig::getLocationPath(std::string requestPath)
{
 
  size_t dot = requestPath.find_last_of('.');
  std::string file;
  //file
  if (dot != std::string::npos)
  {
    file = requestPath.substr(dot);
    for (std::map<std::string, LocationConfig>::const_iterator it = _locations.begin();
            it != _locations.end(); ++it)
        {
            if (it->first == file)
                return (file);
        }
  }
  //directory
    std::string dir = requestPath;
    while (!dir.empty())
    {
      
        size_t slash = dir.find_last_of('/');
        dir = dir.substr(0, slash + 1);
        std::map<std::string, LocationConfig>::const_iterator it ;
        for (it = _locations.begin(); it != _locations.end(); ++it)
        {
            if (it->first == dir)
                return (it->first);
        }
        if (it == _locations.end() && isSlashAtEnd(dir) && !dir.empty())
          dir.erase(dir.end() - 1);
    }
  
  return ("");

}
std::string ServerConfig::getLocationRoot(std::string path, std::string requestPath)
{
  std::string pathRoot;
  std::string root;

  root = _locations[path].getRoot();
  if (root.empty())
    root = _root;
  if (!_locations[path].getUploads().empty())
    return(root + requestPath);
  pathRoot = requestPath.substr(path.length(), requestPath.length());


    if (requestPath[requestPath.length() - 1 ] == '/' &&
            _locations[path].getAutoindex() == true)
    {
		if (pathRoot[pathRoot.size() - 1 == '/'])
            return (root + pathRoot);
        else
            return (root + pathRoot + "/");
    }
   //check if Uri already specified a file, if not add based on "index"
    /* if (pathRoot.empty() || pathRoot == "/")
    {
        fileName = _locations[path].getIndex();
        if (fileName.empty())
            fileName =  "/" + _index;
    } */

    if (pathRoot[0] != '/')
        pathRoot = "/" + pathRoot;
    return (_root + pathRoot); // + fileName
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
