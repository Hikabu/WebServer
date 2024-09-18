/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:40:45 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 02:37:48 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.hpp"
#include "config/ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "Multiplex.hpp"

class Server
{
	private:
	//ATTRIBUTES
		std::map<std::string, std::vector<ServerConfig>	>	configGroups;
		std::vector<ServerSocket>							serverSocketList;
		Multiplex											*multiplex;

	//METHODS
	void	groupDuplicateHostPorts(const std::vector<ServerConfig> &config);
	void	startListening();
	void	setUpServerSocket(std::map<std::string, std::vector<ServerConfig> >::iterator &it);

	public:
		Server();
		~Server();
		void	run(const std::vector<ServerConfig> &config);
};
