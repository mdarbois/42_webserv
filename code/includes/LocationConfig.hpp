#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <string>
#include <vector>

class LocationConfig
{

	public:

		LocationConfig();
		LocationConfig(std::stringstream &elements);
		LocationConfig( LocationConfig const & src );
		~LocationConfig();

		LocationConfig &		operator=( LocationConfig const & rhs );

	private:
	  std::vector<std::string> _methods;
	  std::string _redirection; // called return
	  std::string _root;
	  std::string _index;
	  bool _autoindex;
	  std::string _uploads; // cgi_path
	   

};

std::ostream &			operator<<( std::ostream & o, LocationConfig const & i );

#endif /* ************************************************** LOCATIONCONFIG_H */