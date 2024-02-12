/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:13 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/12 23:59:35 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket() {}
//Socket::Socket( const Socket & src ) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

//Socket &				Socket::operator=( Socket const & rhs ) {}

std::ostream &			operator<<( std::ostream & o, Socket const & i )
{
	//o << "Value = " << i.getValue();
	(void)i;
	return o;
}

bool	Socket::operator==(const Socket& other)
{
	return this->_pollFD.fd == other.getFD();
}

/*
** --------------------------------- METHODS ----------------------------------
*/
void	Socket::setPollFD(int fd, short events, short revents)
{
	_pollFD.events = events;
	_pollFD.revents = revents;
	_pollFD.fd = fd;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
SocketType	Socket::getType() const
{
	return (_type);
}

struct pollfd	Socket::getPollFD() const
{
	return (_pollFD);
}

int	Socket::getFD() const
{
	return (_pollFD.fd);
}

ServerConfig	Socket::getServerConfig() const
{
	return (_config);
}
/* ************************************************************************** */