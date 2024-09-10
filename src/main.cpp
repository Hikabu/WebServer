/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:09:33 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/10 00:54:00 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/include.hpp"

#include "../includes/Server.hpp"
#include "../includes/ConfigParser.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/HttpRequest.hpp"
// #include "../includes/ResponseBuilder.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file_path>" << std::endl;
        return 1;
    }

	std::vector<ServerConfig> config = ConfigParser::parse(argv[1]);
	
	//print each one
	for (size_t i = 0; i < config.size(); i++) {
		config[i].print();
	}
	try {
		Server server(config);
		server.run();	
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return (1);
	}
    return 0;
}
