/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/22 12:11:48 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerManager.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerManager::ServerManager(Config const &config)
{
	_numberServers = config.getServers().size();
	std::vector<ServerConfig> serversList = config.getServers();
	std::vector<ServerConfig>::iterator it = serversList.begin();
	for (; it != serversList.end(); ++it)
	{
		std::vector<unsigned int> portsList = (*it).getPorts();
		std::vector<unsigned int>::iterator portIT = portsList.begin();
		for (; portIT != portsList.end(); ++portIT)
			_sockets.push_back(new ServerSocket(static_cast<int>(*portIT), (*it).getIp()));
	}
	_updatePollFDArray();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerManager::~ServerManager()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

std::ostream &			operator<<( std::ostream & o, ServerManager const & i )
{
	//o << "Value = " << i.getValue();
	(void)i;
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
void	ServerManager::_updatePollFDArray()
{
	for (size_t i = 0; i < MAX_CONNECTIONS; i++)
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
	std::string	lengthStr = intToString(static_cast<int>(body.length()));

	response += "HTTP/1.1 200 OK" + crlf;			// Status-Line
	response += "Content-Type: text/html" + crlf;	// Entity-Header-Field
	response += "Content-Length: " + lengthStr + crlf;		// Entity-Header-Field
	response += crlf;
	response += body + crlf;				// message-body
	response += crlf;
	
	return response;
}

void	ServerManager::_acceptNewClient(ServerSocket *socket)
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

void	ServerManager::_receiveRequest(ClientSocket *client)
{
	CommunicationStatus	status = client->receiveRequest();
	if (status == COM_CONN_CLOSED)
		_deleteClient(client, NO_ERROR);
	else if (status == COM_ERROR)
		_deleteClient(client, HTTP_400);
	else if (status == COM_DONE)
		client->setPollFD(client->getFD(), POLLOUT, 0);
	else if (status == COM_IN_PROGRESS)
		client->setPollFD(client->getFD(), POLLIN, 0);
	_updatePollFDArray();
}

void	ServerManager::_sendResponse(ClientSocket *client)
{
	ParserHTTP parser(client->getRequest().buffer);
	
	std::cout << "TRY TO SEND SIMPLE RESPONSE" << std::endl;
	//For Testing purposes - just send a simple respond to the client 
	std::string	response = _buildResponse("Hello Client: " + intToString(client->getFD()));
	send(client->getFD(), response.c_str(), strlen(response.c_str()), 0);
	client->setPollFD(client->getFD(), POLLIN, 0);
	_updatePollFDArray();
	std::cerr << "Sent response" << std::endl;
	_deleteClient(client, HTTP_200); //TESTING
}

void	ServerManager::_deleteClient(ClientSocket *client, HttpStatus code)
{
	if (code < 0)
		return ;
	for (std::vector<Socket *>::iterator it = _sockets.begin() ; it != _sockets.end(); it++)
	{
		if ((*it)->getFD() == client->getFD())
		{
			_sockets.erase(it);
			close(client->getFD());
			delete client;
			_updatePollFDArray();
			std::cout << "Deleted client" << std::endl;
			return ;
		}
	}
}

bool	ServerManager::_checkPollErrors(Socket *socket, short int revent)
{
	if (socket->getType() == SERVER)
	{
		if (revent & POLLNVAL || revent & POLLHUP || revent & POLLERR)
		{
			std::cout << "POLL error server" << std::endl;
			dynamic_cast<ServerSocket *>(socket)->restartServerSocket();
			return (true);
		}
		return (false);
	}
	else if (socket->getType() == CLIENT)
	{
		if (revent & POLLNVAL || revent & POLLHUP || revent & POLLERR)
		{
			std::cout << "POLL error client" << std::endl;
			_deleteClient(dynamic_cast<ClientSocket *>(socket), HTTP_500);
			return (true);
		}
		return (false);
	}
	return (true);
}

void	ServerManager::run()
{
	int	pollResult;
	
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
			if (_checkPollErrors(_sockets[i], _pollFDs[i].revents))
				continue;

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
				_sendResponse(dynamic_cast<ClientSocket *>(_sockets[i])); //send the response --> CAUTION: CAN BE SEND IN MULTIPLE CHUNKS

			//Check if the client has a timeout
			else if ( _sockets[i]->getType() == CLIENT && dynamic_cast<ClientSocket *>(_sockets[i])->hasReceiveTimeOut())
				_deleteClient(dynamic_cast<ClientSocket *>(_sockets[i]), HTTP_408); // If cleients timed out, do we have to send a response to it?
		}
	}
	// Close the server socket
	for (int i = 0; i < _numberServers; i++)
		close(_sockets[i]->getFD());
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */