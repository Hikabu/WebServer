/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 05:36:51 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 09:37:24 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include "includes.hpp"
#include "Multiplex.hpp"
#include "config/ServerConfig.hpp"
#include "response_builder/RequestResponse.hpp"
#include "response_builder/ResponseBuilder.hpp"
#include "request_parser/RequestParser.hpp"
#include "request_parser/HttpRequest.hpp"

#define WRITE_BUFFER_SIZE 1024 //delete if inheritance
# define READ_BUFFER_SIZE 1024 //delete if inheritance
class ServerSocket;
class Connection //: public ClientSocket -> implement later
{
	private:
		int							step;
		int							writeOffset;
		int							readOperations, writeOperations;
		std::string					readBuffer, writeBuffer;
		std::vector<ServerConfig>	possibleConfigs;
		ServerConfig				matchConfig;
		int							fd; //delete if inheritance
		struct timeval				timeout;//delete if inheritance
		Multiplex					*multiplex;//delete if inheritance
		ServerSocket				*serverSocket;//delete if inheritance
		void	readRequest();
		bool	isReadRequestIncomplete();
		void	findMatchConfig(const std::string host);
		void	writeResponse();
		void	processHttp();

	public:
		Connection();
		~Connection();
		void	processConnection();
		void	setFd(int value);//delete if inheritance
		void	setMultiplex(Multiplex *value);//delete if inheritance
		void	setServerSocket(ServerSocket *value);//delete if inheritance
		void	setPossibleConfigs(std::vector<ServerConfig> value);//delete if inheritance
};
# endif 