/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 18:45:44 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	_mainSocket = ServerSocket(18000, 2130706433); //127.0.0.1 = 2130706433
}

Server::Server( const Server & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
	void	Server::run()
	{
		std::cout << "Server listening on port " << 18000 << "...\n";
		while (true)
		{
			// Accept connection
			_testClientSocket = new ClientSocket(_mainSocket.getFD());
			// Handle request in a separate function

			//read out the request
			char buf[10];
			int n;
			while ((n = read(_testClientSocket->getFD(), buf, sizeof(buf) - 1)) > 0)
			{
				buf[n] = '\0';
				std::cout << buf;
			}
			if (n < 0)
			{
				// Handle the error case (e.g., print an error message)
				std::cerr << "Error reading from socket\n";
				return ;
			}
			
			const char* response = "HTTP/1.1 200 OK\r\n\r\nMARIEEEEEsadfasdfadfa";
			send(_testClientSocket->getFD(), response, strlen(response), 0);
			close(_testClientSocket->getFD());
			delete _testClientSocket;
		}
		// Close the server socket
		close(_mainSocket.getFD());
	}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */