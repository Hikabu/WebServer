/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:38:57 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/03 14:00:52 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <string>
#include <vector>
#include <map>

struct LocationConfig {
    std::string path;
    std::string root;
    bool autoindex;
    std::vector<std::string> allowMethods;
    std::string index;
    std::string return_url;
    std::string alias;
};

struct CGIConfig {
    std::string root;
    std::string path;
    std::string ext;
};

class ServerConfig {
public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

    // Getters
    int getListen() const { return listen; }
    const std::string& getHost() const { return host; }
    const std::string& getServerName() const { return serverName; }
    const std::map<int, std::string>& getErrorPages() const { return errorPages; }
    size_t getClientMaxBodySize() const { return clientMaxBodySize; }
    const std::string& getRoot() const { return root; }
    const std::string& getIndex() const { return index; }
    const std::vector<LocationConfig>& getLocations() const { return locations; }
    const CGIConfig& getCgi() const { return cgi; }

private:
    int listen;
    std::string host;
    std::string serverName;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize;
    std::string root;
    std::string index;
    std::vector<LocationConfig> locations;
    CGIConfig cgi;
};

#endif 