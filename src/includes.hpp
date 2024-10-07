/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 20:08:02 by bperez-a          #+#    #+#             */
/*   Updated: 2024/10/07 07:36:30 by artclave         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
#define INCLUDES_HPP
#include <sys/types.h>    // Provides basic system data types
#include <sys/socket.h>   // Core socket programming functions
#include <netinet/in.h>   // Internet address family and structures
#include <cstring>        // String manipulation functions
#include <arpa/inet.h>    // Functions for manipulating IP addresses
#include <unistd.h>       // POSIX operating system API (file operations, etc.)
#include <stdio.h>        // Standard input/output operations
#include <vector>         // Dynamic array container
#include <list>           // Doubly linked list container
#include <sys/select.h>   // Synchronous I/O multiplexing
#include <iostream>       // C++ standard I/O streams
#include <stdexcept>      // Standard exception classes
#include <fstream>        // File stream operations
#include <fcntl.h>        // File control options
#include <map>            // Associative container (key-value pairs)
#include <sstream>        // String stream for in-memory I/O
#include <cstdlib>        // General purpose functions (e.g., memory allocation)
#include <sys/stat.h>     // File status and information
#include <set>            // Set container for unique elements
#include <dirent.h>       // Directory entry operations
#include <string>
#include <algorithm>      //for find
#include <netinet/tcp.h> // For TCP_KEEPIDLE
#include <list>
#include <signal.h>
#include <sys/wait.h>
#include <csignal>
#include <ctime> // Import the ctime library


# define MAX_BUFFER_SIZE 100
# define MAX_TIME_CGI	10 //should be 10-30 please update!

enum connection_states
{
	READING,
	HTTP,
	CGI,
	FILES,
	WRITE,
	DISCONNECT,
};

# define EXIT 0 
# define DONT_EXIT 1


#endif
