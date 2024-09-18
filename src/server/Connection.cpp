/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 05:28:34 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 10:04:15 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "ServerSocket.hpp"

Connection::Connection(){timeout.tv_sec = 2; timeout.tv_usec = 500000; step = 0;}
Connection::~Connection(){}

void	Connection::setFd(int value) {fd = value;}
void	Connection::setMultiplex(Multiplex *value){multiplex = value;}
void	Connection::setServerSocket(ServerSocket *value){serverSocket = value;}
void	Connection::setPossibleConfigs(std::vector<ServerConfig> value){possibleConfigs = value;}

void	Connection::processConnection()
{
	writeOperations = false;
	readOperations = false;
	try 
	{
		switch(step)
		{
			case 0:
				std::cout<<"STARTED READ REQUEST\n";
				readRequest();
				std::cout<<"FINISHED READ REQUEST\n\n";
				step++;
			case 1:
				std::cout<<"STARTED HTTP REQUEST\n";
				processHttp();
				std::cout<<"FINISHED HTTP REQUEST\n\n";
				step++;
			// case 2:
			// 	monitorFiles(); 
			// //before reading or writing we need to add the files to multiplex array of active fds to be able to use select
			// 	step++;
			//	//if we dont have to read bodyfile or save file we can keep going else, monitorFIles wil thorow exception and state remains unchanged ...
			// case 5:
			// 	buildBodyFromFile();
			// 	step++;
			// case 6:
			// 	saveFile();
			// 	step++;
			// case 6:
			// 	cgi();
			case 2:
				std::cout<<"STARTED WRITE\n";
				writeResponse();
				std::cout<<"FINISHED WRITE\n\n";
		}
		ClientSocket::closeConnection(fd, serverSocket, multiplex);
	}
	catch(const char *mssg)
	{
		//std::cerr<<mssg<<"\n";
	}
}

void	Connection::readRequest()
{
	if (readOperations > 0 || !multiplex->isReadyForReading(fd))
	{
		throw("Can not read right now\n");
	}
	std::string	buff(READ_BUFFER_SIZE, 0);
	int bytes = recv(fd, &buff[0], READ_BUFFER_SIZE, 0);
	if (bytes <= 0)
	{
		ClientSocket::closeConnection(fd, serverSocket, multiplex);
		throw("Client disconnected\n");
	}
	readOperations++;
	for (int i = 0; i < bytes; i++)
		readBuffer += buff[i];
	if (isReadRequestIncomplete())
	{
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout));
		throw("Reading incomplete\n");
	}
}

bool	Connection::isReadRequestIncomplete()
{
	std::size_t	pos_zero, pos_content_length, pos_header_end;
	pos_header_end = readBuffer.find("\r\n\r\n");
	if (pos_header_end == std::string::npos)
		return true;
	pos_content_length = readBuffer.find("Content-Length:");
	if (pos_content_length != std::string::npos)
	{
		int expected_body_size = std::atoi(readBuffer.substr(pos_content_length + 16, pos_header_end).c_str());
		if (static_cast<int>(readBuffer.size() - pos_header_end) < expected_body_size)
			return true;
	}
	else if (readBuffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		pos_zero = readBuffer.find("0\r\n\r\n");
		if (pos_zero == std::string::npos || readBuffer[pos_zero + 5] != 0)
			return true;
	}
	return false;
}

void	Connection::findMatchConfig(const std::string host)
{
	matchConfig = possibleConfigs[0];
	std::vector<ServerConfig>::iterator it; 
	for (it = possibleConfigs.begin(); it != possibleConfigs.end(); it++)
	{
		std::vector<std::string> names = it->getServerNames();
		if (std::find(names.begin(), names.end(), host) != names.end())
		{
			matchConfig = *it;
			return ;
		}
	}
}

void	Connection::processHttp()
{
	HttpRequest request = RequestParser::parse(readBuffer);
	findMatchConfig(request.getHost());
	writeBuffer = ResponseBuilder::build(request, matchConfig).toString();
}

void	Connection::writeResponse()
{
	if (writeOperations > 0 || !multiplex->isReadyForWriting(fd))
	{
		throw("Can not read right now\n");
	}
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	int bytes = send(fd, &writeBuffer[writeOffset], sizeof(writeBuffer), 0);
	if (bytes < 0)
	{
		ClientSocket::closeConnection(fd, serverSocket, multiplex);
		throw("client disconnected\n");
	}
	writeOperations++;
	writeOffset += bytes;
	//std::cout<<"bytes ("<<bytes<<"), offset("<<writeOffset<<", total("<<writeBuffer.size()<<")\n";
	if (writeOffset < static_cast<int>(writeBuffer.size()))
	{
		throw("writing incomplete ... \n");
	}
}
