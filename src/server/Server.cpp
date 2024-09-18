/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:41:28 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 05:26:10 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	multiplex = new Multiplex();
	// signal(SIGPIPE, SIG_IGN);
}

Server::~Server() { delete multiplex;}

void Server::run(const std::vector<ServerConfig> &config)
{
	groupDuplicateHostPorts(config);
	startListening();
	while (1)
	{
	 	multiplex->startMonitor();
		for (int i = 0; i < static_cast<int>(serverSocketList.size()); i++)
		{
			serverSocketList[i].handleClients();
		}
	}
}

void Server::groupDuplicateHostPorts(const std::vector<ServerConfig> &config)
{
	for (int i = 0; i < static_cast<int>(config.size()); i++)
	{
		configGroups[config[i].getListen()].push_back(config[i]);
	}
}

void	Server::startListening()
{
	std::map<std::string, std::vector<ServerConfig> >::iterator it;
	for (it = configGroups.begin(); it != configGroups.end(); it++)
	{
		setUpServerSocket(it);
	}
	if (serverSocketList.empty())
	{
		std::cerr<<"Error: no valid server sockets\n";
		exit(2);
	}
}

void	Server::setUpServerSocket(std::map<std::string, std::vector<ServerConfig> >::iterator &it)
{
	ServerSocket	serverSocket;
	serverSocket.initialize(it, multiplex);
	try
	{
		int serverFd = serverSocket.createBindListen();
		serverSocketList.push_back(serverSocket);
		multiplex->add(serverFd);
	} 
	catch (std::string errorMessage)
	{
		std::cerr<<errorMessage<<"\n";
	}
}