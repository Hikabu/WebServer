#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "include.hpp"
# include <netinet/in.h>

class ServerSocket {
    public:
        ServerSocket();
        ~ServerSocket();
        int getListenSocket() const;
        void initListenSocket(std::string ipAddress, int port);
        int getPort() const;
        std::string getIp() const;
    private:
        int listenSocket;
        struct sockaddr_in serverAddr;
        int port;
        std::string ipAddress;
        int backlog;
};
#endif