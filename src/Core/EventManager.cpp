#include "../../include/EventManager.hpp"

EventManager::EventManager() : maxSocket(0) {
    FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&read_master);
	FD_ZERO(&write_master);
    servers = std::vector<Server>();
}

EventManager::~EventManager() {

}

void EventManager::copyArray(std::vector<Server> &arrServers) {
    for (size_t i = 0; i < arrServers.size(); i++) {
        this->servers.push_back(arrServers[i]);
    }
}


void EventManager::addServerSocket(ServerSocket &serverSocket) {
    serverSockets.push_back(serverSocket);

	FD_SET(serverSocket.getListenSocket(), &read_master);
	if (serverSocket.getListenSocket() > maxSocket) {
        maxSocket = serverSocket.getListenSocket();
    }
}

void EventManager::CreateAddClientSocket(ServerSocket &Socket) {
	struct sockaddr_in clientAddr;
    int serverSocket = Socket.getListenSocket();
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientAddrLen);
	if (clientSocket == -1) {
		perror("Error accepting connection");
		return;
	}
    fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    FD_SET(clientSocket, &read_master);

    if (clientSocket > maxSocket) {
        maxSocket = clientSocket;
    }
	Client *newClient = new Client(clientSocket, clientAddr);
    newClient->setPort(Socket.getPort());
    newClient->setIp(Socket.getIp());
	clientSockets.push_back(newClient);
}

void EventManager::waitAndHandleEvents() {
    while (maxSocket) {
		readSet = read_master;
		writeSet = write_master;
        int activity = select(maxSocket + 1, &readSet, &writeSet, NULL, NULL);
        if (activity < 0) {
            perror("Error in select");
            continue;
        }
        for (size_t i = 0; i < serverSockets.size(); i++) {
            if (FD_ISSET(serverSockets[i].getListenSocket(), &readSet)) {
                CreateAddClientSocket(serverSockets[i]);
            }
        }
		for (std::list<Client *>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
            Client &current = **it;
			int currentSocket = current.getClientSocket();
			char buffer[1024];
			memset(buffer, 0, 1024);
            if (FD_ISSET(currentSocket, &readSet)) {
                int bytesRead = recv(currentSocket, buffer, 1024,0);
                if (bytesRead == -1) {
                    delete (*it);
                    it = clientSockets.erase(it);
                    --it;
                    close(currentSocket);
                    FD_CLR(currentSocket, &read_master);
                }
                else if (bytesRead < 1024) {
                    current.request.request += std::string(buffer, bytesRead);
                    FD_CLR(currentSocket, &read_master);
                    FD_SET(currentSocket, &write_master);

                    current.request.Parsing(current.request.request);
                    current.response.setIpAddress(current.getIp());
                    current.response.setPort(current.getPort());
                    current.response.generateResponse(current.request, servers);
                    current.request.request.clear();
                } else {
                    current.request.request += std::string(buffer, bytesRead);
                }
            }
            if (FD_ISSET(currentSocket, &writeSet)) {
                int byteToWrite = 1024;
                std::string response = current.response.response;
                int &sentLength = current.response.sentLength;
                int length = current.response.response.size();
                int writingRemainder = length - current.response.sentLength;
                if (byteToWrite > writingRemainder)
                    byteToWrite = writingRemainder;
                int wasSent = 0;
                if (sentLength < length)
                    wasSent = send(currentSocket, response.substr(sentLength).c_str(), byteToWrite, 0);
                if(wasSent == -1 || sentLength + wasSent >= length)
                {
                    delete (*it);
                    it = clientSockets.erase(it);
                    --it;
                    close(currentSocket);
                    FD_CLR(currentSocket, &write_master);
                }
                sentLength += wasSent;
            }
		}
	}
}
