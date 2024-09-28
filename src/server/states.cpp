/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:22:33 by artclave          #+#    #+#             */
/*   Updated: 2024/09/28 12:25:06 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::read_request(struct clientSocket &client)
{
	if (client.state != READING || !FD_ISSET(client.fd, &read_set))
		return ;
	char buff[READ_BUFFER_SIZE];
	memset(buff, 0, READ_BUFFER_SIZE);
	std::size_t	pos_zero, pos_content_length, pos_header_end;
	int bytes = recv(client.fd, &buff[0], READ_BUFFER_SIZE, 0);
	if (bytes <= 0)
	{
		strerror(errno);
		client.state = DISCONNECT;
		return ;
	}
	client.read_operations++;
	for (int i = 0; i < bytes; i++)
		client.read_buffer += buff[i];
	pos_header_end = client.read_buffer.find("\r\n\r\n");
    if (pos_header_end == std::string::npos)// Header is incomplete, wait for more data
    		return;
	pos_content_length = client.read_buffer.find("Content-Length:");
	if (pos_content_length != std::string::npos)
	{
		long expected_body_size = std::atol(client.read_buffer.substr(pos_content_length + 16, pos_header_end).c_str());
		if (static_cast<int>(client.read_buffer.size() - pos_header_end - 4) < expected_body_size)// Body is incomplete, wait for more data
            return;
	}
	else if (client.read_buffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		pos_zero = client.read_buffer.find("0\r\n\r\n");
		if (pos_zero == std::string::npos) // Chunked body is incomplete, wait for more data
            return;
	}
	client.state++;
}

void	Server::init_http_process(struct clientSocket &client, struct serverSocket &server)
{
	if (client.state != HTTP)
		return ;
	client.request = RequestParser::parse(client.read_buffer);
	find_match_config(client, server.possible_configs, client.request.getHost());
	client.response = ResponseBuilder::build(client.request, client.match_config);
	//std::cout << "DEBUG: Response built" << std::endl;
	if (!client.response.getFilePathForBody().empty())
	{
		client.file_fd = open(client.response.getFilePathForBody().c_str(), O_RDONLY);
	}	
	client.state++;
}

void	Server::execute_cgi(struct clientSocket &client)
{
	if (client.state != EXECUTECGI)
		return ;
	if (client.response.getCgiPath().empty())
	{
		client.state = FILES;
		return ;
	}
	if (client.write_operations > 0 || !FD_ISSET(client.fd, &write_set))
		return;
    client.cgi_pid = fork();
    if (client.cgi_pid == -1) {
        std::cerr << "ERROR: Fork failed. Errno: " << errno << " - " << strerror(errno) << std::endl;
        client.response = ResponseBuilder::buildErrorResponse(client.match_config, client.request, "500", "Internal Server Error");
		client.state = FILES;
		return ;
    } 
	else if (client.cgi_pid == 0) {
      //  std::cout << "DEBUG: Child process. Executing CGI script" << std::endl;
		dup2(client.fd, STDOUT_FILENO);
		std::string cgi_path = client.response.getCgiPath();
		const CGIConfig& cgiConfig = client.match_config.getCgi();
 		char* args[] = {const_cast<char*>(cgiConfig.path.c_str()), const_cast<char*>(cgi_path.c_str()), NULL};
         execv(cgiConfig.path.c_str(), args);
        std::cerr << "ERROR: execv failed. Errno: " << errno << " - " << strerror(errno) << std::endl;
        exit(1);
    }
	else
	{
		client.write_operations++;
		client.read_operations++;
		client.state++;
	}
}


void	Server::wait_cgi(struct clientSocket &client)
{
	if (client.state != WAITCGI)
		return;
	int status;
	if (waitpid(client.cgi_pid, &status, WNOHANG) == 0)
		return ;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		//std::cout << "DEBUG: CGI program executed successfully" << std::endl;
		client.state = DISCONNECT;
	} else {
		//std::cerr << "ERROR: CGI program failed. Exit status: " << WEXITSTATUS(status) << std::endl;
		client.response =  ResponseBuilder::buildErrorResponse(client.match_config, client.request, "500", "Internal Server Error");
		client.state = WRITE;
	}
}

void	Server::manage_files(struct clientSocket &client)
{
	if (client.state != FILES)
		return ;
	//std::cout<<"MANAGING FILES\n";
	if (!client.response.getFilePathForBody().empty() && client.file_fd > 0)
	{
		if (client.read_operations > 0)
			return ;
		int body_done = client.response.buildBodyFromFile(client.match_config, client.file_fd);
		client.read_operations ++;
		if (body_done == false)
			return ;
	}
	if (!client.request.getPostFileContent().empty())
	{
		if (client.write_operations > 0)
			return ;
		int bytes = write(client.request.getPostFileFd(), client.request.getPostFileContent().c_str(), WRITE_BUFFER_SIZE * 10);
		if (bytes < 0)
			return ; //some error saving teh file what to do here?
		client.write_operations++;
		client.request.getPostFileContent().erase(0, bytes);
		client.write_offset = 0;
		close(client.request.getPostFileFd());
	}
	client.write_buffer = client.response.toString();
	client.state++;
}

void	Server::write_response(struct clientSocket &client)
{
	if (client.state != WRITE || client.write_operations > 0 || !FD_ISSET(client.fd, &write_set))
		return ;
	setsockopt(client.fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	int bytes = send(client.fd, &client.write_buffer[client.write_offset], WRITE_BUFFER_SIZE, 0);
	if (bytes < 0)
	{
		 client.state = DISCONNECT;
		 return ;
	}
	client.write_operations++;
	client.write_offset += bytes;
	if (client.write_offset >= static_cast<int>(client.write_buffer.size()))
	{
		 client.state = DISCONNECT;
		return ;
	}
}
