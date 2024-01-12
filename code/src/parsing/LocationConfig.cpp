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
	while (std::getline(elements, line, ';'))
	{
		trimSpaces(line);
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
	_setDefaultLocations();
}

LocationConfig::LocationConfig( const LocationConfig & src )
{
	_methods = src._methods;
	_redirection = src._redirection;
	_root = src._root;
	_index = src._index;
	_autoindex = src._autoindex;
	_uploads = src._uploads;
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
	std::vector<std::string> methods;
	while (line[i])
	{
		if (line.find("GET") != std::string::npos)
		{
			methods.push_back("GET");
			line = extractString(line, i, strlen("GET"));
		}
		else if (line.find("POST") != std::string::npos)
		{
			methods.push_back("POST");
			line = extractString(line, i, strlen("POST"));
		}
		else if (line.find("DELETE") != std::string::npos)
		{
			methods.push_back("DELETE");
			line = extractString(line, i, strlen("DELETE"));
		}
		else
		{
			throw (std::runtime_error("Location: Error, unknown keyword in location/methods"));
		}
		i++;
	}
	return methods;
}

void LocationConfig::_setDefaultLocations()
{
	if (_methods.empty())
	{
		_methods.push_back("GET");
		_methods.push_back("POST");
		_methods.push_back("DELETE");
	}
	if (_index.empty())
    	_index = "index.html";
	if (_uploads.empty())
		_uploads = "uploads.html";
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