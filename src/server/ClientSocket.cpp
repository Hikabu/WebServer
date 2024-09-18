/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:05:22 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 09:55:55 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

ClientSocket::ClientSocket() : connection(){ timeout.tv_sec = 2; timeout.tv_usec = 500000;}
ClientSocket::~ClientSocket(){}
int	ClientSocket::getFd() const {return fd;}

void	ClientSocket::initialize(ServerSocket *parentSocket, Multiplex *parentMultiplex)
{
	serverSocket = parentSocket;
	multiplex = parentMultiplex;
	connection.setMultiplex(multiplex);
	connection.setServerSocket(serverSocket);
	connection.setPossibleConfigs(serverSocket->getPossibleConfigs());
}

int	ClientSocket::connect(int parentFd, Address &parentAddress)
{
	fd = accept(parentFd, parentAddress.ptr, &parentAddress.len);
	if (fd < 0)
		throw ("No new client connections\n");
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout));
	int enable = 1;
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
	int idle_time = 3;
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle_time, sizeof(idle_time));
	connection.setFd(fd);
	return fd;
}

void	ClientSocket::closeConnection(int deleteFd, ServerSocket *socket, Multiplex *multiplex)
{
	multiplex->remove(deleteFd);
	close(deleteFd);
	std::vector<ClientSocket>::iterator it;
	for (it = socket->clientSocketList.begin(); it != socket->clientSocketList.end(); it++)
	{
		if (it->getFd() == deleteFd)
		{
			socket->clientSocketList.erase(it);
			std::cout<<"Close connection\n";
			return ;
		}
	} 

};
