/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:26:51 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 02:35:20 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Address.hpp"

Address::Address(){}
Address::~Address(){}

void Address::initialize(int port, uint32_t host)
{
	memset(&ipv4Struct, 0, sizeof(ipv4Struct));
	ipv4Struct.sin_family = AF_INET;
	ipv4Struct.sin_port = htons(port);
	ipv4Struct.sin_addr.s_addr = htonl(host);
	ptr = (struct sockaddr *)&ipv4Struct;
	len = sizeof(ipv4Struct);
}