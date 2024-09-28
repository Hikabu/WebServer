/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:28:29 by artclave          #+#    #+#             */
/*   Updated: 2024/09/28 12:31:41 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Server.hpp"
//UTILS:

void	Server::find_match_config(struct clientSocket &client, std::vector<ServerConfig> &possible_configs, const std::string host)
{
	std::vector<std::string> possible_names;
	client.match_config = possible_configs[0];
	for (int i = 0; i < static_cast<int>(possible_configs.size()); i++)
	{
		possible_names = possible_configs[i].getServerNames();
		for (int j = 0; j < static_cast<int>(possible_names.size()); j++)
		{
			if (possible_names[j] == host)
			{
				client.match_config = possible_configs[i];
				return ;
			}
		}
	}
}

int	extract_port(const std::string &str){
	std::string::const_iterator semi_colon = std::find(str.begin(), str.end(), ':');
	int pos = std::distance(str.begin(), semi_colon);
	return (std::atoi(str.substr(pos + 1).c_str()));
}

uint32_t	extract_host(const std::string &str){
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
//SIGNALS
int server_running = 1;

void	signalHandler(int signal)
{
	(void)signal;
	server_running = 0;
	//std::cout<<"STOP!\n";
}

void	set_up_signals()
{
	std::signal(SIGTERM, signalHandler);
	std::signal(SIGKILL, signalHandler);
	std::signal(SIGPIPE, signalHandler);
	std::signal(SIGINT, signalHandler);//this is for control c not sure if w should override it too....
}
//END SIGNALS:
//END UTILS: