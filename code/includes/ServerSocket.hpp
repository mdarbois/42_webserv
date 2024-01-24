/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:52 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/23 15:28:04 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <iostream>
# include <string>
# include <fcntl.h>
# include "Socket.hpp"

class ServerSocket: public Socket
{

	public:
		ServerSocket(int port, int ip, Config config);
		~ServerSocket();
		ServerSocket();
		ServerSocket &		operator=( ServerSocket const & rhs );

		int					getPort() const;
		struct sockaddr_in	getAddress() const;
		void				restartServerSocket();

	private:
		struct sockaddr_in	_serverSockAddr;
		int					_port;
		int					_ip;
		
		void				setUpSocket();

		ServerSocket( ServerSocket const & src );
};

std::ostream &			operator<<( std::ostream & o, ServerSocket const & i );

#endif /* **************************************************** SERVERSOCKET_H */