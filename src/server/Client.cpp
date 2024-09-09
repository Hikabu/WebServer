#include "../../includes/Client.hpp"
#include "../../includes/ServerSocket.hpp"

Client::Client(int clientSocket, struct sockaddr_in clientAddr) : clientSocket(clientSocket), clientAddr(clientAddr){(void) clientAddr;}

Client::~Client(){}

void Client::setPort(int port){this->port = port;}
void Client::setIP(std::string ip){this->ipAddress = ip;}

int Client::getClientSocket() const {return clientSocket;}
void Client::printClientAddr() const {
    std::cout << "Client address: " << clientAddr.sin_addr.s_addr << std::endl;
}