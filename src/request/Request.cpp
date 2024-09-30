#include "Request.hpp"

Request::Request() {
    error = false;
}
Request::~Request() {}
void Request::Parsing(std::string const &input) {
	std::map<std::string, std::string>	headers;
	std::string							line;
	std::istringstream					iss(input);

	std::getline(iss, line);
	this->method = this->parseMethod(line);
	this->url = this->parseUrl(line);
	this->version = this->parseVersion(line);
	this->headers = this->parseHeaders(input);
    this->body = this->parseBody(input);
    this->args = this->parseArgs();
    this->script = this->parseScript(url);
    this->hostName = this->parseHostName(input);
}
Request::Request(Request const &src) {
	*this = src;
}
Request &Request::operator=(Request const &src) {
	if (this != &src) {
		this->method = src.method;
		this->url = src.url;
		this->version = src.version;
		this->headers = src.headers;
	}
	return *this;
}

std::string const &Request::getMethod() const {
	return this->method;
}

std::string const &Request::getVersion() const {
	return this->version;
}


std::string const &Request::getUrl() const {
	return this->url;
}

std::string const &Request::getUrlAutoindex() const {
	return this->urlAutoindex;
}

std::map<std::string, std::string> const &Request::getHeaders() const {
	return this->headers;
}

std::string const Request::parseMethod(std::string const &input) {
	std::istringstream	iss(input);
	std::string			method;

	std::getline(iss, method, ' ');
	return method;
}

std::string const Request::parseHostName(std::string const &input) {
    std::string			hostName;

    hostName = input.substr(input.find("Host: ") + 6, input.substr(input.find("Host: ") + 6).find("\r\n"));
    port = std::atoi(hostName.substr(hostName.find(':') + 1).c_str());
    hostName = hostName.substr(0, hostName.find(':'));
    if (hostName.empty())
        throw std::invalid_argument("Invalid HostName");
    return hostName;
}

std::string const Request::parseScript(std::string const &input)
{
    if (input.find("/?") != std::string::npos)
        return input.substr(0, input.find("/?"));
    else if (input.find('?') != std::string::npos)
        return input.substr(0, input.find('?'));
    else
        return input;
}

std::string const Request::parseBody(std::string const &input) {
    std::string			body;

    // std::cout << "input: " << input << std::endl;

    size_t doubleLineBreakPos = input.find("\r\n\r\n");
    if (doubleLineBreakPos != std::string::npos) {

        if (input.find("Content-Length") != std::string::npos) {
            body = input.substr(doubleLineBreakPos + 4);
            return body;
        } else if (input.find("Transfer-Encoding") != std::string::npos) {
            std::string line = input.substr(doubleLineBreakPos + 4);
            std::istringstream iss(line);
            std::string chunk;
            while (std::getline(iss, chunk, '\r')) {
                if (chunk.empty() || chunk == "\n" || chunk == "\r" || chunk == "\r\n")
                    continue;
                body += chunk;
            }
            return body;
        }
    } else {
        return "";
    }
    return "";
}

std::string const Request::parseUrl(std::string const &input) {
	std::istringstream	iss(input);
	std::string			url;

	std::getline(iss, url, ' ');
	if (url.empty())
		throw std::invalid_argument("Invalid URL");
	std::getline(iss, url, ' ');
	if (url.empty())
		throw std::invalid_argument("Invalid URL");

	return url;
}

std::map<std::string, std::string> const Request::parseArgs() {
    std::map<std::string, std::string> args;
    std::string url = this->getUrl();
    if (url.find('=') == std::string::npos && url.find('?') != std::string::npos)
        error = true;
    std::string argsString;
    size_t argsStart = url.find('?');
    if (argsStart != std::string::npos) {
        argsString = url.substr(argsStart + 1);
        url = url.substr(0, argsStart);
    }
    std::istringstream iss(argsString);
    std::string arg;
    while (std::getline(iss, arg, '&')) {
        size_t argStart = arg.find('=');
        if (argStart != std::string::npos) {
            std::string key = arg.substr(0, argStart);
            std::string value = arg.substr(argStart + 1);
            args.insert(std::pair<std::string, std::string>(key, value));
        }
    }
    return args;
}

std::string const Request::parseVersion(std::string const &input) {
	if (input.find("HTTP/") != std::string::npos)
        return (input.substr(input.find("HTTP/")));
    else
        exit(1);
}

std::map<std::string, std::string> const Request::parseHeaders(std::string const &input) {
	std::map<std::string, std::string>	headers;
	std::istringstream					iss(input);
	std::string							line;

	std::getline(iss, line);
	while (std::getline(iss, line)) {
		if (line.empty() || line == "\n" || line == "\r" || line == "\r\n")
			continue;
		headers.insert(std::pair<std::string, std::string>(this->toLower(line.substr(0, line.find(":"))),
															line.substr(line.find(":") + 2)));
	}
	return headers;
}

std::string const Request::toLower(std::string const &input) {
	std::string	output;

	for (size_t i = 0; i < input.length(); i++)
		output += std::tolower(input[i]);
	return output;
}

std::string const &Request::getBody() const {
    return this->body;
}

const std::map<std::string, std::string> &Request::getArgs() const {
    return args;
}

std::string const &Request::getScript() const {
    return this->script;
}

void Request::setUrl(std::string url) {
    this->url = url;
}

void Request::setUrlAutoindex(std::string url) {
	this->urlAutoindex = url;
}

void Request::setScript(std::string script) {
    this->script = script;
}

bool Request::getError() const {
    return error;
}

std::string const &Request::getHostName() const {
    return hostName;
}

bool Request::isFile() const {
    if (url.find('.') != std::string::npos)
        return true;
    return false;
}
