#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../../include/Libraries.hpp"
#include "Request.hpp"
#include "../../include/Server.hpp"

//сделать обработчик для гет запроса с параметрами

class Server;

class Response {
private:

    std::string ipAddress;
    int port;
    std::vector<Server> servers;
    Server currentServer;
    std::string path;


    void getUrl();
    void findImage();
    void createResponse();

    void handleGet(Request &request);
    void handlePost(Request &request);
    void canMakeResponse(Request &request);


    void generateErrorsPage(int code);

public:
    Response();
    void handleRequest(Request &request);
    void generateResponse(Request &request, std::vector<Server> const &servers);
    bool isCGI(std::string path);
    void generateCGIResponse(Request &request, std::vector<Location> locations);
    void generateAutoindexResponse(Request &request);
    void createResponse(Request &request);
    std::string response;
    int sentLength;
    std::string getIpAddress() const;
    void setIpAddress(std::string ipAddress);
    int getPort() const;
    void setPort(int port);
    Server getCurrentServer() const;
    void currentPath();
    void generateRedirectResponse(const std::string &locationToRedir);
    void chooseConfig(std::string hostName, Server &server);
    void deleteFile(const std::string &fileToOpen);
    bool chooseLocation(Request request, Location &location, std::vector<Location> locations);
    std::string rootParsing(const std::string &url, Location &currentLocation) const;


    bool is_method_allowed(Location location, std::string method);
    std::map<std::string, std::string> getFileHeaders(std::map<std::string, std::string> const &headers);
};


#endif