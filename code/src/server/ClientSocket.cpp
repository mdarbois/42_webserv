/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:34 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/18 11:13:29 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket(int connectingServerFD)
{
	_connectingServerFD = connectingServerFD;
	_pollFD.events = 0;
	_pollFD.revents = 0;
	_pollFD.events = _pollFD.events | POLLIN; //set tot POLLIN, to listen to the request
	_resetRequest();
	setUpSocket();
}

ClientSocket::ClientSocket( const ClientSocket & src )
{
	(void)src;
}

ClientSocket::ClientSocket()
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientSocket::~ClientSocket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientSocket &				ClientSocket::operator=( ClientSocket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i )
{
	//o << "Value = " << i.getValue();
	(void)i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/
void	ClientSocket::setUpSocket()
{
	// Accept
	_pollFD.fd = accept(_connectingServerFD, NULL, NULL);
	//client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
	if (_pollFD.fd == -1) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
	}
	
	/*
		fctnl = file control system call to set options on file descriptor.
		F_SETFL: Specifies, that we want to set FLAGS on the fd
		O_NONBLOCK: Option non-blocking, operations on the fd wont block
	*/
	if ( fcntl(_pollFD.fd, F_SETFL, O_NONBLOCK) < 0 ) {
		std::cerr << "Error accepting connection\n";
		exit(EXIT_FAILURE);
	}
	std::cout << "Client socket created with FD: " << _pollFD.fd << std::endl;
}

void	ClientSocket::_resetRequest()
{
	//_request.type = UNSET;
	_request.readBytes = 0;
	_request.contentLength = 0;
	_request.buffer.clear();
}

/* CommunicationStatus	ClientSocket::receiveRequest()
{
	if (_request.type = UNSET)
	{
		char	buffer[4096]; //SIZE NEEDS TO BE RETHOUGHT
		size_t	bytesRead;
		if ((bytesRead = recv(_pollFD.fd, &buffer, 20, O_NONBLOCK)) < 0)
		{
			//DO MORE ?
			return (COM_ERROR);
		}
		if (bytesRead == 0)
			return (COM_CONN_CLOSED);
		_request.readBytes += bytesRead;
		_request.buffer.append(std::string(buffer, bytesRead));
		size_t	ctPos = _request.buffer.find("Content-Type: multipart/form-data;boundary="boundary"");
		size_t	clPos = _request.buffer.find("Content-Length: ");
		size_t	tePos = _request.buffer.find("Transfer-Encoding: chunked");
		
		if (clPos != std::string::npos)
		{
			_request.contentLength = static_cast<size_t>(std::atoi(_request.buffer.substr(clPos + 16).c_str()));
			_request.type = SINGLE;
		}
	}
} */

/*
	- read the header;
	- if there is a content-length
		- set content lenght to the value
		- set status to Read in progess, to read the rest in the next poll loop
*/
CommunicationStatus	ClientSocket::receiveRequest()
{
	std::cout << "Client -> receive" << std::endl;
	char	buffer[20]; //SIZE NEEDS TO BE RETHOUGHT
	size_t	bytesRead;
	
	if ((bytesRead = recv(_pollFD.fd, &buffer, 20, O_NONBLOCK)) < 0)
	{
		std::cout << "COM ERROR" << std::endl;
		return (COM_ERROR);
	}
	if (bytesRead == 0)
	{
		std::cout << "CONN CLOSED" << std::endl;
		return (COM_CONN_CLOSED);
	}
	_request.readBytes += bytesRead;
	_request.buffer.append(std::string(buffer, bytesRead));

	size_t	clPos = _request.buffer.find("Content-Length: ");
	if (clPos != std::string::npos)
		_request.contentLength = static_cast<size_t>(std::atoi(_request.buffer.substr(clPos + 16).c_str()));
	else
		_request.contentLength = 0;
	if (_request.buffer.find("\r\n\r\n") != std::string::npos)
	{
		std::cout << "******************COMPLETED REQUEST*****************************" << std::endl;
		std::cout << _request.buffer << std::endl;
		std::cout << "**********************END***************************************" << std::endl;
		return (COM_DONE);
	}
	else
	{
		std::cout << "******************TEMP REQUEST*****************************" << std::endl;
		std::cout << _request.buffer << std::endl;
		std::cout << "**********************END*****************************" << std::endl;	
		return (COM_IN_PROGRESS);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */