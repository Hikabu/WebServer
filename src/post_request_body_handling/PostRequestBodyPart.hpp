/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestBodyPart.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 22:07:49 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/05 16:22:16 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_REQUEST_BODY_PART_HPP
# define POST_REQUEST_BODY_PART_HPP

#include "includes.hpp"

class PostRequestBodyPart {
public:
    PostRequestBodyPart();

    std::string getName() const;
    std::string getFilename() const;
    std::string getContentType() const;
    std::string getContent() const;

    void setName(const std::string& name);
    void setFilename(const std::string& filename);
    void setContentType(const std::string& content_type);
    void setContent(const std::string& content);

    void appendContent(const std::string& content);

private:
    std::string name_;
    std::string filename_;
    std::string content_type_;
    std::string content_;
};

#endif
