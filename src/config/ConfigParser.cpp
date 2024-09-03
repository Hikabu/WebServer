#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

ConfigParser::ConfigParser(const std::string& configFile) : configFile(configFile) {}

ServerConfig ConfigParser::parse() {
    ServerConfig server;
    std::ifstream file(configFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configFile);
    }
    // Implement parsing logic here

    return server;
}
