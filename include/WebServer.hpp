#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Libraries.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "EventManager.hpp"
#include "ServerSocket.hpp"

class ServerSocket;

class WebServer : public Parser {
private:
    std::string pathConfig;
    std::vector<Server> servers;
    std::vector<std::string> configLines;
    EventManager *eventManager;
    std::set<std::pair<std::string, int > > unique;
    std::string currentPath;

    bool removeComments(std::string &line);
    void saveConfigInConfigLine();
    void addConfigData();
    void createVectorOfServers();
    void fillEachServerWithData();
    void createEventManager();
    void createUniquePair();
    void addDataInCurrentPath();
    void checkValidConfig();


public:
    WebServer(std::string pathConfig);
    void parseConfig();
    void start();
    std::vector<Server> &getServers();
    std::string &getCurrentPath();

};


#endif