# WebServer
little nginx

## About
The goal of the project is to build a C++98 compatible HTTP web server from scratch.
The web server can handle HTTP GET, HEAD, POST, PUT, and DELETE Requests, and can serve static files from a specified root directory or dynamic content using CGI. It is also able to handle multiple client connections concurrently with the help of select().

# Parts of a web server

A basic HTTP web server consists of several components that work together to receive and process HTTP requests from clients and send back responses. Below are the main parts of our webserver.

## Server Core
The networking part of a web server that handles TCP connections and performs tasks such as listening for incoming requests and sending back responses. It is responsible for the low-level networking tasks of the web server, such as creating and managing sockets, handling input and output streams, and managing the flow of data between the server and clients.

Before writing your webserver, I would recommend reading [this](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa) awesome guide on building simple TCP client/server in C as it will help you get a good understanding of how TCP works in C/C++. also you would need to understand I/O multiplixing, [this](https://www.youtube.com/watch?v=Y6pFtgRdUts&ab_channel=JacobSorber) video will help you grasp the main idea of select().

The I/O Multiplexing process in our web server is summarized in the flowchart below. (CGI is not included in the flowchart but may be added in the future)
<br>
<br>
<br>
<br>

## Request Parser

The parsing part of a web server refers to the process that is responsible for interpreting and extracting information from HTTP requests.
In this web server, the parsing of requests is performed by the HttpRequest class. An HttpRequest object receives an incoming request, parses it, and extracts the relevant information such as the method, path, headers, and message body(if present). If any syntax error was found in the request during parsing, error flags are set and parsing stops.
Request can be fed to the object through the method feed() either fully or partially, this is possible because the parser scans the request byte at a time and update the parsing state whenever needed. The same way of parsing is used by Nginx and Nodejs request parsers.

below is an overview of how the parser works.


## Response Builder

The response builder is responsible for constructing and formatting the HTTP responses that are sent back to clients in response to their requests.
In this web server, the Response class is responsible for building and storing the HTTP response, including the status line, headers, and message body.
The response builder may also perform tasks such as setting the appropriate status code and reason phrase based on the result of the request, adding headers to the response to provide additional information about the content or the server, and formatting the message body according to the content type and encoding of the response.
For example, if the server receives a request for a webpage from a client, the server will parse the request and pass it to a Response object which will fetch the contents of the webpage and construct the HTTP response with the HTML content in the message body and the appropriate headers, such as the Content-Type and Content-Length headers.

## Configuration File

Configuration file is a text file that contains various settings and directives that dictate how the web server should operate. These settings can include things like the port number that the web server should listen on, the location of the web server's root directory, and many other settings.

Here is an example fie that shows config file format and supported directives.
<br>

  ```nginx
server {
    listen 8001;                        # listening port, mandatory parameter
    host 127.0.0.1;                     # host or 127.0.0.1 by default
    server_name test;                   # specify server_name, need to be added into /etc/hosts to work
    error_page 404 /error/404.html;     # default error page
    client_max_body_size 1024;          # max request body size in bytes
    root docs/fusion_web/;              # root folder of site directory, full or relative path, mandatory parameter
    index index.html;                   # default page when requesting a directory, index.html by default

    location /tours {                   
        root docs/fusion_web;           # root folder of the location, if not specified, taken from the server. 
                                        # EX: - URI /tours           --> docs/fusion_web/tours
                                        #     - URI /tours/page.html --> docs/fusion_web/tours/page.html 
        autoindex on;                   # turn on/off directory listing
        allow_methods POST GET;         # allowed methods in location, GET only by default
        index index.html;               # default page when requesting a directory, copies root index by default
        return abc/index1.html;         # redirection
        alias  docs/fusion_web;         # replaces location part of URI. 
                                        # EX: - URI /tours           --> docs/fusion_web
                                        #     - URI /tours/page.html --> docs/fusion_web/page.html 
    }

    location cgi-bin {
        root ./;                                                 # cgi-bin location, mandatory parameter
        cgi_path /usr/bin/python3 /bin/bash;                     # location of interpreters installed on the current system, mandatory parameter
        cgi_ext .py .sh;                                         # extensions for executable files, mandatory parameter
    }
}
  ```



## CGI

CGI is a standard for running external programs from a web server. When a user requests a web page that should be handled by a CGI program, the web server executes the program and returns the output to the user's web browser.

CGI programs are simply scripts that can be written in any programming language, such as Perl, Python, or bash, and are typically used to process data submitted by a user through a web browser, or to generate dynamic content on a web page.
