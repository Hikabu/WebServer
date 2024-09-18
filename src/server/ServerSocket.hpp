/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 22:41:57 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 09:33:27 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "includes.hpp"
#include "Multiplex.hpp"
#include "Address.hpp"
#include "config/ServerConfig.hpp"
#include "ClientSocket.hpp"

class	ServerSocket
{
	private:
	//ATTRIBUTES:
		int							fd;
		uint32_t					host;
		int							port;
		Address						address;
		std::vector<ServerConfig>	possibleConfigs;
		Multiplex					*multiplex;
		

	//METHODS:
		int			extract_port(const std::string &str) const;
		uint32_t	extract_host(const std::string &str) const;

	public:
	//after testing that close connection works please move back to private
		std::vector<ClientSocket>	clientSocketList;

		ServerSocket();
		~ServerSocket();
		void		initialize(std::map<std::string, std::vector<ServerConfig> >::iterator &it, Multiplex *sharedMultiplex);
		int			createBindListen();
		void		handleClients();
		void		setUpClientSocket();
		int			getFd();
		std::vector<ServerConfig>	getPossibleConfigs(); //NEED FOR CONNECTION ATM
};

#endif