#include "../../includes/EventManager.hpp"
#include "../../includes/Server.hpp"

EventManager::EventManager() : maxSocket(0)
{
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&read_master);
    FD_ZERO(&write_master);
    servers = std::vector<Server>();
}
EventManager::~EventManager(){}

void EventManager::addServerSocket(ServerSocket &serverSocket)
{
    serverSockets.push_back(serverSocket);

    FD_SET(serverSocket.getListenSocket(), &read_master);
    if (serverSocket.getListenSocket() > maxSocket)
    {
        maxSocket = serverSocket.getListenSocket();
    }
}

void EventManager::CreateAddClientSocket(ServerSocket &Socket)
{
    struct sockaddr_in clientAddr;
    int serverSocket = Socket.getListenSocket();
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientAddrLen);
    if (clientSocket == -1)
    {
        perror ("Error accepting connection");
        return;
    }
    fcntl (clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    FD_SET(clientSocket, &read_master);

    if (clientSocket > maxSocket)
    {
        maxSocket = clientSocket;
    }
    Client *newClient = new Client (clientSocket, clientAddr);
    newClient->setPort(Socket.getPort());
    newClient->setIP(Socket.getIp());
    clientSockets.push_back(newClient);
    (void)clientAddr;
}

void EventManager::waitandleEvents()
{
    while (maxSocket)
    {
        readSet = read_master;
        writeSet = write_master;
        int activity = select(maxSocket + 1, &readSet, &writeSet, NULL, NULL);
        if (activity < 0)
        {
            perror("Error in select");
            continue;
        }
        for(size_t i = 0; i < serverSockets.size(); i++) {
            if (FD_ISSET(serverSockets[i].getListenSocket(), &readSet)){
                CreateAddClientSocket(serverSockets[i]);
            }
        }
        for (std::list<Client *>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it){
            Client &current = **it;
            int currentSocket = current.getClientSocket();
            char buffer[3000];
            memset(buffer, 0, 3000);
            if (FD_ISSET(currentSocket, &readSet)) {
                int bytesRead = recv(currentSocket, buffer, 3000, 0);
                if (bytesRead == -1) {
                    delete (*it);
                    it = clientSockets.erase(it);
                    --it;
                    close(currentSocket);
                    FD_CLR(currentSocket, &read_master);
                }
                else if (bytesRead < 3000) {
                    //add somwhere to request 
                    FD_CLR(currentSocket, &write_master);
                    FD_SET(currentSocket, &write_master);
                    //some important request stuff
                }
            }
            if (FD_ISSET(currentSocket, &writeSet)) {
                int byteWrite = 1024;
                (void) byteWrite;
                std::cout << "Wow im here\n";
                // std::cout << "------------------Request received-------------------" << std::endl;
                // std::cout << buffer << std::endl;
                // HttpRequest request = RequestParser::parse(buffer);
                // std::cout << "------------------Request parsed-------------------" << std::endl;
                // std::cout << request << std::endl;
                // RequestResponse response = ResponseBuilder::build(request, config[i]);
                // std::cout << "------------------Response built-------------------" << std::endl;
                // std::cout << response << std::endl;
                // //std::cout << response.toString() << std::endl;
                // std::string responseStr = response.toString();
                // send(currentSocket, responseStr.c_str(), responseStr.length(), 0);
                // std::cout << "------------------HTML content sent-------------------" << std::endl;
                // close(new_socket);
		    }
        }
    }
}