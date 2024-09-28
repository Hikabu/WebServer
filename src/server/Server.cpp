/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:31:54 by artclave          #+#    #+#             */
/*   Updated: 2024/09/28 16:37:09 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <string.h>
#include <ctime>


Server::Server(std::vector<ServerConfig>& config) : config(config){
	std::map<std::string, std::vector<ServerConfig>	> combos;
	for (int i = 0; i < static_cast<int>(config.size()); i++)
		combos[config[i].getListen()].push_back(config[i]);
	std::vector<struct serverSocket>	new_server(combos.size());
	int i = 0;
	for (std::map<std::string, std::vector<ServerConfig> >::iterator it = combos.begin(); it != combos.end(); it++)
	{
		new_server[i].port = extract_port(it->first);
		new_server[i].host = extract_host(it->first);
		new_server[i].possible_configs = it->second;
		serverList.push_back(new_server[i]);//add server struct to vector of servers...
		i++;
	}
	timeout.tv_sec = 1;       // 2 seconds
    timeout.tv_usec = 0; // 500 milliseconds
	signal(SIGPIPE, SIG_IGN);
}

Server::~Server(){}

int	Server::server_socket_error(std::string type, int *i){
	std::cerr<<type<<" failed: "<<std::strerror(errno)<<"\n";
	std::cerr<<"For server with host and port: "<<serverList[*i].possible_configs[0].getListen()<<"\n";
	close(serverList[*i].fd);
	serverList.erase(serverList.begin() + *i);
	*i = *i - 1;
	return (2);
}

int	Server::server_sockets_for_listening(){
	int opt = 1;
	struct sockaddr_in address_ipv4;
	int	flags;
	for (int i = 0; i < static_cast<int>(serverList.size()); i++)
	{	
		serverList[i].fd = socket(AF_INET, SOCK_STREAM, 0); //created socket fd, NON-BLOCKING flag
		if (serverList[i].fd == -1)
			return (server_socket_error("Socket", &i));
		flags = fcntl(serverList[i].fd, F_GETFL, 0);
		if (flags == -1)
			return (server_socket_error("fcntl F_GETFL", &i));
		if (fcntl(serverList[i].fd, F_SETFL, flags | O_NONBLOCK) == -1)
			return (server_socket_error("fcntl F_SETFL", &i));
		setsockopt(serverList[i].fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//We can bind to an address that is already bound. Without SO_REUSEADDR, server can fail to bind if the socket it is still held by the prev server run. 
		memset(&address_ipv4, 0, sizeof(address_ipv4));
		address_ipv4.sin_family = AF_INET;//family AF_INET for ipv4
		address_ipv4.sin_port = htons(serverList[i].port); //converts port from host byte order to network byte order
		address_ipv4.sin_addr.s_addr = htonl(serverList[i].host); //converts ip int from host byte order to network byte order
		if (bind(serverList[i].fd, (struct sockaddr *)&address_ipv4, sizeof(address_ipv4)) == -1) //binding socket to address of the port we are supposed to listen from
			return (server_socket_error("Bind", &i));
		if (listen(serverList[i].fd, 32) == -1) //listen to max 32 users
			return (server_socket_error("Listen", &i));
		monitor_fds.push_back(serverList[i].fd);
	}
	if (static_cast<int>(serverList.size()) == 0)
	{
		std::cerr<<"Sorry there are no valid servers for listening....\n";
		exit(2);
	}
	return 0;
}

void	Server::init_sets_for_select(){ //master stuff
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	for (std::list<int>::iterator it = monitor_fds.begin(); it != monitor_fds.end(); it++)
	{
		FD_SET(*it, &read_set);
		FD_SET(*it, &write_set);
	}
}


void	Server::run(){
	//set_up_signals(); //we can use this for submission to avoid server dying but for now we should keep like this to see errors...
	server_sockets_for_listening();
	while (server_running)
	{
		init_sets_for_select();
		if (select(*max_element(monitor_fds.begin(), monitor_fds.end()) + 1, &read_set, &write_set, 0, &timeout) < 0)
		{
			std::cerr<<strerror(errno)<<"\n";
		}
		for (int i = 0; i < static_cast<int>(serverList.size()); i++)
		{
			for (int j = 0; j < static_cast<int>(serverList[i].clientList.size()); j++)
			{
				std::cout<<"client "<<serverList[i].clientList[j].fd<<", state "<<serverList[i].clientList[j].state<<"\n";
				process_client_connection(serverList[i].clientList[j], serverList[i]);
			}
			delete_disconnected_clients(serverList[i]);
			accept_new_client_connection(serverList[i]);
		}
	}
	server_running = 1;
	//std::cout<<"BREAKING OUT\n";
	//here we close all fds so we can bind again ... if a signal is oveerridden the loop will break clean and delete object 
	//in case we ran out of memory or break a pipe or something
	for (std::list<int>::iterator it = monitor_fds.begin(); it != monitor_fds.end(); it++)
		close(*it);
}