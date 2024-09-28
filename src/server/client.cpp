/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:20:50 by artclave          #+#    #+#             */
/*   Updated: 2024/09/28 12:25:33 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::init_client_struct(struct clientSocket &client){
	client.read_buffer.clear();
	client.write_buffer.clear();
	client.write_offset = 0;
	client.state = 0;
}

void	Server::accept_new_client_connection(struct serverSocket &server){
	if (!FD_ISSET(server.fd, &read_set))
		return ;
	client.fd = accept(server.fd, server.address_ptr, &server.address_len);//accept connections! (Now the client can connect) can only use flags with accept4 which is not allowed in subject //this client fd is a duplicate of our listenning fd but we will use for reading/writing because other fd is listening.... 
	if (client.fd < 0) //no new connections ....
		return ;
	static int p;
	std::cout<<"\n\nnew client connection accepted "<<p<<"\n\n";
	p++;
	setsockopt(client.fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout));
	int enable = 1;
	setsockopt(client.fd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
	int idle_time = 3; // 30 seconds
    setsockopt(client.fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle_time, sizeof(idle_time));
	init_client_struct(client);
	monitor_fds.push_back(client.fd);
	server.clientList.push_back(client);
}

void	Server::process_client_connection(struct clientSocket &client, struct serverSocket &server)
{
	client.read_operations = 0;
	client.write_operations = 0;
	read_request(client);
	init_http_process(client, server);
	execute_cgi(client);
	wait_cgi(client);	
	manage_files(client);
	write_response(client);
}

void	Server::delete_disconnected_clients(struct serverSocket &server)
{
	for (int j = 0; j < static_cast<int>(server.clientList.size()); )
	{
		if (server.clientList[j].state == DISCONNECT)
		{
			std::cout<<"Disconnected "<<server.clientList[j].fd<<" \n";
			FD_CLR(server.clientList[j].fd, &read_set);
			FD_CLR(server.clientList[j].fd, &write_set);
			monitor_fds.remove(server.clientList[j].fd);
			close(server.clientList[j].fd);
			server.clientList.erase(server.clientList.begin() + j);
			
		}
		else
			j++;
	}
}