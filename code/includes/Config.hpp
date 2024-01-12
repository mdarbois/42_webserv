#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <vector>
#include "utils.hpp"
#include <stdexcept>

class ServerConfig;

class Config
{
  public:
      Config();
      Config(std::string path);
      Config(Config const &src);
      Config &operator=(Config const &rhs);
      ~Config();
/* 
      class ConfigException : public std::exception {
        public:
          ConfigException(std::string message) throw()
          {
            _msg = "Config Parsing problem: " + message;
          };
          virtual const char* what() const throw()
          {
            return(_msg.c_str());
          }
        private:
          std::string _msg;
      }; */
  private:
    bool _foundServer;
    void _parse(std::ifstream &configurationFile);
    std::string _extractElements(std::string &line, std::ifstream &configurationFile);
    std::string _extractPath(std::string &line, size_t pos, size_t length);
    std::vector<ServerConfig> _servers;
};
#endif
