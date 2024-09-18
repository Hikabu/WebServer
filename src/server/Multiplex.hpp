/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 22:14:20 by artclave          #+#    #+#             */
/*   Updated: 2024/09/18 04:04:36 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include "includes.hpp"

class	Multiplex
{
	private:
	//ATRIBUTES:
		fd_set				writeSet;
		fd_set				readSet;
		std::vector<int>	activeFds;

	public:
		Multiplex();
		~Multiplex();
		void	startMonitor();
		void	add(int fd);
		void	remove(int fd);
		int		isReadyForReading(int fd);
		int	isReadyForWriting(int fd);
};

#endif
