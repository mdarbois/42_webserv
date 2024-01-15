/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:22 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/15 17:14:31 by aehrlich         ###   ########.fr       */
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
#include <poll.h>
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"


//For Testing - later set up by config
# define MAX_GET_SIZE 8192
# define MAX_CONNECTIONS 10
#define TIMEOUT_POLL 500

const int serverIPs[] = {2130706433, 2130706433, 2130706433}; 	/* 127.0.0.1 converted to int */
const int serverPorts[] = {18000, 18001, 18002};
const int nbrServers = sizeof(serverPorts) / sizeof(serverPorts[0]);
//end

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
		std::vector<Socket *>	_sockets;
		struct pollfd			_pollFDs[MAX_CONNECTIONS]; //Should we just count the numbers of Clients or also the servers?
		void					_acceptNewClient(ServerSocket *socket);
		void					_deleteClient(ClientSocket *client);
		void					_receiveRequest(ClientSocket *client);
		void					_sendResponse(ClientSocket *client);
		void					_updatePollFDArray();
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */