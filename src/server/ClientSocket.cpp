/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artclave <artclave@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 21:44:25 by artclave          #+#    #+#             */
/*   Updated: 2024/10/04 04:15:28 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"
#include "ServerCore.hpp"
#include "ServerSocket.hpp"
#include "Utils.hpp"

ClientSocket::ClientSocket(Multiplex *server_multiplex, int fd_) : multiplex(server_multiplex), fd(fd_), state(0), write_offset(0), cgi_error_code("500"), cgi_error_message("Internal Server Error") {}

ClientSocket::~ClientSocket(){}

int	ClientSocket::get_fd() const { return fd; }
int	ClientSocket::get_state() const { return state; }

void	ClientSocket::process_connection(ServerSocket &server)
{
	read_operations = 0;
	write_operations = 0;
	read_request();
	init_http_process(server.get_possible_configs());
	execute_cgi();
	wait_cgi();
	correct_cgi();
	incorrect_cgi();
	manage_files();
	write_response();
}

void	ClientSocket::read_request()
{
	if (state != READING) 
		return ;
	if (!multiplex->ready_to_read(fd))
	{
		return ;
	}
	char buff[READ_BUFFER_SIZE];
	memset(buff, 0, READ_BUFFER_SIZE);
	std::size_t	pos_zero, pos_content_length, pos_header_end;
	int bytes = recv(fd, buff, READ_BUFFER_SIZE, 0);
	if (bytes == 0)
	{
		state = DISCONNECT;
		return ;
	}
	if (bytes == -1)
	{
		return ;//need to check what to do nxt (erno)
	}
	read_operations++;
	for (int i = 0; i < bytes; i++)
		read_buffer += buff[i];
	pos_header_end = read_buffer.find("\r\n\r\n");
    if (pos_header_end == std::string::npos)// Header is incomplete, wait for more data
	{
    	return;
	}
	pos_content_length = read_buffer.find("Content-Length:");
	if (pos_content_length != std::string::npos)
	{
		long expected_body_size = std::atol(read_buffer.substr(pos_content_length + 16, pos_header_end).c_str());
		if (static_cast<int>(read_buffer.size() - pos_header_end - 4) < expected_body_size)// Body is incomplete, wait for more data
        {
			return;
		}
	}
	else if (read_buffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		pos_zero = read_buffer.find("0\r\n\r\n");
		if (pos_zero == std::string::npos) // Chunked body is incomplete, wait for more data
        {
			return ;
		}
	}
	state++;
}

void	ClientSocket::init_http_process(Configs &possible_configs)
{
	if (state != HTTP)
		return ;
	request = RequestParser::parse(read_buffer);
	read_buffer.clear();
	find_match_config(possible_configs, request.getHost());
	response = ResponseBuilder::build(request, match_config);
	if (!response.getFilePathForBody().empty())
	{
		file_fd = open(response.getFilePathForBody().c_str(), O_RDONLY);
	}	
	state++;
}

void	ClientSocket::find_match_config(Configs &possible_configs, const std::string host)
{
	std::vector<std::string> possible_names;
	match_config = possible_configs[0];
	for (int i = 0; i < static_cast<int>(possible_configs.size()); i++)
	{
		possible_names = possible_configs[i].getServerNames();
		for (int j = 0; j < static_cast<int>(possible_names.size()); j++)
		{
			if (possible_names[j] == host)
			{
				match_config = possible_configs[i];
				return ;
			}
		}
	}
}


void	ClientSocket::execute_cgi()
{
	if (state != EXECUTECGI)
		return ;
	if (response.getCgiPath().empty())
	{
		state = FILES;
		return ;
	}
	pipe(pipe_fd);
    cgi_pid = fork();
    if (cgi_pid == -1) {
        std::cerr << "ERROR: Fork failed. Errno: " << errno << " - " << strerror(errno) << std::endl;
        response = ResponseBuilder::buildErrorResponse(match_config, request, "500", "Internal Server Error");
		state = FILES;
		return ;
    } 
	else if (cgi_pid == 0) {
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		std::string cgi_path = response.getCgiPath();
		const CGIConfig& cgiConfig = match_config.getCgi();
 		std::string request_body = request.getBody(); // Store the body in a variable
		char* args[] = {
			const_cast<char*>(cgiConfig.path.c_str()),  // Python interpreter path
			const_cast<char*>(cgi_path.c_str()),        // Script path
			const_cast<char*>(request_body.c_str()),    // Argument to the script
			NULL
		};
		execv(cgiConfig.path.c_str(), args);
        std::cerr << "ERROR: execv failed. Errno: " << errno << " - " << strerror(errno) << std::endl;
        exit(1);
    }
	else
	{
		close(pipe_fd[1]);
		state++;
		start_time = clock();
		read_operations++;
		multiplex->add(pipe_fd[0]);
	}
}

void	ClientSocket::wait_cgi()
{
	if (state != WAITCGI)
		return;
	int status;
	if (waitpid(cgi_pid, &status, WNOHANG) == 0)
	{
		if ((clock() - start_time)/CLOCKS_PER_SEC < MAX_TIME_CGI)
			return ;
		kill(cgi_pid, SIGINT);
		cgi_error_code = "504";
		cgi_error_message = "Gateway Timeout";
	}
	state = INCORRECT_CGI;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		state = CORRECT_CGI;
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
	{
		cgi_error_code = "403";
		cgi_error_message = "Forbidden";
	}
}

void	ClientSocket::incorrect_cgi()
{
	if (state != INCORRECT_CGI)
		return;
	read_operations = 0;
	multiplex->remove(pipe_fd[0]);
	response =  ResponseBuilder::buildErrorResponse(match_config, request, cgi_error_code, cgi_error_message);
	write_buffer = response.toString();
	if (!response.getFilePathForBody().empty())
		file_fd = open(response.getFilePathForBody().c_str(), O_RDONLY);
	state = FILES;
}

void	ClientSocket::correct_cgi()
{
	if (state != CORRECT_CGI || read_operations > 0 || !multiplex->ready_to_read(pipe_fd[0]))
		return ;
	char buff[READ_BUFFER_SIZE];
	memset(buff, 0, READ_BUFFER_SIZE);
	int bytes = read(pipe_fd[0], buff, READ_BUFFER_SIZE);
	if (bytes == -1)
		return ;
	read_operations++;
	for (int i = 0; i < bytes; i++)
		write_buffer += buff[i];
	if (bytes < READ_BUFFER_SIZE)
	{
		multiplex->remove(pipe_fd[0]);
		state = WRITE;
	}
}

void	ClientSocket::manage_files()
{
	if (state != FILES)
		return ;
	if (!response.getFilePathForBody().empty()) 
	{
		if (file_fd < 0)
			return ;
		if (read_operations > 0)
			return ;
		int body_done = response.buildBodyFromFile(match_config, file_fd, &state);
		read_operations ++;
		if (body_done == false)
			return ;
		close(file_fd);
	}
	if (request.hasPostFileContents() && request.hasPostFileFds())
	{
		if (write_operations > 0)
			return ;
		int max = static_cast<int>(write_buffer.size()) - write_offset;
		if (max > WRITE_BUFFER_SIZE)
			max = WRITE_BUFFER_SIZE;
		int bytes = write(request.getLastFileFd(), &(request.getLastFileContent())[write_offset], max);
		if (bytes == 0)
		{
			state = DISCONNECT;
			return ;
		}
		if (bytes == -1)
			return ;
		write_operations++;
		write_offset += bytes;
		if (write_offset < static_cast<int>(request.getLastFileContent().size()))
			return ;
		write_offset = 0;
		close(request.getLastFileFd());
		request.popBackPostFileFds();
		request.popBackPostFileContents();
		if (request.hasPostFileContents() && request.hasPostFileFds())
			return ;
	}
	write_buffer = response.toString();
	state++;
}

void	ClientSocket::write_response()
{
	if (state != WRITE || write_operations > 0 || !multiplex->ready_to_write(fd))
		return ;

	int max = static_cast<int>(write_buffer.size()) - write_offset;
	if (max > WRITE_BUFFER_SIZE)
		max = WRITE_BUFFER_SIZE;
	int bytes = send(fd, &write_buffer[write_offset], max, 0);
	if (bytes == 0)
	{
		state = DISCONNECT;
		return ;
	}
	if (bytes == -1)
		return ;
	write_operations++;
	write_offset += bytes;
	if (write_offset >= static_cast<int>(write_buffer.size()))
		state = DISCONNECT;
}
