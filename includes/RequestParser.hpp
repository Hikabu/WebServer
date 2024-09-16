/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:56:07 by bperez-a          #+#    #+#             */
/*   Updated: 2024/09/13 13:28:52 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "../includes/include.hpp"
#include "../includes/HttpRequest.hpp"

// class HttpRequest;

class RequestParser {
public:
    static HttpRequest parse(const std::string& request_str);

    RequestParser() {}
private:
};

#endif
