/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestBodySnatcher.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 22:14:07 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/05 16:22:22 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_REQUEST_BODY_SNATCHER_HPP
# define POST_REQUEST_BODY_SNATCHER_HPP

#include "PostRequestBodyPart.hpp"
#include "includes.hpp"

class PostRequestBodySnatcher {
private:
    PostRequestBodySnatcher();    

public:
    static std::vector<PostRequestBodyPart> parse(const std::string& body, const std::string& boundary);
};

#endif
