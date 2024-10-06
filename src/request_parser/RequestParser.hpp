/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 09:56:07 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/05 17:51:31 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "../includes.hpp"
#include "HttpRequest.hpp"

class RequestParser {
public:
    static HttpRequest parse(const std::string& request_str);

private:
    RequestParser() {}
};

#endif
