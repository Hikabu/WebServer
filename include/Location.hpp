#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Parser.hpp"
#include "Libraries.hpp"


class Location : public Parser {
    private:
        std::vector<std::string> locationConfig;
        std::string pathLocation;
        std::string root;
        std::string index;
        bool autoIndex;
        std::string cgiPassPython;
        std::string cgiPassBash;
        int clientMaxBodySize;
        bool fileUpload;
        std::vector<std::string> methods;
        bool _redirect;
        std::string _redirect_path;

    public:

        Location();
        Location(const Location &copy);
        Location &operator=(const Location &copy);

        void setConfig(std::string &configLine);
        void printDataConfig() const;
        void printLocationData();
        std::string getCgiPassPython() const;
        std::string getCgiPassBash() const;
        std::vector<std::string> getMethods() const;
        bool isRedirect() const;
        bool isAutoIndex() const;
        const std::string &getRoot() const;
        const std::string &getIndex() const;
        const std::string &getPathLocation() const;
        const std::string &getRedirectPath() const;
        void updateDataInLocation();
        void parsePathLocation();


};


#endif