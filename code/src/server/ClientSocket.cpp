/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:34 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:25:33 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket(int serverFD)
{
	_connectingServerFD = serverFD;
	setUpSocket();
}

ClientSocket::ClientSocket( const ClientSocket & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientSocket::~ClientSocket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientSocket &				ClientSocket::operator=( ClientSocket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
void	ClientSocket::setUpSocket()
{
	// Accept
	_fd = accept(_connectingServerFD, NULL, NULL);
	if (_fd == -1) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
	}

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */