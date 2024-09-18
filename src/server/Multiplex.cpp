/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 22:15:18 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 04:04:07 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Multiplex.hpp"

Multiplex::Multiplex(){}
Multiplex::~Multiplex(){}

void	Multiplex::startMonitor()
{
	FD_ZERO(&writeSet);
	FD_ZERO(&readSet);
	for (int i = 0; i < static_cast<int>(activeFds.size()); i++)
	{
		FD_SET(activeFds[i], &writeSet);
		FD_SET(activeFds[i], &readSet);
	}
}

void	Multiplex::add(int fd)
{
	activeFds.push_back(fd);
}

void	Multiplex::remove(int fd)
{
	std::vector<int>::iterator it;
	it = std::find(activeFds.begin(), activeFds.end(), fd);
	if (it != activeFds.end())
		activeFds.erase(it);
}

int	Multiplex::isReadyForReading(int fd)
{
	return (FD_ISSET(fd, &readSet));
}

int	Multiplex::isReadyForWriting(int fd)
{
	return (FD_ISSET(fd, &writeSet));
}