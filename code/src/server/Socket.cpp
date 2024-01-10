/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:13 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 11:59:16 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

//Socket::Socket() {}
//Socket::Socket( const Socket & src ) {}
//Socket::Socket() {}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket() {}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Socket &				Socket::operator=( Socket const & rhs ) {}

std::ostream &			operator<<( std::ostream & o, Socket const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}

bool	Socket::operator==(const Socket& other)
{
	return this->_fd == other.getFD();
}

/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
eSocketType	Socket::getType() const
{
	return (_type);
}

int	Socket::getFD() const
{
	return (_fd);
}

/* ************************************************************************** */