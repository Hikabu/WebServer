
#include "../../include/ServerSocket.hpp"
#include "../../include/WebServer.hpp"


int main(int argc, char **argv) {

    signal(SIGPIPE, SIG_IGN);
    if (argc != 2) {
        std::cout << "Usage: ./webserv <config>" << std::endl;
        return (1);
    }

    WebServer webserv(argv[1]);

    try {
        webserv.parseConfig();
        webserv.start();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (1);
    }

	return (0);
}
