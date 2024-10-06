/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:09:33 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/05 17:55:06 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes.hpp"

#include "server/ServerCore.hpp"
#include "config/ConfigParser.hpp"
#include "config/ServerConfig.hpp"
#include "request_parser/HttpRequest.hpp"
#include "response_builder/ResponseBuilder.hpp"


int main(int argc, char* argv[]) {
	signal(SIGPIPE, SIG_IGN);
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
        return 1;
	}
	std::vector<ServerConfig> config = ConfigParser::parse(argv[1]);
	try {
		ServerCore server(config);
		server.run();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
    return 0;
}
