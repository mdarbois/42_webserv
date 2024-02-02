/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/01 15:04:36 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"
# include "ServerSocket.hpp"
# include "ParserHTTP.hpp"
# include "ResponseHTTP.hpp"
# include "CGI.hpp"

# define CLIENT_RECEIVE_BUFFER_SIZE 1024
# define CLIENT_TIMEOUT_RECEIVE 60

typedef enum e_RequestEndType
{
	UNSET,
	SINGLE,
	CONTENT_LENGTH,
	CHUNKED_ENCODING
}RequestEndType;

typedef struct s_Request
{
	size_t			contentLength;
	size_t			readBytes;
	std::string		buffer;
	std::string		boundary;
	RequestEndType	endType;
}Request;

typedef struct s_ResponseData
{
	ResponseHTTP	response;
	bool			sendInProgress;
	int				bytesSent;
}ResponseData;


class ClientSocket: public Socket
{
	
	public:
		ClientSocket(int connectingServerFD, ServerConfig config);
		~ClientSocket();
		ClientSocket();
		ClientSocket( ClientSocket const & src );
		
		ClientSocket &		operator=( ClientSocket const & rhs );
		
		void				setUpSocket();
		CommunicationStatus	receiveRequest();
		CommunicationStatus	sendResponse();
		bool				hasCommunicationTimeOut();
		Request				getRequest() const;
		bool				requestedCGI() const;
		struct pollfd		getPipeToParentFd() const;
		struct pollfd		getCGIToPipeFd() const;
		void				setCGIToPipeFd(int fd);
		void				setPipeToParentFd(int fd);
		CGI&				getCGI();

	private:
		CGI					_cgi;
		bool				_isCGI;
		int					_connectingServerFD;
		Request				_request;
		time_t				_startTimeCommunication;
		struct pollfd		_pipeToParentFd;
		struct pollfd		_CGIToPipeFd;
		ParserHTTP			_parser;
		ResponseData		_responseData;

		void				_resetRequest();
		bool				_checkContentLength();
		bool				_doneReceiving();

};

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i );

#endif /* **************************************************** CLIENTSOCKET_H */