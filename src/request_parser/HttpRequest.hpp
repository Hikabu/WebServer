/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperez-a <bperez-a@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:11:38 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/04 20:21:32 by bperez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "includes.hpp"

class HttpRequest {
public:
    HttpRequest();
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    std::string getMethod() const;
    std::string getPath() const;
    std::string getProtocol() const;

    void parse(const std::string& request);

private:
    std::string method;
    std::string path;
    std::string protocol;
};

#endif
