/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:18:02 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/13 12:39:25 by aehrlich         ###   ########.fr       */
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

ServerSocket::ServerSocket()
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
	if ( this != &rhs )
	{
		this->_fd = rhs._fd;
		this->_ip = rhs._ip;
		this->_port = rhs._port;
		this->_type = rhs._type;
	}
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
void	ServerSocket::setUpSocket()
{
	// Create socket
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		std::cerr << "Error creating socket\n";
		exit(EXIT_FAILURE);
	}

	// Set up server address structure
	memset( &_serverSockAddr, 0, sizeof(_serverSockAddr) );
	_serverSockAddr.sin_family = AF_INET;
	_serverSockAddr.sin_addr.s_addr = INADDR_ANY;
	_serverSockAddr.sin_port = htons(_port);

	// Bind the socket
	if (bind(_fd, (struct sockaddr*)&_serverSockAddr, sizeof(_serverSockAddr)) == -1) {
		std::cerr << "Error binding socket\n";
		close(_fd);
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	if (listen(_fd, 100) == -1) {
		std::cerr << "Error listening for connections\n";
		close(_fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Created Server Socket listening to port" << _port << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int	ServerSocket::getPort() const
{
	return (_port);
}

struct sockaddr_in ServerSocket::getAddress() const
{
	return (_serverSockAddr);
}


/* ************************************************************************** */