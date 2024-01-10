#include "LocationConfig.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

LocationConfig::LocationConfig()
{
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