#include "../../includes/LocationConfig.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

LocationConfig::LocationConfig() : _methods(), _redirection(""), _root(""), _index(""), _autoindex(false),  _uploads("") 
{
	//std::cout << "LocationConfigonfig default constructor called" << std::endl;
}

LocationConfig::LocationConfig(std::stringstream &elements)
{
	//std::cout << "LocationConfig fancy constructor called" << std::endl;
	std::string line;
	while (std::getline(elements, line))
	{
		trimSpaces(line);
		if (!checkSemiColon(line))
			throw std::runtime_error("Location: missing semicolons at the end of the lines");
		if (line.empty())
			continue;
		if (line.find("methods") != std::string::npos)
			_methods = _extractMethods(line, line.find("methods"), strlen("methods"));
		else if (line.find("redirection") != std::string::npos)
			_redirection = extractString(line, line.find("redirection"), strlen("redirection"));
		else if (line.find("root") != std::string::npos)
			_root = extractString(line, line.find("root"), strlen("root"));
		else if (line.find("index") != std::string::npos)
			_index = extractString(line, line.find("index"), strlen("index"));
		else if (line.find("autoindex") != std::string::npos)
		{
			if (extractString(line, line.find("autoindex"), strlen("autoindex")) == "on")
				_autoindex = true;
		}
		else if (line.find("uploads") != std::string::npos)
			_uploads = extractString(line, line.find("uploads"), strlen("uploads"));
		else
            throw (std::runtime_error("Location: Error, unknown keyword in locations"));
	}
	_validate();
}

LocationConfig::LocationConfig( const LocationConfig & src )
{
	_methods = src._methods;
	_redirection = src._redirection;
	_root = src._root;
	_index = src._index;
	_autoindex = src._autoindex;
	_uploads = src._uploads;
	 //std::cout << "LocationConfig copy constructor called" << std::endl;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

LocationConfig::~LocationConfig()
{
	//std::cout << "LocationConfig destructor called" << std::endl;
}

std::vector<std::string> LocationConfig::_extractMethods(std::string &line, size_t pos, size_t length) 
{
	if (pos != std::string::npos)
		line.erase(pos, length);
	if (line.find(";") != std::string::npos)
  		line.erase(line.find(";"), 1);
	
	size_t	i = 0;
	std::vector<std::string> methods;
	while (line[i])
	{
		trimSpaces(line);
		//printf("line=%s\n", line.c_str());
		if (line.find("GET") != std::string::npos)
		{
			methods.push_back("GET");
			i = line.find("GET") + strlen("GET");
			line = extractString(line, 0, strlen("GET"));
			//printf("line after get=%s i=%lu\n", line.c_str(),i);
			continue;
		}
		else if (line.find("POST") != std::string::npos)
		{
			methods.push_back("POST");
			i = line.find("POST") + strlen("POST");
			line = extractString(line, 0, strlen("POST"));
			//printf("line after post=%s i=%lu\n", line.c_str(),i);
			continue;
		}
		else if (line.find("DELETE") != std::string::npos)
		{
			methods.push_back("DELETE");
			i = line.find("DELETE") + strlen("DELETE");
			line = extractString(line, 0, strlen("DELETE"));
			continue;
		}
		else
			throw (std::runtime_error("Location: Error, unknown keyword in location/methods"));
	i++;
	}
	return methods;
}

/* std::pair<int, std::string> LocationConfig::_extractRedirection(std::string &line, size_t pos, size_t length) 
{
	if (pos != std::string::npos)
		line.erase(pos, length);
	if (line.find(";") != std::string::npos)
  		line.erase(line.find(";"), 1);
	std::string sCode = line.erase(0,3);
	std::istringstream iss(line);
	std::istringstream codess(sCode);
    std::string str;
    std::pair<int, std::string>  newPair;
	int code;
	codess >> code;
    if (iss >> code >> str)
    {
        return (std::make_pair(code, str));
    }
    else
    {
    	throw (std::runtime_error("Error, incomplete key value pair"));
    }
} */

void LocationConfig::_validate()
{
	if (_methods.empty())
		throw std::runtime_error("Location should specify the methods");
	/*if (_root.empty() || _uploads.empty())
		throw std::runtime_error("Location should specify the root and uploads");
	 if (!_index.empty() || _autoindex == true || _redirection.empty())
        throw std::runtime_error("cannot specify index, autoindex or return in CGI");
	 */
	if (!_redirection.empty())
	{
	std::string copy = _redirection;
	std::string number = copy.substr(0,3);
	std::istringstream iss(number);
    int intValue;
	iss >> intValue;
	if (intValue != 0 && (intValue < 301 || intValue > 308))
		throw std::runtime_error("HTTP status code for redirection is out of range");
	}
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

LocationConfig &				LocationConfig::operator=( LocationConfig const & rhs )
{
	if ( this != &rhs )
	{
		_methods = rhs._methods;
		_redirection = rhs._redirection;
		_root = rhs._root;
		_index = rhs._index;
		_autoindex = rhs._autoindex;
		_uploads = rhs._uploads;
	}
	std::cout << "LocationConfig copy assignment operator called" << std::endl;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, LocationConfig const & i )
{
	o << "Methods: ";
    for (size_t j = 0; j < i.getMethods().size(); ++j) {
        o << i.getMethods()[j];
        if (j < i.getMethods().size() - 1) {
            o << ", ";
        }
    }
    o << "\n";
	o << "Redirection:          " << i.getRedirection() << "\n";
    o << "Root:            " << i.getRoot() << "\n";
    o << "Index:           " << i.getIndex() << "\n";
    o << "Autoindex:       " << (i.getAutoindex() ? "On" : "Off") << "\n";
    o << "Uploads:        " << i.getUploads() << "\n";
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::vector<std::string> LocationConfig::getMethods(void) const
{
    return (_methods);
}

std::string LocationConfig::getRedirection(void) const
{
    return (_redirection);
}

std::string LocationConfig::getRoot(void) const
{
    return (_root);
}

std::string LocationConfig::getIndex(void) const
{
    return (_index);
}

bool    LocationConfig::getAutoindex(void) const
{
    return (_autoindex);
}

std::string LocationConfig::getUploads(void) const
{
    return (_uploads);
}


/* ************************************************************************** */