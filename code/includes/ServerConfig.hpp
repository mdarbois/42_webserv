#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <map>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
#include "LocationConfig.hpp"
#include "utils.hpp"
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

class ServerConfig {
  public:
    ServerConfig();
    ServerConfig(std::stringstream &serverStream, int index, std::map<std::string, LocationConfig> locations);
    ServerConfig(ServerConfig const &src);
    ServerConfig &operator=(ServerConfig const &rhs);
    ~ServerConfig();
    void _parseServer(std::stringstream &serverStream);
    void _extractPorts(std::string &line, size_t pos, size_t length);
    void _parseErrorPages(std::string &line);
    unsigned int _ipConvert(const std::string host);
    void _setDefault(int index);
    void _setDefaultErrorPages();
    void _setDefaultLocations();

    unsigned int getPort() const;
    std::vector<unsigned int> getPorts() const;
    std::string getHost(void) const;
    std::string getRoot(void) const;
    std::string getServerName(void) const;
    unsigned int getClientMaxBodySize(void) const;
    unsigned int getIp(void) const;
    std::map<unsigned int, std::string> getErrorPages(void) const;
    std::map<std::string,LocationConfig> getLocations(void) const;
    void _validation(void) const;
    void	setPort(const int &p);

 /*    class ServerConfigException : public std::exception {
        public:
          ServerConfigException(std::string message) throw()
          {
            _msg = "Config Server problem: " + message;
          };
          virtual const char* what() const throw()
          {
            return(_msg.c_str());
          }
        private:
          std::string _msg;
      }; */
  private:
    unsigned int _port;
    std::vector<unsigned int> _ports;
    std::string _host;
    std::string _root;
    std::string _serverName;
    unsigned int _clientMaxBodySize;
    unsigned int _ip;
	  std::map<unsigned int, std::string> _errorPages;
    std::map<std::string, LocationConfig>  _locations;


};
std::ostream& operator<<(std::ostream& os, const ServerConfig& serverConfig);
#endif
