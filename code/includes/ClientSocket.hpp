/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/22 11:50:19 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"
# include "ServerSocket.hpp"

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

class ClientSocket: public Socket
{
	
	public:
		ClientSocket(int connectingServerFD);
		~ClientSocket();
		ClientSocket();
		ClientSocket( ClientSocket const & src );
		ClientSocket &		operator=( ClientSocket const & rhs );
		
		void				setUpSocket();
		CommunicationStatus	receiveRequest();
		bool				hasReceiveTimeOut();
		Request				getRequest() const;

	private:
		int		_connectingServerFD;
		Request	_request;
		time_t	_startTimeReceive;

		void	_resetRequest();
		bool	_checkContentLength();

};

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i );

#endif /* **************************************************** CLIENTSOCKET_H */