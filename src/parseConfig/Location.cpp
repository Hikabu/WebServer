#include "../../include/Location.hpp"

Location::Location() {
    this->root = "";
    this->index = "";
    this->autoIndex = false;
    this->cgiPassPython = "";
    this->cgiPassBash = "";
    this->clientMaxBodySize = 0;
    this->fileUpload = false;
    this->locationConfig = std::vector<std::string>();
    this->methods = std::vector<std::string>();
    this->pathLocation = "";
    this->_redirect = false;
    this->_redirect_path = "";
}

Location::Location(const Location &copy) {
    *this = copy;
}

Location &Location::operator=(const Location &copy) {
    if (this != &copy) {
        this->root = copy.root;
        this->index = copy.index;
        this->autoIndex = copy.autoIndex;
        this->cgiPassPython = copy.cgiPassPython;
        this->cgiPassBash = copy.cgiPassBash;
        this->clientMaxBodySize = copy.clientMaxBodySize;
        this->fileUpload = copy.fileUpload;
        this->methods = copy.methods;
        this->locationConfig = copy.locationConfig;
        this->pathLocation = copy.pathLocation;
        this->_redirect_path = copy._redirect_path;
        this->_redirect = copy._redirect;
    }
    return *this;
}

void Location::setConfig(std::string &configLine) {
    this->locationConfig.push_back(configLine);
}

void Location::printDataConfig() const {
    std::cout << "-----------------" << std::endl << "Location WebServer:" << std::endl << "-----------------" << std::endl;
    for (std::vector<std::string>::const_iterator it = this->locationConfig.begin(); it != this->locationConfig.end(); it++) {
        std::cout << *it << std::endl;
    }
}

bool Location::isRedirect() const {
    return _redirect;
}

const std::string &Location::getRedirectPath() const {
    return _redirect_path;
}

void Location::printLocationData() {
    std::cout << "-----------------" << std::endl << "Location WebServer:" << std::endl << "-----------------" << std::endl;
    std::cout << "pathLocation: " << pathLocation << std::endl;
    std::cout << "root: " << root << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "autoIndex: " << autoIndex << std::endl;
    std::cout << "cgiPass: " << cgiPassPython << std::endl;
    std::cout << "clientMaxBodySize: " << clientMaxBodySize << std::endl;
    std::cout << "fileUpload: " << fileUpload << std::endl;
    std::cout << "redirect: " << _redirect_path << std::endl;
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it) {
        std::cout <<  "Methods: " << *it << std::endl;
    }
}

void Location::updateDataInLocation() {
    for (size_t i = 0; i < locationConfig.size(); i++) {
        cutDataStr(locationConfig[i], "location", pathLocation);
        cutDataStr(locationConfig[i], "root", root);
        cutDataBool(locationConfig[i], "autoindex", autoIndex);
        cutDataStr(locationConfig[i], "redirect", _redirect_path);
        cutDataStr(locationConfig[i], "index", index);
        cutDataStr(locationConfig[i], "cgi_pass_python", cgiPassPython);
        cutDataStr(locationConfig[i], "cgi_pass_bash", cgiPassBash);
        cutDataNum(locationConfig[i], "client_max_body_size", clientMaxBodySize);
        cutDataBool(locationConfig[i], "file_upload", fileUpload);
        cutDataArray(locationConfig[i], "methods", methods);

    }
    parsePathLocation();
}

void Location::parsePathLocation() {
    size_t pos = pathLocation.find_first_of(" ");
    if (pos != std::string::npos) {
        pathLocation = pathLocation.substr(pos + 1);
    }

    size_t pos1 = pathLocation.find_last_of(" ");
    if (pos1 != std::string::npos) {
        pathLocation = pathLocation.substr(0, pos1);
    }
}



std::string Location::getCgiPassPython() const {
    return cgiPassPython;
}

std::string Location::getCgiPassBash() const {
    return cgiPassBash;
}

const std::string &Location::getPathLocation() const {
    return pathLocation;
}

const std::string &Location::getRoot() const {
    return root;
}

const std::string &Location::getIndex() const {
    return index;
}

bool Location::isAutoIndex() const {
    return autoIndex;
}

std::vector<std::string> Location::getMethods() const {
    return methods;
}


