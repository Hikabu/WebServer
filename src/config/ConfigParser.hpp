/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:40:11 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/03 13:56:56 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "ServerConfig.hpp"
#include <vector>
#include <string>

class ConfigParser {
public:
	ConfigParser(const std::string& configFile);
	ServerConfig parse();

private:
	std::string configFile;
	// Add helper methods for parsing different sections
};