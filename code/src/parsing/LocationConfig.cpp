#include "../../includes/LocationConfig.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

LocationConfig::LocationConfig() : _methods(), _redirection(""), _root(""), _index(""), _autoindex(false),  _uploads("") 
{
	
}

LocationConfig::LocationConfig(std::stringstream &elements)
{
	std::string line;
	while (std::getline(elements, line))
	{
		trimSpaces(line);
		if (line.empty())
			continue;
		if (line.find("methods") != std::string::npos)
			_methods = _extractMethods(line, line.find("methods"), strlen("methods"));
		else if (line.find("redirection") != std::string::npos)
			_redirection = _extractString(line, line.find("redirection"), strlen("redirection"));
		else if (line.find("root") != std::string::npos)
			_root = _extractString(line, line.find("root"), strlen("root"));
		else if (line.find("index") != std::string::npos)
			_index = _extractString(line, line.find("index"), strlen("index"));
		else if (line.find("autoindex") != std::string::npos)
			_autoindex = _extractString(line, line.find("autoindex"), strlen("autoindex"));
		else if (line.find("uploads") != std::string::npos)
			_uploads = _extractString(line, line.find("uploads"), strlen("uploads"));
		else
            throw (std::exception("Error, unknown keyword"));
	}
}

LocationConfig::LocationConfig( const LocationConfig & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

LocationConfig::~LocationConfig()
{
}

std::vector<std::string> LocationConfig::_extractMethods(std::string &line, size_t pos, size_t length) 
{
	if (pos != std::string::npos)
		line.erase(pos, length);
	trimSpaces(line);
  	line.erase(line.find(";"), 1);
	size_t	i = 0;
	while (line[i])
	{
		std::vector<std::string> methods;
		if (line.find("GET") != std::string::npos)
		{
			methods.push_back("GET");
			line.extractString(line, i, strlen("GET"));
		}
		else if (line.find("POST") != std::string::npos)
		{
			methods.push_back("POST");
		}
		else if (line.find("DELETE") != std::string::npos)
		{
			methods.push_back("DELETE");
		}
		else
		ERROR
		i++;
	}

	return methods;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

LocationConfig &				LocationConfig::operator=( LocationConfig const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, LocationConfig const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */