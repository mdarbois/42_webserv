/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:35 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/13 00:16:59 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ServerManager.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerManager::ServerManager(Config const &config)
{
	_config = config;
	_numberPollFDs = 0;
	_numberServers = config.getServers().size();
	std::vector<ServerConfig> serversList = config.getServers();
	std::vector<ServerConfig>::iterator it = serversList.begin();
	for (; it != serversList.end(); ++it)
	{
		std::vector<unsigned int> portsList = (*it).getPorts();
		std::vector<unsigned int>::iterator portIT = portsList.begin();
		for (; portIT != portsList.end(); ++portIT)
		{
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
void printFileDescriptors(const struct pollfd fds[], int num_fds) {
	for (int i = 0; i < num_fds; ++i) {
		std::cout << "File Descriptor " << i << ": " << fds[i].fd;
	}
}

/*
	The pollfd of each socket is attatched to the socket. This makes deleting and addign new ones easaier.
	After each deletion or adding of a socket to the _sockets, the pollFD Arr needs to be updated.
	The poll method needs the the pollFD Array.
*/
void	ServerManager::_updatePollFDArray()
{
	_numberPollFDs = 0;
	int j = 0;
	for (size_t i = 0; i < _sockets.size(); i++)
	{
		memset(&_pollFDs[j], 0, sizeof(struct pollfd));
		if (i < _sockets.size())
		{
			_pollFDs[j++] = _sockets[i]->getPollFD();
			_numberPollFDs++;
			if (_sockets[i]->getType() == CLIENT)
			{
				ClientSocket *client = dynamic_cast<ClientSocket *>(_sockets[i]);
				if (client->requestedCGI())
				{
					if (client->getCGIToPipeFd().fd != -1)
					{
						_pollFDs[j++] = client->getCGIToPipeFd();
						_numberPollFDs++;
					}
					if (client->getPipeToParentFd().fd != -1)
					{
						_pollFDs[j++] = client->getPipeToParentFd();
						_numberPollFDs++;
					}
				}
			}
		}
	}
}

void	ServerManager::_updateClientPollFDs()
{
	for (int i = 0; i < _numberPollFDs; i++)
	{
		for (size_t j = 0; j < _sockets.size(); j++)
		{
			if (_sockets[j]->getPollFD().fd == _pollFDs[i].fd)
			{
				_sockets[j]->setPollFD(_pollFDs[i].fd, _pollFDs[i].events, _pollFDs[i].revents);
			}
			if (_sockets[j]->getType() == CLIENT)
			{
				ClientSocket *temp = dynamic_cast<ClientSocket *>(_sockets[j]);
				if (temp->getCGIToPipeFd().fd == _pollFDs[i].fd)
					temp->setCGIToPipeFd(_pollFDs[i].fd, _pollFDs[i].events, _pollFDs[i].revents);
				if (temp->getPipeToParentFd().fd == _pollFDs[i].fd)
					temp->setPipeToParentFd(_pollFDs[i].fd, _pollFDs[i].events, _pollFDs[i].revents);
			}
		}
	}
}

void	ServerManager::_acceptNewClient(ServerSocket *socket)
{
	ClientSocket	*newClient;
	if (_sockets.size() == MAX_CONNECTIONS)
	{
		std::cerr << "Maximum amout of possible connections reached." << std::endl;
		return ;
	}
	newClient = new ClientSocket(socket->getFD(), socket->getServerConfig());
	if ( !newClient || newClient->getFD() < 0)
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
	else if (status == COM_IN_PROGRESS || status == CGI_PENDING)
		client->setPollFD(client->getFD(), POLLIN, 0);
	_updatePollFDArray();
}

void	ServerManager::_sendResponse(ClientSocket *client)
{
	CommunicationStatus responseStatus = client->sendResponse();
	
	if (responseStatus == COM_DONE)
		_deleteClient(client, NO_ERROR);
	else if (responseStatus == COM_ERROR)
		_deleteClient(client, HTTP_500);
	else if (responseStatus == COM_IN_PROGRESS)
		client->setPollFD(client->getFD(), POLLOUT, 0);
	_updatePollFDArray();
}

void	ServerManager::_deleteClient(ClientSocket *client, HttpStatus code)
{
	if (code != NO_ERROR)
		client->closeClient(code);
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

bool	ServerManager::_checkPollErrors(SocketType socketType, int socketIdx, short int revent)
{
	if (socketType == SERVER)
	{
		if (revent & POLLNVAL || revent & POLLHUP || revent & POLLERR)
		{
			std::cerr << "POLL error server" << std::endl;
			dynamic_cast<ServerSocket *>(_sockets[socketIdx])->restartServerSocket();
			return (true);
		}
		return (false);
	}
	else if (socketType == CLIENT)
	{
		if (revent & POLLNVAL || revent & POLLHUP || revent & POLLERR)
		{
			std::cerr << "POLL error client" << std::endl;
			_deleteClient(dynamic_cast<ClientSocket *>(_sockets[socketIdx]), HTTP_500);
			return (true);
		}
		return (false);
	}
	else if (socketType == NO_SOCKET)
	{
		if (revent & POLLNVAL || revent & POLLERR)
		{
			std::cerr << "POLL error Pipe FD" << std::endl;
			_deleteClient(dynamic_cast<ClientSocket *>(_sockets[socketIdx]), HTTP_500);
			return (true);
		}
		return (false);
	}
	return (true);
}


/* 
	PollFd array conatins socket fds and pipe fds, so the index of the _sockets[] and 
	the pollFD[] does not match. In the main loop we iterate over the pollfd and need
	to check if at the current pollFD Idx is an Client or Server. Return the type and also 
	set the inout param socketIndex to the _socket[] index to use it in the main server loop.
 */
SocketType	ServerManager::_getSocketTypeForPollFdIdx(int idx, int *socketIndex) const
{
	int	pollFD = _pollFDs[idx].fd;
	for (size_t i = 0; i < _sockets.size(); i++)
	{
		if (pollFD == _sockets[i]->getFD())
		{
			*socketIndex = i;
			return (_sockets[i]->getType());
		}
	}
	return (NO_SOCKET);
}

ClientSocket*	ServerManager::_getClientForPipeFD(int idx) const
{
	int	pollFD = _pollFDs[idx].fd;
	for (size_t i = 0; i < _sockets.size(); i++)
	{
		if (_sockets[i]->getType() == CLIENT)
		{
			ClientSocket* temp = dynamic_cast<ClientSocket *>(_sockets[i]);
			if (temp->getCGIToPipeFd().fd == pollFD || temp->getPipeToParentFd().fd == pollFD)
				return (temp);
		}
	}
	return (NULL);
}

/* 
	Server loop loops over all pollFDs (pipes and sockets) and listen for write read events.
	The server sockets accept and create new client sockets.
	The client sockets handle request receiving and response sending.
 */

void	ServerManager::run()
{
	int	pollResult;
	int	socketIdx;
	
	//Main server loop
	while (42)
	{
		//Check all monitored fd with poll if an event occured
		//shufflePollFDs(&_pollFDs[0], _numberPollFDs);
		if ( (pollResult = poll(&_pollFDs[0], _numberPollFDs, TIMEOUT_POLL)) < 0)
		{
			std::cerr << "error: poll() failed";
			std::exit(EXIT_FAILURE);
		}
		_updateClientPollFDs();
		//Check every server if a new connection has been requested
			//POLLIN - ready to read/recv from the fd non-blocking
			//POLLOUT - ready to write/send to the fd non-blocking
		for (int i = 0; i < _numberPollFDs; i++)
		{
			//check for poll errors
			if (_checkPollErrors(_getSocketTypeForPollFdIdx(i, &socketIdx), socketIdx, _pollFDs[i].revents))
				continue;
			//Check if there was an in comming POLLIN on the server socket, ask for a new connection
			if ( (_pollFDs[i].revents & POLLIN) && _getSocketTypeForPollFdIdx(i, &socketIdx) == SERVER )
				_acceptNewClient(dynamic_cast<ServerSocket *>(_sockets[socketIdx]));
			//Check if the client socket is readable non-blocking: POLLIN.
			//Client is sending HTTP request
			else if ( (_pollFDs[i].revents & POLLIN) && _getSocketTypeForPollFdIdx(i, &socketIdx) == CLIENT)
				_receiveRequest(dynamic_cast<ClientSocket *>(_sockets[socketIdx]));

			//Must be a file descriptor of a pipe
			else if (_getSocketTypeForPollFdIdx(i, &socketIdx) == NO_SOCKET)
			{
				//since the fds are suffled, get the right client FD for the current pipeFD
				ClientSocket *client = _getClientForPipeFD(i);
				//execute the CGI child process and write to the pipe
				if (_pollFDs[i].revents == POLLOUT && _pollFDs[i].fd == client->getCGIToPipeFd().fd)
				{
					client->getCGI().writeCGIToPipe();
					client->setCGIToPipeFd(-1, 0, 0); //was closed, should be removed from pollFD in the next _updatePollFDArray()
				}
				//execution was done. Read the results from the pipe and write it to the client
				else if ((_pollFDs[i].revents & POLLIN) && _pollFDs[i].fd == client->getPipeToParentFd().fd)
				{
					client->getCGI().readBodyFromPipe();
					client->setPipeToParentFd(-1, 0, 0); //was closed, should be removed from pollFD in the next _updatePollFDArray()
					client->setPollFD(client->getFD(), POLLOUT, 0); //CGI was executed, can be written now to the client socket
				}
				_updatePollFDArray();
			}
			//Check if the client socket is writeable non-blocking: POLLOUT.
			//Server is sending HTTP response
			else if ( (_pollFDs[i].revents & POLLOUT) && _getSocketTypeForPollFdIdx(i, &socketIdx) == CLIENT)
				_sendResponse(dynamic_cast<ClientSocket *>(_sockets[socketIdx])); //send the response --> CAUTION: CAN BE SEND IN MULTIPLE CHUNKS

			//Check if the client has a timeout
			if ( _getSocketTypeForPollFdIdx(i, &socketIdx) == CLIENT && dynamic_cast<ClientSocket *>(_sockets[socketIdx])->hasCommunicationTimeOut())
			{
				std::cerr << "Client has timed out" << std::endl;
				_deleteClient(dynamic_cast<ClientSocket *>(_sockets[socketIdx]), HTTP_408); // If cleients timed out, do we have to send a response to it?
			}
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