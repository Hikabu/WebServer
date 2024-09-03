/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:00:00 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/03 14:00:21 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

// Default constructor
ServerConfig::ServerConfig()
    : listen(80), host("127.0.0.1"), clientMaxBodySize(1024 * 1024), index("index.html") {
}

// Constructor with parameters
ServerConfig::ServerConfig(int listen, std::string host, std::string serverName, size_t clientMaxBodySize, std::string root, std::string index, std::vector<LocationConfig> locations, CGIConfig cgi)
    : listen(listen), host(host), serverName(serverName), clientMaxBodySize(clientMaxBodySize), root(root), index(index), locations(locations), cgi(cgi) {
}

// Copy constructor
ServerConfig::ServerConfig(const ServerConfig& other)
    : listen(other.listen),
      host(other.host),
      serverName(other.serverName),
      errorPages(other.errorPages),
      clientMaxBodySize(other.clientMaxBodySize),
      root(other.root),
      index(other.index),
      locations(other.locations),
      cgi(other.cgi) {
}

// Copy assignment operator
ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
    if (this != &other) {
        listen = other.listen;
        host = other.host;
        serverName = other.serverName;
        errorPages = other.errorPages;
        clientMaxBodySize = other.clientMaxBodySize;
        root = other.root;
        index = other.index;
        locations = other.locations;
        cgi = other.cgi;
    }
    return *this;
}

// Destructor
ServerConfig::~ServerConfig() {
}