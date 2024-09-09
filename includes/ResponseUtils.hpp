/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 09:40:51 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/10 01:17:37 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include "include.hpp"
#include "RequestResponse.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"


enum FileType {
	IS_FILE,
	IS_DIRECTORY,
	NOT_FOUND
};

class ResponseUtils {
public:
	static bool isRequestValid(const HttpRequest& request);
	static LocationConfig findLocation(const std::string& path, const ServerConfig& config);
	static bool isMethodAllowed(const HttpRequest& request);
	static std::string buildBodyFromFile(const ServerConfig& config, const std::string& path);
	static FileType getTargetType(const HttpRequest& request);
	static bool shouldForceDownload(const std::string& extension);
	static std::string getContentType(const std::string& path);	
	static std::string getContentDisposition(const std::string& path);
	
private:
    ResponseUtils();
};

#endif
