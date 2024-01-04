#ifndef PARSING_HPP
#define PARSING_HPP

#include <iostream>
#include <fstream>
# include <sstream>
#include <unistd.h>

class Parsing
{
  public:
      Parsing();
      Parsing(std::string path);
      Parsing(Parsing const &src);
      Parsing &operator=(Parsing const &rhs);
      ~Parsing();
  private:
    int _nbServer;
};
#endif
