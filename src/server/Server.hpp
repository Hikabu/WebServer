/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:33:26 by artclave          #+#    #+#             */
/*   Updated: 2024/09/06 10:28:18 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "includes.hpp"
#include "config/ConfigParser.hpp"
#include "config/ServerConfig.hpp"
#include "request_parser/HttpRequest.hpp"
#include "request_parser/RequestParser.hpp"
#include "response_builder/ResponseBuilder.hpp"

class Server{
	private:
		ServerConfig const &config;
	public:
		Server(ServerConfig const &config);
		~Server();
		void	run();
		void	check_non_blocking(int sockfd);
};

#endif