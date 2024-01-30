/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/29 12:19:54 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerManager.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerManager::ServerManager(Config const &config)
{
	_config = config;
	_numberServers = config.getServers().size();
	std::vector<ServerConfig> serversList = config.getServers();
	std::vector<ServerConfig>::iterator it = serversList.begin();
	for (; it != serversList.end(); ++it)
	{
		std::vector<unsigned int> portsList = (*it).getPorts();
		std::vector<unsigned int>::iterator portIT = portsList.begin();
		for (; portIT != portsList.end(); ++portIT){
			
			_sockets.push_back(new ServerSocket(static_cast<int>(*portIT), (*it).getIp(), *it));
		}
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
void log(const std::string& message, LogColor color) {
	const char* colorCode = "\033[0;";  // Default color
	switch (color) {
		case RED:
			colorCode = "\033[91m";  // Red
			break;
		case GREEN:
			colorCode = "\033[92m";  // Green
			break;
		case YELLOW:
			colorCode = "\033[93m";  // Yellow
			break;
		case BLUE:
			colorCode = "\033[94m";  // Blue
			break;
		case DEFAULT:
			colorCode = "\033[0m";   // Default
			break;
	}

	// Print the message with the specified color
	// Reset color after message
	std::cout << colorCode << message << "\033[0m" << std::endl;
}


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

void	ServerManager::_acceptNewClient(ServerSocket *socket)
{
	//std::cout << "TRY TO ACCEPT NEW CLIENT" << std::endl;
	ClientSocket	*newClient;
	if (_sockets.size() == MAX_CONNECTIONS)
	{
		std::cerr << "Maximum amout of possible connections reached." << std::endl;
		return ;
	}
	newClient = new ClientSocket(socket->getFD(), socket->getServerConfig());
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
	CommunicationStatus responseStatus = client->sendResponse();
	
	if (responseStatus == COM_DONE)
		_deleteClient(client, HTTP_200);
	else if (responseStatus == COM_ERROR)
		_deleteClient(client, HTTP_500);
	else if (responseStatus == COM_IN_PROGRESS)
		client->setPollFD(client->getFD(), POLLOUT, 0);
	_updatePollFDArray();
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
			else if ( _sockets[i]->getType() == CLIENT && dynamic_cast<ClientSocket *>(_sockets[i])->hasCommunicationTimeOut())
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