#include "Parsing.hpp"

Parsing::Parsing() {
  _nbServer = 0;
  std::cout << "Parsing default constructor called" << std::endl;
}

Parsing::Parsing(std::string path) {
  _nbServer = 0;
  std::cout << "Parsing fancy constructor called" << std::endl;
  if (path.empty() || path.length() == 0)
	{
    std::cout << "Path not valid" << std::endl;
    exit(1);
  }
  //should we check more? access permissions to be checked (R_OK, W_OK, X_OK) or the existence (F_OK)?
  if (access(path.c_str(), R_OK) == -1)
  {
    std::cout << "File not accessible" << std::endl;
    exit(1);
  }
  std::fstream configurationFile(path.c_str());
  if(!configurationFile || !configurationFile.is_open())
  {
    std::cout << "Error opening configuration file" << std::endl;
    exit(1);
  }
  // or while loop
  std::stringstream configStream;
  configStream << configurationFile.rdbuf();
  std::string content;
  content = configStream.str();
  if (content.empty())
  {
    std::cout << "Empty file" << std::endl;
    exit(1);
  }
  //remove comments
  //remove spaces
  //countservers
  //extract
}

Parsing::Parsing(Parsing const &src) {
    *this = src;
    std::cout << "Parsing copy constructor called" << std::endl;
}

Parsing &Parsing::operator=(const Parsing &rhs) {
	if (this != &rhs){
        _nbServer = rhs._nbServer;
    }

  std::cout << "Parsing copy assignment operator called" << std::endl;
	return *this;
}

Parsing::~Parsing() {
    std::cout << "Parsing destructor called" << std::endl;
}
