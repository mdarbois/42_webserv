/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/02/15 16:07:04 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include "ServerConfig.hpp"

typedef enum eSocketType
{
	NO_SOCKET,
	CLIENT,
	SERVER
}SocketType;

typedef enum e_ComminationStatus
{
	COM_ERROR = -1,
	COM_IN_PROGRESS,
	COM_DONE,
	CGI_PENDING,
	COM_CONN_CLOSED
}CommunicationStatus;

typedef enum e_HttpStatus
{
	NO_ERROR = 0,
	HTTP_200 = 200,
	HTTP_201 = 201,
	HTTP_202 = 202,
	HTTP_204 = 204,
	HTTP_206 = 206,
	HTTP_300 = 300,
	HTTP_301 = 301,
	HTTP_302 = 302,
	HTTP_303 = 303,
	HTTP_304 = 304,
	HTTP_307 = 307,
	HTTP_308 = 308,
	HTTP_400 = 400,
	HTTP_403 = 403,
	HTTP_404 = 404,
	HTTP_405 = 405,
	HTTP_408 = 408,
	HTTP_409 = 409,
	HTTP_413 = 413,
	HTTP_500 = 500,
	HTTP_504 = 504,
	HTTP_505 = 505
} HttpStatus;

class Socket
{

	public:
		virtual ~Socket();
		Socket();
		Socket( Socket const & src );
		Socket &		operator=( Socket const & rhs );

		virtual void	setUpSocket() = 0;
		SocketType		getType() const;
		int				getFD() const;
		void			setPollFD(int fd, short events, short revents);
		struct pollfd	getPollFD() const;
		ServerConfig	getServerConfig() const;
		bool			operator==(const Socket& other);

	protected:
		SocketType	_type;
		struct pollfd	_pollFD;
		ServerConfig	_config;
};

#endif /* ********************************************************** SOCKET_H */