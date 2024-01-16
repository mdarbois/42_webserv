/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/16 17:44:08 by aehrlich         ###   ########.fr       */
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

typedef enum eSocketType
{
	CLIENT,
	SERVER
}t_socketType;

typedef enum e_ComminationStatus
{
	COM_ERROR = -1,
	COM_IN_PROGRESS,
	COM_DONE,
	COM_CONN_CLOSED
}CommunicationStatus;

class Socket
{

	public:
		virtual ~Socket();
		Socket();
		Socket( Socket const & src );
		Socket &		operator=( Socket const & rhs );

		virtual void	setUpSocket() = 0;
		t_socketType	getType() const;
		int				getFD() const;
		void			setPollFD(int fd, short events, short revents);
		struct pollfd	getPollFD() const;
		bool			operator==(const Socket& other);

	protected:
		t_socketType	_type;
		struct pollfd	_pollFD;
};

//std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */