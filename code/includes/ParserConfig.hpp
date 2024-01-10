#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string>
#include <vector>

class Server;

class Parsing
{
  public:
      Parsing();
      Parsing(std::string path);
      Parsing(Parsing const &src);
      Parsing &operator=(Parsing const &rhs);
      ~Parsing();
      void trimComments(std::string &content);
      void trimSpaces(std::string &content);
      void extractServers(std::string &content);
      size_t startServer(size_t start, std::string &content);
      size_t endServer(size_t start, std::string &content);
      class ParsingException : public std::exception {
        public:
          ParsingException(std::string message) throw()
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
    size_t _nbServer;
    std::vector<Server> _servers;
};
#endif
