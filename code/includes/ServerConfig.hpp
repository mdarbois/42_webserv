#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <map>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>

class ServerConfig {
  public:
    ServerConfig();
    ServerConfig(std::stringstream &serverStream, int index, std::map<std::string, LocationConfig> locations);
    ServerConfig(ServerConfig const &src);
    ServerConfig &operator=(ServerConfig const &rhs);
    ~ServerConfig();


  private:
    unsigned int _port;
    std::vector<unsigned int> _ports;
    std::string _host;
    std::string _root;
    std::string _serverName;
    unsigned int _clientMaxBodySize;
    std::string _index;
    unsigned int _ip;
	  std::map<unsigned int, std::string> _errorPages;
    std::map<std::string, LocationConfig>  _locations;






	 

};
#endif
