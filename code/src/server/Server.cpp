/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/16 09:21:55 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	//This should be set later according to the config file
	for (int i = 0; i < nbrServers; i++)
		_sockets.push_back(new ServerSocket(serverPorts[i], serverIPs[i]));
	_updatePollFDArray();
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

/*
	The pollfd of each socket is attatched to the socket. This makes deleting and addign new ones easaier.
	After each deletion or adding of a socket to the _sockets, the pollFD Arr needs to be updated.
	The poll method needs the the pollFD Array.
*/
void	Server::_updatePollFDArray()
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		memset(&_pollFDs[i], 0, sizeof(struct pollfd));
		if (i < _sockets.size())
			_pollFDs[i] = _sockets[i]->getPollFD();
	}
}

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
	std::cout << "TRY TO ACCEPT NEW CLIENT" << std::endl;
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
	_updatePollFDArray();
}

void	Server::_receiveRequest(ClientSocket *client)
{
	std::cout << "TRY TO RECV REQUEST" << std::endl;
	//Print HTTP Request - Testing
	char buffer[1024];
	std::string receivedString;
	ssize_t bytesRead = recv(client->getFD(), buffer, sizeof(buffer), 0);
	receivedString += std::string(buffer, bytesRead);
	// Print the received string
	std::cout << "************HTTP-REQUEST**************************" << std::endl;
	std::cout << receivedString << std::endl;
	std::cout << "**************************************************" << std::endl << std::endl;
	client->setPollFD(client->getFD(), POLLOUT, 0);
	_updatePollFDArray();
}

void	Server::_sendResponse(ClientSocket *client)
{
	std::cout << "TRY TO SEND SIMPLE RESPONSE" << std::endl;
	//For Testing purposes - just send a simple respond to the client 
	std::string	response = _buildResponse("Hello Client: " + std::to_string(client->getFD()));
	send(client->getFD(), response.c_str(), strlen(response.c_str()), 0);
	client->setPollFD(client->getFD(), POLLIN, 0);
	_updatePollFDArray();
	std::cerr << "Sent response" << std::endl;
	_deleteClient(client);
}

void	Server::_deleteClient(ClientSocket *client)
{
	for (std::vector<Socket *>::iterator it = _sockets.begin() ; it != _sockets.end(); it++)
	{
		if ((*it)->getFD() == client->getFD())
		{
			std::cout << "TRY to DELETE client." << std::endl;
			_sockets.erase(it);
			std::cout << "erased client with fd: " << client->getFD() << ". New socket size: " << _sockets.size() << std::endl;
			close(client->getFD());
			std::cout << "closed client with fd: " << client->getFD() << std::endl;
			delete client;
			std::cout << "deleted client" << std::endl;
			_updatePollFDArray();
			std::cout << "updated the pollfd array" << std::endl;
			return ;
		}
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
		if ( (pollResult = poll(&_pollFDs[0], _sockets.size(), TIMEOUT_POLL)) < 0)
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
			else if ( _pollFDs[i].revents & POLLIN && _sockets[i]->getType() == CLIENT)
				_receiveRequest(dynamic_cast<ClientSocket *>(_sockets[i]));//receive the request --> CAUTION: CAN BE SEND IN MULTIPLE CHUNKS
			
			//Check if the client socket is writeable non-blocking: POLLOUT.
			//Server is sending HTTP response
			else if ( _pollFDs[i].revents & POLLOUT && _sockets[i]->getType() == CLIENT)
				_sendResponse(dynamic_cast<ClientSocket *>(_sockets[i]));

			//Check if the client has a timeout
			//TBD
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