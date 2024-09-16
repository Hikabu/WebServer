#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "include.hpp"
# include <netinet/in.h>
# include "HttpRequest.hpp"
# include "ResponseBuilder.hpp"

class HttpRequest;
class ResponseBuilder;

class Client 
{
    public:
        Client (int clientSocket, struct sockaddr_in clientAddr);
        ~Client();
        void setPort(int port);
        void setIP(std::string ip);
        std::string getIp() const;
        int getPort() const;
        HttpRequest request;
        ResponseBuilder response;
        int getClientSocket() const;
        void printClientAddr() const; //for werror
    private:
        int clientSocket;
        struct sockaddr_in clientAddr;
        std::string ipAddress;
        int port;

};


#endif