/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:34 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/14 17:52:22 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket(int connectingServerFD)
{
	_connectingServerFD = connectingServerFD;
	setUpSocket();
}

ClientSocket::ClientSocket( const ClientSocket & src )
{
}

ClientSocket::ClientSocket()
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
	//client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
	if (_fd == -1) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
	}

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */