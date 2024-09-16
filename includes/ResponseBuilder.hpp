/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:15:00 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/13 13:24:55 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_BUILDER_HPP
#define RESPONSE_BUILDER_HPP

#include "include.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"
#include "RequestResponse.hpp"
#include "Server.hpp"

class Server;

class ResponseBuilder {
public:
	int sentLenght; //for 
    static RequestResponse build(HttpRequest& request, ServerConfig& config);
	
	static RequestResponse buildGetResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location);
	static RequestResponse buildGetFileResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location);
	static RequestResponse buildGetDirectoryResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location);

	static RequestResponse buildPostResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location);
	//...
	
	static RequestResponse buildDeleteResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location);
	//...

	static RequestResponse buildErrorResponse(ServerConfig& config, HttpRequest& request, const std::string& code, const std::string& message);
	static RequestResponse buildSuccessResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location, std::string& path);
	static RequestResponse buildAutoindexResponse(ServerConfig& config, HttpRequest& request, LocationConfig& location, std::string& path);

	//getters setters
	int getPort() const;
	std::string getIpAddress() const;
	void setIpAddress(std::string ipAddress);
	void setPort(int port);

	
private:
    ResponseBuilder();
	std::string ipAddress;
	int port;
	std::vector<Server> servers;
	// Server currentServer;
	std::string path;
};

#endif

