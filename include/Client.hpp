#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Libraries.hpp"
#include "../src/request/Response.hpp"
#include "../src/request/Request.hpp"

class Response;
class Request;

class Client {
public:
	Client(int clientSocket, struct sockaddr_in clientAddr);
    ~Client();

    ssize_t readData(char* buffer, size_t bufferSize);
    ssize_t writeData(const char* data, size_t dataSize);
	int getClientSocket() const;
	struct sockaddr_in getStruct() const;
    Response response;
    Request request;
    void setPort(int port);
    void setIp(std::string ip);
    std::string getIp() const;
    int getPort() const;


private:
    int clientSocket;
	struct sockaddr_in clientAddr;
    std::string ipAddress;
    int port;
};

#endif