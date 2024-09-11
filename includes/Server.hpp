/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:33:26 by artclave          #+#    #+#             */
/*   Updated: 2024/09/12 02:15:12 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "include.hpp"
#include "ServerSocket.hpp"
#include "ConfigParser.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "RequestParser.hpp"
#include "EventManager.hpp"
// #include "ResponseBuilder.hpp"
class ServerSocket;
class EventManager;

class Server {
private:
    EventManager *eventManager;
public:
    Server &operator=(const Server &copy){
        if (this != &copy){}
        return (*this);
    }
    //unique pairs of host and ip address
    std::set<std::pair<std::string, int > > unique; // automatically prevents duplicate host-port combination --- but why ? We nned a socket per server even if they have duplicate host-port... if they have a different server-name then we need different sockets... Arturo asking
    std::vector<ServerConfig>& config;
    std::vector<Server> servers;
    Server(std::vector<ServerConfig>& config);
    ~Server();
    void	run();
    void    createUniquePair();
    void    createEventManager();
    // void	check_non_blocking(int sockfd);
    // std::vector<ServerConfig>& getConfig() ;
	std::vector<int> create_listen_sockets(struct socaddr &addr);
};


#endif