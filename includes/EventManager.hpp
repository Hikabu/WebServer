#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "include.hpp"
#include "ConfigParser.hpp"
#include "ResponseBuilder.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"
#include <sys/select.h>

class ServerSocket;
class Server;
class Client;

class EventManager{
    private:
        fd_set readSet;
        fd_set writeSet;
        fd_set read_master;
        fd_set write_master;
        std::vector<ServerSocket> serverSockets;
        int maxSocket;
        std::vector<Server> servers;
        std::list<Client *> clientSockets;
    public:
        EventManager();
        ~EventManager();
        void addServerSocket(ServerSocket &serverSocket);
        void waitandleEvents();
        void CreateAddClientSocket(ServerSocket &Socket);
};

#endif