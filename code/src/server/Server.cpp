/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/13 11:29:40 by aehrlich         ###   ########.fr       */
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
std::string	_buildResponse(std::string body)
{
	std::string	crlf = "\r\n";
	std::string	response;
	std::string	lengthStr = std::to_string(body.length());

	response += "HTTP/1.1 200 OK" + crlf;			// Status-Line
	response += "Content-Type: text/html" + crlf;	// Entity-Header-Field
	response += "Content-Length: " + lengthStr + crlf;		// Entity-Header-Field
	response += crlf;
	response += body + crlf;				// message-body
	response += crlf;
	
	return response;
}
	void	Server::run()
	{
		std::cout << "Server listening on port " << 18000 << "...\n";
		while (true)
		{
			// Accept connection
			_testClientSocket = new ClientSocket(_mainSocket.getFD());
			// Handle request in a separate function
			if (_testClientSocket->getFD() < 0)
			{
				std::cerr << "Err creating client socket" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::string	response = _buildResponse("YOOOOOO WASSUP?!");
			send(_testClientSocket->getFD(), response.c_str(), strlen(response.c_str()), 0);
			close(_testClientSocket->getFD());
			std::cerr << "Sent response" << std::endl;
			delete _testClientSocket;
		}
		// Close the server socket
		close(_mainSocket.getFD());
	}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */