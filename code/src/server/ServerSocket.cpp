/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:18:02 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:20:07 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerSocket::ServerSocket(int port, int ip): Socket()
{
	_type = SERVER;
	_port = port;
	_ip = ip;
	setUpSocket();
}

ServerSocket::ServerSocket( const ServerSocket & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerSocket::~ServerSocket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ServerSocket &				ServerSocket::operator=( ServerSocket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ServerSocket const & i )
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
void	ServerSocket::setUpSocket()
{
	// Create socket
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		std::cerr << "Error creating socket\n";
		exit(EXIT_FAILURE);
	}

	// Set up server address structure
	struct sockaddr_in	server_addr;
	memset( &server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	// Bind the socket
	if (bind(_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Error binding socket\n";
		close(_fd);
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(_fd, MAX_CONNECTIONS) == -1) {
		std::cerr << "Error listening for connections\n";
		close(_fd);
		exit(EXIT_FAILURE);
	}
}

/* ************************************************************************** */