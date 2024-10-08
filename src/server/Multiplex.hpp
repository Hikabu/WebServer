/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Multiplex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 07:06:04 by artclave          #+#    #+#             */
/*   Updated: 2024/10/07 05:08:14 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLEX_HPP
# define MULTIPLEX_HPP

#include "../includes.hpp"

class Multiplex
{
	private:
		fd_set			read_set;
		fd_set			write_set;
		std::list<int>	active_fds;

	public:
		struct timeval	timeout;
		Multiplex();
		~Multiplex();

		void	reset_select();
		void	add(int fd);
		void	remove(int fd);
		bool	ready_to_read(int fd);
		bool	ready_to_write(int fd);
};

#endif