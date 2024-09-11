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

void EventManager::copyArray(std::vector<Server> &arrServers)
{
    for (size_t i = 0; i < arrServers.size(); i++)
    {
        this->servers.push_back(arrServers[i]);
    }
}
EventManager::~EventManager(){}

void EventManager::addServerSocket(ServerSocket &serverSocket) //adds the fd of the new server socket to the master
{
    serverSockets.push_back(serverSocket); //and evman can keep track of all the server sockets it's managing

    FD_SET(serverSocket.getListenSocket(), &read_master); // should remember
    if (serverSocket.getListenSocket() > maxSocket)
    {
        maxSocket = serverSocket.getListenSocket();
    }
}

void EventManager::CreateAddClientSocket(ServerSocket &Socket)//accepting a new client connection and setting it up for non-blocking io
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
    /* O_NONBLOCK read(), write(), send(), recv()) on that socket will not block indefinitely
    for select this flag handle mulyiple connrctions withoit getting stuck waing on any single client
    FD_CLOEXEC marks the fd to be cloded automaticslly when the procss calls exec() to run a new program
    and smt about  child processes in future to not inherit
    */
    fcntl (clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC); // need to change(read more about flags)
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

void EventManager::waitandleEvents() //handle multiple client connections
{
    while (maxSocket)
    {
        readSet = read_master; //keep all fd
        writeSet = write_master;
        /*wait for activity on multiple sockets without consuming CPU resources.*/
        int activity = select(maxSocket + 1, &readSet, &writeSet, NULL, NULL); //monitor multiple fd, waiting fd become ready for some I/O operation
        if (activity < 0)
        {
            perror("Error in select");
            continue;
        }
        for(size_t i = 0; i < serverSockets.size(); i++) {
            if (FD_ISSET(serverSockets[i].getListenSocket(), &readSet)){ //  if any of the server sockets are ready to accept a new connection
                CreateAddClientSocket(serverSockets[i]);
            }
        }
        for (std::list<Client *>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it){
            Client &current = **it;
            int currentSocket = current.getClientSocket();
            char buffer[3000];
            memset(buffer, 0, 3000);
            if (FD_ISSET(currentSocket, &readSet)) {//Checks if a fd is set in a given set
                int bytesRead = recv(currentSocket, buffer, 3000, 0); //Receives data from a socket
                if (bytesRead == -1) { //not shure of structure
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
            if (FD_ISSET(currentSocket, &writeSet)) { //sending data to clients that are ready to receive.
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
                // send(currentSocket, responseStr.c_str(), responseStr.length(), 0);// Sends data on a socket.
                // std::cout << "------------------HTML content sent-------------------" << std::endl;
                // close(new_socket);
		    }
        }
    }
}