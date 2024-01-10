/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:03 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:05:45 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

typedef enum eSocketType
{
	CLIENT,
	SERVER
};

class Socket
{

	public:
		~Socket();

		virtual void	setUpSocket() = 0;
		eSocketType		getType() const;
		int				getFD() const;

		bool			operator==(const Socket& other);

	protected:
		eSocketType	_type;
		int			_fd;
		Socket();
		Socket();
		Socket( Socket const & src );
		Socket &		operator=( Socket const & rhs );
};

//std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */