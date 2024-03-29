/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:34 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/19 09:42:57 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ClientSocket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientSocket::ClientSocket(int connectingServerFD, ServerConfig config)
{
	_isCGI = false;
	_cgiFound = false;
	_hasInternalCGIError = false;
	
	_startTimeCommunication = time(0);
	_responseData.bytesSent = 0;
	_responseData.sendInProgress = false;

	_pipeToParentFd.fd = -1;
	_pipeToParentFd.revents = 0;
	_pipeToParentFd.events = POLLIN;

	_CGIToPipeFd.fd = -1;
	_CGIToPipeFd.revents = 0;
	_CGIToPipeFd.events = POLLOUT;
	
	_type = CLIENT;
	_config = config;
	_connectingServerFD = connectingServerFD;
	_pollFD.revents = 0;
	_pollFD.events = 0;
	_pollFD.events = _pollFD.events | POLLIN; //set tot POLLIN, to listen to the request
	setUpSocket();
	_resetRequest();
}

ClientSocket::ClientSocket( const ClientSocket & src ): Socket()
{
	this->_cgi = src._cgi;
	this->_isCGI = src._isCGI;
	this->_connectingServerFD = src._connectingServerFD;
	this->_request = src._request;
	this->_startTimeCommunication = src._startTimeCommunication;
	this->_pipeToParentFd = src._pipeToParentFd;
	this->_CGIToPipeFd = src._CGIToPipeFd;
	this->_parser = src._parser;
	this->_responseData	= src._responseData;
	this->_cgiFound = src._cgiFound;
	this->_hasInternalCGIError = src._hasInternalCGIError;
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
	this->_cgi = rhs._cgi;
	this->_isCGI = rhs._isCGI;
	this->_connectingServerFD = rhs._connectingServerFD;
	this->_request = rhs._request;
	this->_startTimeCommunication = rhs._startTimeCommunication;
	this->_pipeToParentFd = rhs._pipeToParentFd;
	this->_CGIToPipeFd = rhs._CGIToPipeFd;
	this->_parser = rhs._parser;
	this->_responseData	= rhs._responseData;
	this->_cgiFound = rhs._cgiFound;
	this->_hasInternalCGIError = rhs._hasInternalCGIError;
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
	//std::cout << "Client socket created with FD: " << _pollFD.fd << std::endl;
}

void	ClientSocket::_resetRequest()
{
	_request.readBytes = 0;
	_request.contentLength = 0;
	_request.buffer.clear();
	_request.endType = UNSET;
}

bool	ClientSocket::hasCommunicationTimeOut()
{
	if (_startTimeCommunication == -1)
		return (true);
	if (time(0) - _startTimeCommunication >= CLIENT_TIMEOUT_RECEIVE)
		return (true);
	return (false);
}

/*
	- read the header;
	- if there is a content-length
		- set content lenght to the value
		- set status to Read in progess, to read the rest in the next poll loop
*/

bool	ClientSocket::_checkContentLength()
{
	size_t		endHeader = _request.buffer.find("\r\n\r\n");
	std::string	currentBody = _request.buffer.substr(endHeader + std::strlen("\r\n\r\n"));

	if (endHeader == std::string::npos)
		return (false);
	if (_request.contentLength == currentBody.length())
		return (true);
	return (false);
}

bool	ClientSocket::_doneReceiving()
{
	if (_request.buffer.find("\r\n\r\n") != std::string::npos && _request.endType == SINGLE)
		return (true);
	else if (_request.endType == CONTENT_LENGTH && _checkContentLength())
		return (true);
	else if (_request.buffer.find("0\r\n\r\n") != std::string::npos && _request.endType == CHUNKED_ENCODING)
		return (true);
	return (false);
}

CommunicationStatus	ClientSocket::receiveRequest()
{
	char	buffer[CLIENT_RECEIVE_BUFFER_SIZE];
	ssize_t	bytesRead;
	if ((bytesRead = recv(_pollFD.fd, buffer, CLIENT_RECEIVE_BUFFER_SIZE, O_NONBLOCK)) < 0)
	{
		std::cerr << "COM ERROR" << std::endl;
		return (COM_ERROR);
	}
	if (bytesRead == 0)
	{
		//std::cerr << "CONN CLOSED" << std::endl;
		return (COM_CONN_CLOSED);
	}
	_request.readBytes += bytesRead;
	_request.buffer.append(std::string(buffer, bytesRead));

	size_t	clPos = _request.buffer.find("Content-Length: ");
	size_t	tePos = _request.buffer.find("Transfer-Encoding: chunked");
	if (clPos != std::string::npos)
	{
		_request.endType = CONTENT_LENGTH, //POST with content length
		_request.contentLength = static_cast<size_t>(std::atoi(_request.buffer.substr(clPos + 16).c_str()));
	}
	else if (tePos != std::string::npos)
	{
		_request.endType = CHUNKED_ENCODING; //POST with chunked encoding
		_request.contentLength = 0;
	}
	else
		_request.endType = SINGLE; //DELETE OR GET
	
	if (_doneReceiving())
	{
		//Parse the request string
		_parser = ParserHTTP (_request.buffer);
		if (_parser.isCGI())
		{
			
			_isCGI = true;
			if (access((_config.getRoot() + _parser.getPath()).c_str(), F_OK) == 0)
			{
				_cgiFound = true;
				_cgi = CGI(_parser, _config); //Set up the CGI with the pipes and environment
				setCGIToPipeFd(_cgi.output_pipe[1], POLLOUT, 0);
				setPipeToParentFd(_cgi.output_pipe[0], POLLIN, 0);
				return (CGI_PENDING);
			}
			else
				_cgiFound = false;
		}
		return (COM_DONE);
	}
	else
		return (COM_IN_PROGRESS);
}

CommunicationStatus	ClientSocket::sendResponse()
{
	if (!_responseData.sendInProgress)
	{
		//TODO: Timer!
		_startTimeCommunication = time(0);

		//Make a HTTP-Response
		try
		{
			if (_isCGI && !_cgiFound)
				_responseData.response = ResponseHTTP(HTTP_404, _config);
			else if (_hasInternalCGIError)
				_responseData.response = ResponseHTTP(HTTP_500, _config);
			else if (_isCGI)
				_responseData.response = ResponseHTTP(_cgi, _config);
			else
				_responseData.response = ResponseHTTP(_parser, _config);
			_responseData.sendInProgress = true;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			_responseData.response.setResponseLine(HTTP_500);
		}
	}
	const std::string& fullResponse = _responseData.response.getFullResponseString();
	int sendReturn = send(
		_pollFD.fd, fullResponse.c_str() + _responseData.bytesSent,
		_responseData.response.getResponseLength() - _responseData.bytesSent,
		0);
	if (sendReturn <= 0)
	{
		std::cerr << "COM ERRROR" << std::endl;
		return (COM_ERROR);
	}
	_responseData.bytesSent += sendReturn;
	if (_responseData.bytesSent >= static_cast<int>(fullResponse.size()))
		return (COM_DONE);
	return (COM_IN_PROGRESS);
}

void	ClientSocket::closeClient(HttpStatus status)
{
	try
	{
		_responseData.response = ResponseHTTP(status, _config);
		_responseData.sendInProgress = true;
		sendResponse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
Request	ClientSocket::getRequest() const
{
	return (_request);
}

bool	ClientSocket::requestedCGI() const
{
	return (_isCGI);
}

struct pollfd	ClientSocket::getPipeToParentFd() const
{
	return (_pipeToParentFd);
}

struct pollfd	ClientSocket::getCGIToPipeFd() const
{
	return (_CGIToPipeFd);
}

bool	ClientSocket::cgiFound() const
{
	return (_cgiFound);
}

CGI&	ClientSocket::getCGI()
{
	return (_cgi);
}

void	ClientSocket::setCGIToPipeFd(int fd, short events, short revents)
{
	_CGIToPipeFd.fd = fd;
	_CGIToPipeFd.events = events;
	_CGIToPipeFd.revents = revents;
}

void	ClientSocket::setPipeToParentFd(int fd, short events, short revents)
{
	_pipeToParentFd.fd = fd;
	_pipeToParentFd.events = events;
	_pipeToParentFd.revents = revents;
}

void	ClientSocket::setCGIErrorState(bool state)
{
	_hasInternalCGIError = state;
}

/* ************************************************************************** */