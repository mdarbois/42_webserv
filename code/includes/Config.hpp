#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string>
#include <vector>

class ServerConfig;

class Config
{
  public:
      Config();
      Config(std::string path);
      Config(Config const &src);
      Config &operator=(Config const &rhs);
      ~Config();
      void trimComments(std::string &content);
      void trimSpaces(std::string &content);
      class ConfigException : public std::exception {
        public:
          ConfigException(std::string message) throw()
          {
            _msg = "Parsing problem: " + message;
          };
          virtual const char* what() const throw()
          {
            return(_msg.c_str());
          }
        private:
          std::string _msg;
      };
  private:
    bool _foundServer;
    void _parse(std::ifstream &configurationFile);
    std::string _extractElements(std::string &line, std::ifstream &configurationFile);
    std::string _extractPath(std::string &line);
    std::vector<ServerConfig> _servers;
};
#endif
