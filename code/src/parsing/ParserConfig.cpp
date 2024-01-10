#include "Server.hpp"

Server::Server(){
  std::cout << "Server default constructor called" << std::endl;
  _ip = "";
  _clientMaxBodySize = 1;
  _port = 0;
  _host = 0;
  _server_names = "";
  _errorPages[404] = "";
  _errorPages[405] = "";
  _errorPages[501] = "";
  _errorPages[413] = "";
  _errorPages[200] = "";
  //_key =
  //location
  _methods[0] = "GET";
  _methods[1] = "POST";
  _methods[2] = "DELETE";
  _redirection = "";
  _root = "";
  _uploads = "/uploads";
  _autoindex = false;
  _index = "index.html";
}


Server::Server(Server const &src) {
  if(this != &src)
  {
    this->_ip = src._ip;
    this->_clientMaxBodySize = src._clientMaxBodySize;
    this->_port = src._port;
    this->_host = src._host;
    this->_server_names = src._server_names;
    this->_errorPages = src._errorPages;
    this->_methods = src._methods;
    this->_redirection = src._redirection;
    this->_root = src._root;
    this->_uploads = src._uploads;
    this->_autoindex = src._autoindex;
    this->_index = src._index;
  }
  std::cout << "Server copy constructor called" << std::endl;
}

Server &Server::operator=(const Server &rhs) {
	if (this != &rhs){
      this->_ip = rhs._ip;
      this->_clientMaxBodySize = rhs._clientMaxBodySize;
      this->_port = rhs._port;
      this->_host = rhs._host;
      this->_server_names = rhs._server_names;
      this->_errorPages = rhs._errorPages;
      this->_methods = rhs._methods;
      this->_redirection = rhs._redirection;
      this->_root = rhs._root;
      this->_uploads = rhs._uploads;
      this->_autoindex = rhs._autoindex;
      this->_index = rhs._index;
    }

  std::cout << "Server copy assignment operator called" << std::endl;
	return *this;
}

Server::~Server() {
    std::cout << "Server destructor called" << std::endl;
}
