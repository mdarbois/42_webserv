
#include "Parsing.hpp"

int main(int argc, char **argv)
{
  if (argc < 1 || argc > 2)
  {
    std::cout << "Invalid number of arguments" << std::endl;
    return (1);
  }
  else
  {
    try
    {
      std::string path;
      if (argc == 1)
        path = "configuration_file.txt";
      else
        path = argv[1];
      Parsing parsing(path);

    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return (1);
    }
  }




  return (0);
}
