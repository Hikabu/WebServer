/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:40:11 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/03 15:10:40 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "ServerConfig.hpp"
#include <vector>
#include <string>
#include <cstdlib>

class ConfigParser {
public:
	ConfigParser(const std::string& configFile);
	ServerConfig parse();
	
	

private:
	std::string configFile;
};

#endif
