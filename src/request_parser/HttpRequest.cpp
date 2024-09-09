/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:13:14 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/10 00:54:30 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() { }

HttpRequest::HttpRequest(const HttpRequest& other)
    : method(other.method),
      path(other.path),
      protocol(other.protocol) { }

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
    if (this != &other) {
        this->method = other.method;
        this->path = other.path;
        this->protocol = other.protocol;
    }
    return *this;
}

HttpRequest::~HttpRequest() { }

std::string HttpRequest::getMethod() const { return this->method; }
std::string HttpRequest::getPath() const { return this->path; }
std::string HttpRequest::getProtocol() const { return this->protocol; }
std::string HttpRequest::getType() const { return this->type; }

void HttpRequest::setMethod(const std::string& method) { this->method = method; }
void HttpRequest::setPath(const std::string& path) { this->path = path; }
void HttpRequest::setProtocol(const std::string& protocol) { this->protocol = protocol; }
void HttpRequest::setType(const std::string& type) { this->type = type; }

std::ostream& operator<<(std::ostream& os, const HttpRequest& request) {
    os << "Method: " << request.getMethod() << std::endl;
    os << "Path: " << request.getPath() << std::endl;
    os << "Protocol: " << request.getProtocol() << std::endl;
	os << "Type: " << request.getType() << std::endl;
    return os;
}