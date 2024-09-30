#include "../../include/Parser.hpp"

std::string Parser::fullTrim(const std::string &line) {
    size_t posLeft = line.find_first_not_of(" \t\r\n");
    size_t posRight = line.find_last_not_of(" \t\r\n");

    if (posLeft == std::string::npos || posRight == std::string::npos)
        return "";
    return line.substr(posLeft, posRight - posLeft + 1);
}

std::string Parser::trim(const std::string &line) {
    size_t posLeft = line.find_first_not_of(" \t\r\n");

    if (posLeft == std::string::npos)
        return "";
    return line.substr(posLeft);
}

std::string Parser::removeSpaces(std::string &line) {
    std::string result = "";
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\r') {
            result += line[i];
        }
    }
    line = result;
    return line;
}

void Parser::ParseAndCheckLine(std::string &line, std::string finder) {
    line = trim(line);
    if (line.find(finder) != 0) {
        throw std::out_of_range("Error: invalid " + finder + " value in config file");
    }
    line = line.substr(finder.length());
    if (line.find_first_not_of(" \t") != 1) {
        throw std::out_of_range("Error: invalid " + finder + " value in config file");
    }
    line = fullTrim(line);
    if (line.find(';') != line.length() - 1) {
        throw std::out_of_range("Error: invalid " + finder + " value in config file");
    }
    line.erase(line.find(';'), 1);
    if (line.find_last_not_of(" \t") != line.length() - 1) {
        throw std::out_of_range("Error: invalid " + finder + " value in config file");
    }
}

void Parser::checkServerLine(std::string line) {
    line = removeSpaces(line);
    if (line != "server{")
        throw std::out_of_range("Error: invalid server line in config file");
}

void Parser::cutDataStr(std::string &line, std::string finder, std::string &data) {
    
    size_t pos = line.find(finder);
    if (pos != std::string::npos) {
        if (line.find("location") == std::string::npos) {
            ParseAndCheckLine(line, finder);
        }
        data = trim(line);
        line.erase();
    }
}

void Parser::cutDataNum(std::string &line, std::string finder, int &data) {

    int numValue = 0;

    size_t pos = line.find(finder);
    if (pos != std::string::npos) {
        ParseAndCheckLine(line, finder);
        line = trim(line);
        if (!isValidNum(line, numValue)) {
            throw std::out_of_range("Error: invalid " + finder + " value in config file");
        }
        data = std::stoul(line);
        line.erase();
    }
}

void Parser::cutDataMap(std::string &line, std::string finder, std::map<int, std::string> &data) {

    size_t pos = line.find(finder);
    if (pos != std::string::npos) {
        ParseAndCheckLine(line, finder);
        line = trim(line);
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            std::string strKey = line.substr(0, spacePos);
            if (!isValidNum(strKey, 0)) {
                throw std::out_of_range("Error: invalid " + finder + " value in config file");
            }
            int key = std::stoi(strKey);
            data[key] = trim(line.substr(spacePos + 1));
        }
        else
            throw std::out_of_range("Error: invalid " + finder + " value in config file");
        line.erase();
    }
}

void Parser::cutDataBool(std::string &line, std::string finder, bool &data) {

        size_t pos = line.find(finder);
        if (pos != std::string::npos) {
            ParseAndCheckLine(line, finder);
            line = trim(line);
            if (line == "on")
                data = true;
            else if (line == "off")
                data = false;
            else
                throw std::out_of_range("Error: invalid " + finder + " value in config file");
            line.erase();
        }
}

void Parser::cutDataArray(std::string &line, std::string finder, std::vector<std::string> &data) {
    size_t pos = line.find(finder);
    if (pos != std::string::npos) {
        ParseAndCheckLine(line, finder);
        line = trim(line);
        std::string s;
        std::istringstream ss(line);
        while (std::getline(ss, s, ' ')) {
            data.push_back(s);
        }
        line.erase();
    }
}