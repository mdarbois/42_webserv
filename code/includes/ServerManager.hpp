/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:22 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/16 15:31:41 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <poll.h>
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "Config.hpp"
#include "ServerConfig.hpp"
#include "utils.hpp"


//For Testing - later set up by config
# define MAX_GET_SIZE 8192
# define MAX_CONNECTIONS 10
#define TIMEOUT_POLL 500

class ServerManager
{

	public:
		
		ServerManager(Config const &config);
		~ServerManager();

		void	run();
		void	shutdown();

	private:
		std::vector<Socket *>	_sockets;
		struct pollfd			_pollFDs[MAX_CONNECTIONS]; //Should we just count the numbers of Clients or also the servers?
		int						_numberServers;
		void					_acceptNewClient(ServerSocket *socket);
		void					_deleteClient(ClientSocket *client);
		void					_receiveRequest(ClientSocket *client);
		bool					_checkPollErrors(Socket *socket, short int revent);
		void					_sendResponse(ClientSocket *client);
		void					_updatePollFDArray();

		ServerManager();
		ServerManager( ServerManager const & src );
		ServerManager &		operator=( ServerManager const & rhs );
};

std::ostream &			operator<<( std::ostream & o, ServerManager const & i );

#endif /* ********************************************************** SERVER_H */