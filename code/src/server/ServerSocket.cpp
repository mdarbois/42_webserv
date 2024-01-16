/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:18:02 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/16 11:47:23 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerSocket::ServerSocket(int port, int ip)
{
	_type = SERVER;
	_port = port;
	_ip = ip;
	_pollFD.events = 0;
	_pollFD.revents = 0;
	_pollFD.events = _pollFD.events | POLLIN;
	setUpSocket();
}

ServerSocket::ServerSocket( const ServerSocket & src )
{
	(void)src;
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
		this->_pollFD.fd = rhs._pollFD.fd;
		this->_ip = rhs._ip;
		this->_port = rhs._port;
		this->_type = rhs._type;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ServerSocket const & i )
{
	//o << "Value = " << i.getValue();
	(void)i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
void	ServerSocket::setUpSocket()
{
	// Create socket
	_pollFD.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_pollFD.fd == -1) {
		std::cerr << "Error creating socket\n";
		exit(EXIT_FAILURE);
	}

	// Reusable Port and Socket
	int on = 1;
	if ( setsockopt(_pollFD.fd, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(int)) < 0 ) //fails when adding | SO_REUSEPORT
	{
		//perror("setsockop()");
		std::exit(EXIT_FAILURE);
	}

	/*
		fctnl = file control system call to set options on file descriptor.
		F_SETFL: Specifies, that we want to set FLAGS on the fd
		O_NONBLOCK: Option non-blocking, operations on the fd wont block
	*/
	if ( fcntl(_pollFD.fd, F_SETFL, O_NONBLOCK) < 0 ) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
	}
	
	// Set up server address structure
	memset( &_serverSockAddr, 0, sizeof(_serverSockAddr) );
	_serverSockAddr.sin_family = AF_INET;
	_serverSockAddr.sin_addr.s_addr = INADDR_ANY; //should be replaced by the ip adress later
	_serverSockAddr.sin_port = htons(_port);

	// Bind the socket
	if (bind(_pollFD.fd, (struct sockaddr*)&_serverSockAddr, sizeof(_serverSockAddr)) == -1) {
		std::cerr << "Error binding socket\n";
		close(_pollFD.fd);
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections. Make it a passive partner of the connection
	if (listen(_pollFD.fd, 100) == -1) {
		std::cerr << "Error listening for connections\n";
		close(_pollFD.fd);
		exit(EXIT_FAILURE);
	}
	std::cout << "Created Server Socket with FD: " << _pollFD.fd << " listening to port: " << _port << std::endl;
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