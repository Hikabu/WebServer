/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:57:15 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/12 10:50:01 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"


HttpRequest RequestParser::parse(const std::string& request_str) {
    HttpRequest request;
    std::istringstream iss(request_str);
    std::string line;

    // Parse request line
    if (!std::getline(iss, line)) {
        std::cerr << "Error: Could not read request line" << std::endl;
        return request;
    }
	//how to add host here ? 
    std::istringstream request_line(line);
	 //added hsot below for network version 1 :
    std::string method, path, protocol, host;
    if (!(request_line >> method >> path >> protocol >> host)) {
        std::cerr << "Error: Invalid request line format" << std::endl;
        return request;
    }

    request.setMethod(method);
    std::string decodedPath = path;
    size_t pos = 0;
    while ((pos = decodedPath.find("%20", pos)) != std::string::npos) {
        decodedPath.replace(pos, 3, " ");
        pos += 1;
    }
    request.setPath(decodedPath);
    request.setProtocol(protocol);
	request.setHost(host); //for network version 1 
	std::cout<<"Host is: "<<host<<"\n";  //for network version 1 

    std::cout << "Parsed request line: " << method << " " << path << " " << protocol << std::endl;

    // Parse headers
    while (std::getline(iss, line) && !line.empty() && line != "\r") {
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = (colon_pos + 1 < line.length()) ? line.substr(colon_pos + 1) : "";
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (!key.empty()) {
                std::cout << "Adding header: '" << key << "' : '" << value << "'" << std::endl;
                request.addHeader(key, value);
            }
        }
    }

    // Parse body (if present)
    std::string body;
    while (std::getline(iss, line)) {
        body += line + "\n";
    }
    request.setBody(body);

    return request;
}

