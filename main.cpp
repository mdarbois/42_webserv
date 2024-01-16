
#include "code/includes/ServerConfig.hpp"
#include "code/includes/Config.hpp"
#include "code/includes/LocationConfig.hpp"
#include "code/includes/utils.hpp"
#include "code/includes/ClientSocket.hpp"
#include "code/includes/ParserHTTP.hpp"
#include "code/includes/Server.hpp"
#include "code/includes/ServerSocket.hpp"
#include "code/includes/Socket.hpp"

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
        path = "./resources/configuration_file.txt";
      else
        path = argv[1];
      Config config(path);
      Server server(config);
      server.run();
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return (1);
    }
  }
  

  return (0);
}
