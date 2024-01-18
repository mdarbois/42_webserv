/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/18 14:32:09 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"
# include "ServerSocket.hpp"

//class ServerSocket;

typedef enum e_RequestType
{
	SINGLE = 0,
	CONTENT_LENGTH,
	CHUNKED_ENCODING,
	MULTIPART_FORM_DATA
}RequestType;

typedef struct s_Request
{
	size_t		contentLength;
	size_t		readBytes;
	std::string	buffer;
	RequestType	type;
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

	private:
		int		_connectingServerFD;
		Request	_request;

		void	_resetRequest();

};

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i );

#endif /* **************************************************** CLIENTSOCKET_H */