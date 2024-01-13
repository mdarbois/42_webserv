/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/13 13:03:25 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	for (int i = 0; i < nbrServers; i++)
	{
		_sockets.push_back(new ServerSocket(serverPorts[i], serverIPs[i]));
		struct pollfd newPollFD;
		_pollFDs.push_back(newPollFD);
		_pollFDs[i].fd = _sockets[i]->getFD();
		_pollFDs[i].events = 0;
		_pollFDs[i].events = _pollFDs[i].events | POLLIN;
	}
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
		int timeOut = TIMEOUT_POLL;
		int pollRegistered = 0;
		ClientSocket	*temp;
		while (4242)
		{
			while (!pollRegistered)
			{
				if ( (pollRegistered = poll(&_pollFDs[0], nbrServers, timeOut)) < 0)
				{
					std::cerr << "error: poll() failed";
					std::exit(EXIT_FAILURE);
				}
			}
			
			//Check every server if a new connection has been requested
			for (int i = 0; i < nbrServers; i++)
			{
				if ( (_pollFDs[i].revents & POLLIN) == POLLIN )
				{
					std::cout << "\r" << "Client connected on server: " << i << " with FD : " << _pollFDs[i].fd << "on PORT: " << _sockets[i]->getFD() << std::endl;
					temp = new ClientSocket(_sockets[i]->getFD());
					if ( temp->getFD() < 0)
					{
						std::cerr << "Err creating client socket" << std::endl;
						std::exit(EXIT_FAILURE);
					}
					std::string	response = _buildResponse("Hello from Server " + std::to_string(i));
					send(temp->getFD(), response.c_str(), strlen(response.c_str()), 0);
					close(temp->getFD());
					std::cerr << "Sent response" << std::endl;
					delete temp;
				}
			}
			pollRegistered = 0;
		}
		// Close the server socket
		for (int i = 0; i < nbrServers; i++)
			close(_sockets[i]->getFD());
	}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */