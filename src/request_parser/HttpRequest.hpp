/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:11:38 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/28 11:48:36 by artclave         ###   ########.fr       */
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
    std::string getBody() const;
    std::string getHeader(const std::string& key) const;
	std::string getHost() const; //for network version 1
    const std::map<std::string, std::string>& getHeaders() const;
	std::string getCgiPath() const; 
	int getPostFileFd() const;
	std::string &getPostFileContent();

    void setMethod(const std::string& method);
    void setPath(const std::string& path);
    void setProtocol(const std::string& protocol);
    void setBody(const std::string& body);
    void addHeader(const std::string& key, const std::string& value);
	void setHost(const std::string & host); //for network version 1
	void setCgiPath(const std::string &path);
    bool hasHeader(const std::string& key) const;
    void printHeaders() const;
	void setPostFileContent(const std::string & content);
	void setPostFileFd(int fd);
private:
    std::string method_;
    std::string path_;
    std::string protocol_;
    std::map<std::string, std::string> headers_;
    std::string body_;
	std::string host_; //for network version 1
	std::string		cgiPath_;
	std::string postFileContent;
	int postFileFd;
};

std::ostream& operator<<(std::ostream& os, const HttpRequest& request);

#endif
