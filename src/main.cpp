/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:09:33 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/05 11:44:26 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes.hpp"

#include "server/ServerCore.hpp"
#include "config/ConfigParser.hpp"
#include "config/ServerConfig.hpp"
#include "request_parser/HttpRequest.hpp"
#include "response_builder/ResponseBuilder.hpp"


int main(int argc, char* argv[]) {
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
