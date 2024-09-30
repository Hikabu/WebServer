#include "../../include/Server.hpp"

Server::Server() {
    this->serverConfig = std::vector<std::string>();
    this->port = 0;
    this->clientMaxBodySize = 0;
    this->serverName = "";
    this->ipAddress = "";
    this->locations = std::vector<Location>();
    this->errorPages = std::map<int, std::string>();

}

void Server::setConfig(std::string &configLine) {
    this->serverConfig.push_back(configLine);
}

void Server::printDataConfig() const {
    std::cout << "----------------------------------" << std::endl;
    for (std::vector<std::string>::const_iterator it = this->serverConfig.begin(); it != this->serverConfig.end(); it++) {
        std::cout << *it << std::endl;
    }
}

void Server::printServerData() {
    std::cout << "-----------------" << std::endl << "Server WebServer:" << std::endl << "-----------------" << std::endl;
    std::cout << "ipAddress: " << ipAddress << std::endl;
    std::cout << "serverName: " << serverName << std::endl;
    std::cout << "port: " << port << std::endl;
    std::cout << "clientMaxBodySize: " << clientMaxBodySize << std::endl;
    std::cout << "errorPages: " << std::endl;
    for (std::map<int, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
        std::cout << "Key: " << it->first << " | Value: " << it->second << std::endl;
    }
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it) {
        it->printLocationData();
    }
}

void Server::createVectorOfLocations() {
    addConfigInArray<Location>(locations, serverConfig, "location");
}

void Server::fillEachLocationWithData() {
    for (size_t i = 0; i < locations.size(); i++) {
        locations[i].updateDataInLocation();
    }
}

const std::string &Server::getHost() const {
    return ipAddress;
}

const std::string &Server::getServerName() const {
    return serverName;
}

int Server::getPort() const {
    return port;
}


void Server::updateDataInServer() {
    for (size_t i = 0; i < serverConfig.size(); i++) {
        cutDataStr(serverConfig[i], "listen", ipAddress);
        cutDataStr(serverConfig[i], "server_name", serverName);
        cutDataNum(serverConfig[i], "port", port);
        if (port == 0) {
            port = 80;
        }
        cutDataNum(serverConfig[i], "client_max_body_size", clientMaxBodySize);
        cutDataMap(serverConfig[i], "error_page", errorPages);
        if (serverConfig[i].find("location") != std::string::npos) {
            break;
        }
    }
}

void Server::addDataInServer() {
    createVectorOfLocations();
    fillEachLocationWithData();
    updateDataInServer();
//    printServerData();
}

const std::vector<Location> &Server::getLocations() const {
    return locations;
}

int Server::getClientMaxBodySize() const {
    return clientMaxBodySize;
}

std::map<int, std::string> Server::getErrorPages() const {
    return errorPages;
}

Server &Server::operator=(const Server &src) {
    if (this != &src) {
        this->serverConfig = src.serverConfig;
        this->ipAddress = src.ipAddress;
        this->port = src.port;
        this->clientMaxBodySize = src.clientMaxBodySize;
        this->serverName = src.serverName;
        this->errorPages = src.errorPages;
        this->locations = src.locations;
    }
    return *this;
}






