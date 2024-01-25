/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/24 16:53:00 by aehrlich         ###   ########.fr       */
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

# define CLIENT_RECEIVE_BUFFER_SIZE 1024
# define CLIENT_TIMEOUT_RECEIVE 5

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

	private:
		int					_connectingServerFD;
		Request				_request;
		time_t				_startTimeCommunication;

		ResponseData		_responseData;

		void				_resetRequest();
		bool				_checkContentLength();

};

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i );

#endif /* **************************************************** CLIENTSOCKET_H */