/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:34 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/15 11:44:12 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket(int connectingServerFD)
{
	_connectingServerFD = connectingServerFD;
	_pollFD.events = 0;
	_pollFD.revents = 0;
	_pollFD.events = _pollFD.events | POLLIN; //set tot POLLIN, to listen to the request
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
	_pollFD.fd = accept(_connectingServerFD, NULL, NULL);
	//client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
	if (_pollFD.fd == -1) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
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
	std::cout << "Client socket created with FD: " << _pollFD.fd << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */