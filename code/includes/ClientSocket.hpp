/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aehrlich <aehrlich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:16:15 by aehrlich          #+#    #+#             */
/*   Updated: 2024/01/10 12:25:00 by aehrlich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <iostream>
# include <string>
#include "Socket.hpp"

class ClientSocket: public Socket
{

	public:
		ClientSocket(int serverFD);
		void	setUpSocket();

	private:
		int	_connectingServerFD;
		~ClientSocket();
		ClientSocket( ClientSocket const & src );
		ClientSocket &		operator=( ClientSocket const & rhs );

};

std::ostream &			operator<<( std::ostream & o, ClientSocket const & i );

#endif /* **************************************************** CLIENTSOCKET_H */