#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../../include/Libraries.hpp"

class Request {
	private:
		std::string method;
		std::string version;
		std::string url;
		std::string urlAutoindex;
        std::string script;
        std::map<std::string, std::string> args;
        std::string body;
        std::string hostName;
        int port;
		std::map<std::string, std::string> headers;

		std::string const parseMethod(std::string const &input);
		std::string const parseUrl(std::string const &input);
		std::string const parseVersion(std::string const &input);
        std::string const parseHostName(std::string const &input);
        std::map<std::string, std::string> const parseArgs();
        std::string const parseBody(std::string const &input);
		std::string const toLower(std::string const &input);
		std::map<std::string, std::string> const parseHeaders(std::string const &input);
        bool error;

	public:
        Request();
		~Request();
		void Parsing(std::string	const &input);
		Request(Request const &src);
		Request &operator=(Request const &src);

		std::string const parseScript(std::string const &input);
		std::string const &getMethod() const;
		std::string const &getVersion() const;
		std::string const &getUrl() const;
		std::string const &getUrlAutoindex() const;
        void setUrl(std::string url);
		void setUrlAutoindex(std::string url);
        void setScript(std::string script);
		std::string const &getScript() const;
        std::string const &getBody() const;
        std::string const &getHostName() const;
        const std::map<std::string, std::string> &getArgs() const;
		std::map<std::string, std::string> const &getHeaders() const;
        std::string request;
        bool getError() const;
        bool isFile() const;

};

#endif
