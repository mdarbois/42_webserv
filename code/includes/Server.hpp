/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:22 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 17:00:03 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

class Server
{

	public:
		Server();
		~Server();
		Server( Server const & src );
		Server &		operator=( Server const & rhs );

		void	run();
		void	shutdown();

	private:
		ServerSocket				_mainSocket;
		ClientSocket				*_testClientSocket;
		//std::vector<ClientSocket>	_clientSockets;
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */