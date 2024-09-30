#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include "EventManager.hpp"
#include "Libraries.hpp"

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();
    void initListenSocket(std::string ipAddress, int port);
    void stop();
    int getListenSocket() const;
    int getPort() const;
    std::string getIp() const;

private:
    int listenSocket;
    int backlog;
    std::string ipAddress;
    int port;
    struct sockaddr_in serverAddr;
};


#endif