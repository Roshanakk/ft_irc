#pragma once

#include <sstream>
#include <vector>
#include <string>

class Utilities {
  public:
    static std::vector<std::string> split(const std::string& str, char delim);
    static std::vector<std::string> clientSplit(const std::string& str, char delim);

  private:
    Utilities(void);
    ~Utilities(void);
};