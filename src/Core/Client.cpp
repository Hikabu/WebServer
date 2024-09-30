#include "../../include/Client.hpp"
#include "../../include/ServerSocket.hpp"

Client::Client(int clientSocket, struct sockaddr_in clientAddr) : clientSocket(clientSocket), clientAddr(clientAddr) {
	//socklen_t clientAddrLen = sizeof(clientAddr);
    //request.request.clear();
    //response.response.clear();
}

Client::~Client() {
    // Закрытие клиентского сокета при уничтожении объекта
//	std::cout << "Client " << clientSocket << " destructor" << std::endl;
    //close(clientSocket);
}

// Метод для чтения данных из клиентского сокета в предоставленный буфер.
ssize_t Client::readData(char* buffer, size_t bufferSize) {
    return read(clientSocket, buffer, bufferSize);
}

// Метод для записи данных из предоставленного буфера в клиентский сокет
ssize_t Client::writeData(const char* data, size_t dataSize) {
    return write(clientSocket, data, dataSize);
}

int Client::getClientSocket() const
{
	return clientSocket;
}

struct sockaddr_in Client::getStruct() const
{
	return clientAddr;
}

void Client::setPort(int port) {
    this->port = port;
}

void Client::setIp(std::string ip) {
    this->ipAddress = ip;
}

int Client::getPort() const {
    return port;
}

std::string Client::getIp() const {
    return ipAddress;
}

