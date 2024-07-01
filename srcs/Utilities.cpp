#include "Utilities.hpp"

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
