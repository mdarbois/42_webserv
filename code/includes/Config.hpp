#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>

class Server {
  public:
      Server();
      Server(Server const &src);
      Server &operator=(Server const &rhs);
      ~Server();
  private:
    std::string _ip;
    unsigned int _clientMaxBodySize;
    unsigned int _port;
    unsigned int _host;
    std::string _server_names;
	  std::map<int, std::string> _errorPages;
	  std::string _key;
	  std::vector<std::string> _methods;
	  std::string _redirection;
	  std::string _root;
	  std::string _uploads;
	  bool _autoindex;
	  std::string _index;

};
#endif
