/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 22:41:37 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 09:35:49 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
// #include "ClientSocket.hpp"

ServerSocket::ServerSocket(){}
ServerSocket::~ServerSocket(){}
int	ServerSocket::getFd(){ return fd;}
std::vector<ServerConfig>	ServerSocket::getPossibleConfigs() {return possibleConfigs;}//NEED FOR CONNECTION ATM

int	ServerSocket::extract_port(const std::string &str) const
{
	std::string::const_iterator semi_colon = std::find(str.begin(), str.end(), ':');
	int pos = std::distance(str.begin(), semi_colon);
	return (std::atoi(str.substr(pos + 1).c_str()));
}

uint32_t	ServerSocket::extract_host(const std::string &str) const
{
	std::string::const_iterator it[5];
	int	oct[4];
	it[0] = str.begin();
	for (int i = 1; i < 4; i++)
		it[i] = std::find(it[i - 1] + 1, str.end(), '.') + 1;
	it[4] = std::find(str.begin(), str.end(), ':') + 1;
	for (int i = 0; i < 4; i++)
	{
		std::string substr = str.substr(it[i] - str.begin(), it[i + 1] - it[i] - 1);
		oct[i] = static_cast<uint8_t>(std::atoi(substr.c_str()));
	}
	return (oct[0] << 24 | oct[1] << 16 | oct[2] << 8 | oct[3]);
}

void	ServerSocket::initialize(std::map<std::string, std::vector<ServerConfig> >::iterator &it, Multiplex *sharedMultiplex)
{
	multiplex = sharedMultiplex;
	port = extract_port(it->first);
	host = extract_host(it->first);
	address.initialize(port, host);
	possibleConfigs = it->second;
}

int		ServerSocket::createBindListen()
{
	fd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
	if (fd < 0)
		throw ("Server socket's socket() failed\n");
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(fd, address.ptr, address.len) == -1)
		throw ("Server socket's bind() failed\n");
	if (listen(fd, 32) == -1)
		throw ("Server socket's listen() failed\n");
	return fd;
}

void		ServerSocket::handleClients()
{
	for (int i = 0; i < static_cast<int>(clientSocketList.size()); i++)
	{
		clientSocketList[i].connection.processConnection();
	}
	setUpClientSocket();
}

void		ServerSocket::setUpClientSocket()
{
	ClientSocket	clientSocket;
	clientSocket.initialize(this, multiplex);
	try
	{
		multiplex->isReadyForReading(fd);
		int clientFd = clientSocket.connect(fd, address);
		clientSocketList.push_back(clientSocket);
		multiplex->add(clientFd);
	} 
	catch (char const * errorMessage)
	{
		//std::cerr<<errorMessage<<"\n";
	}
}

