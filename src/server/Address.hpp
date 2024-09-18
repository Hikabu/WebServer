/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:23:03 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 02:35:08 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "includes.hpp"

class	Address
{
	private:
		struct sockaddr_in	ipv4Struct;

	public:
		Address();
		~Address();
		struct sockaddr		*ptr;
		socklen_t			len;
		void	initialize(int port, uint32_t host);
};

#endif