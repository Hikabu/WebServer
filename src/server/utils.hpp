/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:30:21 by artclave          #+#    #+#             */
/*   Updated: 2024/09/28 12:36:31 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "includes.hpp"

int			extract_port(const std::string &str);
uint32_t	extract_host(const std::string &str);
void		signalHandler(int signal);
void		set_up_signals();

#endif