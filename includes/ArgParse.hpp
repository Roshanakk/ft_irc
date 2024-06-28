#pragma once

#include <iostream>
#include <cstdlib>
#include <string>

#include "ServerException.hpp"

class ArgParse {
  public:
    static void numArgs(int argc);
    static int parsePort(char *port);
    static std::string parsePass(char *pass);

  private:
    // Making the constructor private so that the class can't be instantiated
    ArgParse();
    ~ArgParse();
};