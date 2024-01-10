#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <string>

class LocationConfig
{

	public:

		LocationConfig();
		LocationConfig( LocationConfig const & src );
		~LocationConfig();

		LocationConfig &		operator=( LocationConfig const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, LocationConfig const & i );

#endif /* ************************************************** LOCATIONCONFIG_H */