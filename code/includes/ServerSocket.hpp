/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:17:52 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:13:29 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

# include <iostream>
# include <string>
# include "Socket.hpp"

#define MAX_CONNECTIONS 10

class ServerSocket: public Socket
{

	public:
		ServerSocket(int port, int ip);
		~ServerSocket();

	private:
		int	_port;
		int	_ip;
		
		void	setUpSocket();

		ServerSocket( ServerSocket const & src );
		ServerSocket &		operator=( ServerSocket const & rhs );
};

std::ostream &			operator<<( std::ostream & o, ServerSocket const & i );

#endif /* **************************************************** SERVERSOCKET_H */