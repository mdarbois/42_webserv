/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/14 17:52:15 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

void	Server::_setPollFD(struct pollfd *pfd, int fd, short events, short revents)
{
	pfd->events = events;
	pfd->revents = revents;
	pfd->fd = fd;
}

Server::Server()
{
	//initialize the pollFDs
	for (int i = 0; i < MAX_CONNECTIONS; i++)
		_setPollFD(&_pollFDs[i], -1, 0, 0);
	
	//This should be set later according to the config file
	for (int i = 0; i < nbrServers; i++)
	{
		_sockets.push_back(new ServerSocket(serverPorts[i], serverIPs[i]));
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

void	Server::_acceptNewClient(ServerSocket *socket)
{
	ClientSocket	*newClient;
	if (_sockets.size() == MAX_CONNECTIONS)
	{
		std::cerr << "Maximum amout of possible connections reached." << std::endl;
		return ;
	}
	newClient = new ClientSocket(socket->getFD());
	if ( newClient->getFD() < 0)
	{
		std::cerr << "Err creating client socket" << std::endl;
		delete newClient;
		std::exit(EXIT_FAILURE);
	}
	_sockets.push_back(newClient);

	//For Testing purposes - just send a respond to the client 
	{
		std::string	response = _buildResponse("Hello from Server " + std::to_string(socket->getPort()));
		send(newClient->getFD(), response.c_str(), strlen(response.c_str()), 0);
		close(newClient->getFD());
		std::cerr << "Sent response" << std::endl;
		delete newClient;
	}
}

void	Server::run()
{
	int				pollResult;
	int				currPollFDIndex;
	
	//Main server loop
	while (4242) //react to sigint later
	{
		//Check all monitored fd with poll if an event occured
		if ( (pollResult = poll(&_pollFDs[0], nbrServers, TIMEOUT_POLL)) < 0)
		{
			std::cerr << "error: poll() failed";
			std::exit(EXIT_FAILURE);
		}
		
		//Check every server if a new connection has been requested
			//POLLIN - ready to read/recv from the fd non-blocking
			//POLLOUT - ready to write/send to the fd non-blocking
		for (int i = 0; i < static_cast<int>(_sockets.size()); i++)
		{
			//check for poll errors
			//TBD

			//Check if there was an in comming POLL IN on the server socket, ask for a new connection
			if ( _pollFDs[i].revents & POLLIN && _sockets[i]->getType() == SERVER )
				_acceptNewClient(dynamic_cast<ServerSocket *>(_sockets[i]));

			//Check if the client socket is readable non-blocking: POLLIN.
			//Client is sending HTTP request
			//if ( _pollFDs[i].revents & POLLIN && _sockets[i]->getType() == CLIENT)
				//receive the request --> CAUTION: CAN BE SEND IN MULTIPLE CHUNKS
			
			//Check if the client socket is writeable non-blocking: POLLOUT.
			//Server is sending HTTP response
			//if ( _pollFDs[i].revents & POLLOUT && _sockets[i]->getType() == CLIENT)
				//Send response to the client fd
			
			//Check if the client has a timeout
		}
	}
	// Close the server socket
	for (int i = 0; i < nbrServers; i++)
		close(_sockets[i]->getFD());
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */