#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigParser::ConfigParser(const std::string& configFile) : configFile(configFile) {}

std::vector<ServerConfig> ConfigParser::parse() {
    std::vector<ServerConfig> servers;
    std::ifstream file(configFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configFile);
    }
    // Implement parsing logic here

    return servers;
}
