#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP


#include "utils.hpp"


class LocationConfig
{

	public:
		LocationConfig();
		LocationConfig(std::stringstream &elements);
		LocationConfig( LocationConfig const & src );
		~LocationConfig();

		LocationConfig &		operator=( LocationConfig const & rhs );
		std::vector<std::string> _extractMethods(std::string &line, size_t pos, size_t length);
		void _setDefaultLocations();
		
		std::vector<std::string> getMethods(void) const;
		std::string getRedirection(void) const;
		std::string getRoot(void) const;
		std::string getIndex(void) const;
		bool    getAutoindex(void) const;
		std::string getUploads(void) const;

		/* class LocationConfigException : public std::exception {
        public:
          virtual const char* what(std::string) const throw()
          {
            return(_msg.c_str());
          }
        private:
          std::string _msg;
      }; */

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