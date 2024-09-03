/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:00:00 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/03 15:00:49 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

// Default constructor
ServerConfig::ServerConfig() { }


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

// Getters
int ServerConfig::getListen() const { return listen; }
const std::string& ServerConfig::getHost() const { return host; }
const std::string& ServerConfig::getServerName() const { return serverName; }
const std::map<int, std::string>& ServerConfig::getErrorPages() const { return errorPages; }
size_t ServerConfig::getClientMaxBodySize() const { return clientMaxBodySize; }
const std::string& ServerConfig::getRoot() const { return root; }
const std::string& ServerConfig::getIndex() const { return index; }
std::vector<LocationConfig>& ServerConfig::getLocations() { return locations; }
const std::vector<LocationConfig>& ServerConfig::getLocations() const { return locations; }
const CGIConfig& ServerConfig::getCgi() const { return cgi; }

// Setters
void ServerConfig::setListen(int value) { listen = value; }
void ServerConfig::setHost(const std::string& value) { host = value; }
void ServerConfig::setServerName(const std::string& value) { serverName = value; }
void ServerConfig::setErrorPages(const std::map<int, std::string>& value) { errorPages = value; }
void ServerConfig::setClientMaxBodySize(size_t value) { clientMaxBodySize = value; }
void ServerConfig::setRoot(const std::string& value) { root = value; }
void ServerConfig::setIndex(const std::string& value) { index = value; }
void ServerConfig::setLocations(const std::vector<LocationConfig>& value) { locations = value; }
void ServerConfig::setCgi(const CGIConfig& value) { cgi = value; }