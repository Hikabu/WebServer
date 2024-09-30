#include "../../include/WebServer.hpp"

WebServer::WebServer(std::string pathConfig) {
    this->pathConfig = pathConfig;
    this->configLines = std::vector<std::string>();
    this->servers = std::vector<Server>();
    eventManager = new EventManager();
}


bool WebServer::removeComments(std::string &line) {
    std::string::size_type pos = line.find('#');
    if (pos != std::string::npos) {
        line.erase(pos);
    }
    return true;
}

void WebServer::saveConfigInConfigLine() {
    std::ifstream inputFile(this->pathConfig);
    if (!inputFile.is_open()) {
        throw std::out_of_range("Error: could not open file");
    }
    std::string line;
    while(getline(inputFile, line)) {
        removeComments(line);
        if (line.empty()) { continue; }
        configLines.push_back(line);
    }
    inputFile.close();
}

void WebServer::checkValidConfig() {
    for (size_t i = 0; i < configLines.size(); i++) {
        std::string str = configLines[i].c_str();
        if (str.find("server") != std::string::npos) {
            continue;
        } else if (str.find("port") != std::string::npos) {
            continue;
        } else if (str.find("error_page") != std::string::npos) {
            continue;
        } else if (str.find("client_max_body_size") != std::string::npos) {
            continue;
        } else if (str.find("location") != std::string::npos) {
            continue;
        } else if (str.find("root") != std::string::npos) {
            continue;
        } else if (str.find("methods") != std::string::npos) {
            continue;
        } else if (str.find("index") != std::string::npos) {
            continue;
        } else if (str.find("file_upload") != std::string::npos) {
            continue;
        } else if (str.find("cgi_pass") != std::string::npos) {
            continue;
        } else if (str.find("redirect") != std::string::npos) {
            continue;
        } else if (str.find("autoindex") != std::string::npos) {
            continue;
        } else if (str.find("server_name") != std::string::npos) {
            continue;
        } else if (str.find("listen") != std::string::npos) {
            continue;
        } else if (str.find("}") != std::string::npos) {
            continue;
        } else {
            str = removeSpaces(str);
            if (str.compare("\n") == 0 || str.compare("\0") == 0) {
                continue;
            } else {
                throw std::out_of_range("Error: undefined block " + str + " in config file");
            }
        }
    }
}

void WebServer::addDataInCurrentPath() {
    char currentPath[PATH_MAX];
    if (getcwd(currentPath, sizeof(currentPath)) != NULL) {
        this->currentPath = currentPath;
    } else {
        perror("getcwd() error");
    }
}


void WebServer::createVectorOfServers() {
    addConfigInArray<Server>(servers, configLines, "server");
}

void WebServer::fillEachServerWithData() {
    for (size_t i = 0; i < servers.size(); i++) {
        servers[i].addDataInServer();
    }
}

std::string &WebServer::getCurrentPath() {
    return currentPath;
}

std::vector<Server> &WebServer::getServers() {
    return servers;
}

void WebServer::addConfigData() {
    createVectorOfServers();
    fillEachServerWithData();
}

void WebServer::parseConfig() {
    addDataInCurrentPath();
    saveConfigInConfigLine();
    checkValidConfig();
    addConfigData();
}

void WebServer::createUniquePair() {
    for (size_t i = 0; i < servers.size(); i++) {
        std::pair<std::string, int> uniquePair = std::make_pair(servers[i].getHost(), servers[i].getPort());
        unique.insert(uniquePair);
    }
}

void WebServer::createEventManager() {
   for (std::set<std::pair<std::string, int> >::iterator it = unique.begin(); it != unique.end(); it++) {
       ServerSocket listenSocket;
       listenSocket.initListenSocket(it->first, it->second);
       eventManager->addServerSocket(listenSocket);
   }
}


void WebServer::start() {
    createUniquePair();
    createEventManager();
    eventManager->copyArray(servers);
    eventManager->waitAndHandleEvents();

//    stop();
}