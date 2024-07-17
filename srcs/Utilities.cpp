#include "Utilities.hpp"

#include <iostream>

Utilities::~Utilities(void) {}

std::vector<std::string> Utilities::split(const std::string& str, char delim) {
    std::vector<std::string> retVec;
    std::istringstream iss(str);
    std::string line;

    while (std::getline(iss, line, delim)) {
        retVec.push_back(line);
    }

    return retVec;
}

std::vector<std::string> Utilities::kickSplit(const std::string& str, char delim) {
    std::vector<std::string> retVec;
    std::istringstream iss(str);
    std::string line;

    while (std::getline(iss, line, delim)) {
        retVec.push_back(line.substr(1));
    }

    return retVec;
}

std::vector<std::string> Utilities::clientSplit(const std::string& str, char delim) {
    std::vector<std::string> retVec;
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string::npos) {
        // Include the delimiter in the substring
        retVec.push_back(str.substr(start, end - start + 1));
        start = end + 1;
        end = str.find(delim, start);
    }

    // Add the remaining part of the string, if any
    if (start < str.length()) {
        retVec.push_back(str.substr(start));
    }

    return retVec;
}