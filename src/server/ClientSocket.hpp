/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:14:52 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 09:22:06 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

#include "includes.hpp"
#include "Connection.hpp"

# define READ_BUFFER_SIZE 1024

class ServerSocket;

class ClientSocket
{
	private:
		int							fd;
		ServerSocket				*serverSocket;
		struct timeval				timeout;
		// int							step;
		// int							writeOffset;
		// int							writeOperations;
		// int							readOperations;
		// std::vector<ServerConfig>	possibleConfigs;
		// std::string					writeBuffer, readBuffer;
		
	public:
		Connection					connection;
				Multiplex					*multiplex;//change to private if inheritnca, or preotcted i should say. ..
		ClientSocket();
		~ClientSocket();
		void	initialize(ServerSocket *parentSocket, Multiplex *parentMultiplex);
		int		connect(int parentSocketFd, Address &parentAddress);
		//void	processConnection();
		static void	closeConnection(int deleteFd, ServerSocket *socket, Multiplex *multiplex);
		int		getFd() const;
		
};

#endif