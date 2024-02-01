/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:37:22 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/31 09:22:49 by aehrlich         ###   ########.fr       */
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
#include "ParserHTTP.hpp"


//For Testing - later set up by config
# define MAX_GET_SIZE 8192
# define MAX_CONNECTIONS 10
#define TIMEOUT_POLL 500

// Enum to represent different colors
enum LogColor {
	RED,
	GREEN,
	YELLOW,
	BLUE,
	DEFAULT // Default color (usually white or terminal default)
};

class ServerManager
{

	public:
		
		ServerManager(Config const &config);
		~ServerManager();

		void	run();
		void	log(const std::string& message, LogColor color);
		void	shutdown();

	private:
		std::vector<Socket *>	_sockets;
		struct pollfd			_pollFDs[1000]; //Should we just count the numbers of Clients or also the servers?
		int						_numberServers;
		int						_numberPollFDs;
		Config					_config;
		void					_acceptNewClient(ServerSocket *socket);
		void					_deleteClient(ClientSocket *client, HttpStatus code);
		void					_receiveRequest(ClientSocket *client);
		bool					_checkPollErrors(SocketType socketType, int socketIdx, short int revent);
		void					_sendResponse(ClientSocket *client);
		void					_updatePollFDArray();
		SocketType				_getSocketTypeForPollFdIdx(int idx, int *socketIndex) const;

		ServerManager();
		ServerManager( ServerManager const & src );
		ServerManager &		operator=( ServerManager const & rhs );
};

std::ostream &			operator<<( std::ostream & o, ServerManager const & i );

#endif /* ********************************************************** SERVER_H */