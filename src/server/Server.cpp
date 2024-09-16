/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:31:54 by artclave          #+#    #+#             */
/*   Updated: 2024/09/13 11:52:07 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/EventManager.hpp"
#include <string.h>

Server::Server() : port(0), 			
				   clientMaxBodySize(0), 
				   serverName(""),
				   serverConf(std::vector<std::string>()),
				   config(*(new std::vector<ServerConfig>())) {}
	// this->locations = std::vector<Location>();}
	
Server::Server(std::vector<ServerConfig>& config) : config(config) {
	eventManager = new EventManager();
	(void)config;
}

Server::~Server(){}

void	check_non_blocking(int sockfd){
	 int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr<<"fcntl";
        exit(EXIT_FAILURE);
    }
    if (flags & O_NONBLOCK) {
        std::cout<<"Socket is non-blocking.\n";
    } else {
        std::cout<<"Socket is blocking.\n";
    }
}

void Server::createUniquePair() //create unique pairs of host (IP address) and port from a collection of server_configs
{
	std::cout << config.size() << std::endl;
	for (size_t i = 0; i < config.size(); i++) {
		std::cout << config[i].getHost() << std::endl;
		std::pair<std::string, int> uniquePair = std::make_pair(config[i].getHost(), config[i].getPort()); //auto type /pair from different type;
		unique.insert(uniquePair);
	}
}

void Server::createEventManager() {
	for(std::set<std::pair<std::string, int> >::iterator it = unique.begin(); it != unique.end(); it++){
		ServerSocket listenSocket;
		listenSocket.initListenSocket(it->first, it->second);
		eventManager->addServerSocket(listenSocket);
	}
	std::cout << "Event manager creation completed." << std::endl;
}
void Server::run(){
	createUniquePair(); //prevent duplicate configurations, prepares the data for the createEventManager()
	createEventManager();
	eventManager->copyArray(servers);
	eventManager->waitandleEvents();
}